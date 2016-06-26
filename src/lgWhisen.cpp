/*
 lgWhisen.cpp - control lg whisen ac using ir
  chaeplin at gmail.com
*/
#include "lgWhisen.h"

lgWhisen::lgWhisen()
{
  this->_state          = AC_OFF;
  this->actype          = NOT_DEFINED;
  this->heating         = NOT_DEFINED;
  this->temperature     = NOT_DEFINED;
  this->flow            = NOT_DEFINED;
  this->irpin           = NOT_DEFINED;
  this->ac_code_to_send = NOT_DEFINED;
  setDecodeparas();
}

lgWhisen::lgWhisen(int actype)
{
  this->_state          = AC_OFF;
  this->heating         = NOT_DEFINED;
  this->temperature     = NOT_DEFINED;
  this->flow            = NOT_DEFINED;
  this->irpin           = NOT_DEFINED;
  this->ac_code_to_send = NOT_DEFINED;
  setActype(actype);
  setDecodeparas();
}

lgWhisen::lgWhisen(int actype, int heating)
{
  this->_state          = AC_OFF;
  this->temperature     = NOT_DEFINED;
  this->flow            = NOT_DEFINED;
  this->irpin           = NOT_DEFINED;
  this->ac_code_to_send = NOT_DEFINED;
  setActype(actype);
  setHeating(heating);
  setDecodeparas();
}

lgWhisen::lgWhisen(int actype, int heating, int irpin)
{
  this->_state          = AC_OFF;
  this->temperature     = NOT_DEFINED;
  this->flow            = NOT_DEFINED;
  this->ac_code_to_send = NOT_DEFINED;
  setActype(actype);
  setHeating(heating);
  setIrpin(irpin);
  setDecodeparas();
}

lgWhisen::lgWhisen(int actype, int heating, int temperature, int flow, int irpin)
{
  this->_state          = AC_OFF;
  this->ac_code_to_send = NOT_DEFINED;
  setActype(actype);
  setHeating(heating);
  setTemp(temperature);
  setFlow(flow);
  setIrpin(irpin);
  setDecodeparas();
}

boolean lgWhisen::send_code() 
{
  if (this->ac_code_to_send == NOT_DEFINED)
  {
    return false;
  }
  else
  {
    #ifdef ESP8266
      IRsend irsend(this->irpin );
      irsend.begin();
    #else
      IRsend irsend;
    #endif

    Serial.print("code to send : ");
    Serial.print(this->ac_code_to_send, BIN);
    Serial.print(" : ");
    Serial.println(this->ac_code_to_send, HEX);

    irsend.sendLG(this->ac_code_to_send, 28);
    return true;
  }
}

boolean lgWhisen::activate()
{
  if (this->temperature == NOT_DEFINED || this->flow == NOT_DEFINED)
    return false;

  return activate(this->temperature, this->flow);
}

boolean lgWhisen::activate(int temperature, int flow)
{
  if (this->heating == NOT_DEFINED || this->actype == NOT_DEFINED)
    return false;

  if (temperature != this->temperature)
    this->temperature = temperature;

  if (flow != this->flow)
    this->flow = flow;

  int ac_msbits1 = 8;
  int ac_msbits2 = 8;
  int ac_msbits3 = 0;
  int ac_msbits4 ;
  if ( this->heating == 1 ) {
    // heating
    ac_msbits4 = 4;
  } else {
    // cooling
    ac_msbits4 = 0;
  }
  
  int ac_msbits5 = temperature - 15;
  int ac_msbits6 ;

  if ( this->actype == 0) {
    ac_msbits6 = ac_flow_tower[this->flow];
  } else {
    ac_msbits6 = ac_flow_wall[this->flow];
  }

  int ac_msbits7 = (ac_msbits3 + ac_msbits4 + ac_msbits5 + ac_msbits6) & B00001111;

  this->ac_code_to_send = ac_msbits1 << 4 ;
  this->ac_code_to_send = (this->ac_code_to_send + ac_msbits2) << 4;
  this->ac_code_to_send = (this->ac_code_to_send + ac_msbits3) << 4;
  this->ac_code_to_send = (this->ac_code_to_send + ac_msbits4) << 4;
  this->ac_code_to_send = (this->ac_code_to_send + ac_msbits5) << 4;
  this->ac_code_to_send = (this->ac_code_to_send + ac_msbits6) << 4;
  this->ac_code_to_send = (this->ac_code_to_send + ac_msbits7);

  if (send_code())
  {
    this->_state = AC_ON;
    return true;
  }
  else
  {
    return false;
  }
}

