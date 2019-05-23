import numpy as np
from functools import reduce


def dwht(input_data, direction=1):
    input_length = len(input_data)

    if is_power_of_two(input_length):
        length = input_length
        bits_in_length = int(np.log2(length))
    else:
        bits_in_length = np.log2(input_length)
        length = 1 << bits_in_length

    data = input_data[0:length]

    hadamard_base2_matrix = np.array([[1, 1], [1, -1]])
    hadamard_matrix = []
    for i in range(bits_in_length - 1):
        if i == 0:
            hadamard_matrix = np.kron(hadamard_base2_matrix, hadamard_base2_matrix)
        else:
            hadamard_matrix = np.kron(hadamard_matrix, hadamard_base2_matrix)

    result = np.dot(hadamard_matrix, data)

    if direction == 1:
        result /= length

    return result


def fwht(input_data, direction=1):
    input_length = len(input_data)

    if is_power_of_two(input_length):
        length = input_length
        bits_in_length = int(np.log2(length))
    else:
        bits_in_length = np.log2(input_length)
        length = 1 << bits_in_length

    data = sequency_reordering(input_data)
    # or, to produce a Hadamard ordered result, use
    # data = input_data[:]
    for ldm in range(bits_in_length, 0, -1):
        m = 2 ** ldm
        mh = int(m / 2)
        for k in range(mh):
            for r in range(0, length, m):
                u = data[r + k]
                v = data[r + k + mh]

                data[r + k] = u + v
                data[r + k + mh] = u - v

    if direction == 1:
        data = list(np.divide(data, length))

    return data


def sequency_reordering(input_data):
    input_length = len(input_data)

    if is_power_of_two(input_length):
        length = input_length
        bits_in_length = int(np.log2(length))
    else:
        bits_in_length = np.log2(input_length)
        length = 1 << bits_in_length

    data = input_data[0:length]
    for i in range(length):
        data[i] = input_data[gray_to_binary(reverse_bits(i, bits_in_length))]

    return data


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


def gray_to_binary(num):
    mask = num
    while mask != 0:
        mask >>= 1
        num ^= mask

    return num


def dwt(data, direction=1):
    time_offset = 0.001
    length = len(data)

    transformed_result = []
    for n in range(length):
        temp = sum(
            [
                data[i] * walsh_function(
                    n if direction == 1 else i, (i if direction == 1 else n) / length + time_offset, length
                ) for i in range(length)
            ]
        )

        transformed_result.append(temp)

    if direction == 1:
        transformed_result = list(map(lambda x: x / length, transformed_result))

    return transformed_result


def rademacher_function(t, k):
    x = np.sin(2 ** k * np.pi * t)

    return 1 if x > 0 else -1


def walsh_function(n, t, length):
    r = int(np.log2(length))
    rademacher_values = \
        [rademacher_function(t, k) ** xor_bit(bit(n, k - 1), bit(n, k)) for k in range(1, r + 1)]

    return reduce(lambda x, y: x * y, rademacher_values)


def xor_bit(a, b):
    return +(bool(a) ^ bool(b))


def bit(num, pos):
    if num == 0:
        return 0

    mask = 1
    mask <<= pos

    return 1 if num & mask != 0 else 0
