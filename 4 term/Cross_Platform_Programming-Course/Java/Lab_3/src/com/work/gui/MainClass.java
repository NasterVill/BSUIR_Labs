package com.work.gui;


/**
 * <h1>MainClass</h1>
 *
 * MainClass simply implements "main" method
 */
public class MainClass {
    public static void main(String[] args) {
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                GUI userInterface = new GUI();
                userInterface.createAndShowGUI();
            }
        });
    }
}