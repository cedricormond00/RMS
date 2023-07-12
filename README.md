to monito# RMS
Master thesis code repo. This repository will contain all the source code

## General description of project
This project is a remote monitoring system to be used on the [Blue Diversion Autarky Project](https://www.eawag.ch/en/research/humanwelfare/wastewater/projects/autarky/), developed at EAWAG. 

The waterfall, currently only a prototype, requires that an operator come onsite regularly, to monitor the microbial water quality in the clean water tank. This is time-consuming and limits the possibility of performing high-quality field tests.
This project aims to tackle this challenge by providing a real-time, online microbial water quality monitoring. This ensures that the water is of sufficiently high microbial water quality such that the waterwall may be used at no risk for the user. 

A machine model was developed by XXX link, and implemented here. The only parameter is the ORP reading. Decision making of water being safe or unsafe is based on a cut-off value found from the trained model.
For example a XXX and LRT XXX, the ORP value was found to be at. 
For varying values, 


## User guide
<!--- The project is divided in 3 folders 
- final project
- RTC calibration
- ML model --->

## Component selection
the following components were used for the prototype.

| Item description 	| Comment 	| Brand 	| Quantity 	| Unitary cost 	| Total cost 	| manufacturer link 	| Manufacturer code 	| Supplier link 	|
|---	|---	|---	|---	|---	|---	|---	|---	|---	|
| MKR NB 1500 	|  	| Arduino 	| 1 	| CHF 71.39 	| CHF 71.39 	| https://store.arduino.cc/products/arduino-mkr-nb-1500 	|  	| https://www.distrelec.ch/fr/arduino-mkr-nb-1500-arduino-abx00019/p/30117099?track=true&no-cache=true&marketingPopup=false 	|
| MKR MEM Shield 	|  	| Arduino 	| 1 	| CHF 22.84 	| CHF 22.84 	| https://docs.arduino.cc/hardware/mkr-mem-shield 	|  	| https://www.distrelec.ch/fr/shield-arduino-mkr-mem-arduino-asx00008/p/30117217?trackQuery=Arduino+MKR+MEM+Shield&pos=1&origPos=1&origPageSize=50&track=true 	|
| Electrically Isolated EZO™ Carrier Board 	|  	| Atlas Scientific 	| 1 	| CHF 28.95 	| CHF 28.95 	| https://atlas-scientific.com/orp 	|  	| https://sensorsandprobes.com 	|
| EZO ORP Circuit 	|  	| Atlas Scientific 	| 1 	| CHF 43.00 	| CHF 43.00 	| https://atlas-scientific.com/orp 	|  	| https://www.whiteboxes.ch/shop/ezo-orp/ 	|
| Mini Lab Grade ORP Probe 	|  	| Atlas Scientific 	| 1 	| CHF 62.00 	| CHF 62.00 	| https://atlas-scientific.com/orp 	|  	| https://www.whiteboxes.ch/shop/mini-orp-probe/ 	|
| RTC DS3231 	|  	| Adafruit 	| 1 	| CHF 25.00 	| CHF 25.00 	| https://www.adafruit.com/product/3013 	|  	| https://www.distrelec.ch/fr/rtc-de-precision-ds3231-adafruit-3013/p/30091211?trackQuery=DS3231&pos=1&origPos=1&origPageSize=50&track=true 	|
| Antenna 	| not the exact on the prototype 	| Arduino 	| 1 	| CHF 5.70 	| CHF 5.70 	|  	| X000016 	| https://www.distrelec.ch/fr/antenne-gsm-2g-3g-4g-dbi-fl-support-adhesif-arduino-x000016/p/30101972?&cq_src=google_ads&cq_cmp=18318188127&cq_con=&cq_term=&cq_med=pla&cq_plac=&cq_net=x&cq_pos=&cq_plt=gp&gclid=Cj0KCQjwnrmlBhDHARIsADJ5b_mY_m0nUqeMlJN3qyH_5C25cz63-XVXhBc8LV99wYNgYx2_ZzH7XpMaAo-4EALw_wcB&gclsrc=aw.ds 	|
| Orange LED 	|  	| RND Components 	| 1 	| CHF 1.75 	| CHF 1.75 	|  	| RND 210-00674 	| https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-orange-dc-2v-rnd-components-rnd-210-00674/p/30176538?trackQuery=LED+Indicator%2c+PCB+Pins%2c+Fixed%2c+Orange%2c+DC%2c+2V%2c+RND+Components&pos=13&origPos=13&origPageSize=50&track=true 	|
| Yellow LED 	|  	| RND Components 	| 1 	| CHF 1.95 	| CHF 1.95 	|  	| RND 210-00675 	| https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-yellow-dc-2v-rnd-components-rnd-210-00675/p/30176539?trackQuery=210-00675&pos=1&origPos=1&origPageSize=50&track=true 	|
| Green LED 	|  	| RND Components 	| 1 	| CHF 1.95 	| CHF 1.95 	|  	| RND 210-00673 	| https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-green-dc-2v-rnd-components-rnd-210-00673/p/30176537?queryFromSuggest=true 	|
| Red LED 	|  	| RND Components 	| 1 	| CHF 1.95 	| CHF 1.95 	|  	| RND 210-00672 	| https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-red-dc-2v-rnd-components-rnd-210-00672/p/30176536?queryFromSuggest=true 	|
| Blue LED 	|  	| RND Components 	| 1 	| CHF 1.98 	| CHF 1.98 	|  	| RND 210-00676 	| https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-blue-dc-2v-rnd-components-rnd-210-00676/p/30176540?trackQuery=LED+Indicator%2c+PCB+Pins%2c+Fixed%2c+Blue%2c+DC%2c+2V%2c+RND+Components&pos=6&origPos=6&origPageSize=50&track=true 	|
| Toggle switch 	| battery kill switch 	| Bulgin 	| 1 	| CHF 6.02 	| CHF 6.02 	|  	| C3910BAAAA 	| https://www.distrelec.ch/en/toggle-switch-on-on-20-1co-ip67-bulgin-limited-c3910baaaa/p/30013174?track=true&no-cache=true&marketingPopup=false 	|
| Micro USB B to 5 pin header 	| to be built manually 	| NA 	| 1 	| CHF 2.00 	| CHF 2.00 	|  	|  	|  	|
| Mini USB B Bucaneer  	|  	| Bulgin 	| 1 	| 11.97 	| CHF 11.97 	| https://www.distrelec.ch/Web/Downloads/_t/ds/PX0443_eng_tds.pdf 	| PX0446 	| https://www.distrelec.ch/en/usb-connector-ports-mini-usb-bulgin-limited-px0446/p/30092073?track=true&no-cache=true&marketingPopup=false 	|
| USB Cable gland sealing cap  	|  	| Bulgin 	| 1 	| 4.12 	| CHF 4.12 	| https://www.distrelec.ch/Web/Downloads/_t/ds/PX0443_eng_tds.pdf 	| PX0484 	| https://www.distrelec.ch/en/sealing-cap-polyamide-size-bulgin-limited-px0484/p/30092076?track=true&no-cache=true&marketingPopup=false 	|
| USB Connector USB-A Socket - USB Micro-B Plug 	|  	| Bulgin 	| 1 	| CHF 17.86 	| CHF 17.86 	| https://www.distrelec.ch/Web/Downloads/_t/ds/PX0443_eng_tds.pdf 	| PX0441/2M00 	| https://www.distrelec.ch/en/usb-connector-usb-socket-usb-micro-plug-2m-usb-black-bulgin-limited-px0441-2m00/p/30092069?track=true&no-cache=true&marketingPopup=false 	|
| Cable socket 	| Alternative power supply 	| Hirschmann 	| 1 	| CHF 8.24 	| CHF 8.24 	|  	| ELKA 4012 PG7 BLACK 	| https://www.distrelec.ch/en/cable-box-m12-pin-socket-poles-straight-screw-terminal-cable-mount-hirschmann-elka-4012-pg7-black/p/14436720?track=true&no-cache=true&marketingPopup=false 	|
| Circular Sensor Connector Plug 4 Poles 	| Alternative power supply 	| RND Connect 	| 1 	| CHF 8.25 	| CHF 8.25 	|  	| RND 205-01173 	| https://www.distrelec.ch/en/circular-sensor-connector-plug-poles-straight-solder-panel-mount-rnd-connect-rnd-205-01173/p/30164698?track=true&no-cache=true&marketingPopup=false 	|
| resistors 3.7 kOhms 	| pullup for I2C Sensor 	| RND Connect 	| 2 	|  	| CHF 0.00 	|  	|  	|  	|
| resistors 10 kOhms 	| pull-up for button press 	| RND Connect 	| 1 	| CHF 0.14 	| CHF 0.14 	|  	| MBB02070C1002FCT00 	| https://www.distrelec.ch/fr/resistance-600mw-10kohm-vishay-mbb02070c1002fct00/p/16059239?trackQuery=10kOhms&pos=21&origPos=21&origPageSize=50&track=true 	|
| resistors 390 Ohms 	| control current flow for LEDs 	| RND Components 	| 5 	| CHF 0.04 	| CHF 0.19 	|  	|  	| https://www.distrelec.ch/fr/resistance-fixe-couches-de-carbone-500mw-390ohm-rnd-components-rnd-155rd12jn391t52/p/30156013?trackQuery=390+Ohms+resistors&pos=2&origPos=2&origPageSize=50&track=true 	|
| capacitor 0.1 microF 	| Switch debouncer 	|  	| 1 	|  	| CHF 0.00 	|  	|  	|  	|
| velcro straps 	|  	| Velcro 	| 1 	| 7.35 	| CHF 7.35 	|  	|  	| https://www.distrelec.ch/en/velcro-fastener-200-13mm-fabric-polypropylene-black-velcro-vel-ow64301/p/30110092?trackQuery=velcro&pos=2&origPos=5&origPageSize=50&track=true 	|
| Enclosure 150x200x100mm 	|  	|  	| 1 	| 14.4 	| CHF 14.40 	|  	| RL6685BK 	| https://www.distrelec.ch/en/plastic-enclosure-rl-150x200x100mm-black-abs-ip54-hammond-rl6685bk/p/30122546?queryFromSuggest=true 	|
| Plastic platform to cut 	|  	|  	| 1 	| 1 	| CHF 1.00 	|  	|  	|  	|
| TOTAL 	|  	|  	|  	|  	| CHF 350.00 	|  	|  	|  	|

### final_project
This folder contains the source code for the project.
To use the system, clone the repo, assemble the hardware as described in XXX and upload the platformIO project to the board



## development environment
This project was developped using VSCode and the platformIO extension.

## Known bugs

## Master thesis report

