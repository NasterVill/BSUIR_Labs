from operations import SignalProcessor
import numpy as np
import matplotlib.pyplot as plt


def main():
    n = 64
    arguments = np.arange(0, n) * np.pi / 6
    function_values_1 = list(map(np.sin, arguments))
    function_values_2 = list(map(lambda x: np.sin(4 * x), arguments))

    basic_correlation = SignalProcessor.correlation_convolution(function_values_1, function_values_2, 1)
    print('Basic correlation complexity: {}'.format(SignalProcessor.complexity_counter))
    basic_convolution = SignalProcessor.correlation_convolution(function_values_1, function_values_2, -1)
    print('Basic convolution complexity: {}'.format(SignalProcessor.complexity_counter))

    fft_based_correlation = \
        SignalProcessor.correlation_convolution_fft_based(function_values_1, function_values_2, 1)
    print('FFT-based correlation complexity: {}'.format(SignalProcessor.complexity_counter))
    fft_based_convolution = \
        SignalProcessor.correlation_convolution_fft_based(function_values_1, function_values_2, -1)
    print('FFT-based convolution complexity: {}'.format(SignalProcessor.complexity_counter))

    np_correlation = np.correlate(function_values_1, function_values_2, mode='same')
    np_convolution = np.convolve(function_values_1, function_values_2, mode='same')

    # plotting part
    _, ((ax1, ax2), (ax3, ax4), (ax5, ax6), (ax7, ax8)) = plt.subplots(4, 2)

    ax1.plot(arguments, function_values_1)
    ax1.set(title='First sequence')
    ax1.grid()

    ax2.plot(arguments, function_values_2)
    ax2.set(title='Second sequence')
    ax2.grid()

    ax3.plot(arguments, basic_correlation)
    ax3.set(title='Basic correlation')
    ax3.grid()

    ax4.plot(arguments, basic_convolution)
    ax4.set(title='Basic convolution')
    ax4.grid()

    ax5.plot(arguments, fft_based_correlation)
    ax5.set(title='FFT-based correlation')
    ax5.grid()

    ax6.plot(arguments, fft_based_convolution)
    ax6.set(title='FFT-based convolution')
    ax6.grid()

    ax7.plot(arguments, np_correlation)
    ax7.set(title='Numpy correlation')
    ax7.grid()

    ax8.plot(arguments, np_convolution)
    ax8.set(title='Numpy convolution')
    ax8.grid()

    plt.show()


if __name__ == '__main__':
    main()

