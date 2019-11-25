import numpy as np
import matplotlib.pyplot as plt
import sys
filename = 'beacons_record_1.csv'
file = open(filename)
data = file.readlines()
rssi = [[float(s) for s in line.strip().split(',')[:3]] for line in data]
rssi = np.array(rssi)
N = 50
if len(sys.argv)>1:
    N = int(sys.argv[1])
first = np.convolve(rssi[:,0], np.ones((N,))/N, mode='valid')
second = np.convolve(rssi[:,1], np.ones((N,))/N, mode='valid')
third = np.convolve(rssi[:,2], np.ones((N,))/N, mode='valid')
t = np.arange(len(first))
plt.plot(t, first, 'r', t, second, 'b', t, third, 'g')
plt.show()
