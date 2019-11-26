import numpy as np
import cv2
from scipy import ndimage

def get_position(bases, radii):
    height, width = 2000, 2000

    # Base station position to draw at
    x1, y1 = bases[0,0], bases[0,1]
    x2, y2 = bases[1,0], bases[1,1]
    x3, y3 = bases[2,0], bases[2,1]
    
    img1 = np.zeros((height, width, 3), np.uint8)
    img1[:, :] = [255, 255, 255]
    img2 = np.zeros((height, width, 3), np.uint8)
    img2[:, :] = [255, 255, 255]
    img3 = np.zeros((height, width, 3), np.uint8)
    img3[:, :] = [255, 255, 255]

    cv2.circle(img1,(x1, y1), radii[0], (255,0,0), -1)
    cv2.circle(img2,(x2, y2), radii[1], (255,0,0), -1)
    cv2.circle(img3,(x3, y3), radii[2], (255,0,0), -1)

    img2gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    img2gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    img2gray3 = cv2.cvtColor(img3, cv2.COLOR_BGR2GRAY)

    _, mask1 = cv2.threshold(img2gray1, 200, 255, cv2.THRESH_BINARY)
    _, mask2 = cv2.threshold(img2gray2, 200, 255, cv2.THRESH_BINARY)
    _, mask3 = cv2.threshold(img2gray3, 200, 255, cv2.THRESH_BINARY)
    
    #mask1 mask2 mask3 indicate circles centered at each base
    mask1 = cv2.bitwise_not(mask1)
    mask2 = cv2.bitwise_not(mask2)
    mask3 = cv2.bitwise_not(mask3)
    
    mask12 = cv2.bitwise_and(mask1, mask2)
    mask13 = cv2.bitwise_and(mask1, mask3)
    mask23 = cv2.bitwise_and(mask2, mask3)
    mask123 = cv2.bitwise_and(cv2.bitwise_and(mask1, mask2), mask3)
    mask = cv2.bitwise_or(cv2.bitwise_or(mask1, mask2), mask3)

    bg = np.zeros((height, width, 3), np.uint8)
    bg[:, :] = [255, 255, 255]
    bg = cv2.bitwise_and(bg, bg, mask = cv2.bitwise_xor(mask, mask123))
    
    fg = np.zeros((height, width, 3), np.uint8)
    fg[:, :] = [255, 255, 0]
    fg = cv2.bitwise_and(fg, fg, mask = mask123)

    img = cv2.add(bg, fg)
    
    cv2.circle(img,(x1, y1), radii[0], (255,0,0), 5)
    cv2.circle(img,(x2, y2), radii[1], (0,255,0), 5)
    cv2.circle(img,(x3, y3), radii[2], (0,0,255), 5)
    
    try:
        y, x = ndimage.measurements.center_of_mass(mask123)
    except:
        print("There is no overlapping")
        y, x = 0, 0

    x, y = int(x), int(y)
    cv2.circle(img,(x, y), 5, (0,255,255), -1)

    cv2.circle(img,(x1, y1), 5, (255,0,0), -1)
    cv2.circle(img,(x2, y2), 5, (0,255,0), -1)
    cv2.circle(img,(x3, y3), 5, (0,0,255), -1)
    
    cv2.line(img, (x1, y1), (x2, y2), (0,0,0), 2)
    cv2.line(img, (x2, y2), (x3, y3), (0,0,0), 2)
    cv2.line(img, (x1, y1), (x3, y3), (0,0,0), 2)
    cv2.imwrite("map.jpg", img)
    return x, y

if __name__== "__main__":
    bases = np.array([[500, 600],[1000, 600],[800, 1200]])
    radii = np.array([400,400,400])
    x, y = get_position(bases, radii)
    print(x,y)
