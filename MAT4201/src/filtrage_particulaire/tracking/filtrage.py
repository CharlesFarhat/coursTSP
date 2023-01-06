import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import os

from Donnees_Moodle.filtrage_particulaire import *

from backend import *
from variables import *
from utils import *


def particules_filter():
    prem_image, names, nb_images, folder_name = lecture_image()
    res_obs = selectionner_zone()  # Format : [x, y, long, larg]
    # res_obs = [420.65584416, 140.17532468,  38.44155844,  42.5974026]

    long = res_obs[2]  # On suppose la longueur
    larg = res_obs[3]  # Idem pour la largeur
    # On enregistre l'histrogramme de couleur du visage
    img_obs, colormap, hist_obs = calcul_histogramme(prem_image, res_obs, Nb)

    # on initialise les poids et les etats estimés
    W = np.zeros((T, N))
    X = np.zeros((T, N, 3))  # default X = np.zeros((T, N, 2))
    ests = np.zeros((N, 3))
    w_init = 1/N * np.ones(N)
    w_seuil = 0.6

    # On initialise le filtrage
    X[0], W[0], ests[0] = filtrage_particulaire_init(
        prem_image, res_obs, hist_obs, colormap, w_init)

    # variable pour garder les poids et analyser lambda
    # history_w = []
    # history_w.append(W[0])
    N_eff = []

    # On propage le filtrage
    for i in range(1, nb_images):
        X[i], W[i], ests[i], N_eff = filtrage_particulaire_global(
            i, long, larg, X[i-1], W[i-1], hist_obs, colormap, w_seuil, N_eff)

    # np.savetxt("tab.txt", N_eff)

    print('--- Fin du filtrage --- ')


print("Launch tracking")
particules_filter()
