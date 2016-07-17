#include <IRremoteESP8266.h>

#define RECV_PIN 14

#define NOT_DEFINED 255

IRrecv irrecv(RECV_PIN);

struct
{
  uint8_t ac_on;
  uint8_t heating;
  uint8_t airclean;
  uint8_t specialmode;
  uint8_t temperature;
  uint8_t flow;
} ac_status;

void setup()
{
  ac_status.ac_on        = NOT_DEFINED;
  ac_status.heating      = NOT_DEFINED;
  ac_status.airclean     = NOT_DEFINED;
  ac_status.specialmode  = NOT_DEFINED;
  ac_status.temperature  = NOT_DEFINED;
  ac_status.flow         = NOT_DEFINED;

  Serial.begin(115200);
  irrecv.enableIRIn();
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println();
  Serial.println("  - - - T E S T - - -   ");
}

// http://www.phanderson.com/arduino/arduino_display.html
void print_binary(int v, int num_places)
{
    int mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << num_places-1))
        {
             Serial.print("1");
        }
        else
        {
             Serial.print("0");
        }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            Serial.print("_");
        }
    }
}

void print_hex(int v, int num_places)
{
    int mask=0, n, num_nibbles, digit;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask; // truncate v to specified number of places

    num_nibbles = num_places / 4;
    if ((num_places % 4) != 0)
    {
        ++num_nibbles;
    }

    do
    {
        digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
        Serial.print(digit, HEX);
    } while(--num_nibbles);

}

void dumpInfo(decode_results *results)
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
    uint8_t ac_msbits7 = tempdata & B1111;
    tempdata = tempdata >> 4;
    uint8_t ac_msbits6 = tempdata & B1111;
    tempdata = tempdata >> 4;
    uint8_t ac_msbits5 = tempdata & B1111;
    tempdata = tempdata >> 4;
    uint8_t ac_msbits4 = tempdata & B1111;
    tempdata = tempdata >> 4;
    uint8_t ac_msbits3 = tempdata & B1111;
    tempdata = tempdata >> 4;
    uint8_t ac_msbits2 = tempdata & B1111;
    tempdata = tempdata >> 4;
    uint8_t ac_msbits1 = tempdata;

    if (  ac_msbits1 == 8 &&
          ac_msbits2 == 8 &&
          ac_msbits7 == ((ac_msbits3 + ac_msbits4 + ac_msbits5 + ac_msbits6) & B1111)
      )
    {
      Serial.println("ir received --");
      print_hex(data, 28);
      Serial.print(" - ");
      print_binary(data, 28);
      Serial.println(); 

      if ( data == 0x88C0051)
      {
        ac_status.ac_on = 0;
        ac_status.airclean = 0;
        ac_status.specialmode = 0;
      }
      else if (ac_msbits3 == 0x0)
      {
        ac_status.ac_on = 1;
        ac_status.airclean = 0;
        ac_status.specialmode = 0;
        if ( ac_msbits4 == 0 || ac_msbits4 == 4)
        {
          ac_status.heating = ac_msbits4;
        }
        ac_status.temperature = ac_msbits5 + 15;
        // need actype;
        ac_status.flow = ac_msbits6;
      }
      else if (ac_msbits3 == 0x1 && ac_msbits5 == 0x0 )
      {
        ac_status.ac_on = 1;
        ac_status.airclean = 0;
        ac_status.specialmode = 1;
      }
      else if (ac_msbits3 == 0xC && ac_msbits5 == 0x0 && ac_msbits6 == 0x0)
      {
        ac_status.ac_on = 1;
        ac_status.airclean = 1;
        ac_status.specialmode = 0;
      }

      Serial.print("on/off : ");
      Serial.print(ac_status.ac_on);

      Serial.print(" - heating : ");
      Serial.print(ac_status.heating);

      Serial.print(" - airclean : ");
      Serial.print(ac_status.airclean);

      Serial.print(" - specialmode : ");
      Serial.print(ac_status.specialmode);

      Serial.print(" - temperature : ");
      Serial.print(ac_status.temperature);

      Serial.print(" - flow : ");
      Serial.println(ac_status.flow);

    }
  }

  delay(10);
  irrecv.enableIRIn();
}

void loop() {
  decode_results results;

  if (irrecv.decode(&results))
  {
    dumpInfo(&results);
  }
}
