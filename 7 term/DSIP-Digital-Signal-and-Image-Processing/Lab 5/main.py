import time

from shapes import shapes, N, M
from SelfOrganizingMap import SelfOrganizingMap
from utils import get_shapes_with_noise, print_shape


def main():
    BETA = 0.25

    net = SelfOrganizingMap(N * N, len(shapes), BETA)

    print('Training')
    start = time.time()
    net.train(shapes, 100)
    end = time.time()
    print(f'Training finished in {end - start} seconds')

    for shape in shapes:
        noised_shapes = get_shapes_with_noise(shape)
        for noised_shape in noised_shapes:
            winner = net.recognize(noised_shape[0])
            print('---------------------------------------------------------\n')

            print(f'Initial shape: ')
            print_shape(shape, N, M)
            print(f'Tested shape with noise level of {noised_shape[1]}: ')
            print_shape(noised_shape[0], N, M)
            print(f'Recognized: ')
            print_shape(shapes[winner], N, M)

        print('|******************************************************|\n')


if __name__ == '__main__':
    main()
