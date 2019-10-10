import numpy as np

import utils
from HopfieldNetwork import HopfieldNetwork


def process_shape(initial_shape: np.ndarray, net: HopfieldNetwork):
    nosied_shapes = []

    for i in range(5, 105, 5):
        noise_level = i / 100

        nosied_shapes.append(utils.noise_shape(initial_shape, noise_level))

    for shape in nosied_shapes:
        recognized, adjusted_shape, iterations = net.recognize(shape)

        print(recognized)
        print(adjusted_shape)
        print(iterations)


def main():
    first_shape = np.array([
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
        [0, 1, 1, 0, 0, 0, 1, 1, 0, 0],
        [0, 0, 1, 1, 0, 1, 1, 0, 0, 0],
        [0, 0, 0, 1, 1, 1, 0, 0, 0, 0],
        [0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
        [0, 0, 1, 1, 0, 0, 0, 0, 0, 0],
        [0, 1, 1, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
    ])

    second_shape = np.array([
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
    ])

    third_shape = np.array([
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 1, 1, 0, 0, 0, 0, 1, 1, 1],
        [1, 1, 1, 1, 0, 0, 1, 1, 1, 1],
        [1, 1, 0, 1, 1, 1, 1, 0, 1, 1],
        [1, 1, 0, 0, 1, 1, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 1, 0, 0, 0, 0, 0, 0, 1, 1],
    ])

    # transform shapes to 1 / -1 values
    first_shape = utils.normalize(first_shape)
    second_shape = utils.normalize(second_shape)
    third_shape = utils.normalize(third_shape)

    dimension = 10

    # create network instance
    net = HopfieldNetwork(dimension)

    # teaching the net
    net.teach(first_shape)
    net.teach(second_shape)
    net.teach(third_shape)

    # recreating nosified shapes
    process_shape(first_shape, net)
    process_shape(second_shape, net)
    process_shape(third_shape, net)


if __name__ == '__main__':
    main()

