import cmath
import numpy as np


def generate_function_values(function, beginning_value, amount, step):
    return [function(x) for x in (np.arange(0, amount) * step + beginning_value)]


# Discrete fourier transform
def dft(input_data, length):
    if length:
        n = length
    else:
        n = len(input_data)

    output = []
    for m in range(n):  # For each output element
        s = complex(0)
        w = -2j * cmath.pi * m / n
        for k in range(n):  # For each input element
            angle = w * k
            s += input_data[k] * cmath.exp(-angle)
        output.append(s)
    return output


# Inverse discrete fourier transform
def inverse_dft(input_data, length):
    if length:
        n = length
    else:
        n = len(input_data)

    output = []
    for m in range(n):
        s = 0
        w = 2j * cmath.pi * m / n
        for k in range(n):
            angle = w * k
            s += input_data[k] * cmath.exp(angle)
        s /= n
        output.append(s)
    return output


# Configurable dft
def conf_dft(input_data, length, direction):
    if length:
        n = length
    else:
        n = len(input_data)

    output = []
    for m in range(n):
        s = complex(0)
        w = -2j * cmath.pi * m / n
        for k in range(n):
            angle = w * k
            s += input_data[k] * cmath.exp(direction * angle)
        if direction == -1:
            s /= n
        output.append(s)
    return output


# dif-fft implementation
def conf_dif_fft(input_data, input_length, direction):
    fft_result = dif_fft(input_data, input_length, direction)

    if direction == -1:
        for i in range(input_length):
            fft_result[i] /= input_length

    return fft_result


def dif_fft(input_data, input_length, direction=1):
    if is_power_of_two(input_length):
        length = input_length
        bits_in_length = int(np.log2(length))
    else:
        bits_in_length = np.log2(input_length)
        length = 1 << bits_in_length

    data = []
    for i in range(length):
        data.append(complex(input_data[i]))

    for ldm in range(bits_in_length, 0, -1):
        m = 2 ** ldm
        mh = int(m / 2)
        for k in range(mh):
            w = np.exp(direction * -2j * np.pi * k / m)
            for r in range(0, length, m):
                u = data[r + k]
                v = data[r + k + mh]

                data[r + k] = u + v
                data[r + k + mh] = (u - v) * w

    for i in range(length):
        j = reverse_bits(i, bits_in_length)
        if j > i:
            temp = data[j]
            data[j] = data[i]
            data[i] = temp

    return data


def fft_shift(fft_result):
    length = len(fft_result)

    first_half = fft_result[0:int(length / 2)]
    second_half = first_half[:]
    first_half.reverse()
    shifted_result = first_half + second_half

    return shifted_result


def is_power_of_two(n):
    return n > 1 and (n & (n - 1)) == 0


def reverse_bits(n, bits_count):
    reversed_value = 0

    for i in range(bits_count):
        next_bit = n & 1
        n >>= 1

        reversed_value <<= 1
        reversed_value |= next_bit

    return reversed_value
