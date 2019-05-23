package com.coursework.g75.lightningtuner.src.gui.views;

import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

import com.coursework.g75.lightningtuner.R;
import com.coursework.g75.lightningtuner.src.tuning_attributes.*;

public class TuningView extends View {
    private int selectedIndex;
    private Tuning tuning;
    private float tuningItemWidth;
    private Paint paint = new Paint();
    private Rect tempRect = new Rect();
    private int normalTextColor;
    private int selectedTextColor;
    private float offset = 0;
    private ValueAnimator offsetAnimator = null;

    public TuningView(Context context) {
        this(context, null);
    }

    public TuningView(Context context, AttributeSet attrs) {
        this(context, attrs, R.attr.tuningViewStyle);
    }

    public TuningView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs, defStyleAttr);
    }

    private void init(Context context, AttributeSet attrs, int defStyleAttr) {
        final TypedArray array = context.obtainStyledAttributes(attrs,
                R.styleable.TuningView, defStyleAttr,
                R.style.DarkTuningView);
        normalTextColor = array.getColor(R.styleable.TuningView_normalTextColor, 0);
        selectedTextColor = array.getColor(R.styleable.TuningView_selectedTextColor, 0);
        float textSize = array.getDimension(R.styleable.TuningView_textSize, 0);
        paint.setTextSize(textSize);
        tuningItemWidth = array.getDimension(R.styleable.TuningView_itemWidth, 0);
        array.recycle();
    }

    public int getSelectedIndex() {
        return selectedIndex;
    }

    public void setSelectedIndex(int selectedIndex, boolean animate) {
        if (selectedIndex == this.selectedIndex)
            return;

        this.selectedIndex = selectedIndex;
        float newOffset = (getWidth() - tuningItemWidth) / 2F - this.selectedIndex * tuningItemWidth;
        stopAnimation();
        if (animate) {
            offsetAnimator = ValueAnimator.ofFloat(offset, newOffset);
            offsetAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
                @Override
                public void onAnimationUpdate(ValueAnimator animation) {
                    offset = (float) animation.getAnimatedValue();
                    invalidate();
                }
            });
            offsetAnimator.start();
        } else {
            offset = newOffset;
        }
    }

    public void setSelectedIndex(int selectedIndex) {
        setSelectedIndex(selectedIndex, false);
    }

    private void stopAnimation() {
        if (offsetAnimator != null) {
            offsetAnimator.cancel();
        }
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        stopAnimation();
        offset = (w - tuningItemWidth) / 2f - selectedIndex * tuningItemWidth;
    }

    public float getTextSize() {
        return paint.getTextSize();
    }

    public void setTextSize(float textSize) {
        paint.setTextSize(textSize);
    }

    public int getNormalTextColor() {
        return normalTextColor;
    }

    public void setNormalTextColor(int color) {
        normalTextColor = color;
    }

    public int getSelectedTextColor() {
        return selectedTextColor;
    }

    public void setSelectedTextColor(int selectedTextColor) {
        this.selectedTextColor = selectedTextColor;
    }

    public Tuning getTuning() {
        return tuning;
    }

    public void setTuning(Tuning tuning) {
        this.tuning = tuning;
    }

    public float getTuningItemWidth() {
        return tuningItemWidth;
    }

    public void setTuningItemWidth(float tuningItemWidth) {
        this.tuningItemWidth = tuningItemWidth;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (tuning == null)
            return;

        int height = getHeight();


        for (int i = 0; i < tuning.getNotes().length; i++) {
            if (i == selectedIndex) {
                paint.setColor(selectedTextColor);
            } else {
                paint.setColor(normalTextColor);
            }
            String text =  tuning.getNotes()[i].getName();
            float textWidth = paint.measureText(text);
            paint.getTextBounds(text, 0, text.length(), tempRect);
            canvas.drawText(text, offset + i * tuningItemWidth + (tuningItemWidth - textWidth) / 2f, (height + tempRect.height()) / 2f, paint);
        }


    }
}
