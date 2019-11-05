import numpy as np


class SelfOrganizingMap:
    def __init__(self, n: int, m: int, beta: float):
        self._n = n
        self._m = m
        self._beta = beta
        self._W = np.random.uniform(0, 1, [m, n])

    def train(self, standards: list, threshold: int):
        standards_len = len(standards)
        neurons_wins = np.ones(standards_len, dtype=int)
        distances = np.zeros(standards_len)

        while True:
            for j in range(standards_len):
                squared_distance = 0
                for i in range(self._n):
                    squared_distance += (standards[j][i] - self._W[j, i]) ** 2
                distances[j] = np.sqrt(squared_distance)

            neurons_evaluation = distances * neurons_wins
            winner = neurons_evaluation.argmin()
            neurons_wins[winner] += 1

            weights_temp = np.array(self._W.copy())
            weight_vec = self._W[winner] + self._beta * (standards[winner] - self._W[winner])
            self._W[winner] = weight_vec / np.sqrt(np.sum(weight_vec ** 2))

            if np.array_equal(weights_temp, self._W) or threshold < 0:

                return neurons_wins

    def recognize(self, sample: np.ndarray):
        distances = np.zeros(self._m)
        for j in range(self._m):
            squared_distance = 0
            for i in range(self._n):
                squared_distance += (sample[i] - self._W[j, i]) ** 2
            distances[j] = np.sqrt(squared_distance)

        return distances.argmin()
