package com.work.model;

import com.work.entities.*;
import com.work.product.Blank;
import com.work.product.Shape;


/**<h1>Model</h1>
 *
 * The Model class represents the program data
 * structure, which is being manipulated by the
 * user with the help of GUI class.
 * <p>
 * The data fields are not sealed in order to ease
 * the manipulation with them, because they either will be changing
 * throughout the program execution, or their methods are
 * used very often during the session.
 * </p>
 */
public class Model {
    public Saw saw;
    public JackPlane jackPlane;
    public ScrewDriver screwdriver;
    public Blank blankItem;
    public Workman employee;

    public boolean allowCut;
    public boolean allowSmooth;
    public boolean allowScrew;


    public Model() {
         blankItem = new Blank(new Shape());
         saw = new Saw(15, 500);
         jackPlane = new JackPlane(600);
         screwdriver = new ScrewDriver(300);
         employee = new Workman("Dave Mustaine", saw, jackPlane, screwdriver);

        this.allowCut = true;
        this.allowSmooth = true;
        this.allowScrew = true;
    }

    public Model(String name, Blank item, Saw saw, JackPlane jackPlane, ScrewDriver screwdriver)
        throws NullPointerException{
        this.blankItem = item;
        this.saw = saw;
        this.jackPlane = jackPlane;
        this.screwdriver = screwdriver;
        employee = new Workman("Dan Smith", saw, jackPlane, screwdriver);

        this.allowCut = true;
        this.allowSmooth = true;
        this.allowScrew = true;
    }

    public void initializeBlankItem() {
        this.blankItem = new Blank(new Shape());
    }
}
