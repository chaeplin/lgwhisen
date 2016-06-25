#include <IRremoteESP8266.h>
#include <lgWhisen.h>

#define RECV_PIN 14
#define IRPIN 2

IRrecv irrecv(RECV_PIN);
lgWhisen lgWhisen(1, 0, 27, 0, IRPIN);

void setup()
{
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
 
