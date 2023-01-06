import numpy as np

C1 = 150
C2 = C1
C3 = 2/100

R = np.array([[C1, 0, 0], [0, C2, 0], [0, 0, C3]], dtype='float64')

Nb = 10
lamb = 30  # default 20

N = 100
T = 50
