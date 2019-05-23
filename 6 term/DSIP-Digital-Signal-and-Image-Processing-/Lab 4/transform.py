def easy_fwt(signal: list, decomposition_depth: int):
    if len(signal) == 1 or decomposition_depth == 0:
        return signal

    detail = [(signal[i] - signal[i + 1]) / 2 for i in range(0, len(signal), 2)]
    approximation = [(signal[i] + signal[i + 1]) / 2 for i in range(0, len(signal), 2)]

    result = detail + easy_fwt(approximation, decomposition_depth - 1)

    return result


def easy_ifwt(wavelet_coefficients: list, decomposition_depth: int):
    interpolated_depth = np.log2(len(wavelet_coefficients)) - decomposition_depth + 1

    if interpolated_depth <= 0:
        raise Exception('Param decomposition_depth is invalid! Got: {}'.format(decomposition_depth))

    return __easy_ifwt(wavelet_coefficients, interpolated_depth)


def __easy_ifwt(wavelet_coefficients: list, decomposition_depth: int):
    if len(wavelet_coefficients) == 1:
        return wavelet_coefficients

    detail = [x for x in wavelet_coefficients[int(len(wavelet_coefficients) / 2)::]]

    approximation = __easy_ifwt(detail, decomposition_depth - 1)

    if decomposition_depth > 1:
        return approximation

    result = []
    for i in range(int(len(wavelet_coefficients) / 2)):
        result.append(approximation[i] + wavelet_coefficients[i])
        result.append(approximation[i] - wavelet_coefficients[i])

    return result
