import numpy as np


def creer_trajectoire(F, Q, x_init, T):
    vecteur_x = np.zeros((4, T))
    vecteur_x[:, 0] = x_init
    for i in range(1, len(vecteur_x[0])):
        vecteur_x[:, i] = F@vecteur_x[:, i-1] + \
            np.random.multivariate_normal(np.zeros((4)), Q)
    return vecteur_x


def creer_observations(H, R, vecteur_x, T):
    vecteur_y = np.zeros((2, T))
    for i in range(0, len(vecteur_y[0])):
        vecteur_y[:, i] = H@vecteur_x[:, i] + \
            np.random.multivariate_normal(np.zeros(2), R)
    return vecteur_y


def filtre_de_kalman(F, Q, H, R, y_k, x_kalm_prec, P_kalm_prec):

    # prédiction :
    m_prediction = F@x_kalm_prec
    P_prediction = Q + P_kalm_prec@F.T

    # update
    K = P_prediction@H.T@np.linalg.inv(H@P_prediction@H.T + R)
    # Mise à jour des vecteurs
    x_kalm_k = m_prediction + K@(y_k - H@m_prediction)
    P_kalm_k = (np.eye(len(P_kalm_prec[0])) - K@H)@P_prediction

    # Return à l'étape k des vecteurs
    return [x_kalm_k, P_kalm_k]


def erreur_quad(x_est, vecteur_x):
    sum = 0
    T = len(vecteur_x[0])
    for i in range(T):
        sum += np.sqrt((vecteur_x[:, i] - x_est[:, i]).T @
                       (vecteur_x[:, i] - x_est[:, i]))
    return (1/T)*sum
