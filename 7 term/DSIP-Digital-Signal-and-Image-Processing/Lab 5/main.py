import time

from shapes import shapes, N, M
from ConcurrentNet import ConcurrentNet
from utils import get_shapes_with_noise, print_shape


def main():
    BETA = 0.25

    net = ConcurrentNet(N * N, len(shapes) - 1, BETA)

    print('Training')
    start = time.time()
    net.train(shapes, 1500, 0.0000001)
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
            print(f'Recognized class: {winner}')
            # print_shape(shapes[winner], N, M)

        print('|******************************************************|\n')


if __name__ == '__main__':
    main()
