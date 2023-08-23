
- [General description of project](#general-description-of-project)
- [User guide](#user-guide)
  - [Project structure](#project-structure)
    - [final_project](#final_project)
    - [misc](#misc)
  - [QuickStart guide](#quickstart-guide)
    - [Configuration settings](#configuration-settings)
    - [LED error signal description](#led-error-signal-description)
    - [Development environment](#development-environment)
- [Component selection](#component-selection)
- [Improvements](#improvements)
  - [codebase](#codebase)
    - [Wake up from deepsleep, from low
      battery](#wake-up-from-deepsleep-from-low-battery)
    - [Storage optimisation](#storage-optimisation)
    - [Adding new sensors to the predicting
      algorithm](#adding-new-sensors-to-the-predicting-algorithm)
- [References](#references)

# General description of project

<!-- badges: start -->

[![DOI](https://zenodo.org/badge/630480035.svg)](https://zenodo.org/badge/latestdoi/630480035)
<!-- badges: end -->

This project is a remote monitoring system developed for the Water Wall
of the [Blue Diversion Autarky
Project](https://www.eawag.ch/en/research/humanwelfare/wastewater/projects/autarky/),
developed at EAWAG.

Insufficiently treated reused water can transmit
diseases<sup>\[1\]</sup>. It is necessary to ensure that the treated
water is microbially safe at all times<sup>\[2\]</sup>. To monitor the
microbial water quality in the clean water tank, to ensure sufficently
high microbial water quality, currently, the operator must perform
biweekly, onsite water sampling<sup>\[3\]</sup>. This is time-consuming
and limits the possibility of performing high-quality field tests.

This RMS (Remote Monitoring System) project aims to tackle this
challenge by providing a real-time, online microbial water quality
prediction system. This RMS communicates problems to operators (in order
to fix the problem) as well as to users (to make sure they are made
aware in case the water is unsafe).

This RMS is an implementation of the machine learning model developed at
EAWAG<sup>\[4\]</sup>. Currently, the only input parameter is the ORP
reading. Decision making of water being safe or unsafe is based on a
cut-off value found from the trained model. In practice, this value may
be set by the operator, according to their needs.

# User guide

<!--- The project is divided in 3 folders 
- final project
- RTC calibration
- ML model --->

## Project structure

This proejct is divided in 2 folders:

- final_project
- misc

### final_project

This folder contains the source code for the project.

### misc

This folder contains aditional files useful for the project.

## QuickStart guide

This section details the step to take to start using the device.

To use the RMS:

1.  clone this repo. Youn may alternatively download the .zip file from
    the DOI link provided at the top.
2.  in the terminal, navigate your way to the final_project folder (‘cd
    yourPath/RMS/final_project’)
3.  in the terminal, type ‘code .’. (You will need to have installed
    VSCode beforehand)
4.  assemble the hardware as shown in figure [Figure 1](#fig-assembly)
5.  configure settings as described in section
    [Section 2.2.1.1](#sec-configDescription)
6.  build the project in VSCode
7.  upload the platformIO project to the board. Once uploaded, the
    device will automatically start.

<img src="./readme_misc/images/230809_Annotated_V1_2.png"
title="Title: assembly diagram" id="fig-assembly" style="width:50.0%"
data-fig-alt="RMS assembly schematic"
alt="Figure 1: RMS assembly diagram" />

<figcaption>
Figure 1: RMS assembly diagram.
</figcaption>

### Configuration settings

If the device is used for the first time, make sure the device is
connected to a PC. This will help debugging potential wrong settings.

The first automatic run will create a RMS_V1.CFG file, with default,
hard-coded settings. Once the device has run at least once, you can
safely disconnect all power supply, remove the SD card, and easily tune
the configuration settings.

Alternatively, you can easily upload the RMS_V1.CFG file to the SD card
before the first run. RMS_V1.CFG can be found in the “misc” folder.

#### Configuration settings description

The parameters that may be tuned during the configuarion are the
following.

*logitThreshold* : (mv) any ORP reading above this value will translate
into a safe waterquality. This setting is found from running the code
found in<sup>\[4\]</sup>.

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
plus a maximum of *s/u/fwqSleepPeriod*. Indeed the heartbeat SMS is sent
after the device woke up for a watermonitoring function.

*swqSleepPeriod* : (s) how much time should elapse between two water
monitoring events, while the water quality is safe.

*uwqSleepPeriod* : (s) how much time should elapse between two water
monitoring events, while the water quality is unsafe.

*fwqSleepPeriod* : (s) how much time should elapse between two water
monitoring events, while there has been faulty reading.

*uraSMSInterval* : (s) how much time should elapse, before a new SMS can
be sent to the operator, informing them that the user raised an alarm.
This is to prevent spamming the operator with notifications in case the
user uses too often the button press

*wmSMSInterval* : (s) how much time should elapse, before a new SMS can
be sent to the operator, updating them on the waterquality status, after
an unsafe or faulty reading was first detected. This is to prevent
spamming the operator with notification in the case of unsafe water
quality or faulty reading.

*sendSMS* : (boolean) sets whether the SMS should be sent to the
operator or not.

*remoteNumberLength* : (integer) holds the length of the operator phone
number is.

*remoteNumber* : (string) holds the operator phone number.

### LED error signal description

When the RMS is started, in case of problems during initialisations,
LEDs provide some feedback.

One or two LEDs are on, to indicate what region is affected, while
another LED will blink to differentiate between the possible causes for
the issue. The table below provides a guide to debug potential problems.

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

### Development environment

This project was developped using VSCode and the platformIO extension.

# Component selection

The following components were used for the prototype. For ease of
reproducibility, links to the used suppliers are provided. The final
cost of the hardware amounts to just under CHF 380.

| Item description                              | Comment                                  | Brand            | Quantity | Unitary cost | Total cost | manufacturer link                                                | Manufacturer code   | Supplier link                                                                                                                                                                                                                                                                                                            |
|-----------------------------------------------|------------------------------------------|------------------|----------|--------------|------------|------------------------------------------------------------------|---------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| MKR NB 1500                                   |                                          | Arduino          | 1        | CHF 71.39    | CHF 71.39  | https://store.arduino.cc/products/arduino-mkr-nb-1500            |                     | https://www.distrelec.ch/fr/arduino-mkr-nb-1500-arduino-abx00019/p/30117099?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                                                |
| MKR MEM Shield                                |                                          | Arduino          | 1        | CHF 22.84    | CHF 22.84  | https://docs.arduino.cc/hardware/mkr-mem-shield                  |                     | https://www.distrelec.ch/fr/shield-arduino-mkr-mem-arduino-asx00008/p/30117217?trackQuery=Arduino+MKR+MEM+Shield&pos=1&origPos=1&origPageSize=50&track=true                                                                                                                                                              |
| Electrically Isolated EZO™ Carrier Board      |                                          | Atlas Scientific | 1        | CHF 28.95    | CHF 28.95  | https://atlas-scientific.com/orp                                 |                     | https://sensorsandprobes.com                                                                                                                                                                                                                                                                                             |
| EZO ORP Circuit                               |                                          | Atlas Scientific | 1        | CHF 43.00    | CHF 43.00  | https://atlas-scientific.com/orp                                 |                     | https://www.whiteboxes.ch/shop/ezo-orp/                                                                                                                                                                                                                                                                                  |
| Mini Lab Grade ORP Probe                      |                                          | Atlas Scientific | 1        | CHF 62.00    | CHF 62.00  | https://atlas-scientific.com/orp                                 |                     | https://www.whiteboxes.ch/shop/mini-orp-probe/                                                                                                                                                                                                                                                                           |
| RTC DS3231                                    |                                          | Adafruit         | 1        | CHF 25.00    | CHF 25.00  | https://www.adafruit.com/product/3013                            |                     | https://www.distrelec.ch/fr/rtc-de-precision-ds3231-adafruit-3013/p/30091211?trackQuery=DS3231&pos=1&origPos=1&origPageSize=50&track=true                                                                                                                                                                                |
| Antenna                                       | not this exact one used in the prototype | Arduino          | 1        | CHF 5.70     | CHF 5.70   |                                                                  | X000016             | https://www.distrelec.ch/fr/antenne-gsm-2g-3g-4g-dbi-fl-support-adhesif-arduino-x000016/p/30101972?&cq_src=google_ads&cq_cmp=18318188127&cq_con=&cq_term=&cq_med=pla&cq_plac=&cq_net=x&cq_pos=&cq_plt=gp&gclid=Cj0KCQjwnrmlBhDHARIsADJ5b_mY_m0nUqeMlJN3qyH_5C25cz63-XVXhBc8LV99wYNgYx2_ZzH7XpMaAo-4EALw_wcB&gclsrc=aw.ds |
| Orange LED                                    |                                          | RND Components   | 1        | CHF 1.75     | CHF 1.75   |                                                                  | RND 210-00674       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-orange-dc-2v-rnd-components-rnd-210-00674/p/30176538?trackQuery=LED+Indicator%2c+PCB+Pins%2c+Fixed%2c+Orange%2c+DC%2c+2V%2c+RND+Components&pos=13&origPos=13&origPageSize=50&track=true                                                                         |
| Yellow LED                                    |                                          | RND Components   | 1        | CHF 1.95     | CHF 1.95   |                                                                  | RND 210-00675       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-yellow-dc-2v-rnd-components-rnd-210-00675/p/30176539?trackQuery=210-00675&pos=1&origPos=1&origPageSize=50&track=true                                                                                                                                            |
| Green LED                                     |                                          | RND Components   | 1        | CHF 1.95     | CHF 1.95   |                                                                  | RND 210-00673       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-green-dc-2v-rnd-components-rnd-210-00673/p/30176537?queryFromSuggest=true                                                                                                                                                                                       |
| Red LED                                       |                                          | RND Components   | 1        | CHF 1.95     | CHF 1.95   |                                                                  | RND 210-00672       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-red-dc-2v-rnd-components-rnd-210-00672/p/30176536?queryFromSuggest=true                                                                                                                                                                                         |
| Blue LED                                      |                                          | RND Components   | 1        | CHF 1.98     | CHF 1.98   |                                                                  | RND 210-00676       | https://www.distrelec.ch/en/led-indicator-pcb-pins-fixed-blue-dc-2v-rnd-components-rnd-210-00676/p/30176540?trackQuery=LED+Indicator%2c+PCB+Pins%2c+Fixed%2c+Blue%2c+DC%2c+2V%2c+RND+Components&pos=6&origPos=6&origPageSize=50&track=true                                                                               |
| Toggle switch                                 | battery kill switch                      | Bulgin           | 1        | CHF 6.02     | CHF 6.02   |                                                                  | C3910BAAAA          | https://www.distrelec.ch/en/toggle-switch-on-on-20-1co-ip67-bulgin-limited-c3910baaaa/p/30013174?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                           |
| Micro USB B to 5 pin header                   | to be built manually                     | NA               | 1        | CHF 2.00     | CHF 2.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| Mini USB B Bucaneer                           |                                          | Bulgin           | 1        | 11.97        | CHF 11.97  | https://www.distrelec.ch/Web/Downloads/\_t/ds/PX0443_eng_tds.pdf | PX0446              | https://www.distrelec.ch/en/usb-connector-ports-mini-usb-bulgin-limited-px0446/p/30092073?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                                  |
| USB Cable gland sealing cap                   |                                          | Bulgin           | 1        | 4.12         | CHF 4.12   | https://www.distrelec.ch/Web/Downloads/\_t/ds/PX0443_eng_tds.pdf | PX0484              | https://www.distrelec.ch/en/sealing-cap-polyamide-size-bulgin-limited-px0484/p/30092076?track=true&no-cache=true&marketingPopup=false                                                                                                                                                                                    |
| USB Connector USB-A Socket - USB Micro-B Plug |                                          | Bulgin           | 1        | CHF 17.86    | CHF 17.86  | https://www.distrelec.ch/Web/Downloads/\_t/ds/PX0443_eng_tds.pdf | PX0441/2M00         | https://www.distrelec.ch/en/usb-connector-usb-socket-usb-micro-plug-2m-usb-black-bulgin-limited-px0441-2m00/p/30092069?track=true&no-cache=true&marketingPopup=false                                                                                                                                                     |
| Cable socket                                  | Alternative power supply                 | Hirschmann       | 1        | CHF 8.24     | CHF 8.24   |                                                                  | ELKA 4012 PG7 BLACK | https://www.distrelec.ch/en/cable-box-m12-pin-socket-poles-straight-screw-terminal-cable-mount-hirschmann-elka-4012-pg7-black/p/14436720?track=true&no-cache=true&marketingPopup=false                                                                                                                                   |
| Circular Sensor Connector Plug 4 Poles        | Alternative power supply                 | RND Connect      | 1        | CHF 8.25     | CHF 8.25   |                                                                  | RND 205-01173       | https://www.distrelec.ch/en/circular-sensor-connector-plug-poles-straight-solder-panel-mount-rnd-connect-rnd-205-01173/p/30164698?track=true&no-cache=true&marketingPopup=false                                                                                                                                          |
| resistors 3.7 kOhms                           | pullup for I2C Sensor                    | RND Connect      | 2        |              | CHF 0.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| resistors 10 kOhms                            | pull-up for button press                 | RND Connect      | 1        | CHF 0.14     | CHF 0.14   |                                                                  | MBB02070C1002FCT00  | https://www.distrelec.ch/fr/resistance-600mw-10kohm-vishay-mbb02070c1002fct00/p/16059239?trackQuery=10kOhms&pos=21&origPos=21&origPageSize=50&track=true                                                                                                                                                                 |
| resistors 390 Ohms                            | control current flow for LEDs            | RND Components   | 5        | CHF 0.04     | CHF 0.19   |                                                                  |                     | https://www.distrelec.ch/fr/resistance-fixe-couches-de-carbone-500mw-390ohm-rnd-components-rnd-155rd12jn391t52/p/30156013?trackQuery=390+Ohms+resistors&pos=2&origPos=2&origPageSize=50&track=true                                                                                                                       |
| capacitor 0.1 microF                          | Switch debouncer                         |                  | 1        |              | CHF 0.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| velcro straps                                 |                                          | Velcro           | 1        | 7.35         | CHF 7.35   |                                                                  |                     | https://www.distrelec.ch/en/velcro-fastener-200-13mm-fabric-polypropylene-black-velcro-vel-ow64301/p/30110092?trackQuery=velcro&pos=2&origPos=5&origPageSize=50&track=true                                                                                                                                               |
| Enclosure 150x200x100mm                       |                                          |                  | 1        | 14.4         | CHF 14.40  |                                                                  | RL6685BK            | https://www.distrelec.ch/en/plastic-enclosure-rl-150x200x100mm-black-abs-ip54-hammond-rl6685bk/p/30122546?queryFromSuggest=true                                                                                                                                                                                          |
| Plastic platform to cut                       |                                          |                  | 1        | 1            | CHF 1.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| micro SD Card                                 |                                          | Kingston         | 1        | 5.6          | CHF 5.60   |                                                                  | SDCS2/32GBSP        | https://www.distrelec.ch/en/memory-card-microsd-32gb-100mb-85mb-black-kingston-sdcs2-32gbsp/p/30201902?trackQuery=microSD+card&pos=18&origPos=18&origPageSize=50&track=true                                                                                                                                              |
| SIM card                                      | dependent on country                     |                  | 1        | 2            | CHF 2.00   |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |
| TOTAL                                         |                                          |                  |          |              | CHF 357.60 |                                                                  |                     |                                                                                                                                                                                                                                                                                                                          |

# Improvements

In case this project is carried forward, or reused, below are some
directions to take, to try and improve the project further.

## codebase

### Wake up from deepsleep, from low battery

If the RMS is off the grid power, and has been running on the battery
for some time, the battery voltage will drop. We have set a
<span style="font-family: 'Courier New', monospace;">batteryEmptyVoltage</span>
at 3.5V. If the voltage drops below 3.5V + 0.05x3.5V, the device enters
a <span style="font-family: 'Fira Code', monospace;">criticalEL</span>
state, and must go to deepSleep, so as to consume as little as power
possible, while informing the user that the device is not running.

However, if the grid power eventually turns back on, we want the device
to be able to power back up. For this to occur, just before going to
sleep, an interrupt from the PMIC is attached to the arduino. That way,
if the PMIC detects a stable power supply, it may interrupt the Arduino,
thus waking it up. This is another reason why we send the device to deep
sleep, and not completely off.

Because of the amount of time it takes to deplete the battery, thorough
test could not be performed.

What remains to be tested, is whether the device can effectively carry
on its usual routine after the device has woken up from a deep sleep.

Also, we want to know what happens when the battery voltage gets too
low. Will the circuit disconnect?

### Storage optimisation

Currently, a lot of variables are being stored inside the rms class.
Some of which are updated during the setup phase with values from the
configuration struct. These values where either read from the SD card,
or set from the default settings. This is the case for - SWQ sleep
duration - UWQ sleep duration - FWQ sleep duration

The code may be modified so that it uses the values in the
ConfigurationStruct instead of that from the
<span style="font-family: 'Courier New', monospace;">rmsClass</span>
object. This would allow to make the
<span style="font-family: 'Courier New', monospace;">rmsClass</span>
object lighter and easier to read.

### Adding new sensors to the predicting algorithm

Nonetheless, if one wants to include more sensors, the following steps
should be included to the function `FSM_implementMLDecision`

- Using the code, accessible from the paper<sup>\[4\]</sup>
  - Extract the standardization parameters (mean + SD) from the training
    dataset
  - Extract the PCA components from the training dataset
  - Extract the β and α<sub>0</sub>, …, α<sub>n</sub> parameters from
    the model
- Extract the most conservative threshold probability, based on target
  values set (LRTMS2, or LogICC)
- Standardize the input parameters, using the input parameter from the
  trained model
- PCA the input parameters, using the input parameter from the trained
  model
- Pass the transformed input parameters, into the probability function  

  ![p=\frac{e^{\beta+\alpha\_{0}+...+\alpha\_{n}}}{1+e^{\beta+\alpha\_{0}+...+\alpha\_{n}}}](https://latex.codecogs.com/svg.latex?p%3D%5Cfrac%7Be%5E%7B%5Cbeta%2B%5Calpha_%7B0%7D%2B...%2B%5Calpha_%7Bn%7D%7D%7D%7B1%2Be%5E%7B%5Cbeta%2B%5Calpha_%7B0%7D%2B...%2B%5Calpha_%7Bn%7D%7D%7D "p=\frac{e^{\beta+\alpha_{0}+...+\alpha_{n}}}{1+e^{\beta+\alpha_{0}+...+\alpha_{n}}}")
- Compute the probability
- Compare the probability with the probability limit found from training

# References

<div id="refs" class="references csl-bib-body hanging-indent"
line-spacing="2">

<div id="ref-RN66" class="csl-entry">

1\. Howe, K. J., Hand, D. W., Crittenden, J. C., Trussell, R. R., &
Tchobanoglous, G. (2012). *Principles of water treatment* \[Book\]. John
Wiley & Sons.

</div>

<div id="ref-RN68" class="csl-entry">

2\. Organization, W. H. (2016). *Quantitative microbial risk assessment:
Application for water safety management* \[Journal Article\].

</div>

<div id="ref-RN10" class="csl-entry">

3\. Sutherland, C., Reynaert, E., Dhlamini, S., Magwaza, F., Lienert,
J., Riechmann, M. E., Buthelezi, S., Khumalo, D., Morgenroth, E., &
Udert, K. M. (2021). Socio-technical analysis of a sanitation innovation
in a peri-urban household in durban, south africa \[Journal Article\].
*Science of the Total Environment*, *755*, 143284.

</div>

<div id="ref-RN30" class="csl-entry">

4\. Reynaert, E., Steiner, P., Yu, Q., D’Olif, L., Joller, N.,
Schneider, M. Y., & Morgenroth, E. (2023). Predicting microbial water
quality in on-site water reuse systems with online sensors \[Journal
Article\]. *Water Research*, *240*, 120075.
<https://www.sciencedirect.com/science/article/pii/S0043135423005110?via%3Dihub>

</div>

</div>
