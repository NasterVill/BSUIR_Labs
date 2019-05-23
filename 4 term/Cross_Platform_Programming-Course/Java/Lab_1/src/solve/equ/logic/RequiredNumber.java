package solve.equ.logic;

/**
 * <h1>RequiredNumber</h1>
 * RequiredNumber is a class, which serves
 * to store the solution of the equation.
 * It is highly recommended to use it only in the
 * context of equation solving by class Equation;
 * <p>
 * @see Equation 
 * 
 * @author g75
 */
public final class RequiredNumber {
	private double _value;
	private boolean _suitable;

	public RequiredNumber() {
		_value = 0;
		_suitable = false;
	}

	public RequiredNumber(double value, boolean suitable) {
		this._value = value;
		this._suitable = suitable;
	}

	public double value() {
		return _value;
	}

	public boolean isSuitable() {
		return _suitable;
	}
}
