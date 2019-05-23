package com.coursework.g75.lightningtuner.src.audio_procesing.fequency_tools;

import com.coursework.g75.lightningtuner.src.audio_procesing.fequency_tools.fft_utils.FastFourierTransform;
import com.coursework.g75.lightningtuner.src.audio_procesing.fequency_tools.windows.Window;
import com.coursework.g75.lightningtuner.src.auxiliary_types.Pair;

import java.util.Arrays;

public class FrequencyDetector
{
    private static final int peaksAmount = 15;

    public double findFrequency(double[] soundData, int sampleRate, double minFreq, double maxFreq, FastFourierTransform specificFFT, Window window) {
        if(specificFFT == null) {
            throw new IllegalArgumentException("The value of param FastFourierTransform can not be null!");
        }

        double[] processedData;
        if(window != null) {
            processedData = window.applyWindow(soundData);
        } else {
            processedData = soundData;
        }

        double[] spectrogram = specificFFT.calculateSpectrogram(processedData);

        int usefulMinSpectrum = Math.max(0,
                (int)(minFreq * spectrogram.length / sampleRate));
        int usefulMaxSpectrum = Math.min(spectrogram.length,
                (int)(maxFreq * spectrogram.length / sampleRate) + 1);

        int[] peakIndices;
        peakIndices = findPeaks(spectrogram, usefulMinSpectrum, usefulMaxSpectrum - usefulMinSpectrum,
                peaksAmount);
        if (Arrays.binarySearch(peakIndices, usefulMinSpectrum) >= 0) {
            // no detectable sound due to appearance of a minimal
            // useful spectrum bin
            return 0;
        }

        // select fragment to check peak values: data offset
        final int verifyFragmentOffset = 0;
        int verifyFragmentLength = (int)(sampleRate / minFreq);


        // trying all peaks to find one with smaller difference value
        double minPeakValue = Double.MAX_VALUE;
        int minOptimalInterval = 0;
        for (int i = 0; i < peakIndices.length; i++) {
            int index = peakIndices[i];
            int binIntervalStart = spectrogram.length / (index + 1);
            int binIntervalEnd = spectrogram.length / index;
            int tempInterval = 0;
            double tempPeakValue = 0;
            // scan bins frequencies/intervals
            Pair<Double, Integer> scanResult = scanSignalIntervals(processedData, verifyFragmentOffset, verifyFragmentLength,
                    binIntervalStart, binIntervalEnd);

            tempPeakValue = scanResult.getFirst();
            tempInterval = scanResult.getSecond();

            if (tempPeakValue < minPeakValue) {
                minPeakValue = tempPeakValue;
                minOptimalInterval = tempInterval;
            }
        }

        return (double)sampleRate / minOptimalInterval;
    }

    private Pair<Double, Integer> scanSignalIntervals(double[] x, int index, int length,
                                                             int intervalMin, int intervalMax) {
        double optimalValue = Double.MAX_VALUE;
        int optimalInterval = 0;

        // distance between min and max range value can be big
        // limiting it to the fixed value
        final int maxAmount = 30;
        int steps = intervalMax - intervalMin;
        if (steps > maxAmount)
            steps = maxAmount;
        else if (steps <= 0)
            steps = 1;

        // trying all intervals in the range to find one with
        // smaller difference in signal waves
        for (int i = 0; i < steps; i++) {
            int interval = intervalMin + (intervalMax - intervalMin) * i / steps;

            double sum = 0;
            for (int j = 0; j < length; j++) {
                double diff = x[index + j] - x[index + j + interval];
                sum += diff * diff;
            }
            if (optimalValue > sum) {
                optimalValue = sum;
                optimalInterval = interval;
            }
        }

        return new Pair<>(optimalValue, optimalInterval);
    }

    private int[] findPeaks(double[] values, int index, int length, int peaksCount) {
        double[] peakValues = new double[peaksCount];
        int[] peakIndices = new int[peaksCount];

        for (int i = 0; i < peaksCount; i++) {
            peakValues[i] = values[peakIndices[i] = i + index];
        }

        // find min peaked value
        double minStoredPeak = peakValues[0];
        int minIndex = 0;
        for (int i = 1; i < peaksCount; i++) {
            if (minStoredPeak > peakValues[i]) minStoredPeak = peakValues[minIndex = i];
        }

        for (int i = peaksCount; i < length; i++) {
            if (minStoredPeak < values[i + index]) {
                // replace the min peaked value with bigger one
                peakValues[minIndex] = values[peakIndices[minIndex] = i + index];

                // and find min peaked value again
                minStoredPeak = peakValues[minIndex = 0];
                for (int j = 1; j < peaksCount; j++) {
                    if (minStoredPeak > peakValues[j]) minStoredPeak = peakValues[minIndex = j];
                }
            }
        }

        return peakIndices;
    }
}