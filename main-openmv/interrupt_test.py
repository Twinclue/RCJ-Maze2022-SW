# Untitled - By: hajim - 土 1 15 2022

import sensor, image, time, pyb
from pyb import Pin ,UART,LED
start = Pin('P0',Pin.IN,Pin.PULL_UP)
lighton = Pin('P1',Pin.IN,Pin.PULL_UP)
status = Pin('P7',Pin.OUT_PP,Pin.PULL_NONE)
deta = Pin('P4',Pin.OUT_PP,Pin.PULL_NONE)
intr = Pin('P5',Pin.OUT_PP,Pin.PULL_NONE)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

clock = time.clock()

state = False;

while(True):
    if start.value() == True:
        intr.on()
        time.sleep_ms(100)
        intr.off()

    if lighton.value() == True:
        state = not state
        deta.value(state);
        status.value(state)
        time.sleep_ms(100)

    clock.tick()
    img = sensor.snapshot()
    print(state)
