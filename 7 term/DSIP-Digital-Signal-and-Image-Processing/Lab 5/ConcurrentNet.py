import numpy as np


class ConcurrentNet:
    def __init__(self, input_neurons_amount: int, amount_of_classes: int, beta: float):
        self._input_neurons_amount = input_neurons_amount
        self._amount_of_classes = amount_of_classes
        self._beta = beta
        self._W = np.random.uniform(0, 1, [amount_of_classes, input_neurons_amount])

    def train(self, standards: list, epochs: int, threshold: float):
        neurons_wins = np.ones(self._amount_of_classes, dtype=int)

        while True:
            for standard in standards:
                distances = np.zeros(self._amount_of_classes)

                for j in range(self._amount_of_classes):
                    squared_distance = 0
                    for i in range(self._input_neurons_amount):
                        squared_distance += (standard[i] - self._W[j, i]) ** 2
                    distances[j] = np.sqrt(squared_distance)

                neurons_evaluation = distances * neurons_wins
                winner = neurons_evaluation.argmin()
                neurons_wins[winner] += 1

                weights_temp = np.array(self._W.copy())
                weight_vec = self._W[winner] + self._beta * (standard - self._W[winner])
                self._W[winner] = weight_vec / np.sqrt(np.sum(weight_vec ** 2))

            if epochs == 0:
                return

            acceptable_distances = 5
            for standard in standards:
                distances = np.zeros(self._amount_of_classes)

                for j in range(self._amount_of_classes):
                    squared_distance = 0
                    for i in range(self._input_neurons_amount):
                        squared_distance += (standard[i] - self._W[j, i]) ** 2
                    distances[j] = np.sqrt(squared_distance)

                    if min(distances) < threshold:
                        acceptable_distances -= 1

            if acceptable_distances <= 0:
                return

            epochs -= 1

    def recognize(self, sample: np.ndarray):
        distances = np.zeros(self._amount_of_classes)
        for j in range(self._amount_of_classes):
            squared_distance = 0
            for i in range(self._input_neurons_amount):
                squared_distance += (sample[i] - self._W[j, i]) ** 2
            distances[j] = np.sqrt(squared_distance)

        return distances.argmin()
