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
The base type for this is a Packet, which contains an enum / int that specifies the packet type and a union which specifies the packet content. This union can take the form of a command packet, 
a status packet and a log packet. Specific data structures can be seen below. Declaring the packages as structs makes accessing data very easy, as data can be accessed without the need for 
conversion to and from a string.  The drawback of this is that the overall package length is fixed to the size of the largest packet, which is the log packet, containing the 
log message with 64 characters, making the overall package size 72 bytes.

Log packet:
  short count - The length of the log message.
  const char[] message - The log message.

Status packet:
  bool objectDetected - Wether there is an object detected by the ultrasonic sensor.
  float objectDistance - The distance to the detected object in cm.
  int leftMotorSpeed - The measured speed of the left motor.
  int rightMotorSpeed - The measured speed of the right motor.
  float leftMotorDistanceTraveled - The distance the left wheel has traveled in m.
  float rightMotorDistanceTraveled - The distance the right wheel has traveled in m.
  int leftIrSensorValue - The analog value read by the left IR sensor (used for debugging purposes).
  int rightIrSensorValue - The analog value read by the right IR sensor (used for debugging purposes).

Command packet:
  CommandType type - The type of command sent. This is an enum with the possible states START, STOP, RESET_DISTANCE.
  uint16_t data - additional attributes for the command. Planned for future use, as for example to set the speed.


Pseudo code for the Arduino on the Buggy:

function setup:
    Start serial interface
    setup motor pins
    setup ultrasonic pins
    attatch encoder interrupts
    attatch ultrasonic interrupt
    setup wifi access point
    setup tcp server
    set buggy state to line following state
    notify over serial that init is done

function loop:
    packet := receive packet
    if packet then
        if packet is command packet then
            if command is stop
                set state to idle
            if command is start
                set state to line following
    send status packet
    call update of current state

function idle state update:
  do nothing

function line follower state update:
    leftSensor := take analog reading from left sensor
    rightSensor := take analog reading from right sensor
    difference := leftSensor - rightSensor
    if abs(difference) <= 200 then
        set left motor to full speed
        set right motor to full speed
    if difference > 200 then
        set left motor to full speed
        stop right motor
    else if difference < 200
        stop left motor
        set right motor to full speed

    distance := take reading from ultrasonic sensor
    if distance < 20cm then
        set buggy state to object detected state

function object detected state update:
    distance := take reading from ultrasonic sensor
    if distance > 20cm then
        set buggy state to line following state

Pseudo code for the ground station computer:


function setup
  set window size
  create alert screen
  ceate start button and link to start callback
  ceate stop button and link to start callback
  ceate reset deistance button and link to start callback
  initialize matrix display

function draw
  if !connected and last attempt more than 3 seconds ago then
    connect to buggy
  
  if connected and message available then
    buffer := read from server
    parse packet(buffer)

  show wheel distance as (left wheel distance + right wheel distance) / 2

  set alert screen background
  if object detected then
    set alert screen background to red
    set alert message to object detected
    show obstacle distance
    change matrix state to object detected state
  else
    set alert message to no object detected
    set matrix state to line following state

  if matrix state is idle state then
    copy stop pattern pattern into matrix pattern buffer
  if matrix state is object detected state then
    copy warning pattern into matrix pattern buffer
  if matrix state is line following state then
    copy arrow pattern into matrix pattern buffer
  
  draw matrix pattern buffer

function parse packet(buffer)
  create byte buffer from buffer
  set byte buffer order to little endian
  packet type := pop int from byte buffer
  if type is status packet then
    object detected := pop boolean from byte buffer
    obstacle distance := pop float from byte buffer
    left motor speed := pop int from byte buffer
    right motor speed := pop int from byte buffer
    left wheel distance := pop float from byte buffer
    right wheel distance := pop float from byte buffer
    left sensor value := pop int from byte buffer
    right sensor value := pop int from byte buffer





stop button pressed event
  send stop packet

start button pressed event
  send start packet

reset distance button pressed event
  send reset distance packet

