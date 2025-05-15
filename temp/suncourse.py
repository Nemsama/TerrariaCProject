import math

N = 1024
start_angle = -2.36
end_angle   = -0.78
step =  (end_angle - start_angle) / N
r = 1000
x_0 = 640
y_0 = 1070

with open("assets/data/suncourse.dat", "w") as f:
    f.write( str(N) + "\n")
    for i in range(N):
        t = start_angle + i * step
        x = x_0 + r * math.cos(t)
        y = y_0 + r * math.sin(t)
        
        f.write(f"{x:.3f} {y:.3f}\n")
    
    f.close()