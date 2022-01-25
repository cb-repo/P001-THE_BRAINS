# THE_BRAINS v3.0

<img src="/Switch.jpg" width="500">

THE_BRAINS is a dual DC-motor ESC: originally designed for antweight combat robots, but is suitable for any appliction requiring a lightweight motor driver.

Designed and built in [Christchurch, New Zealand ](https://www.google.co.nz/maps/place/Christchurch+New+Zealand) by Connor Benton, visit [CB-Technology](https://www.cb-tech.co.nz/) for details.

## FEATURES

- 2x Bi-Directional DC motor outputs.
- 2x Signal output channels for additional electroncis (BLDC ESC, servo, etc).
- Ultra-compact and lightweight.
- Under-voltage, over-current, over-temperature, and reverse polarity protections.
- Internal BEC to provide power to the radio reciever and/or additional ESC's.
- Status LED's to indicate run mode, warnings and calibration .
- Calibrate function allows ESC behaviour to be tailored to any given application.
- Includes a 100nF capacitor across each motor to filter noise and voltage spikes.

## SPECIFICATIONS

- **Dimensions:** 10x30x4 mm
- **Weight:** 2g *excluding wires*
- **Input Voltage - Rated:** 2S lipo (6-10.4V)
- **Input Voltage - Max:** 12.0V
- **Motor Output:** 2.1A cont (3A peak) per channel 
- **Motor Braking:** Yes, enabled by default. Can be diabled in configuration.
- **BEC:** 3.3V, 200mA. Designed to power the radio reciever and other downstream ESC's but no motors or servos.
- **Signal Input:** Servo PWM, PPM, iBUS, and SBUS
- **Signal Mixing:** Onboard signal mixing for TANK/ARCADE, servo stretching, and drive inverting.
- **Signal Output:** Servo PWM 

## Installation
 1. Wire the battery, motors, and signal out as per the diagram:
diagram
    Note: The battery input has reverse polarity protection up to 20V.
 2. Wire up the radio input depending on your chosen signal bus
     - Servo PWM: 
     diagram
     - PPM, iBUS, SBUS:
     diagram
  
## Operation
 - Every time THE_BRAINS is powered ON it will wait for 2 seconds before arming (and subsequently driving) any motors or signal outputs. 
   This boot-time lock-out window gives the other electroncis time to turn ON and the operator time to move out of the way before things start moving.
 - To enter CALIBRATION mode, the two calibrate pads must be short-circuited during this 2 second boot lock-out window. See the calibration section below for more details.
 - Following the boot lockout, and any calibration steps, THE_BRAINS will start operating in RUN mode: driving the motors and signal output as expected.
 - There is only one way THE_BRAINS will exit RUN mode, that is if a FAULT event occurs. Possible fault events and their corresponding status LED behavour is captured below.
   
###### Fault Conditions
 - **Under-Voltage:** An under-voltage event has two states depending on severity:
    - At 3.5V the warning LED will illuminate to indicate a low battery warning. (Outputs continue to operate)
    - At 3.3V the device will enter an under-voltage fault state until battery voltage is restored. (All functionality stopped)
 - **Over-Temperature:** An over-temperature event also has two states:
    - At 80oC the warning LED will illuminate to indicate a high temperature warning. (Outputs continue to operate)
    - At 100oC the device will enter a fault state until the temperature decreases. (All functionality stopped)
 - **Signal-Input:** A signal-input fault can occur if a signal cannot be detect or signal is lost with the radio reciever.
    - During signal-input faults, a 'No Signal' fail safe method is implemented: THE_BRAINS will stop driving the motors and stop sending signals to the outputs.
    - When a stable radio connection has been re-established (>1 second), THE_BRAINS will exit the fault.

###### Status LEDs
There are two LEDs on THE_BRAINS (1x red, 1x green) and the behaviour of each indicate what state the device is in.
 - **Boot:** Green: OFF, Red: OFF
 - **Normal Operation** Green: OFF, Red: ON
 - **Warning Codes**
    - **Low Battery** Green: Slow Flash, Red: ON 
    - **High Temp** Green: Fast Flash, Red: ON
 - **Fault Codes:**
    - **Signal Input** Green: ON, Red: ON
    - **Under-Voltage** Green: Slow Flash, Red: Slow Flash
    - **Over-Temp** Green: Fast Flash, Red: Fast Flash

## Calibration
THE_BRAINS has a number of calibration parateters that can be chosen from, these are:
- **Input Signal:**
    - Servo PWM (standard 1-2ms pulse from most radio recievers)
    - PPM
    - IBUS
    - SBUS
- **Driving Mode:**
    - Tank (Each motor is controlled by a different input)
    - Arcade (One input controls the both motor speeds and one is mixed for the steering) 
- **Signal Mapping:**
    - Any input channel can be mapped to any of the 4 outputs (2x motors + 2x signal output). 
    - The reversing of any channel is auto-detected during the calibration stage.
    - The signal limits and center (1000-2000µs by default) are auto-detected during calibration stage.
- **Motor Braking:**
    - Motor braking can be enabled or disabled during calibration. 
- **Invert Trigger:**
    - The invert trigger is the input channel that that can be used to  

By default the calibration parameters are:
- **Input Signal:** Servo PWM
- **Driving Mode:** Tank
- **Signal Mapping:** 
     - Ch2=M1, Ch3=M2, All other inputs ignored (No input mapped to output 1 or 2)
     - Signal range (1000,1500,2000)=(min,center,max)
- **Motor Braking:** Enabled
- **Invert Trigger:** Disabled.

###### Calibration Procedure
 1. Ensure the remote is turned ON, it has been previously bound to the radio reciever, and all sticks are in their zero position.
     - The is important because THE_BRAINS will update the zero position for each channel when an input is detected.
 2. Plug the radio reciever and motors into THE_BRAINS.
     - It is important that the motors are connected as they will in the final robot because of the Simon-says calibration technique.
 3. Turn ON THE_BRAINS and within the 2 second boot window short-circuit the two calibrate pads. 
     - This can be done with anything conductive and only needs to be held for a very short time. 
 4. Both status LEDs will pulse three times to indicate it has entered CALIBRATION mode.
 5. THE_BRAINS will immediatly begin checking for the radio and auto-detect the associated input signal type.
     - When a radio input is detected:
         - Both status LEDs will pulse three times to indicate success.
         - The input type and the zero positions for all channels are recorded.
     - If no signal can be detected, THE_BRAINS will enter a signal-input fault state. 
         - THE_BRAINS will continually check for a radio and will only exit this state if one is detected or is power cycled.
 6. The Simon-says calibration technique is then run to detect the driving mode and signal mapping. 
     - Two test are run, each will twitch the two motors and cause the robot to move forward, reverse, left, or right.
     - The user must then input that motion back into the remote, eg:
         - Robot twitches forward: Tank drive, push both sticks forward. Arcarde drive, push one stick forward.
         - Robot twitched left: Tank drive, push left stick back and right forward. Arade drive, push one stick left
     - It is important to push the sticks all the way to the endstop as this step also sets the signal limits.
     - Following each test the status LEDs will pulse three times to success.
     - If no user input is detected after 10 seconds THE_BRAINS will revert to the default driving mode and signal mapping configuration.
 7. THE_BRAINS will not move anything for the following steps, only flashing the LEDs to indicate success. 
 8. Output signal 1 detection:
     - Move the input (on the radio transmitter) that you want to be mapped to the signal 1 output. 
     - First move the input to the fully positive position and then to the fully negative position. 
     - The status LEDs will pulse three times to indicate success.
     - If no input is detected after 10 second THE_BRAINS will revert to the default.
 9. Output signal 2 detection:
     - Move the input (on the radio transmitter) that you want to be mapped to the signal 2 output. 
     - First move the input to the fully positive position and then to the fully negative position. 
     - The status LEDs will pulse three times to indicate success.
     - If no input is detected after 10 second THE_BRAINS will revert to the default.
 10. Invert trigger detection: 
     - Move the input (on the radio transmitter) that you want to be mapped to the invert trigger. 
     - The input must move more than 100us to trigger the input.
     - The status LEDs will pulse three times to indicate success.
     - The non-inverted driving mode will be take from the channel 'zero position' and the inverted driving mode will trigger when the input deviates >100us from zero.
     - If no input is detected after 10 second THE_BRAINS will revert to the default.
 11. Motor Braking detection:
     - Move any of the inputs on the radio transmitter to disable motor braking. 
     - If no inputs are moved within 5 seconds, motor braking will be enabled. 
     - The input must move more than 100us to trigger the change. 
     - The status LEDs will pulse three times to indicate success.
 12. Calibration Complete
     - Both status LEDs will pulse three times to indicate successfull calibration and THE_BRAINS will wait another 2 seconds before entering RUN mode. 
     - If THE_BRAINS is turned OFF before completing the calibration process, NONE of the new parameters are stored and it will revert to a previous configuraion. 
 13. To reset THE_BRAINS back to default settings, short-circuit the calibrate pads for 10 seconds.
     - Both status LEDs will pulse 3 times to indicate a successful reset and the THE_BRAINS will wait another 5 seconds before entering RUN mode. 
