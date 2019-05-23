package solve.equ.logic;

import solve.equ.exceptions.*;

/**
 * <h1>Equation</h1>
 * Eqution class represents the logic of
 * the equation: it incapsulates the required data, 
 * such as the borders of the x value and other simple
 * equation's components like the result and the known argument 
 * <p>
 * 
 * @author g75
 *
 */
public class Equation {
	private Operation operation;
	private double exodusNumber;
	private double equResult;
	private double lowerBoundary;
	private double upperBoundary;

	public Equation(){
		this.operation = Operation.ADD;
		this.exodusNumber = 0;
		this.equResult = 0;
		this.lowerBoundary = 0;
		this.upperBoundary = 0;
	}

	/**
	 * Public constructor for class Equation.
	 * In this instance the operation is set with
	 * the help of Operation enum type
	 * @param operationSymbol	Symbol of the arithmetical operation
	 * @param exodusNumer		The given argument of the equation
	 * @param equResult			The result of the equation
	 * @param lowerBoundary		The lower boundary of x value
	 * @param upperBoundary		The upper boundary of x value
	 * @throws NullPointerException
	 * @throws NumberFormatException
	 * @see Operation
	 */
	public Equation(Operation operation,
					double exodusNumer, double equResult,
					double lowerBoundary, double upperBoundary)
							throws NumberFormatException, NullPointerException {
		this.operation = operation;
		this.exodusNumber = exodusNumer;
		this.equResult = equResult;
		this.lowerBoundary = lowerBoundary;
		this.upperBoundary = upperBoundary;
	}
	
	/**
	 * Public constructor for class Equation.
	 * In this instance the operation is set with
	 * the help of relative operation Symbol
	 * @param operationSymbol	Symbol of the arithmetical operation
	 * @param exodusNumer		The given argument of the equation
	 * @param equResult			The result of the equation
	 * @param lowerBoundary		The lower boundary of x value
	 * @param upperBoundary		The upper boundary of x value
	 * @throws InvalidOperationException
	 * @throws NumberFormatException
	 * @throws NullPointerException
	 */
	public Equation(char operationSymbol,
					double exodusNumer, double equResult,
					double lowerBoundary, double upperBoundary)
							throws InvalidOperationException, NumberFormatException, NullPointerException {
		switch(operationSymbol)  {
			case '+':
				this.operation = Operation.ADD;
				break;
			case '-':
				this.operation = Operation.SUBTRACT;
				break;
			case '*':
				this.operation = Operation.MULTIPLY;
				break;
			case '/':
				this.operation = Operation.DIVIDE;
				break;
			default:
				throw new InvalidOperationException("Invalid equation's operation!");
		}

		this.exodusNumber = exodusNumer;
		this.equResult = equResult;
		this.lowerBoundary = lowerBoundary;
		this.upperBoundary = upperBoundary;
	}

	public void setOperation(Operation operation) {
		this.operation = operation;
	}
	public Operation getOperation() {
		return this.operation;
	}

	public void setExodusNumber(int exodusNumber) {
		this.exodusNumber = exodusNumber;
	}
	public double getExodusNumber() {
		return this.exodusNumber;
	}

	public void setEquResult(int equResult) {
		this.equResult = equResult;
	}
	public double getEquResult() {
		return this.equResult;
	}
	
     /**
	 * The method solves the equation,
	 * using the data of the calling Equation object
	 * @return RequiredNumber
	 * @throws InfiniteResultRangeException
	 * @throws ArithmeticException
	 * @throws InvalidOperationException
	 */
	public RequiredNumber solve()
		throws InfiniteResultRangeException, ArithmeticException, InvalidOperationException
	{
		double value = 0;
		boolean suitable = true;

		switch(this.operation) {
		case ADD:
			value = this.equResult - this.exodusNumber;
			break;
		case SUBTRACT:
			value = this.equResult + this.exodusNumber;
			break;
		case MULTIPLY:
			if(this.exodusNumber == 0) {
				throw new InfiniteResultRangeException("Infinite range of answers!");
			} else if (this.equResult == 0) {
				value = 0;
			} else {
				value = this.equResult / this.exodusNumber;
			}
			break;
		case DIVIDE:
			if (this.exodusNumber == 0) {
				throw new ArithmeticException("Division by zero!");
			} else {
				value = this.equResult * this.exodusNumber;
			}
			break;
		default:
			throw new InvalidOperationException("Invalid equation's operation!");
		}
		
		if(value < this.lowerBoundary || value > this.upperBoundary) {
			suitable = false;
		}
		
		return new RequiredNumber(value, suitable);
	}
}
