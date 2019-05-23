package com.coursework.g75.lightningtuner.src.utilities;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.os.Build;
import android.support.v4.content.ContextCompat;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewAnimationUtils;
import android.content.Intent;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;


import com.coursework.g75.lightningtuner.R;
import com.coursework.g75.lightningtuner.src.gui.activities.SettingsActivity;

import com.coursework.g75.lightningtuner.src.preferences.Preferences;

public final class AppUtilities {
    private static final double LOG2 = Math.log(2);

    private AppUtilities() {}

    public static float dpToPixels(Context context, float dp) {
        Resources r = context.getResources();
        return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 14, r.getDisplayMetrics());
    }

    public static int getAttrColor(Context context, int attrId) {
        TypedValue typedValue = new TypedValue();
        Resources.Theme theme = context.getTheme();
        theme.resolveAttribute(attrId, typedValue, true);
        if (typedValue.type >= TypedValue.TYPE_FIRST_COLOR_INT && typedValue.type <= TypedValue.TYPE_LAST_COLOR_INT) {
            return typedValue.data;
        } else {
            return context.getResources().getColor(typedValue.resourceId);
        }
    }

    public static double log2(double v) {
        return Math.log(v) / LOG2;
    }

    public static void reveal(View view) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            // get the center for the clipping circle
            int cx = view.getWidth() / 2;
            int cy = view.getHeight() / 2;

            // get the final radius for the clipping circle
            float finalRadius = (float) Math.hypot(cx, cy);

            // create the animator for this view (the start radius is zero)
            Animator anim =
                    ViewAnimationUtils.createCircularReveal(view, cx, cy, 0, finalRadius);

            // make the view visible and start the animation
            view.setVisibility(View.VISIBLE);
            anim.start();
        } else {
            view.setVisibility(View.VISIBLE);
            view.animate().alpha(1f).start();
        }
    }

    public static void hide(final View view) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            // get the center for the clipping circle
            int cx = view.getWidth() / 2;
            int cy = view.getHeight() / 2;

            // get the initial radius for the clipping circle
            float initialRadius = (float) Math.hypot(cx, cy);

            // create the animation (the final radius is zero)
            Animator anim =
                    ViewAnimationUtils.createCircularReveal(view, cx, cy, initialRadius, 0);

            // make the view invisible when the animation is done
            anim.addListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                    view.setVisibility(View.INVISIBLE);
                }
            });

            // start the animation
            anim.start();
        } else {
            view.animate().alpha(0f).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                    view.setVisibility(View.INVISIBLE);
                }
            }).start();
        }
    }

    public static void setupActivityTheme(Activity activity) {
        boolean dark = Preferences.getBoolean(activity, activity.getString(R.string.pref_dark_theme_key), true);
        if (dark) {
            activity.setTheme(R.style.AppThemeDark);
        } else {
            activity.setTheme(R.style.AppThemeLight);
        }
    }

    public static boolean checkPermission(Context context, String permission) {
        return ContextCompat.checkSelfPermission(context,
                permission) == PackageManager.PERMISSION_GRANTED || Build.VERSION.SDK_INT < Build.VERSION_CODES.M;
    }

    public static void showSettingsActivity(Context context) {
        Intent intent = new Intent(context, SettingsActivity.class);
        context.startActivity(intent);
    }

    public static void showPermissionDialog(Context context, String message, DialogInterface.OnClickListener listener) {
        new AlertDialog.Builder(context)
                .setTitle(R.string.permission)
                .setMessage(message)
                .setPositiveButton(android.R.string.ok, listener)
                .show();
    }
}
