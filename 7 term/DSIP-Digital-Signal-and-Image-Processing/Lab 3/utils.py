import random
import numpy as np


def normalize(image_matrix: np.ndarray):
    return np.where(image_matrix == 0, -1, image_matrix)


def noise_shape(image_matrix: np.ndarray, noise_level: float):
    new_image_matrix = image_matrix.copy()

    random_indices = random.sample(range(0, new_image_matrix.size), int(noise_level * new_image_matrix.size))

    for i in random_indices:
        y = int(i / image_matrix.shape[0])
        x = i % image_matrix.shape[1]

        new_image_matrix[y][x] = -new_image_matrix[y][x]

    return new_image_matrix
