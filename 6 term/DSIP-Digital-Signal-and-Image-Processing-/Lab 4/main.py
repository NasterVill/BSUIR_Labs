import transform as tr
import matplotlib.pyplot as plt


def main():
    max_decomposition_depth = 4
    initial_data = [10, 6, 6, 1, 7, 2, 13, 4, 12, 10, 11, 8, 6, 10, 12, 13]
    initial_len = len(initial_data)
    time_stems = [i for i in range(1, len(initial_data) + 1)]

    wavelet_transform_result = tr.easy_fwt(initial_data, max_decomposition_depth)
    inverse_wavelet_results = []
    for i in range(max_decomposition_depth):
        inverse_wavelet_results.append(tr.easy_ifwt(wavelet_transform_result, i + 1))

    for i in range(max_decomposition_depth):
        extended_result = []
        for x in inverse_wavelet_results[i]:
            extended_result += [x for i in range(int(initial_len / len(inverse_wavelet_results[i])))]

        inverse_wavelet_results[i] = extended_result

    print(wavelet_transform_result)
    print(inverse_wavelet_results[0])

    fig, ((ax1, ax2), (ax3, ax4), (ax5, ax6)) = plt.subplots(3, 2)

    ax1.plot(time_stems, initial_data)
    ax1.set(title='Initial signal')
    ax1.grid()

    ax3.plot(time_stems, inverse_wavelet_results[0])
    ax3.set(title='IFWT level 1')
    ax3.grid()

    ax4.plot(time_stems, inverse_wavelet_results[1])
    ax4.set(title='IFWT level 2')
    ax4.grid()

    ax5.plot(time_stems, inverse_wavelet_results[2])
    ax5.set(title='IFWT level 3')
    ax5.grid()

    ax6.plot(time_stems, inverse_wavelet_results[3])
    ax6.set(title='IFWT level 4')
    ax6.grid()

    fig.delaxes(ax2)

    plt.show()


if __name__ == '__main__':
    main()
