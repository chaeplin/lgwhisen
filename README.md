lgwhisen
===============

control LG WHISEN AC.

### Can do
- encoding and decoding LG WHISEN A/C I.


### Library used
- https://github.com/z3t0/Arduino-IRremote for Arduino
- https://github.com/markszabo/IRremoteESP8266 for esp8266/Arduino

### Limitations
- no variable checking(type, temperature, heating/cooling, flow)
- heating is not yet tested


### Usage
* check example

* Variable
- type           : 0 -> tower, 1 -> wall
- heating        : 0 -> cooling, 1 -> heating
- temterature    : 18 ~ 30
- flow           : 0 -> low, 1 -> mid, 2 -> high, if type == 1, 3 -> change
- pin no of ir   : esp8266 only (for tx)

```
- lgWhisen lgWhisen()
- lgWhisen lgWhisen(int actype)
- lgWhisen lgWhisen(int actype, int heating)
- lgWhisen lgWhisen(int actype, int heating, int irpin)
- lgWhisen lgWhisen(int actype, int heating, int temperature, int flow, int irpin)
```

```
- lgWhisen.activate()
- lgWhisen.activate(int temperature, int flow)
- lgWhisen.power_down()
- lgWhisen.autodry_on()
- lgWhisen.autodry_off()
```

```
- lgWhisen.setActype(int actype)
- lgWhisen.setHeating(int heating)
- lgWhisen.setTemp(int temperature)
- lgWhisen.setFlow(int flow)
- lgWhisen.setIrpin(int irpin)
- lgWhisen.state()
```

Enjoy.

// chaeplin