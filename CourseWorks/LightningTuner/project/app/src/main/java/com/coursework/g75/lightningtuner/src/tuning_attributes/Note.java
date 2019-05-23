package com.coursework.g75.lightningtuner.src.tuning_attributes;

public class Note {
    private double frequency;
    private String name;

    public Note(double frequency, String name) {
        this.frequency = frequency;
        this.name = name;
    }

    public double getFrequency() {
        return this.frequency;
    }

    public void setFrequency(double frequency) {
        this.frequency = frequency;
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
