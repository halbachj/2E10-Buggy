### Pseudo code

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
    start time := current time
    packet := receive packet
    if packet then
        if packet is command packet then
            if command is stop
                set state to idle
            if command is start
                set state to line following
    send status packet
    call update of current state
    end time := current time
    loop time := end time - start time

function idle state update:
    do nothing

function line follower state update:
    leftSensor := take analog reading from left sensor
    rightSensor := take analog reading from right sensor
    difference := leftSensor - rightSensor
    if abs(difference) >= 200) then
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
            
