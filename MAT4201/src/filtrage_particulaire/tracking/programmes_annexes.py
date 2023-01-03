import time

import cv2 as cv
import matplotlib.pyplot
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

from Donnees_Moodle.filtrage_particulaire import *


def est_to_images(image, est, long, larg, path): # Affiche et sauvegarde la prédiction sur l'image
    fig, ax = plt.subplots()
    ax.imshow(image)
    rect = patches.Rectangle(est, long, larg, linewidth=1, edgecolor='r', facecolor='none')
    ax.add_patch(rect)

    plt.pause(0.01)

    fig.savefig(path)
    plt.close(fig)


def images_to_videos(): #Cette fonction va chercher une suite d'image dans un dossier choisi et créée une vidéo à partir de ces dernières.
    im = cv.imread('../Plots/Exercice_2/Suivi_ll_fixes/suivi_corps/image_1.png')
    height, width = im.shape[0], im.shape[1]
    print(height, width)
    fourcc = cv.VideoWriter_fourcc(*'mp4v')
    video = cv.VideoWriter('video_suivi_corps.mp4', fourcc, 1.5, (width, height))

    for i in range(48):
        img = cv.imread('../Plots/Exercice_2/Suivi_ll_fixes/suivi_corps/image_' + str(i) + '.png')
        video.write(img)

    cv.destroyAllWindows()
    video.release()

    cap = cv.VideoCapture('video_suivi_corps.mp4')

    if not(cap.isOpened()):
        print("Error opening video  file")

    while True:
        ret, frame = cap.read()

        if not ret:
            break
        time.sleep(0.5)
        cv.imshow('output', frame)

        if (cv.waitKey(1) & 0xFF == ord('q')):
            break

    cap.release()
    cv.destroyAllWindows()


def test_on_display():
    im = plt.imread('Donnees_Moodle/sequences/sequence1/sequence10004.bmp')

    fig, ax = plt.subplots()
    ax.imshow(im)
    rect = patches.Rectangle((2, 2), 10, 10, linewidth=1, edgecolor='r', facecolor='none')
    ax.add_patch(rect)

    fig.show()
    fig.savefig('Test_Display.png')


def make_histo():
    im = Image.open('../Plots/Exercice_2/Suivi_ll_fixes/suivi_corps/image_1.png')

    fig1 = plt.figure()
    plt.imshow(im)

    fig2 = plt.figure()
    plt.plot(im.histogram())
    plt.show()
