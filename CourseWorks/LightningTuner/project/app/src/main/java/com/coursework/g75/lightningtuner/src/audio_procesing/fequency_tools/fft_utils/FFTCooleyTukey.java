package com.coursework.g75.lightningtuner.src.audio_procesing.fequency_tools.fft_utils;

public class FFTCooleyTukey implements FastFourierTransform
{
    @Override
    public double[] calculateSpectrogram(double[] inputData)
    {
        int length;
        int bitsInLength;
        if (isPowerOfTwo(inputData.length)) {
            length = inputData.length;
            bitsInLength = log2(length) - 1;
        } else {
            bitsInLength = log2(inputData.length);
            length = 1 << bitsInLength;
        }

        double[] data = new double[length * 2];
        for (int i = 0; i < inputData.length; i++) {
            int j = reverseBits(i, bitsInLength) * 2;
            data[j] = inputData[i];
        }

        // Cooley-Tukey
        for (int i = 0; i < bitsInLength; i++) {
            int m = 1 << i;
            int n = m << 1;
            double alpha = -(2 * Math.PI / n);

            for (int k = 0; k < m; k++) {
                // computing e^(-2*pi/N*k)
                // in both cases e^0 = 1, so we don't need to multiply by it
                double oddPartMultiplierReal = Math.cos(alpha * k);
                double oddPartMultiplierImaginary = Math.sin(alpha * k);

                for (int j = k; j < length; j += n) {
                    // never will get out of range because step is n > m and 2 * length = data.length
                    int currentIndex = j << 1;
                    int indexWithOffset = (j + m) << 1;

                    // setting complex args
                    double evenPartReal = data[currentIndex];
                    double evenPartImaginary = data[currentIndex + 1];

                    // multiplying complex args
                    double oddPartReal = data[indexWithOffset] * oddPartMultiplierReal - data[indexWithOffset + 1] * oddPartMultiplierImaginary;
                    double oddPartImaginary =  data[indexWithOffset] * oddPartMultiplierImaginary + data[indexWithOffset + 1] * oddPartMultiplierReal;

                    // adding complex args
                    data[currentIndex] = evenPartReal + oddPartReal;
                    data[currentIndex + 1] = evenPartImaginary + oddPartImaginary;

                    // subtracting complex args
                    data[indexWithOffset] = evenPartReal - oddPartReal;
                    data[indexWithOffset + 1] = evenPartImaginary - oddPartImaginary;
                }
            }
        }

        // calculate spectrogram
        double[] spectrogram = new double[length];
        for (int i = 0, j = 0; i < spectrogram.length; i++, j += 2) {
            spectrogram[i] = Math.sqrt(data[j] * data[j] + data[j + 1] * data[j + 1]);
        }
        return spectrogram;
    }

    private int log2(int n) {
        int i = 0;
        while (n > 0) {
            i++;
            n >>= 1;
        }
        return i;
    }

    private int reverseBits(int n, int bitsCount) {
        int reversed = 0;
        for (int i = 0; i < bitsCount; i++) {
            int nextBit = n & 1;
            n >>= 1;

            reversed <<= 1;
            reversed |= nextBit;
        }
        return reversed;
    }

    private boolean isPowerOfTwo(int n) {
        return n > 1 && (n & (n - 1)) == 0;
    }
}