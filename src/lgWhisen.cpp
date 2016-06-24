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

int lgWhisen::state() {
  return this->_state;
}

// end of lgWhisen.cpp
