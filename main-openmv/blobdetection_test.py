import sensor, image, time, math


sensor.reset()
sensor.set_pixformat(sensor.RGB565) # or RGB565.
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking

thr = [(0,15,-10,10,-20,20),(70,90,-90,-65,60,80),(60,80,-30,15,60,80),(45,55,70,85,65,75)]#code 1,2,4,8/Black Green,Yellow,Red

min_degree = 0
max_degree = 179


clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8)
    for blob in img.find_blobs(thr, pixels_threshold=200, area_threshold=200,merge = True):
        #print(blob.code())
        if blob.code()==1:#black
            linecount = 0
            for l in img.find_lines(roi = blob.rect() ,threshold = 1000, theta_margin = 50, rho_margin = 30):#直線検知しきい値 あまりいじる必要なし
                if (min_degree <= l.theta()) and (l.theta() <= max_degree):
                    img.draw_line(l.line(), color = (255, 0, 0))
                    img.draw_rectangle(blob.rect())
                    linecount += 1
            if linecount == 1:
                print("S")
            elif linecount == 2:
                print("U")
            elif linecount == 3:
                print("H")
            else:
                print("none")
        elif blob.code()==2:#green
            print("Green")
        elif blob.code()==4:#Yellow
            print("Yellow")
        elif blob.code()==8:#Red
            print("Red")
        else:
            print("none")
