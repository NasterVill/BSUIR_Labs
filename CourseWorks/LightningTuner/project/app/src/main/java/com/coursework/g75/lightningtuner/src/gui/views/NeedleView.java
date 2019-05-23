package com.coursework.g75.lightningtuner.src.gui.views;

import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;
import java.util.HashMap;
import java.util.Map;

import com.coursework.g75.lightningtuner.R;
import com.coursework.g75.lightningtuner.src.utilities.AppUtilities;

public class NeedleView extends View {

    private double angle;
    private Paint paint;
    private float strokeWidth;
    private float textStrokeWidth;
    private float tickLabelTextSize;
    private float arcOffset;
    private float tickLength;
    private int needleColor;
    private int smallTicksColor;
    private int bigTicksColor;
    private int textColor;
    private Map<Float, String> tickLabels = new HashMap<>();
    private float tipPosition;


    public NeedleView(Context context) {
        this(context, null);
    }

    public NeedleView(Context context, AttributeSet attrs) {
        this(context, attrs, R.attr.needleViewStyle);
    }

    public NeedleView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs, defStyleAttr);
    }

    private void init(Context context, AttributeSet attrs, int defStyleAttr) {
        paint = new Paint();
        paint.setAntiAlias(true);
        textStrokeWidth = paint.getStrokeWidth();
        strokeWidth = getResources().getDimension(R.dimen.needle_view_stroke_width);
        arcOffset = getResources().getDimension(R.dimen.needle_view_ticks_margin_top);
        tickLabelTextSize = getResources().getDimension(R.dimen.needle_view_tick_label_text_size);
        tickLength = AppUtilities.dpToPixels(context, 5);
        setTipPosition(0);

        final TypedArray array = context.obtainStyledAttributes( attrs,
                R.styleable.NeedleView, defStyleAttr,
                R.style.LightNeedleView);
        needleColor = array.getColor(R.styleable.NeedleView_needleColor, 0);
        smallTicksColor = array.getColor(R.styleable.NeedleView_smallTicksColor, 0);
        bigTicksColor = array.getColor(R.styleable.NeedleView_bigTicksColor, 0);
        textColor = array.getColor(R.styleable.NeedleView_textColor, 0);
        array.recycle();

    }

    public void animateTip(float toPos) {

        toPos = Math.min(1.0F, Math.max(-1.0F, toPos));
        int width = getWidth() - getPaddingLeft() - getPaddingRight();
        int height = getHeight() - getPaddingTop() - getPaddingBottom();

        double toAngle;
        if(height > width/2f)
            toAngle = 90 + toPos * (90 - Math.toDegrees(Math.acos(((width) / 2.0F - strokeWidth) / height)));
        else
            toAngle = 90 + toPos * 90;

        ValueAnimator animator = ValueAnimator.ofFloat((float) angle, (float) toAngle);
        animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                angle = (float) animation.getAnimatedValue();
                invalidate();
            }
        });
        animator.setDuration(200);
        animator.start();
    }

    public double getAngle() {
        return angle;
    }

    public float getTipPosition() {
        return tipPosition;
    }

    public void setTipPosition(float pos) {
        tipPosition = Math.min(1.0F, Math.max(-1.0F, pos));
        int width = getWidth() - getPaddingLeft() - getPaddingRight();
        int height = getHeight() - getPaddingTop() - getPaddingBottom();
        if(height > width/2f)
            angle = 90 + pos * (90 - Math.toDegrees(Math.acos(((width) / 2.0F - strokeWidth) / height)));
        else
            angle = 90 + pos * 90;

    }

    public void setTickLabel(float pos, String label) {
        tickLabels.put(pos, label);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        int width = getWidth() - getPaddingLeft() - getPaddingRight();
        int height = getHeight() - getPaddingTop() - getPaddingBottom();
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeCap(Paint.Cap.BUTT);

        drawTickLabels(canvas, width, height);
        float tickLabelHeight = paint.descent() - paint.ascent();

        drawTicks(canvas, width, height, tickLabelHeight);
        drawNeedle(canvas, width, height, tickLabelHeight);
    }

    private void drawTickLabels(Canvas canvas, int width, int height) {
        float cx = width / 2.0F + getPaddingLeft();
        float cy = height + getPaddingTop();
        paint.setColor(textColor);
        paint.setTextSize(tickLabelTextSize);
        paint.setStrokeWidth(textStrokeWidth);
        for (Map.Entry<Float, String> entry : tickLabels.entrySet()) {
            String text = entry.getValue();
            float textWidth = paint.measureText(text);
            float pos = entry.getKey();
            if (pos == 0) {
                canvas.drawText(text, (width - textWidth) / 2.0F + getPaddingLeft(), getPaddingTop() - paint.ascent() / 2.0F, paint);
            } else {
                float angle = (float) (pos * (90 - Math.toDegrees(Math.acos((width / 2.0F) / height))));
                canvas.save();
                canvas.rotate(angle, cx, cy);
                if (pos > 0) {
                    //right
                    canvas.drawText(text, width / 2.0F - textWidth + getPaddingLeft(), getPaddingTop(), paint);
                } else {
                    //left
                    canvas.drawText(text, width / 2.0F + getPaddingLeft(), getPaddingTop(), paint);
                }
                canvas.restore();
            }

        }
    }

    private void drawNeedle(Canvas canvas, int width, int height, float tickLabelHeight) {
        paint.setStrokeWidth(strokeWidth);
        paint.setStrokeCap(Paint.Cap.SQUARE);
        paint.setColor(needleColor);
        double angleRad = Math.toRadians(angle);
        float needleLength = height - arcOffset - tickLabelHeight;
        float cx = width / 2.0F + getPaddingLeft();
        float cy = height + getPaddingTop();
        float tipX = (float) (-needleLength * Math.cos(angleRad) + cx);
        float tipY = (float) (-needleLength * Math.sin(angleRad) + cy);

        canvas.drawLine(cx, cy, tipX, tipY, paint);
        paint.setColor(textColor);
        paint.setStyle(Paint.Style.FILL);
        canvas.drawCircle(cx, cy, strokeWidth, paint);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(strokeWidth /2f);
        canvas.drawCircle(cx, cy, strokeWidth *1.5f, paint);

    }

    private void drawTicks(Canvas canvas, int width, int height, float tickLabelHeight) {
        paint.setStrokeCap(Paint.Cap.SQUARE);
        float cx = width / 2.0F + getPaddingLeft();
        float cy = height + getPaddingTop();
        float startAngle;
        if(height > width /2f)
            startAngle = (float) Math.toDegrees(Math.acos((width / 2.0F - strokeWidth) / height));
        else
            startAngle = 0;
        float currentAngle = startAngle;
        float endAngle = 180 - startAngle;
        float midAngle = startAngle + (endAngle - startAngle) / 2.0F;
        float step = (endAngle - startAngle) / (2.0F * 10);

        this.drawBigTick(canvas, height, tickLabelHeight, cx, cy, currentAngle);
        currentAngle += step;

        while (currentAngle < midAngle) {
            this.drawSmallTick(canvas, height, tickLabelHeight, cx, cy, currentAngle);
            currentAngle += step;
        }
        currentAngle = midAngle;
        this.drawBigTick(canvas, height, tickLabelHeight, cx, cy, currentAngle);
        currentAngle += step;

        while (currentAngle < endAngle) {
            this.drawSmallTick(canvas, height, tickLabelHeight, cx, cy, currentAngle);
            currentAngle += step;
        }
        currentAngle = endAngle;
        this.drawBigTick(canvas, height, tickLabelHeight, cx, cy, currentAngle);


    }

    private void drawSmallTick(Canvas canvas, float height, float tickLabelHeight, float cx, float cy, float angle) {
        paint.setColor(smallTicksColor);
        paint.setStrokeWidth(strokeWidth / 2.0F);

        double angleRad = Math.toRadians(angle);
        float tipX = (float) (-(height - arcOffset - tickLabelHeight) * Math.cos(angleRad) + cx);
        float tipY = (float) (-(height - arcOffset - tickLabelHeight) * Math.sin(angleRad) + cy);

        float tickLength = this.tickLength;


        canvas.drawLine((float) (tipX + Math.cos(angleRad) * tickLength), (float) (tipY + Math.sin(angleRad) * tickLength), tipX, tipY, paint);
    }

    private void drawBigTick(Canvas canvas, float height, float tickLabelHeight, float cx, float cy, float angle) {
        paint.setColor(bigTicksColor);
        paint.setStrokeWidth(strokeWidth);

        double angleRad = Math.toRadians(angle);
        float tipX = (float) (-(height - arcOffset - tickLabelHeight) * Math.cos(angleRad) + cx);
        float tipY = (float) (-(height - arcOffset - tickLabelHeight) * Math.sin(angleRad) + cy);

        float tickLength = this.tickLength * 2;


        canvas.drawLine((float) (tipX + Math.cos(angleRad) * tickLength), (float) (tipY + Math.sin(angleRad) * tickLength), tipX, tipY, paint);
    }

}
