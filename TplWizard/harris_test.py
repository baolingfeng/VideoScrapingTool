#coding=utf8
import cv2
import numpy as np

if __name__ == '__main__':
    img = cv2.imread('img-00010.png',0)
    harris = cv2.cornerHarris(img,2,3,0.04)
    print img
    #cv2.imshow('har',harris)