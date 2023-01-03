import time

import PIL.Image
import numpy as np
import scipy.stats as stats
import PIL.Image as Image
import matplotlib.pyplot as plt
import matplotlib.patches as patches


from variables_p2 import *
from programmes_annexes import *
from Donnees_Moodle.filtrage_particulaire import *


def erreur_quadra(x_est, x_reel):
    return np.sqrt((x_est - x_reel).dot(x_est - x_reel) / T)


def reechantillonage(X, W):
    print('Réechantillonage')
    return np.random.choice(X, len(X), p=W), np.ones(len(X))/len(X)


def D_squared(hist1, hist2):
    return 1 - np.sum(np.sqrt(hist1*hist2))


def filtrage_particulaire_unit(prem_image, res_obs, hist_obs, colormap, w_init):

    # On initialise les particules à partir de la position donnée par selectionner_zone
    prem_parts = np.random.multivariate_normal(res_obs[0:2], R, size=N)
    res_parts = []

    for i in range(len(prem_parts)):
        res_parts.append(
            [prem_parts[i, 0], prem_parts[i, 1], res_obs[2], res_obs[3]])
    res_parts = np.array(res_parts)

    hists_pred = []
    for i in range(len(res_parts)):
        hists_pred.append(calcul_histogramme(
            prem_image, res_parts[i], colormap)[-1])
    hists_pred = np.array(hists_pred)

    prem_w = []
    for i in range(len(hists_pred)):
        prem_w.append(
            np.exp(-lamb * D_squared(hist_obs, hists_pred[i])) * w_init[i])
    prem_w = np.array(prem_w)

    print('Poids total des poids avant norm :' + str(np.sum(prem_w)))
    prem_w = prem_w / np.sum(prem_w)

    est = np.array([0, 0], dtype='float64')
    for i in range(len(prem_parts)):
        est += prem_parts[i] * prem_w[i]

    print(' \n Première Estimation : ' + str(est))
    print('Première Observation : ' + str(res_obs[0:2]))

    return prem_parts, prem_w, est


def filtrage_part_global(i, long, larg, x, w, hist_obs, colormap, w_seuil):
    # Comme la matrice de cov est [[1, 0], [0, 1]] cela revient au même..
    new_parts = np.random.normal(x.flatten(), C1).reshape(N, 2)
    hist_parts = []
    new_w = []

    im = Image.open('Donnees_Moodle/sequences/sequence1/im' +
                    '0' * (i < 10) + str(i) + '.bmp')

    # On calcule les poids des nouvelles particules
    for j in range(len(new_parts)):
        hist_parts.append(calcul_histogramme(im, np.array(
            [new_parts[j, 0], new_parts[j, 1], long, larg]), colormap)[-1])
        new_w.append(np.exp(-lamb * D_squared(hist_parts[j], hist_obs)) * w[j])
    new_w = np.array(new_w)

    print('Somme poids avant normalisation : ' + str(np.sum(new_w)))
    new_w = new_w / np.sum(new_w)

    # On calcule l'estimation une fois les poids normalisés
    est = np.array([0, 0], dtype='float64')
    for k in range(len(new_parts)):
        est += new_parts[k] * new_w[k]

    print('Estimation ' + str(est))
    # Ne Fonctionne pas...
    est_to_images(im, est, long, larg,
                  '../Plots/Exercice_2/Suivi_ll_fixes/image_' + str(i) + '.png')

    # Si un poids a une probabilité élevée (définie par w_seuil) cela signifie que beaucoup doivent avoir un
    # poids faibles et les particules associées sont peu pertinentes. Dans ce cas on rééchantillonne.
    if (new_w > w_seuil).any():
        tirages, new_w = reechantillonage(range(len(new_parts)), new_w)
        new_parts = new_parts[tirages]

    return new_parts, new_w, est
