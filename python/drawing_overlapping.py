import numpy as np
import cv2

##Anyway the most important part is mask123 which tell where is overlapping in red color.
##For the other it is just for visulization
height, width = 1500, 2000
img1 = np.zeros((height, width, 3), np.uint8)
img1[:, :] = [255, 255, 255]
img2 = np.zeros((height, width, 3), np.uint8)
img2[:, :] = [255, 255, 255]
img3 = np.zeros((height, width, 3), np.uint8)
img3[:, :] = [255, 255, 255]

# Pixel position to draw at
x1, y1 = 600, 500
x2, y2 = 1200, 500
x3, y3 = 900, 1200

cv2.circle(img1,(x1, y1), 500, (255,0,0), -1)
cv2.circle(img2,(x2, y2), 500, (0,255,0), -1)
cv2.circle(img3,(x3, y3), 500, (0,0,255), -1)

img2gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
img2gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
img2gray3 = cv2.cvtColor(img3, cv2.COLOR_BGR2GRAY)

_, mask1 = cv2.threshold(img2gray1, 200, 255, cv2.THRESH_BINARY)
_, mask2 = cv2.threshold(img2gray2, 200, 255, cv2.THRESH_BINARY)
_, mask3 = cv2.threshold(img2gray3, 200, 255, cv2.THRESH_BINARY)

mask1 = cv2.bitwise_not(mask1)
mask2 = cv2.bitwise_not(mask2)
mask3 = cv2.bitwise_not(mask3)

mask12 = cv2.bitwise_and(mask1, mask2)
mask13 = cv2.bitwise_and(mask1, mask3)
mask23 = cv2.bitwise_and(mask2, mask3)
mask123 = cv2.bitwise_and(mask12, mask3)
mask = cv2.bitwise_or(mask1, mask2)
mask = cv2.bitwise_or(mask, mask3)

other_mask = cv2.bitwise_xor(mask, cv2.bitwise_or(cv2.bitwise_or(mask12, mask13), mask23))


bg = np.zeros((height, width, 3), np.uint8)
bg[:, :] = [255, 255, 255]

fg123 = np.zeros((height, width, 3), np.uint8)
fg123[:, :] = [0, 0, 255]
fg123 = cv2.bitwise_and(fg123, fg123, mask = mask123)

fg12 = np.zeros((height, width, 3), np.uint8)
fg12[:, :] = [0, 255, 255]
fg12 = cv2.bitwise_and(fg12, fg12, mask = cv2.bitwise_xor(mask12, mask123))

fg13 = np.zeros((height, width, 3), np.uint8)
fg13[:, :] = [255, 0, 255]
fg13 = cv2.bitwise_and(fg13, fg13, mask = cv2.bitwise_xor(mask13, mask123))

fg23 = np.zeros((height, width, 3), np.uint8)
fg23[:, :] = [255, 255, 0]
fg23 = cv2.bitwise_and(fg23, fg23, mask = cv2.bitwise_xor(mask23, mask123))

other = np.zeros((height, width, 3), np.uint8)
other[:, :] = [255, 255, 255]
other = cv2.bitwise_and(other, other, mask = other_mask)

bg = cv2.bitwise_and(bg, bg, mask = ~mask)
img = cv2.add(bg, fg12)
img = cv2.add(img, fg13)
img = cv2.add(img, fg23)
img = cv2.add(img, fg123)
img = cv2.add(img, other)

cv2.imwrite("square_circle_opencv.jpg", img)
##Anyway the most important part is mask123 which tell where is overlapping in red color.
