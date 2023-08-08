
# General description of project

<!-- badges: start -->

[![DOI](https://zenodo.org/badge/630480035.svg)](https://zenodo.org/badge/latestdoi/630480035)
<!-- badges: end -->

This project is a remote monitoring system to be used on the [Blue
Diversion Autarky
Project](https://www.eawag.ch/en/research/humanwelfare/wastewater/projects/autarky/),
developed at EAWAG<sup>\[1\]</sup>.

The waterfall, currently only a prototype, requires that an operator
come onsite regularly, to monitor the microbial water quality in the
clean water tank. This is time-consuming and limits the possibility of
performing high-quality field tests. This project aims to tackle this
challenge by providing a real-time, online microbial water quality
monitoring. This ensures that the water is of sufficiently high
microbial water quality such that the waterwall may be used at no risk
for the user.

A machine model was developed by XXX link, and implemented here. The
only parameter is the ORP reading. Decision making of water being safe
or unsafe is based on a cut-off value found from the trained model. For
example a XXX and LRT XXX, the ORP value was found to be at. For varying
values,

# Component selection

The following components were used for the prototype. For ease of
reproducibility, links to the used suppliers are provided. The final
cost of the hardware amounts to just under CHF 380.

| Item description                              | Comment                        | Brand            | Quantity | Unitary cost | Total cost | manufacturer link                                                | Manufacturer code   | Supplier link                                                                                                                                                                                                                                                                                                            |
|-----------------------------------------------|--------------------------------|------------------|----------|--------------|------------|------------------------------------------------------------------|---------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| MKR NB 1500                                   |                                | Arduino          | 1        | CHF 71.39    | CHF 71.39  | https://store.arduino.cc/products/arduino-mkr-nb-1500            |                     | https://www.distrelec.ch/fr/arduino-mkr-nb-1500-arduino-abx00019/p/30117099?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                                                |
| MKR MEM Shield                                |                                | Arduino          | 1        | CHF 22.84    | CHF 22.84  | https://docs.arduino.cc/hardware/mkr-mem-shield                  |                     | https://www.distrelec.ch/fr/shield-arduino-mkr-mem-arduino-asx00008/p/30117217?trackQuery=Arduino+MKR+MEM+Shield&pos=1&origPos=1&origPageSize=50&track=true                                                                                                                                                              |
| Electrically Isolated EZO™ Carrier Board      |                                | Atlas Scientific | 1        | CHF 28.95    | CHF 28.95  | https://atlas-scientific.com/orp                                 |                     | https://sensorsandprobes.com                                                                                                                                                                                                                                                                                             |
| EZO ORP Circuit                               |                                | Atlas Scientific | 1        | CHF 43.00    | CHF 43.00  | https://atlas-scientific.com/orp                                 |                     | https://www.whiteboxes.ch/shop/ezo-orp/                                                                                                                                                                                                                                                                                  |
| Mini Lab Grade ORP Probe                      |                                | Atlas Scientific | 1        | CHF 62.00    | CHF 62.00  | https://atlas-scientific.com/orp                                 |                     | https://www.whiteboxes.ch/shop/mini-orp-probe/                                                                                                                                                                                                                                                                           |
| RTC DS3231                                    |                                | Adafruit         | 1        | CHF 25.00    | CHF 25.00  | https://www.adafruit.com/product/3013                            |                     | https://www.distrelec.ch/fr/rtc-de-precision-ds3231-adafruit-3013/p/30091211?trackQuery=DS3231&pos=1&origPos=1&origPageSize=50&track=true                                                                                                                                                                                |
| Antenna                                       | not the exact on the prototype | Arduino          | 1        | CHF 5.70     | CHF 5.70   |                                                                  | X000016             | https://www.distrelec.ch/fr/antenne-gsm-2g-3g-4g-dbi-fl-support-adhesif-arduino-x000016/p/30101972?&cq_src=google_ads&cq_cmp=18318188127&cq_con=&cq_term=&cq_med=pla&cq_plac=&cq_net=x&cq_pos=&cq_plt=gp&gclid=Cj0KCQjwnrmlBhDHARIsADJ5b_mY_m0nUqeMlJN3qyH_5C25cz63-XVXhBc8LV99wYNgYx2_ZzH7XpMaAo-4EALw_wcB&gclsrc=aw.ds |
| Orange LED                                    |                                | RND Components   | 1        | CHF 1.75     | CHF 1.75   |                                                                  | RND 210-00674       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-orange-dc-2v-rnd-components-rnd-210-00674/p/30176538?trackQuery=LED+Indicator%2c+PCB+Pins%2c+Fixed%2c+Orange%2c+DC%2c+2V%2c+RND+Components&pos=13&origPos=13&origPageSize=50&track=true                                                                         |
| Yellow LED                                    |                                | RND Components   | 1        | CHF 1.95     | CHF 1.95   |                                                                  | RND 210-00675       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-yellow-dc-2v-rnd-components-rnd-210-00675/p/30176539?trackQuery=210-00675&pos=1&origPos=1&origPageSize=50&track=true                                                                                                                                            |
| Green LED                                     |                                | RND Components   | 1        | CHF 1.95     | CHF 1.95   |                                                                  | RND 210-00673       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-green-dc-2v-rnd-components-rnd-210-00673/p/30176537?queryFromSuggest=true                                                                                                                                                                                       |
| Red LED                                       |                                | RND Components   | 1        | CHF 1.95     | CHF 1.95   |                                                                  | RND 210-00672       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-red-dc-2v-rnd-components-rnd-210-00672/p/30176536?queryFromSuggest=true                                                                                                                                                                                         |
| Blue LED                                      |                                | RND Components   | 1        | CHF 1.98     | CHF 1.98   |                                                                  | RND 210-00676       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-blue-dc-2v-rnd-components-rnd-210-00676/p/30176540?trackQuery=LED+Indicator%2c+PCB+Pins%2c+Fixed%2c+Blue%2c+DC%2c+2V%2c+RND+Components&pos=6&origPos=6&origPageSize=50&track=true                                                                               |
| Toggle switch                                 | battery kill switch            | Bulgin           | 1        | CHF 6.02     | CHF 6.02   |                                                                  | C3910BAAAA          | https://www.distrelec.ch/en/toggle-switch-on-on-20-1co-ip67-bulgin-limited-c3910baaaa/p/30013174?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                           |
| Micro USB B to 5 pin header                   | to be built manually           | NA               | 1        | CHF 2.00     | CHF 2.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| Mini USB B Bucaneer                           |                                | Bulgin           | 1        | 11.97        | CHF 11.97  | https://www.distrelec.ch/Web/Downloads/\_t/ds/PX0443_eng_tds.pdf | PX0446              | https://www.distrelec.ch/en/usb-connector-ports-mini-usb-bulgin-limited-px0446/p/30092073?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                                  |
| USB Cable gland sealing cap                   |                                | Bulgin           | 1        | 4.12         | CHF 4.12   | https://www.distrelec.ch/Web/Downloads/\_t/ds/PX0443_eng_tds.pdf | PX0484              | https://www.distrelec.ch/en/sealing-cap-polyamide-size-bulgin-limited-px0484/p/30092076?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                                    |
| USB Connector USB-A Socket - USB Micro-B Plug |                                | Bulgin           | 1        | CHF 17.86    | CHF 17.86  | https://www.distrelec.ch/Web/Downloads/\_t/ds/PX0443_eng_tds.pdf | PX0441/2M00         | https://www.distrelec.ch/en/usb-connector-usb-socket-usb-micro-plug-2m-usb-black-bulgin-limited-px0441-2m00/p/30092069?track=true&no-cache=true&marketingPopup=false                                                                                                                                                     |
| Cable socket                                  | Alternative power supply       | Hirschmann       | 1        | CHF 8.24     | CHF 8.24   |                                                                  | ELKA 4012 PG7 BLACK | https://www.distrelec.ch/en/cable-box-m12-pin-socket-poles-straight-screw-terminal-cable-mount-hirschmann-elka-4012-pg7-black/p/14436720?track=true&no-cache=true&marketingPopup=false                                                                                                                                   |
| Circular Sensor Connector Plug 4 Poles        | Alternative power supply       | RND Connect      | 1        | CHF 8.25     | CHF 8.25   |                                                                  | RND 205-01173       | https://www.distrelec.ch/en/circular-sensor-connector-plug-poles-straight-solder-panel-mount-rnd-connect-rnd-205-01173/p/30164698?track=true&no-cache=true&marketingPopup=false                                                                                                                                          |
| resistors 3.7 kOhms                           | pullup for I2C Sensor          | RND Connect      | 2        |              | CHF 0.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| resistors 10 kOhms                            | pull-up for button press       | RND Connect      | 1        | CHF 0.14     | CHF 0.14   |                                                                  | MBB02070C1002FCT00  | https://www.distrelec.ch/fr/resistance-600mw-10kohm-vishay-mbb02070c1002fct00/p/16059239?trackQuery=10kOhms&pos=21&origPos=21&origPageSize=50&track=true                                                                                                                                                                 |
| resistors 390 Ohms                            | control current flow for LEDs  | RND Components   | 5        | CHF 0.04     | CHF 0.19   |                                                                  |                     | https://www.distrelec.ch/fr/resistance-fixe-couches-de-carbone-500mw-390ohm-rnd-components-rnd-155rd12jn391t52/p/30156013?trackQuery=390+Ohms+resistors&pos=2&origPos=2&origPageSize=50&track=true                                                                                                                       |
| capacitor 0.1 microF                          | Switch debouncer               |                  | 1        |              | CHF 0.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| velcro straps                                 |                                | Velcro           | 1        | 7.35         | CHF 7.35   |                                                                  |                     | https://www.distrelec.ch/en/velcro-fastener-200-13mm-fabric-polypropylene-black-velcro-vel-ow64301/p/30110092?trackQuery=velcro&pos=2&origPos=5&origPageSize=50&track=true                                                                                                                                               |
| Enclosure 150x200x100mm                       |                                |                  | 1        | 14.4         | CHF 14.40  |                                                                  | RL6685BK            | https://www.distrelec.ch/en/plastic-enclosure-rl-150x200x100mm-black-abs-ip54-hammond-rl6685bk/p/30122546?queryFromSuggest=true                                                                                                                                                                                          |
| Plastic platform to cut                       |                                |                  | 1        | 1            | CHF 1.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| micro SD Card                                 |                                | Kingston         | 1        | 5.6          | CHF 5.60   |                                                                  | SDCS2/32GBSP        | https://www.distrelec.ch/en/memory-card-microsd-32gb-100mb-85mb-black-kingston-sdcs2-32gbsp/p/30201902?trackQuery=microSD+card&pos=18&origPos=18&origPageSize=50&track=true                                                                                                                                              |
| SIM card                                      | dependent on country           |                  | 1        | 2            | CHF 2.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| TOTAL                                         |                                |                  |          |              | CHF 357.60 |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |

# User guide

<!--- The project is divided in 3 folders 
- final project
- RTC calibration
- ML model --->

## final_project

This folder contains the source code for the project. To use the system,
clone the repo, assemble the hardware as described in XXX (report DOI, +
section link of circuit structure) and upload the platformIO project to
the board. Once uploaded, the device will automatically start.

## Configuration settings

If the device is used for the first time, make sure the device is
connected to a PC. This will help for debugging potential mis-settings.

The first automatic run will create a RMS_V1.CFG file, with default,
hard-coded settings. Once the device has run at least run, you can
safely disconnect all power supply, remove the SD card, and easily tune
the configuration settings.

Alternatively, if you can easily upload the RMS_V1.CFG file to the SD
card before the first run. RMS_V1.CFG can be found in XXX (folder
location on github)

### Configuration parameters

*logitThreshold* : (mv) any ORP reading above this value will translate
into a safe waterquality. This setting is found from running the code
found in XXX.

*uraPressDuration* : (ms) how long the user should press the button for
an alarm to be raised. In case an SMS has already been sent less then
*uraSMSInterval*, only a water monitoring will occur, but no SMS will be
spamming the user.

*hbTargetHour* : (hour, 24) At what time should a heartbeat be sent to
the operator.

*hbIntervalHour* : (hour) At what frequency should the heartbeat be
sent, starting from the *hbTargetHour*. Ensure this value is less or
equal to 24. For example, consider *hbTargetHour* = 9 and
*hbIntervalHour* = 6. A heartbeat will be sent to the operator at 9:00,
15:00, 21:00, 03:00. Please note that the exact time at which a
heartbeat is sent is the closest time to the aforementioned full hour,
plus a maximum of *xwqSleepPeriod*. Indeed the heartbeat SMS is sent
after the device woke up for a watermonitoring function.

*swqSleepPeriod* : (ms) how much time should elapse between two water
monitoring events, while the water quality is safe.

*uwqSleepPeriod* : (ms) how much time should elapse between two water
monitoring events, while the water quality is unsafe.

*fwqSleepPeriod* : (ms) how much time should elapse between two water
monitoring events, while there has been faulty reading.

*uraSMSInterval* : (ms) how much time should elapse, before a new SMS
can be sent to the operator, informing them that the user raised an
alarm. This is to prevent spamming the operator with notifications in
case the user uses too often the button press

*wmSMSInterval* : (ms) how much time should elapse, before a new SMS can
be sent to the operator, updating them on the waterquality status, after
an unsafe or faulty reading was first detected. This is to prevent
spamming the operator with notification in the case of unsafe water
quality or faulty reading.

*sendSMS* : (boolean) sets whether the SMS should be sent to the
operator or not.

*remoteNumberLength* : (integer) tells how long the operator phone
number is.

*remoteNumber* : (string) holds the operator phone number.

### LED error signal description

In case the RMS is started without being connected to a PC, the LEDs
provide some feedback in case some problems occured during
initialisation. There is always one or two LEDs on, to indicate what
region is affected, while another LED will blink to differentiate
between the possible causes for the issue.

| Region  | Exact problem                                        | On LED         | Blinking LED |
|---------|------------------------------------------------------|----------------|--------------|
| Battery | Battery not conected                                 | Orange         | Red          |
| Battery | Low battery energy level                             | Orange         | Yellow       |
| SD Card | Failed to initialise                                 | Green          | Red          |
| SD Card | Failed to create a valid filename                    | Green          | Yellow       |
| SMS     | Inappropriate phone number                           | Yellow         | Red          |
| RTC     | Failed to initialise internal RTC using external RTC | Green + orange | Red          |
| RTC     | Failed to set-up heartbeat                           | Green + orange | Yellow       |
| Unknow  | Unknown error                                        | Red            | Blue         |

## Development environment

This project was developped using VSCode and the platformIO extension.

# Impreovements

## codebase

### wake up from deepsleep, from low battery

If the RMS is off the grid power, and has been running on the battery
for some time, the battery voltage will drop. We have set a
batteryEmptyVoltage at 3.5V. If the voltage drops below 3.5V + 5%3.5V,
the device enters a “criticalEL” state, and must go to deepSleep, so as
to consume as little as power possibe, while informing the user that the
device is not running.

However, if the grid power eventually turns back on, we want the device
to be able to power back up. For this to occur, we have implemented some
measures to ensure the device wakes back up. Just before going to sleep,
we attach an interrupt from the PMIC to the arduino. That way, if the
PMIC detects a stable power supply, it may interrupt the Arduino, thus
waking it up.

Because of the amount of time it takes to deplete the battery, thorough
test could not be performed

What remains to be tested, is whether the device can effectively carry
on its usual business after the device has woken up from a deep sleep.

Also, we want to know what happens when the battery voltage gets too
low. Will the circuit disconnect?

### Storage optimisation

Currently, a lot of variables are being stored inside the rms class.
Some of which are updated during the setup phase with values from the
configuration struct. These values where either read from the SD card,
or set from the default settings. This is the case for - SWQ sleep
duration - UWQ sleep duration - FWQ sleep duration

# Appendix

## Master thesis report

<div id="refs" class="references csl-bib-body hanging-indent"
line-spacing="2">

<div id="ref-RN32" class="csl-entry">

1\. Victorin, K., Hellström, K. G., & Rylander, R. (1972). Redox
potential measurements for determining the disinfecting power of
chlorinated water \[Journal Article\]. *Journal of Hygiene*, *70*(2),
313–323. <https://doi.org/10.1017/S0022172400022361>

</div>

</div>
