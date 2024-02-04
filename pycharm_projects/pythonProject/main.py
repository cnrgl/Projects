import numpy as np
import cv2
cv2.__version__
cap = cv2.VideoCapture("./slow_traffic_small.mp4")

ret, frame = cap.read()

r, h, c, w = 250, 90, 400, 125
track_window = (r, h, c, w)

roi = frame[r:r+h, c:c+w]
hsv_roi = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)
mask = cv2.inRange(hsv_roi, np.array((0.,60.,32.)),np.array((180.,255.,255.)))
roi_hist = cv2.calcHist([hsv_roi], [0], mask, [180], [0, 180])
cv2.normalize(roi_hist, roi_hist, 0, 255, cv2.NORM_MINMAX)

term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)

while( True ):
    ret, frame = cap.read()

    if ret == True:
        # filter background via ROI
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        dst = cv2.calcBackProject([hsv], [0], roi_hist, [0, 180],1)

        # main function
        ret, track_window = cv2.meanShift(dst, track_window, term_crit)

        # drawing
        x, y, w, h = track_window
        img2 = cv2.rectangle(frame, (x,y), (x+w,y+h), 255,2)
        cv2.show('img2',img2)

        k = cv2.waitKey(60) & 0xff
        if k == 27:
            break
        else:
            cv2.imwrite(chr(k)+".jpg", img2)
    else:
        break
cv2.destroyAllWindows()
cap.release()
