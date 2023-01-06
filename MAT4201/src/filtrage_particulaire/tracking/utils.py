import time

import cv2 as cv
import matplotlib.pyplot
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

from Donnees_Moodle.filtrage_particulaire import *


# Affiche et sauvegarde la prédiction sur l'image
def est_to_images(image, est, long, larg, path):
    fig, ax = plt.subplots()
    ax.imshow(image)
    rect = patches.Rectangle(est, long, larg, linewidth=1,
                             edgecolor='r', facecolor='none')
    ax.add_patch(rect)

    # Permet de voir en temps réel avec matplotlib
    # plt.pause(0.01)

    fig.savefig(path)
    plt.close(fig)
