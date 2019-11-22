import numpy as np
import cv2

# Width and height of the black window
width = 600
height = 500

# Create a black window of 400 x 300
img = np.zeros((height, width, 3), np.uint8)

# Three vertices(tuples) of the triangle
p1 = (300, 300)
p2 = (200, 100)
p3 = (400, 100)

# Drawing the triangle with the help of lines
#  on the black window With given points
# cv2.line is the inbuilt function in opencv library
cv2.line(img, p1, p2, (255, 0, 0), 3)
cv2.line(img, p2, p3, (255, 0, 0), 3)
cv2.line(img, p1, p3, (255, 0, 0), 3)

# finding centroid using the following formula


# Drawing the centroid on the window
#cv2.circle(img, p1, 80, (0, 255, 0), 3)
cv2.circle(img, p2, 80, (0, 255, 0), 3)
cv2.circle(img, p3, 80, (0, 255, 0), 3)

# image is the title of the window
cv2.imshow("image", img)
cv2.waitKey(0) 
