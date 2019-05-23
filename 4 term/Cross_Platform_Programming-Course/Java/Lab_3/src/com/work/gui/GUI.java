/**
 * <h1>Work representation</h1>
 * This program represents the work with
 * different instruments and blanks
 * <p>
 *
 * @author  Vitaliy Pankov
 * @version 1.0
 * @since   2018-03-20
 */


package com.work.gui;
import com.work.model.Model;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;

public class GUI {

    /**
     * This is the default constructor , which serves to initialize
     * the data model, which is going to be used throughout the program.
     *
     * Also setting the listener for checkBox elements,
     * which describe the properties of the work by "Workman"
     */
    public GUI() {
        model = new Model();

        itemListener = new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                Object source = e.getItemSelectable();

                if(source == cutButton) {
                    model.allowCut = !model.allowCut;
                } else if(source == smoothButton) {
                    model.allowSmooth = !model.allowSmooth;
                } else if(source == screwButton) {
                    model.allowScrew = !model.allowScrew;
                }
            }
        };
    }

    /**
     * This method is used to configure the pane
     * for the program to be able to interact with user
     * @param pane This is the parameter, which contains all
     *             the GUI elements, like JLable, JButton and etc.
     * @return void.
     * @see Container
     */
    private void addComponentsToPane(Container pane) {
        pane.setLayout(new GridBagLayout());
        GridBagConstraints constraints = new GridBagConstraints();
        constraints.fill = GridBagConstraints.HORIZONTAL;
        constraints.weightx = 1;
        constraints.insets.left = 0;
        constraints.insets.right = 10;

        constraints.gridx = 0;
        constraints.gridy = 0;
        emloyeeLabel = new JLabel("Employee: ");
        pane.add(emloyeeLabel, constraints);
        constraints.gridx = 1;
        constraints.gridy = 0;
        constraints.gridwidth = 3;
        emolyeeName = new JLabel(model.employee.getName());
        pane.add(emolyeeName, constraints);
        constraints.gridx = 0;
        constraints.gridy = 1;
        constraints.gridwidth = 1;
        pane.add(new JLabel("Blank properties: "), constraints);


        constraints.gridx = 1;
        constraints.gridy = 1;
        constraints.gridwidth = 1;
        lengthLabel = new JLabel("Length : " + model.blankItem.getShape().getLength());
        pane.add(lengthLabel, constraints);
        constraints.gridx = 2;
        constraints.gridy = 1;
        constraints.gridwidth = 1;
        widthLabel = new JLabel("Width : " + model.blankItem.getShape().getWidth());
        pane.add(widthLabel, constraints);
        constraints.gridx = 3;
        constraints.gridy = 1;
        constraints.gridwidth = 1;

        String screwedStr;
        if (model.blankItem.getProductInfo().isReady()) {
            screwedStr = "Ready";
        } else {
            screwedStr = "Unscrewed";
        }
        screwedLable = new JLabel(screwedStr);
        pane.add(screwedLable, constraints);
        constraints.gridx = 4;
        constraints.gridy = 1;
        constraints.gridwidth = 1;
        newBlankItemButton = new JButton("New blank item");
        newBlankItemButton.setPreferredSize(new Dimension(120, 20));
        pane.add(newBlankItemButton, constraints);


        constraints.gridx = 0;
        constraints.gridy = 2;
        constraints.gridwidth = 1;
        constraints.fill = GridBagConstraints.HORIZONTAL;
        workButton = new JButton("Work");
        workButton.setPreferredSize(new Dimension(80, 20));
        pane.add(workButton, constraints);

        constraints.gridx = 1;
        constraints.gridy = 2;
        constraints.fill = GridBagConstraints.HORIZONTAL;
        paramLabel = new JLabel("Parameters: ");
        pane.add(paramLabel, constraints);

        constraints.gridx = 2;
        constraints.gridy = 2;
        constraints.gridwidth = 1;
        cutButton = new JCheckBox("Cut");
        cutButton.setMnemonic(KeyEvent.VK_C);
        cutButton.setSelected(true);
        pane.add(cutButton, constraints);

        constraints.gridx = 3;
        constraints.gridy = 2;
        constraints.gridwidth = 1;
        smoothButton = new JCheckBox("Smooth");
        smoothButton.setMnemonic(KeyEvent.VK_G);
        smoothButton.setSelected(true);
        pane.add(smoothButton, constraints);

        constraints.gridx = 4;
        constraints.gridy = 2;
        constraints.gridwidth = 1;
        screwButton = new JCheckBox("Screw/Unscrew");
        screwButton.setMnemonic(KeyEvent.VK_H);
        screwButton.setSelected(true);
        pane.add(screwButton, constraints);

        cutButton.addItemListener(itemListener);
        smoothButton.addItemListener(itemListener);
        screwButton.addItemListener(itemListener);

        newBlankItemButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try {
                    Integer newLength = Integer.parseInt(
                            JOptionPane.showInputDialog(frame, "Please enter new blank's length",
                                    "New Blank Item", JOptionPane.PLAIN_MESSAGE));

                    if(newLength <= 0) {
                        JOptionPane.showMessageDialog(null,
                                "The value of length must be positive!",
                                "Exception caught!", JOptionPane.WARNING_MESSAGE);
                        return;
                    }

                    Integer newWidth = Integer.parseInt(
                            JOptionPane.showInputDialog(frame, "Please enter new blank's width",
                            "New Blank Item", JOptionPane.PLAIN_MESSAGE));

                    if(newWidth <= 0) {
                        JOptionPane.showMessageDialog(null,
                                "The value of width must be positive!",
                                "Exception caught!", JOptionPane.WARNING_MESSAGE);
                        return;
                    }

                    model.blankItem.getShape().changeLength(newLength);
                    model.blankItem.getShape().changeWidth(newWidth);

                    lengthLabel.setText("Length : " + model.blankItem.getShape().getLength());
                    widthLabel.setText("Width : " + model.blankItem.getShape().getWidth());

                    String str = new String();
                    if (model.blankItem.getProductInfo().isReady()) {
                        str = "Ready";
                    } else {
                        str = "Unscrewed";
                    }
                    screwedLable.setText(str);
                }
                catch (Exception exception) {
                    JOptionPane.showMessageDialog(null,
                            "Description : " + exception.getMessage(),
                            "Exception caught!", JOptionPane.WARNING_MESSAGE);

                }
            }
        });

        workButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try {
                    model.employee.work(model.blankItem, model.allowCut, model.allowSmooth, model.allowScrew);
                    lengthLabel.setText("Length : " + model.blankItem.getShape().getLength());
                    widthLabel.setText("Width : " + model.blankItem.getShape().getWidth());

                    String str = new String();
                    if (model.blankItem.getProductInfo().isReady()) {
                        str = "Ready";
                    } else {
                        str = "Unscrewed";
                    }
                    screwedLable.setText(str);
                }
                catch (Exception exception) {
                    JOptionPane.showMessageDialog(null,
                            "Description : " + exception.getMessage(),
                            "Exception caught!", JOptionPane.WARNING_MESSAGE);

                }
            }
        });
    }

    public void createAndShowGUI() {
        frame = new JFrame("Working with blanks");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 120);
        frame.setResizable(false);

        addComponentsToPane(frame.getContentPane());
        frame.setVisible(true);
    }

    private JFrame frame;
    private JLabel emloyeeLabel;
    private JLabel emolyeeName;
    private JLabel lengthLabel;
    private JLabel widthLabel;
    private JLabel screwedLable;
    private JButton newBlankItemButton;
    private JButton workButton;
    private JLabel paramLabel;
    private JCheckBox cutButton;
    private JCheckBox smoothButton;
    private JCheckBox screwButton;
    private ItemListener itemListener;

    private Model model;
}