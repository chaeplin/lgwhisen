
#include <lgWhisen.h>

#define IRPIN 2

/*
- type           : 0 -> tower, 1 -> wall
- heating        : 0 -> cooling, 1 -> heating
- temterature    : 18 ~ 30
- flow           : 0 -> low, 1 -> mid, 2 -> high, if type == 1 3 -> change
- pin no of ir   : esp8266 only

lgWhisen lgWhisen()
lgWhisen lgWhisen(int actype, int heating)
lgWhisen lgWhisen(int actype, int heating, int irpin)
lgWhisen lgWhisen(int actype, int heating, int temperature, int flow, int irpin)

lgWhisen.setActype(int actype)
lgWhisen.setHeating(int heating)
lgWhisen.setTemp(int temperature)
lgWhisen.setFlow(int flow)
lgWhisen.setIrpin(int irpin)

1)
lgWhisen lgWhisen();
void setup()
{
  lgWhisen.setActype(0)
  lgWhisen.setHeating(0)
  lgWhisen.setTemp(27)
  lgWhisen.setFlow(1)
  // for esp8266
  lgWhisen.setIrpin(IRPIN)  
}

2)
lgWhisen lgWhisen(0, 0)
void setup()
{
  lgWhisen.setTemp(27)
  lgWhisen.setFlow(1)
  // for esp8266
  lgWhisen.setIrpin(IRPIN)  
}

3)....

*/

// for esp8266
lgWhisen lgWhisen(0, 0, 27, 0, IRPIN);
bool x = false;

void setup()
{

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println();
  Serial.println("  - - - T E S T - - -   ");
}

void loop() {
  if (!x)
  {
    lgWhisen.activate();
    delay(10000);

    lgWhisen.setTemp(26);
    lgWhisen.activate();
    delay(10000);

    lgWhisen.setFlow(0);
    lgWhisen.activate();
    delay(5000);

    lgWhisen.setFlow(1);
    lgWhisen.activate();
    delay(5000);

    lgWhisen.setFlow(2);
    lgWhisen.activate();
    delay(5000);

    lgWhisen.setFlow(3);
    lgWhisen.activate();
    delay(5000);

    lgWhisen.activate(25, 2);
    delay(10000);
    lgWhisen.power_down();
    x = true;
  }
}
