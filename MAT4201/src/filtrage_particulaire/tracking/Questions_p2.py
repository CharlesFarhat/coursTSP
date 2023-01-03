import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import os

from Donnees_Moodle.filtrage_particulaire import *

from main_p2 import *
from variables_p2 import *
from programmes_annexes import *


def question_filt_ll_unique():
    prem_image, names, nb_images, folder_name = lecture_image()
    res_obs = selectionner_zone()  # Format : [x, y, long, larg]

    long = res_obs[2]  # On suppose la longueur
    larg = res_obs[3]  # Idem pour la largeur
    print('Longueur, Largeur : ' + str(long) + ', ' + str(larg))
    # On enregistre lamb'histrogramme de couleur du visage
    est, colormap, hist_obs = calcul_histogramme(prem_image, res_obs, Nb)

    w_init = 1/N * np.ones(N)
    filtrage_particulaire_unit(
        prem_image, res_obs, hist_obs, colormap, w_init)


def question_filt_ll_fixes():
    prem_image, names, nb_images, folder_name = lecture_image()
    res_obs = selectionner_zone()  # Format : [x, y, long, larg]

    long = res_obs[2]  # On suppose la longueur
    larg = res_obs[3]  # Idem pour la largeur
    # On enregistre l'histrogramme de couleur du visage
    img_obs, colormap, hist_obs = calcul_histogramme(prem_image, res_obs, Nb)

    # On stocke les estimations dans un fichier (ça m'a servi pour créer différents visuels, pour éviter d'avoir à tout recalculer plusieurs fois
    if not(os.path.isfile('filtrage_sequence_4.txt')):
        W = np.zeros((T, N))
        X = np.zeros((T, N, 2))
        ests = np.zeros((N, 2))
        w_init = 1/N * np.ones(N)
        w_seuil = 0.6

        X[0], W[0], ests[0] = filtrage_particulaire_unit(
            prem_image, res_obs, hist_obs, colormap, w_init)

        for i in range(1, nb_images):
            X[i], W[i], ests[i] = filtrage_part_global(
                i, long, larg, X[i-1], W[i-1], hist_obs, colormap, w_seuil)

        # Sauvegarde en conservant le format d'un array numpy
        np.savetxt('filtrage_sequence_4.txt', ests)

    print('--- Fin du filtrage --- ')


question_filt_ll_unique()
question_filt_ll_fixes()
# images_to_videos()
# make_histo()
