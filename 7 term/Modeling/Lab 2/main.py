import numpy as np
import matplotlib.pyplot as plt
import random
import math
from functools import reduce


SEQUENCE_LENGTH = 1000000


def generate_random_sequence(length):
    return list(map(lambda x: random.random(), range(0, length)))


def generate_uniform_distribution(a, b):
    return list(
        map(
            lambda x: a + (b - a) * x,
            generate_random_sequence(SEQUENCE_LENGTH)
        )
    )


def generate_gauss_distribution(mean, std, n=6):
    return list(
        map(
            lambda x: mean + std * math.sqrt(12 / n) * (sum(generate_random_sequence(n)) - n / 2),
            range(0, SEQUENCE_LENGTH)
        )
    )


def generate_exponential_distribution(lambda_param):
    return list(
        map(
            lambda x: - 1 / lambda_param * math.log(x),
            generate_random_sequence(SEQUENCE_LENGTH)
        )
    )


def generate_gamma_distribution(eta, lambda_param):
    return list(
        map(
            lambda x: -1 / lambda_param * math.log(reduce(lambda y, z: y * z, generate_random_sequence(eta))),
            range(0, SEQUENCE_LENGTH)
        )
    )


#  2 * (b - x) / (b - a)^2
def generate_min_triangle_distribution(a, b):
    return list(
        map(
            lambda x: a + (b - a) * min(generate_random_sequence(2)),
            range(0, SEQUENCE_LENGTH)
        )
    )


#  2 * (x - a) / (b - a)^2
def generate_max_triangle_distribution(a, b):
    return list(
        map(
            lambda x: a + (b - a) * max(generate_random_sequence(2)),
            range(0, SEQUENCE_LENGTH)
        )
    )


def generate_simpson_distribution(a, b):
    return list(
        map(
            lambda x, y: x + y,
            generate_uniform_distribution(a / 2, b / 2),
            generate_uniform_distribution(a / 2, b / 2)
        )
    )


def plot_hist(sequence, num_of_bins):
    weights = np.ones_like(sequence) / float(len(sequence))

    plt.hist(
        sequence,
        bins=np.linspace(min(sequence), max(sequence), num_of_bins + 1),
        weights=weights,
        histtype='bar',
        color='#C40100',
        rwidth=0.9
    )
    plt.show()


def show_distribution(sequence):
    plot_hist(sequence, num_of_bins=20)

    print(
        '\nMean: ', np.mean(sequence),
        '\nVariance: ', np.var(sequence),
        '\nStandard deviation: ', np.std(sequence)
    )


def main():
    print('\nUniform distribution: ')
    show_distribution(generate_uniform_distribution(int(input('A: ')), int(input('B: '))))

    print('\nGauss distribution: ')
    show_distribution(generate_gauss_distribution(int(input('Mean: ')), int(input('Std: ')), int(input('N: '))))

    print('\nExponential distribution: ')
    show_distribution(generate_exponential_distribution(int(input('λ: '))))

    print('\nGamma distribution: ')
    show_distribution(generate_gamma_distribution(int(input('η: ')), int(input('λ: '))))

    print('\nMin-Triangle distribution')
    show_distribution(generate_min_triangle_distribution(int(input('A: ')), int(input('B: '))))

    print('\nMax-Triangle distribution')
    show_distribution(generate_max_triangle_distribution(int(input('A: ')), int(input('B: '))))

    print('\nSimpson distribution')
    show_distribution(generate_simpson_distribution(int(input('A: ')), int(input('B: '))))


if __name__ == '__main__':
    main()
