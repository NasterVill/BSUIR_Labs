import random
import numpy as np


def generate_noise(image_matrix: np.ndarray, noise_level: float):
    new_image_matrix = image_matrix.copy()

    random_indices = random.sample(range(0, new_image_matrix.size), int(noise_level * new_image_matrix.size))

    for i in random_indices:
        y = int(i / image_matrix.shape[0])
        x = i % image_matrix.shape[1]

        new_image_matrix[y][x] = np.abs(new_image_matrix[y][x] - 1)

    return new_image_matrix
