specify the messages, signals, events and feedback between the components and the buggy hardware interface. 

The Buggy Hardware interface is abstracted away through different driver classes. That means that all of the Hardware relevant communication is handled by
a ceratin set of objects that perform the read and write operations to the hardware. Hence the hardware messages, signals, events and feedback is listed
on a class basis to simplify things.

Motor driver class:
    Direction signal sent to the H-Bridge. The direction signal is carried by two lines hooked up to two digital pins on the Arduino. Each line carries the inverse logic
    Signal of the other. That means when line A is high line B will be low. The used H-Bridge will switch it's output polarity, hence inverting the direction of the connected motor.

    Speed signal sent to the H-Bridge. The speed signal is a pulse width modulated (PWM) signal going from one of the digital PWM enabled pins to the enable pin on the H-Bridge.
    This pin is pulsed on and off essentally turning on and off the motor at a high frequency. By varying the pulse width the motor is turned on for a longer period of time during each pulse,
    hence varying its speed.

    Encoder signal. The encoder signal is connected to one of the pins with interrupt capability and the corresponding interrupt is then attatched in the software.
    There is an individual interrupt for the left and right motor. This interrupt is run every time the hall sensor senses a change in magnetic field polarity. The interrupt mode is 
    therefore set to change, firing both on a rising and falling edge of the signal. This interrupt then counts up the degree variable by 45 degrees, as the magnetic disk has 8 magnets on it,
    and updates two timing variables. One timing variable hold the time from the previous change and on holds the time from the current change. Both are updated during the interrupt with the time 
    currently stored in the count variable of the second asyncronus general timer (AGT2).

Ultrasonic driver class:
    Trigger signal sent to the Ultrasonic sensor is hooked up to one of the digitial pins on the Arduino. This pin is used to trigger a measurement pulse on the sensor that is then reeived later.
    From the specifications this pin needs to be high for at least 10µs to trigger a pulse.

    The Echo signal received from the ultrasonic sensor is hooked up to a pin with interrupt capabilities. The interrupt mode is set up to fire on a rising and falling edge of the signal.
    This is done to create non blocking code, as the usual method using ´pulseIn()´ would wait for the signal to be received in full without allowing for other things to happen in the mean time.
    The interrupt therefore checks if the echo pin is currently high or low, indicating the start or the end of the echo pulse. On a rising edge the current time in µs is recorded and on a falling
    edge the time difference is calculated giving us a distance measured.

Infrared sensor class:
    The analog output of the infrared sensor is connected to one of the anaog input pins on the arduino, measuring the contrast below the infrared snesor as an analog value.


Timer2 class:
    The second AGT timer of the Renesas RA4M1 is setup to interrupt every 500µs. This is done to get more accurate timing to measure the time taken for the wheel rotation.

LED Matrix class:
    The LED matrix class communicates with the Arduino library for the bultin LED matrix on the Arduino Uno R4 WiFi. 

Serial port:
    The serial port is setup and used as a logging interface to get output from the Arduino during runtime. The software is built in a way where this output can be redirected to a tcp server,
    and sent of via WiFi.



specify the key data structures and control flow algorithm in pseudo code for both the Supervisor PC and Arduino Buggy components. 

Packet data structure:
The packages sent over WiFi adhere to a very specific structure. We came to the conclusion that sending commands and data using strings would not be very effective as a long term solution.
We agreed on using structs to create our packets as they can be easily converted into a binary format and sent over TCP. This approach also simplyfies the receiving of packages, as long as 
the received data can be converted back to the structs. To make this approach as flexible as possible we used composition to create a versatile structure.
The base type for this is a Packet, which contains an enum / int that specifies the packet type and a union which specifies the packet content. This union can take the form of 