boolean lgWhisen::power_down()
{
  this->ac_code_to_send = 0x88C0051;
  if (send_code())
  {
    this->_state = AC_OFF;
    return true;
  }
  else
  {
    return false;
  }
}

boolean lgWhisen::decode(decode_results *results)
{
  if (results->bits == 28 || results->bits == 32)
  {
    unsigned long data = 0;
    unsigned long tempdata = 0;
    for (int i = 3;  i < results->rawlen;  i++) {
      unsigned long  x = results->rawbuf[i] * USECPERTICK;
      if (!(i & 1))
      {
        if ( x > 1000)
        {
          data = (data << 1) | 1;
        }
        else
        {
          data <<= 1;
        }
      }
    }

    tempdata = data;
    int ac_msbits7 = tempdata & B1111;
    tempdata = tempdata >> 4;
    int ac_msbits6 = tempdata & B1111;
    tempdata = tempdata >> 4;
    int ac_msbits5 = tempdata & B1111;
    tempdata = tempdata >> 4;
    int ac_msbits4 = tempdata & B1111;
    tempdata = tempdata >> 4;
    int ac_msbits3 = tempdata & B1111;
    tempdata = tempdata >> 4;
    int ac_msbits2 = tempdata & B1111;
    tempdata = tempdata >> 4;
    int ac_msbits1 = tempdata;

    if (  ac_msbits1 == 8 &&
          ac_msbits2 == 8 &&
          ac_msbits7 == ((ac_msbits3 + ac_msbits4 + ac_msbits5 + ac_msbits6) & B1111)
      )
    {
      if ( data == 0x88C0051)
      {
        this->ir_mode = AC_OFF;
      }
      else if (ac_msbits3 == 0x0)
      {
        this->ir_mode = AC_ON;

        if ( ac_msbits4 == 0)
        {
          this->ir_heating = AC_COOLING;
        }
        else if (ac_msbits4 == 4)
        {
          this->ir_heating = AC_HEATING;
        }
        
        this->ir_temperature = ac_msbits5 + 15;
        if (this->actype == 0)
        {
          switch (ac_msbits6)
          {
            case 0:
              this->ir_flow = 0;
              break;

            case 4:
              this->ir_flow = 1;
              break;

            case 6:
              this->ir_flow = 2;
              break;              

             default:
               break;
          }
        }
        else if (this->actype == 1)
        {
          switch (ac_msbits6)
          {          
            case 0:
              this->ir_flow = 0;
              break;

            case 2:
              this->ir_flow = 1;
              break;

            case 4:
              this->ir_flow = 2;
              break;

            case 5:
              this->ir_flow = 3;
              break;              

             default:
               break;
          }
        }
      }
      else if (ac_msbits3 == 0x1 && ac_msbits5 == 0x0 )
      {
        this->ir_mode = AC_SPECIAL;
      }
      else if (ac_msbits3 == 0xC && ac_msbits5 == 0x0 && ac_msbits6 == 0x0)
      {
        this->ir_mode = AC_AIRCLEAN;
      }
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

lgWhisen& lgWhisen::setActype(int actype)
{
  this->actype = actype;
  return *this;
}

lgWhisen& lgWhisen::setHeating(int heating)
{
  this->heating = heating;
  return *this;
}

lgWhisen& lgWhisen::setTemp(int temperature)
{
  this->temperature = temperature;
  return *this;
}

lgWhisen& lgWhisen::setFlow(int flow)
{
  this->flow = flow;
  return *this;
}

lgWhisen& lgWhisen::setIrpin(int irpin)
{
  this->irpin = irpin;
  return *this;
}

lgWhisen& lgWhisen::setDecodeparas()
{
  this->ir_mode         = NOT_DEFINED;
  this->ir_heating      = NOT_DEFINED;
  this->ir_temperature  = NOT_DEFINED;
  this->ir_flow         = NOT_DEFINED;
  return *this;
}

uint8_t lgWhisen::get_ir_mode()
{
  return this->ir_mode;
}

uint8_t lgWhisen::get_ir_heating()
{
  return this->ir_heating;
}

uint8_t lgWhisen::get_ir_temperature()
{
  return this->ir_temperature;
}

uint8_t lgWhisen::get_ir_flow()
{
  return this->ir_flow;
}

int lgWhisen::state() {
  return this->_state;
}

// end of lgWhisen.cpp
