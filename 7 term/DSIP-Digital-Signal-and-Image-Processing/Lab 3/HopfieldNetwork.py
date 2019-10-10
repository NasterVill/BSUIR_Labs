import random
import math
import numpy as np


class HopfieldNetwork:
    def __init__(self, dimension: int, threshold: int = 1000):
        self._taught_shapes = []
        self._dimension = dimension
        self._adjusted_shape = None
        self._amount_of_neurons = int(math.pow(dimension, 2))

        self._W = np.zeros(
            (self._amount_of_neurons, self._amount_of_neurons),
            dtype=float
        )

        self._threshold = threshold
        self._useless_iteration_count = 0

    def teach(self, shape):
        self._taught_shapes.append(shape)
        for i in range(0, self._amount_of_neurons):
            for j in range(0, self._amount_of_neurons):
                if i == j:
                    self._W[i][j] = 0
                else:
                    y1 = int(i / self._dimension)
                    x1 = i % self._dimension

                    y2 = int(j / self._dimension)
                    x2 = j % self._dimension

                    self._W[i][j] += (shape[y1][x1] * shape[y2][x2])

    def recognize(self, shape: np.ndarray):
        self._useless_iteration_count = 0
        self._adjusted_shape = shape
        iterations_amount = 0

        while not self._is_shape_recognized():
            iterations_amount += 1

            self._recognition_step()

            if self._useless_iteration_count >= self._threshold:
                return False, self._adjusted_shape, iterations_amount

        return True, self._adjusted_shape, iterations_amount

    def _recognition_step(self):
        index = random.randint(0, self._amount_of_neurons - 1)

        temp = 0
        for i in range(0, self._amount_of_neurons):
            y = int(i / self._dimension)
            x = i % self._dimension
            temp += (self._adjusted_shape[y][x] * self._W[i][index])

        adjusted_value = 1 if temp > 0 else -1

        y = int(index / self._dimension)
        x = index % self._dimension

        if adjusted_value != self._adjusted_shape[y][x]:
            self._adjusted_shape[y][x] = adjusted_value
            self._useless_iteration_count = 0
        else:
            self._useless_iteration_count += 1

    def _is_shape_recognized(self):
        for shape in self._taught_shapes:
            if np.array_equal(self._adjusted_shape, shape):
                return True

        return False
