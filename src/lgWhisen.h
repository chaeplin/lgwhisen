/*
 lgWhisen.h - control lg whisen ac using ir
  chaeplin at gmail.com
*/

#ifndef lgWhisen_h
#define lgWhisen_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifdef ESP8266
	#include <IRremoteESP8266.h>
#else
  	#include <IRremote.h>
#endif

#define AC_OFF 0;
#define AC_ON  1;

#define NOT_DEFINED 255

class lgWhisen 
{
private:
	const int ac_flow_tower[3] = {0, 4, 6};
	const int ac_flow_wall[4]  = {0, 2, 4, 5};
	unsigned long ac_code_to_send;
	boolean send_code();
	int actype;
	int heating;
	int temperature;
	int flow;
	int irpin;
	int _state;

public:
	lgWhisen();
	lgWhisen(int, int);
	lgWhisen(int, int, int);
	lgWhisen(int, int, int, int, int);

	lgWhisen& setActype(int actype);
	lgWhisen& setHeating(int heating);
	lgWhisen& setTemp(int temperature);
	lgWhisen& setFlow(int flow);
	lgWhisen& setIrpin(int irpin);

	boolean activate();
	boolean activate(int temperature, int flow);
	boolean power_down();
	int state();
};

#endif

// end of lgWhisen.h