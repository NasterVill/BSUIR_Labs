package com.coursework.g75.lightningtuner.src.audio_procesing.fequency_tools.windows;

public class HammingWindow implements Window {
    @Override
    public double[] applyWindow(double[] data) {
        double[] newData = new double[data.length];
        for(int i = 0; i < data.length; i++) {
            newData[i] = data[i] * iterationHamming(i, data.length);
        }
        return newData;
    }

    private double iterationHamming(double n, int size) {
        return 0.54 - 0.46 * Math.cos(2 * Math.PI * n / (double)size);
    }
}
