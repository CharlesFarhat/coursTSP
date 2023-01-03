import numpy as np
import matplotlib.pyplot as plt


################################################
############# Global functions #################
################################################

def draw_all(vecteur_trajectoire, vecteur_observation, vecteur_estimations, sigma_Q, sigma_px, sigma_py, err):
    """
        Shapes:
             vecteur_trajectoire : (4, T)
             vecteur_observation : (2, T)
             vecteur_estimations : (4, T)
    """
    plt.figure(figsize=(10, 10))
    plt.plot(vecteur_trajectoire[0, :],
             vecteur_trajectoire[2, :], color="r", label='Trajectoire')
    plt.plot(vecteur_observation[0, :], vecteur_observation[1, :], color="b",
             linestyle='', marker='o', markersize=2, label='Observations')
    plt.plot(vecteur_estimations[0, :], vecteur_estimations[2, :],
             color="g", label="Estimation de la trajectoire")
    plt.legend()
    plt.title(
        f"Résultat du filtre de Kalman simple \n sigma_Q = {sigma_Q} \n sigma_px = {sigma_px} et sigma_py = {sigma_py} \n Erreur vectorielle : {err}")
    plt.show()


def draw_all2(vecteur_trajectoire, vecteur_observation, vecteur_estimations, sigma_Q, sigma_px, sigma_py, err):
    """
        Shapes:
             vecteur_trajectoire : (4, T)
             vecteur_observation : (2, T)
             vecteur_estimations : (4, T)
    """
    plt.figure(figsize=(10, 10))
    plt.plot(vecteur_trajectoire[0, :],
             vecteur_trajectoire[2, :], color="r", label='Trajectoire')
    plt.plot(vecteur_observation[0, :], vecteur_observation[1, :], color="b",
             linestyle='', marker='o', markersize=2, label='Observations')
    plt.plot(vecteur_estimations[0, :], vecteur_estimations[2, :],
             color="g", label="Estimation de la trajectoire")
    plt.legend()
    plt.title(
        f"Résultat du filtre de Kalman étendu \n sigma_Q = {sigma_Q} \n sigma_px = {sigma_px} et sigma_py = {sigma_py} \n Erreur vectorielle : {err}")
    plt.show()
