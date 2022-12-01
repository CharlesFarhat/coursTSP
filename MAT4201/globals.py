import numpy as np
import matplotlib.pyplot as plt

################################################
############# Global  var init #################
################################################

T_e = 1
T = 100
sigma_Q = 1
sigma_px = 30
sigma_py = 30

F = np.eye(4)
F[0, 1], F[2, 3] = T_e, T_e
Q = np.array([
    [T_e**3/3, T_e**2/2, 0, 0],
    [T_e**2/2, T_e, 0, 0],
    [0, 0, T_e**3/3, T_e**2/2],
    [0, 0, T_e**2/2, T_e]], dtype='float64'
)*sigma_Q**2

H = np.zeros((2, 4))
H[0, 0], H[1, 2] = 1, 1
R = np.diag([sigma_px**2, sigma_py**2])

x_init = np.array([3, 40, -4, 20])

x_kalm = x_init  # x^_0|0
P_kalm = np.eye(4)  # P_0|0

# Pour la version EKF:
sigma_dist = 10
sigma_angle = np.pi/180
R_EKF = np.diag([sigma_angle**2, sigma_dist**2])


################################################
############# Global functions #################
################################################

def draw_all(vecteur_trajectoire, vecteur_observation, vecteur_estimations):
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
             linestyle='', marker='o', markersize=1, label='Observations')
    plt.plot(vecteur_estimations[0, :], vecteur_estimations[2, :],
             color="g", label="Estimation de la trajectoire")
    plt.legend()
    plt.show()
