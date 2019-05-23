/**
* <h1>Solving the equation</h1>
* The "Solving the equation" program implements an application that
* which can find out the value of x in a simple equation
* x 'operation' someNumber = resultNumber 
* <p>
*
* @author  g75
* @version 1.0
* @since   2018-03-20 
*/

package solve.equ.gui;
import solve.equ.logic.*;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowData;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Composite;

public class MainClass {
	 /**
	   * This method is used to configure shell(main window)
	   * for the program to be able to interact with user
	   * @param display This is the parameter which will serve as shell's parent.
	   * It's most important function is to implement
	   * the SWT event loop in terms of the platform event model. 
	   * @return Shell : Returns the configured shell object's reference
	   * @see Display
	   * @see Shell
	   */
	public static Shell configureShell(Display display) {
		final int offset = 5;
		final int fontSize = 12;
		final int defaultSpacing = 8;
		
		Shell shell = new Shell(display, SWT.SHELL_TRIM & (~SWT.RESIZE));
		shell.setText("Solving the equation");
		shell.setSize(425, 170);
		RowLayout mainLayout = new RowLayout();
		mainLayout.wrap = false;
		mainLayout.pack = true;
		mainLayout.justify = false;
		mainLayout.type = SWT.VERTICAL;
		mainLayout.spacing = 0;
		shell.setLayout(mainLayout);
		
		
		Composite topComposite = new Composite(shell, SWT.NULL);
		RowLayout topLayout = new RowLayout();
		topLayout.wrap = false;
		topLayout.pack = true;
		topLayout.justify = false;
		topLayout.type = SWT.HORIZONTAL;
		topLayout.spacing = defaultSpacing;
		topLayout.marginLeft =  offset;
		topLayout.marginRight =  offset;
		topComposite.setLayout(topLayout);
		
		Label equText = new Label(topComposite, SWT.NULL);
		equText.setText("Equation: x");
		equText.setFont(new Font(display,"Calibri", fontSize, SWT.NULL));
			
		Combo operationFiled = new Combo(topComposite, SWT.NULL);
		operationFiled.setItems(new String[] {"+", "-", "*", "/"});
		operationFiled.select(0);
		operationFiled.setLayoutData(new RowData(20, 20));
		
		Text exodusNumTextField = new Text(topComposite, SWT.BORDER);
		exodusNumTextField.setLayoutData(new RowData(100, 17));
		
		Label equalsText = new Label(topComposite, SWT.NULL);
		equalsText.setText(" = ");
		
		Text equResultTextField = new Text(topComposite, SWT.BORDER);
		equResultTextField.setLayoutData(new RowData(100, 17));
	
		topComposite.getParent().layout();
		
		
		Composite middleComposite = new Composite(shell, SWT.NULL);
		GridLayout middleLayout = new GridLayout(2, false);
		middleComposite.setLayout(middleLayout);
		RowData middleData = new RowData();
		middleData.width = topComposite.getBounds().width;
		middleComposite.setLayoutData(middleData);
		
		GridData gridData = new GridData();
		gridData.horizontalAlignment = SWT.FILL;
		gridData.grabExcessHorizontalSpace = true;
		
		Label lowBoundaryLabel = new Label(middleComposite, SWT.NULL);
		lowBoundaryLabel.setText("Lower boundary: ");
		lowBoundaryLabel.setFont(new Font(display,"Calibri", fontSize, SWT.NULL));
		
		Text lowerBoundTextField = new Text(middleComposite, SWT.BORDER);
		lowerBoundTextField.setLayoutData(gridData);
		
		Label upBoundaryLabel = new Label(middleComposite, SWT.NULL);
		upBoundaryLabel.setText("Upper boundary: ");
		upBoundaryLabel.setFont(new Font(display,"Calibri", fontSize, SWT.NULL));
		
		Text upperBoundTextField = new Text(middleComposite, SWT.BORDER);
		upperBoundTextField.setLayoutData(gridData);
		
		middleComposite.getParent().layout();
		
		
		Composite bottomComposite = new Composite(shell, SWT.NULL);
		RowLayout bottomLayout = new RowLayout();
		bottomLayout.wrap = false;
		bottomLayout.pack = true;
		bottomLayout.justify = false;
		bottomLayout.type = SWT.HORIZONTAL;
		bottomLayout.spacing = defaultSpacing;
		bottomLayout.marginLeft = offset;
		bottomLayout.marginRight =  offset;
		bottomComposite.setLayout(bottomLayout);
		
		RowData bottomData = new RowData();
		bottomData.width = middleComposite.getBounds().width;
		bottomComposite.setLayoutData(bottomData);
		
		Label resultLabel = new Label(bottomComposite, SWT.NULL);
		resultLabel.setText("Result: ");
		resultLabel.setFont(new Font(display,"Calibri", fontSize, SWT.NULL));
		
		Button calculateButton = new Button(bottomComposite, SWT.PUSH);
		calculateButton.setText("Solve");
		calculateButton.setLayoutData(new RowData(60, 20));
		
		bottomComposite.getParent().layout();
		
	
		calculateButton.addListener(SWT.Selection, new Listener()
		{
			 public void handleEvent(Event event)
			 {
				 try {
					 Equation equation = new Equation (operationFiled.getText().charAt(0),
							 Double.parseDouble(exodusNumTextField.getText()),
							 Double.parseDouble(equResultTextField.getText()),
							 Double.parseDouble(lowerBoundTextField.getText()),
							 Double.parseDouble(upperBoundTextField.getText()));
					 RequiredNumber requiredNum = equation.solve();
					 
					 String str;
					 
					 if (!requiredNum.isSuitable()) {
						 str = "The result doesn't satisfy the boundaries";
					 } else { 
						 str = Double.toString(requiredNum.value());
					 }
					 
					 resultLabel.setText("Result: " + str);
					 resultLabel.setFont(new Font(display,"Calibri", fontSize, SWT.NULL));
					 resultLabel.getParent().layout();
				 }
				 catch(Exception excep) {
					 MessageBox dialog = new MessageBox(shell, SWT.OK | SWT.CANCEL);
					 
					 dialog.setText("Exception caught!");
					 dialog.setMessage("Description: " + excep.getMessage());
					
					 dialog.open();
				 }
			 }
		});
			
		return shell;
	}

	public static void main(String[] args) {
		 Display display = new Display();
		 Shell shell = configureShell(display);
		 shell.open();
		
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
	}
}