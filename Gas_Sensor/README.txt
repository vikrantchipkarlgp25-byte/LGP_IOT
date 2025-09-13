Gas Sensor with Buzzer Alert (ESP8266 + RoboNinja)

By Vikrant

This project uses an ESP8266 (Wemos D1 Mini), a gas sensor (MQ5), and a buzzer to create a simple gas leak alert system. The system reads gas concentration values from the sensor, prints them to the Serial Monitor, and turns on the buzzer if the values cross a certain threshold. The project also attempts to connect with the NinjaIoT platform for cloud integration.


Features

Reads gas sensor values using the ESP8266’s analog pin.
Displays the readings in the Serial Monitor.
Triggers a buzzer alarm in bursts when the threshold is crossed.
Connects to NinjaIoT cloud with your class/device ID (UID).


Hardware Setup

ESP8266 (Wemos D1 Mini)
MQ gas sensor → connect analog output to A0.
Buzzer → connect positive leg to D2 and negative leg to GND.
Power everything via ESP8266 (use 3.3V or 5V depending on sensor module).


Code Highlights

Gas sensor is read using: int gasValue = analogRead(A0);
Buzzer beeps in bursts using tone() and noTone().
Adjustable threshold, beep duration, and burst count defined at the top of the sketch.


Current Issues

The gas values are visible in the Serial Monitor but do not appear on the IoT dashboard (iot.roboninja.in).

Possible reasons:

The NinjaIoT library needs explicit publish() calls for sending data.
Dashboard configuration might be incomplete (no variable linked).

2. The buzzer does not produce sound.

Possible reasons:

Wrong pin assignment (try D5 or D6 instead of D2 depending on your board).
The buzzer is passive, meaning it needs a frequency signal (tone() should work, but wiring/power may be the issue).
If it is an active buzzer, use digitalWrite(buzzer, HIGH/LOW) instead of tone().

How to Run

Upload the code to your ESP8266 using Arduino IDE.
Open the Serial Monitor at 115200 baud.
Check if gas values are printed.
Blow some smoke or gas near the sensor to test.
If buzzer does not sound, check wiring or try a different pin/method as explained above.

Next Steps

Add iot.publish("GasValue", gasValue); in the loop so that values show on the NinjaIoT dashboard.
Verify the dashboard setup (variable type = number, linked to UID).
Test buzzer separately with a simple code to confirm it is working.