import mbot2
import gamepad
import cyberpi
import time
from cyberpi import *

mbot2.servo_set(90,'S2')
mbot2.servo_set(90,"S1")

while True:


    joystick_Ly = gamepad.get_joystick('Ry')
    joystick_Lx = gamepad.get_joystick('Rx')

    left_motor = joystick_Ly - joystick_Lx
    right_motor = joystick_Ly + joystick_Lx

    left_motor  = max(-100, min(100, left_motor))
    right_motor = max(-100, min(100, right_motor))

    mbot2.drive_power(left_motor, right_motor)


    joystick_Ry = gamepad.get_joystick('Ly')
    joystick_Rx = gamepad.get_joystick('Lx')

    shooter_1 = joystick_Ry + joystick_Rx
    shooter_2 = joystick_Ry + joystick_Rx

    shooter_1  = max(-100, min(100, shooter_1))
    shooter_2 = max(-100, min(100, shooter_2))

    mbot2.motor_set(shooter_1 ,"M1")
    mbot2.motor_set(shooter_2, "M2")

    #gripper

    if gamepad.is_key_pressed("N1"):
        mbot2.servo_set(180,"S2")

    elif gamepad.is_key_pressed("N2"):
        mbot2.servo_set(90,"S2")

    elif gamepad.is_key_pressed("N3"):
        mbot2.servo_set(180,"S1")

    elif gamepad.is_key_pressed("N4"):
        mbot2.servo_set(90,"S1")

    # Add delay
    time.sleep(0.05)