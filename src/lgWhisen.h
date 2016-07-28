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

#define AC_OFF      0
#define AC_ON       1
#define AC_AIRCLEAN 2
#define AC_SPECIAL  3

#define AC_COOLING 0
#define AC_HEATING 1

#define NOT_DEFINED 255

enum AC_Type_t 
{
	tower = 0,
	wall  = 1
};

enum AC_Heating_t 
{
	cooling = 0,
	heating = 1
};

enum AC_Status_t
{
	ac_off      = 0,
	ac_on       = 1,
	ac_airclean = 2,
	ac_special  = 3
};

class lgWhisen 
{
private:
	const int ac_flow_tower[3] = {0, 4, 6};
	const int ac_flow_wall[4]  = {0, 2, 4, 5};
	unsigned long ac_code_to_send;
	boolean send_code();
	lgWhisen& setDecodeparas();
	int actype;
	int heating;
	int temperature;
	int flow;
	int irpin;

	uint8_t ir_mode;
	uint8_t ir_heating;
	uint8_t ir_temperature;
	uint8_t	ir_flow;

	int _state;

public:
	lgWhisen();
	lgWhisen(int);
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
	boolean autodry_on();
	boolean autodry_off();
	boolean alrclean_on();
	boolean alrclean_off();
	boolean decode(decode_results *results);
	
	uint8_t get_ir_mode();
	uint8_t get_ir_heating();
	uint8_t get_ir_temperature();
	uint8_t get_ir_flow();
	int state();
};

#endif

// end of lgWhisen.h