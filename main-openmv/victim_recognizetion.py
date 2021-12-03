# Image Histogram Info Example
#
# This script computes the histogram of the image and prints it out.
enable_lens_corr = True # turn on for straighter lines...

import sensor, image, time,pyb
from pyb import Pin


led = Pin('P3',Pin.OUT_PP,Pin.PULL_NONE)
start = Pin('P0',Pin.IN,Pin.PULL_UP)
lighton = Pin('P1',Pin.IN,Pin.PULL_UP)
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
    clock.tick()
    img = sensor.snapshot()
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...
    # Gets the grayscale histogram for the image into 8 bins.
    # Bins defaults to 256 and may be between 2 and 256.
    #print(len(img.histogram(bin=8).a_bins()))
    #print(clock.fps())

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

    #print(Lsum)
    #print(asum_neg)
    #print(asum_pos)
    #print(bsum)

    if Lsum >= 0.01:
        linecount = 0
        for l in img.find_lines(threshold = 750, theta_margin = 70, rho_margin = 20):
            if (min_degree <= l.theta()) and (l.theta() <= max_degree):
                img.draw_line(l.line(), color = (255, 0, 0))
                linecount += 1

        #print(linecount)
        if linecount == 1:
            print("S")
        elif linecount == 2:
            print("U")
        elif linecount ==3:
            print("H")
        #else:
            #print("Black")

    elif bsum >= 0.2:
        if asum_neg >= 0.2:
            print("Green")
        elif asum_pos >= 0.2:
            print("Red")
        else:
            print("Yellow")
    else:
        print("White")

    #print("FPS %f" % clock.fps())
# You can also pass get_histogram() an "roi=" to get just the histogram of that area.
# get_histogram() allows you to quickly determine the color channel information of
# any any area in the image.
