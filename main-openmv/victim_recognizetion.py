enable_lens_corr = True

import sensor, image, time,pyb
from pyb import Pin ,UART,LED


led = Pin('P3',Pin.OUT_PP,Pin.PULL_NONE)
start = Pin('P0',Pin.IN,Pin.PULL_UP)
lighton = Pin('P1',Pin.IN,Pin.PULL_UP)
status = Pin('P7',Pin.OUT_PP,Pin.PULL_NONE)

status.off()
uart = UART(3,9600)

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

led.value(False)
while(start.value() == False):
    if lighton.value() == True:
        led.value(not led.value())
        time.sleep_ms(500)


sensor.reset()
sensor.set_pixformat(sensor.RGB565) # or RGB565.
sensor.set_framesize(sensor.QQQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

min_degree = 0
max_degree = 179


while(True):
    status.on()
    clock.tick()
    img = sensor.snapshot()
    if enable_lens_corr: img.lens_corr(1.8)

    L = img.histogram(bin=8).bins()
    Lsum = 0

    a = img.histogram(bin=8).a_bins()
    asum_neg = 0
    asum_pos = 0

    b = img.histogram(bin=8).b_bins()
    bsum = 0

    for i in range(0,16):
        Lsum += L[i]

    for k in range(48,89):
        asum_neg += a[k]

    for l in range(168,209):
        asum_pos += a[l]

    for m in range(168,209):
        bsum += b[m]

    #uart.write(Lsum)
    #uart.write(asum_neg)
    #uart.write(asum_pos)
    #uart.write(bsum)

    if Lsum >= 0.01:#黒しきい値
        linecount = 0
        for l in img.find_lines(threshold = 750, theta_margin = 70, rho_margin = 20):#直線検知しきい値 あまりいじる必要なし
            if (min_degree <= l.theta()) and (l.theta() <= max_degree):
                img.draw_line(l.line(), color = (255, 0, 0))
                linecount += 1

        #print(linecount)
        if linecount == 1:
            uart.write("S")
            print("S")
            red_led.on()
            blue_led.on()
            green_led.off()
        elif linecount == 2:
            uart.write("U")
            print("U")
            green_led.on()
            red_led.off()
            blue_led.off()
        elif linecount ==3:
            uart.write("H")
            print("H")
            red_led.on()
            green_led.off()
            blue_led.off()
        else:
            uart.write("N")
            print("N")
            green_led.on()
            red_led.on()
            blue_led.on()
        #else:
            #uart.write("Black")

    elif bsum >= 0.2:#その他色しきい値
        if asum_neg >= 0.2:#緑しきい値
            uart.write("G")
            print("G")
            green_led.on()
            red_led.off()
            blue_led.off()
        elif asum_pos >= 0.2:#赤しきい値
            uart.write("R")
            print("R")
            red_led.on()
            green_led.off()
            blue_led.off()
        else:
            uart.write("Y")
            print("Y")
            red_led.on()
            blue_led.on()
            green_led.off()
    else:
        uart.write("N")
        print("N")
        green_led.on()
        red_led.on()
        blue_led.on()
