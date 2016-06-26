#include <lgWhisen.h>

#ifdef ESP8266
  #include <IRremoteESP8266.h>
#else
    #include <IRremote.h>
#endif

#define RECV_PIN 14 // RX
#define IRPIN 2     // TX

IRrecv irrecv(RECV_PIN);

#ifdef ESP8266
  lgWhisen lgWhisen(0, 0, 27, 0, IRPIN);
#else
  lgWhisen lgWhisen(0, 0);
#endif

void setup()
{
#ifdef ESP8266
#else
  lgWhisen.setTemp(27);
  lgWhisen.setFlow(1);
#endif
  
  Serial.begin(115200);
  irrecv.enableIRIn();
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println();
  Serial.println("  - - - T E S T - - -   ");
}

void loop() {
  decode_results results;

  if (irrecv.decode(&results))
  {
    if(lgWhisen.decode(&results))
    {
      Serial.print("mode : ");      
      Serial.print(lgWhisen.get_ir_mode());
      Serial.print(" - heating : ");
      Serial.print(lgWhisen.get_ir_heating());
      Serial.print(" - temperature : ");
      Serial.print(lgWhisen.get_ir_temperature());
      Serial.print(" - flow : ");
      Serial.println(lgWhisen.get_ir_flow());      
    }
    irrecv.enableIRIn();
  }
}

/*
  - - - T E S T - - -   
mode : 1 - heating : 0 - temperature : 18 - flow : 2
mode : 1 - heating : 0 - temperature : 18 - flow : 3
mode : 1 - heating : 0 - temperature : 18 - flow : 0
mode : 2 - heating : 0 - temperature : 18 - flow : 0
mode : 3 - heating : 0 - temperature : 18 - flow : 0
mode : 1 - heating : 0 - temperature : 18 - flow : 2
mode : 1 - heating : 0 - temperature : 19 - flow : 2
mode : 1 - heating : 0 - temperature : 20 - flow : 2
mode : 1 - heating : 0 - temperature : 21 - flow : 2
mode : 1 - heating : 0 - temperature : 21 - flow : 2
mode : 2 - heating : 0 - temperature : 21 - flow : 2
mode : 0 - heating : 0 - temperature : 21 - flow : 2
mode : 1 - heating : 0 - temperature : 21 - flow : 2
mode : 1 - heating : 0 - temperature : 21 - flow : 3
mode : 1 - heating : 0 - temperature : 21 - flow : 0
mode : 1 - heating : 0 - temperature : 21 - flow : 1
mode : 1 - heating : 0 - temperature : 21 - flow : 2
mode : 1 - heating : 0 - temperature : 21 - flow : 3
mode : 0 - heating : 0 - temperature : 21 - flow : 3

*/
 
