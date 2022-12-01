import numpy as np


def evaluate_jacobian(mu):
    # last prediction format : [p_x, v_x, p_y, v_y]
    return np.array([
        [(-mu[2])/(mu[0]**2 + mu[2]**2), 0, mu[0]/(mu[0]**2 + mu[2]**2), 0],
        [mu[0]/np.sqrt(mu[0]**2 + mu[2]**2), 0, mu[2] /
         np.sqrt(mu[0]**2 + mu[2]**2), 0]
    ])


def g(x):
    """
        Passage en coordonnées polaire du vecteur x passé en argument.
        Attention ici g renvois : [theta, r]
    """
    return np.array(
        [np.arctan(x[2]/x[0]),
         np.sqrt(x[0]**2 + x[2]**2)])


def creer_observation_EKF(R, vecteur_x, T):
    """
        Attention ici R = R_EKF et on doit convertir en coordonnées polaire X (ici avec g)
    """
    vecteur_y = np.zeros((2, T))
    for i in range(0, len(vecteur_y[0])):
        H_polar = g(vecteur_x[:, i])
        vecteur_y[:, i] = H_polar + \
            np.random.multivariate_normal(np.zeros(2), R)
    return vecteur_y


def filtre_de_kalman_extended(F, Q, R, y_k, x_kalm_prec, P_kalm_prec):
    # prédiction :
    mu_prediction = F@x_kalm_prec
    # On va utiliser m_prediction comme meilleure approximation de X
    # à l'instant k !
    P_prediction = Q + P_kalm_prec@F.T

    # Variables upadté pour l'update de l'EKF:
    y_k_new = y_k - g(mu_prediction) + \
        evaluate_jacobian(mu_prediction)@mu_prediction
    # La nouvelle matrice H issue de la linéarisation:
    H = evaluate_jacobian(mu_prediction)

    # update
    K = P_prediction@H.T@np.linalg.inv(H@P_prediction@H.T + R)
    # Mise à jour des vecteurs
    x_kalm_k = mu_prediction + K@(y_k_new - H@mu_prediction)
    P_kalm_k = (np.eye(len(P_kalm_prec[0])) - K@H)@P_prediction

    # Return à l'étape k des vecteurs
    return [x_kalm_k, P_kalm_k]
