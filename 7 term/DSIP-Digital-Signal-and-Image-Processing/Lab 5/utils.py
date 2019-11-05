import random
import numpy as np


def generate_noise(image_matrix: np.ndarray, noise_level: float):
    new_image_matrix = image_matrix.copy()

    random_indices = random.sample(range(0, new_image_matrix.size), int(noise_level * new_image_matrix.size))

    for i in random_indices:
        new_image_matrix[i] = np.abs(new_image_matrix[i] - 1)

    return new_image_matrix


def print_shape(shape: np.array, n, m):
    print('\n')
    for i in range(n):
        for j in range(m):
            print(' '.join(' x' if shape[i * m + j] == 1 else '  '), end='')
        print('\n', end='')
    print('\n')


def get_shapes_with_noise(initial_shape: np.array):
    noised_shapes = []

    for i in range(5, 55, 5):
        noise_level = i / 100

        noised_shapes.append((generate_noise(initial_shape, noise_level), noise_level))

    return noised_shapes
