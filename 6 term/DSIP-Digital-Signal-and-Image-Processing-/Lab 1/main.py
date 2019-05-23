import transform as tr
import numpy as np
import cmath as cm
import matplotlib.pyplot as plt


def main():
    # amount of bins equals to sample rate

    n = 32
    arguments = np.arange(0, n) * np.pi / 6
    function_values = list(map(lambda x: np.sin(x) + np.cos(4 * x), arguments))

    fft_result = tr.conf_dif_fft(function_values, n, 1)
    reverse_fft_result = tr.conf_dif_fft(fft_result, n, -1)

    dft_result = tr.conf_dft(function_values, n, 1)
    reverse_dft_result = tr.conf_dft(dft_result, n, -1)

    # normalizing the transformed output
    for i in range(n):
        fft_result[i] /= n
        dft_result[i] /= n

    frequencies = [x for x in range(-int(n / 2), int(n / 2), 1)]

    # shifting second half of transform results
    shifted_fft = tr.fft_shift(fft_result)
    shifted_dft = tr.fft_shift(dft_result)

    fft_amplitude_values = []
    dft_amplitude_values = []
    fft_phase_values = []
    dft_phase_values = []

    # ejecting magnitude and phase
    for i in range(n):
        fft_amplitude_values.append(abs(shifted_fft[i]))
        dft_amplitude_values.append(abs(shifted_dft[i]))
        fft_phase_values.append(cm.phase(shifted_fft[i]))
        dft_phase_values.append(cm.phase(shifted_dft[i]))

    # possible noise reduction
    threshold_fft = abs(max(fft_result)) / 10000
    threshold_dft = abs(max(dft_result)) / 10000

    for i in range(n):
        if abs(fft_phase_values[i]) < threshold_fft:
            fft_phase_values[i] = 0
        if abs(dft_phase_values[i]) < threshold_dft:
            dft_phase_values[i] = 0

    # plotting part
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)

    ax1.plot(arguments, function_values)
    ax1.set(title='Function plot')
    ax1.grid()

    ax2.stem(frequencies, fft_amplitude_values, markerfmt=' ')
    ax2.set(title='Magnitude spectrum plot (FFT)')
    ax2.grid()

    ax3.stem(frequencies, fft_phase_values, markerfmt=' ')
    ax3.set(title='Phase spectrum plot (FFT)')
    ax3.grid()

    ax4.plot(arguments, reverse_fft_result)
    ax4.set(title='Reverse FFT plot')
    ax4.grid()

    fig2, ((ax5, ax6), (ax7, ax8)) = plt.subplots(2, 2)

    ax5.plot(arguments, function_values)
    ax5.set(title='Function plot')
    ax5.grid()

    ax6.stem(frequencies, dft_amplitude_values, markerfmt=' ')
    ax6.set(title='Magnitude spectrum plot (DFT)')
    ax6.grid()

    ax7.stem(frequencies, dft_phase_values, markerfmt=' ')
    ax7.set(title='Phase spectrum plot (DFT)')
    ax7.grid()

    ax8.plot(arguments, reverse_dft_result)
    ax8.set(title='Reverse DFT plot')
    ax8.grid()

    plt.show()


if __name__ == '__main__':
    main()

