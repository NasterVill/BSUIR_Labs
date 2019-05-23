package com.coursework.g75.lightningtuner.src.tuning_attributes;

import android.content.Context;

import com.coursework.g75.lightningtuner.R;

public class Tuning {
    private String name;
    private Note[] notes;

    public Tuning(String name, Note[] notes) {
        this.name = name;
        this.notes = notes;
    }

    public Note[] getNotes() {
        return this.notes;
    }

    public String getName() {
        return name;
    }

    public Note closestNote(double frequency) {
        Note closest = null;

        double optimalDifference = Float.MAX_VALUE;
        for (Note note : notes) {

            double difference = Math.abs(frequency - note.getFrequency());
            if (difference < optimalDifference) {
                closest = note;
                optimalDifference = difference;
            }

        }
        return closest;
    }

    public int closestNoteIndex(double frequency) {
        int index = -1;
        double optimalDifference = Float.MAX_VALUE;

        for (int i = 0; i < notes.length; i++) {
            Note note = notes[i];

            double difference = Math.abs(frequency - note.getFrequency());
            if (difference < optimalDifference) {
                index = i;
                optimalDifference = difference;
            }

        }
        return index;
    }

    public static Tuning getTuning(Context context, String name) {
        if (name.equals(context.getString(R.string.standard_tuning_val))) {
            return new Tuning(name,
                    new Note[]{
                            new Note(82.41F, "E"),
                            new Note(110.00F, "A"),
                            new Note(146.83F, "D"),
                            new Note(196.00F, "G"),
                            new Note(246.94F, "B"),
                            new Note(329.63F, "E"),
                    });
        }
        else if (name.equals(context.getString(R.string.standard_d_tuning))) {
            return new Tuning(name,
                    new Note[]{
                            new Note(73.42F, "D"),
                            new Note(97.99F, "G"),
                            new Note(130.81F, "C"),
                            new Note(174.61F, "F"),
                            new Note(220.00F, "A"),
                            new Note(293.66F, "D"),
                    });
        }
        else if (name.equals(context.getString(R.string.standard_c_tuning))) {
            return new Tuning(name,
                    new Note[]{
                            new Note(65.41F, "C"),
                            new Note(87.31F, "F"),
                            new Note(116.54F, "A#"),
                            new Note(155.56F, "D#"),
                            new Note(195.99F, "G"),
                            new Note(261.66F, "C"),
                    });
        }
        else if (name.equals(context.getString(R.string.drop_d_tuning_val))) {
            return new Tuning(name,
                    new Note[]{
                            new Note(73.42F, "D"),
                            new Note(110.00F, "A"),
                            new Note(146.83F, "D"),
                            new Note(196.00F, "G"),
                            new Note(246.94F, "B"),
                            new Note(329.63F, "E"),
                    });
        }
        else if (name.equals(context.getString(R.string.drop_c_tuning_val))) {
            return new Tuning(name,
                    new Note[]{
                            new Note(65.41F, "C"),
                            new Note(97.99F, "G"),
                            new Note(130.81F, "C"),
                            new Note(174.61F, "F"),
                            new Note(220.00F, "A"),
                            new Note(293.66F, "D"),
                    });
        }
        return null;
    }
}
