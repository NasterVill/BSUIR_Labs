import transform as tr
import numpy as np
import matplotlib.pyplot as plt


def main():
    n = 64
    arguments = np.arange(0, n) * np.pi / 6
    function_values = list(map(lambda x: np.sin(x) + np.cos(4 * x), arguments))

    dwt_result = tr.dwt(function_values, 1)
    dwht_result = tr.dwht(function_values, 1)
    fwht_result = tr.fwht(function_values, 1)
    reverse_dwt_result = tr.dwt(dwt_result, -1)
    reverse_dwht_result = tr.dwht(dwht_result, -1)
    reverse_fwht_result = tr.fwht(fwht_result, -1)

    # plotting part
    fig, ((ax1, ax2), (ax3, ax4), (ax5, ax6)) = plt.subplots(3, 2)

    ax1.plot(arguments, function_values)
    ax1.set(title='Function plot')
    ax1.grid()

    ax3.plot(arguments, dwt_result)
    ax3.set(title='DWT coefficients to secuency indices')
    ax3.grid()

    ax4.plot(arguments, fwht_result)
    ax4.set(title='FWHT coefficients to secuency indices')
    ax4.grid()

    ax5.plot(arguments, reverse_dwt_result)
    ax5.set(title='Reverse DWT plot')
    ax5.grid()

    ax6.plot(arguments, reverse_fwht_result)
    ax6.set(title='Reverse FWHT plot')
    ax6.grid()

    fig.delaxes(ax2)

    plt.show()


if __name__ == '__main__':
    main()
