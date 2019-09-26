import numpy as np
import matplotlib.pyplot as plt


def generate_sequence(a, m, rn, length):
    sequence = []

    for x in range(0, length):
        rn = a * rn % m
        sequence.append(rn / m)

    return sequence


def compute_implicit_criteria(sequence):
    criteria_matchers = 0

    for i in range(0, len(sequence), 2):
        if sequence[i] ** 2 + sequence[i + 1] ** 2 < 1:
            criteria_matchers += 1

    return 2 * criteria_matchers / len(sequence), np.pi / 4


def compute_period(sequence):
    period_indices = []

    for i in range(0, len(sequence)):
        if sequence[i] == sequence[-1]:
            period_indices.append(i)

    return period_indices[1] - period_indices[0]


def compute_aperiodic_interval(sequence, period):
    aperiodic_length = 0

    while sequence[aperiodic_length] != sequence[aperiodic_length + period]:
        aperiodic_length += 1

    return period + aperiodic_length


def plot_hist(sequence, num_of_bins):
    weights = np.ones_like(sequence) / float(len(sequence))

    plt.hist(
        sequence,
        bins=np.linspace(0, 1, num_of_bins + 1),
        weights=weights,
        histtype='bar',
        color='#C40100',
        rwidth=0.9
    )
    plt.hlines(1 / num_of_bins, 0, 1)
    plt.show()


def main():
    sequence = generate_sequence(
        126398,     # a
        911201,     # r0
        1,          # m
        1000000     # n
    )
    plot_hist(sequence, num_of_bins=20)

    print(
        '\nExpected Value: ', np.mean(sequence),
        '\nVariance: ', np.var(sequence),
        '\nStandard deviation: ', np.std(sequence)
    )

    actual, expected = compute_implicit_criteria(sequence)
    print('\nActual: ', actual, '\nExpected: ', expected)

    period = compute_period(sequence)
    aperiodic_interval = compute_aperiodic_interval(sequence, period)
    print('\nPeriod: ', period, '\nAperiodic interval: ', aperiodic_interval)


if __name__ == '__main__':
    main()
