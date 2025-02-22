import cvzone
import cv2
import mediapipe as mp
from cvzone.HandTrackingModule import HandDetector

cap = cv2.VideoCapture(0)
detector = HandDetector(maxHands=1, detectionCon=0.7)

while True:
    success, img = cap.read()
    hands, img = detector.findHands(img, draw=True, flipType=True)
    if hands: # If there are hands in the camera...
        hand1 = hands[0] # First capture the first hand you see.
        fingers1 = detector.fingersUp(hand1) # Detect the fingers on the hand.
        print(fingers1) # Print a 5 bit code for fingers that are raised and fingers that are not. Note, the thumb is the first bit when I tested it and the pinky is the last.
    cv2.imshow('Image', img)
    cv2.waitKey(1)