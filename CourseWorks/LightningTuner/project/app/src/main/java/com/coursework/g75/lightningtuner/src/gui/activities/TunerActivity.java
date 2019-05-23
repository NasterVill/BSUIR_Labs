package com.coursework.g75.lightningtuner.src.gui.activities;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.coursework.g75.lightningtuner.R;
import com.coursework.g75.lightningtuner.src.audio_procesing.AudioProcessor;
import com.coursework.g75.lightningtuner.src.gui.views.*;
import com.coursework.g75.lightningtuner.src.utilities.*;
import com.coursework.g75.lightningtuner.src.preferences.Preferences;
import com.coursework.g75.lightningtuner.src.tuning_attributes.*;


public class TunerActivity extends AppCompatActivity {

    public static final String STATE_NEEDLE_POS = "needle_pos";
    public static final String STATE_NOTE_INDEX = "note_index";
    public static final String STATE_LAST_FREQ = "last_freq";
    private static final int PERMISSION_REQUEST_RECORD_AUDIO = 443;

    private Tuning tuning;
    private AudioProcessor audioProcessor;
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private NeedleView needleView;
    private TuningView tuningView;
    private TextView frequencyView;
    private boolean isProcessing = false;
    private int noteIndex;
    private double lastFrequency;


    @Override
    protected void onStart() {
        super.onStart();
        if(AppUtilities.checkPermission(this, Manifest.permission.RECORD_AUDIO)) {
            startAudioProcessing();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (isProcessing) {
            audioProcessor.stop();
            isProcessing = false;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    private void requestPermissions() {
        if (!AppUtilities.checkPermission(this, Manifest.permission.RECORD_AUDIO)) {

            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.RECORD_AUDIO)) {

                AppUtilities.showPermissionDialog(this, getString(R.string.permission_record_audio), new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        ActivityCompat.requestPermissions(TunerActivity.this,
                                new String[]{Manifest.permission.RECORD_AUDIO},
                                PERMISSION_REQUEST_RECORD_AUDIO);
                    }
                });

            } else {

                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.RECORD_AUDIO},
                        PERMISSION_REQUEST_RECORD_AUDIO);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_REQUEST_RECORD_AUDIO: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    startAudioProcessing();
                }
                break;
            }

        }
    }

    private void startAudioProcessing() {
        if (isProcessing)
            return;

        audioProcessor = new AudioProcessor();
        audioProcessor.init();
        audioProcessor.setFrequencyDetectionListener(new AudioProcessor.FrequencyDetectionListener() {
            @Override
            public void onFrequencyDetected(final double frequency) {

                final int index = tuning.closestNoteIndex(frequency);
                final Note note = tuning.getNotes()[index];
                double interval = 1200 * AppUtilities.log2(frequency / note.getFrequency()); // interval in cents
                final float needlePos = (float) (interval / 100);
                final boolean goodPitch = Math.abs(interval) < 10.0;
                runOnUiThread(new Runnable() {
                    @SuppressLint("DefaultLocale")
                    @Override
                    public void run() {
                        tuningView.setSelectedIndex(index, true);
                        needleView.setTickLabel(0.0F, String.format("%.02fHz", note.getFrequency()));
                        needleView.animateTip(needlePos);
                        frequencyView.setText(String.format("%.02fHz", frequency));


                        final View goodPitchView = findViewById(R.id.good_pitch_view);
                        if (goodPitchView != null) {
                            if (goodPitch) {
                                if (goodPitchView.getVisibility() != View.VISIBLE) {
                                    AppUtilities.reveal(goodPitchView);
                                }
                            } else if (goodPitchView.getVisibility() == View.VISIBLE) {
                                AppUtilities.hide(goodPitchView);
                            }
                        }
                    }
                });

                noteIndex = index;
                lastFrequency = frequency;
            }
        });
        isProcessing = true;
        executor.execute(audioProcessor);
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @SuppressLint("DefaultLocale")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        AppUtilities.setupActivityTheme(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        tuning = Tuning.getTuning(this, Preferences.getString(this, getString(R.string.pref_tuning_key), getString(R.string.standard_tuning_val)));

        needleView = (NeedleView) findViewById(R.id.pitch_needle_view);
        needleView.setTickLabel(-1.0F, "-100c");
        needleView.setTickLabel(0.0F, String.format("%.02fHz", tuning.getNotes()[0].getFrequency()));
        needleView.setTickLabel(1.0F, "+100c");

        int primaryTextColor = AppUtilities.getAttrColor(this, android.R.attr.textColorPrimary);

        tuningView = (TuningView) findViewById(R.id.tuning_view);
        tuningView.setTuning(tuning);


        frequencyView = (TextView) findViewById(R.id.frequency_view);
        frequencyView.setText(String.format("%.02fHz", tuning.getNotes()[0].getFrequency()));

        ImageView goodPitchView = (ImageView) findViewById(R.id.good_pitch_view);
        goodPitchView.setColorFilter(primaryTextColor);
        requestPermissions();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        outState.putFloat(STATE_NEEDLE_POS, needleView.getTipPosition());
        outState.putInt(STATE_NOTE_INDEX, noteIndex);
        outState.putDouble(STATE_LAST_FREQ, lastFrequency);
        super.onSaveInstanceState(outState);
    }

    @SuppressLint("DefaultLocale")
    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        needleView.setTipPosition(savedInstanceState.getFloat(STATE_NEEDLE_POS));
        int noteIndex = savedInstanceState.getInt(STATE_NOTE_INDEX);
        needleView.setTickLabel(0.0F, String.format("%.02fHz", tuning.getNotes()[noteIndex].getFrequency()));
        tuningView.setSelectedIndex(noteIndex);
        frequencyView.setText(String.format("%.02fHz", savedInstanceState.getFloat(STATE_LAST_FREQ)));
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_settings:
                AppUtilities.showSettingsActivity(this);
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main, menu);
        return true;
    }
}
