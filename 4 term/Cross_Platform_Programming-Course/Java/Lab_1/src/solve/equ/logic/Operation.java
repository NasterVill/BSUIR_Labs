package solve.equ.logic;

/**
 * <h1>Operation</h1>
 * The "Operation" enum serves to organize
 * supported types of the eqution's operations
 * <p>
 * The relative symbols for supported operations are:
 * '+' = ADD, '-' = SUBTRACT, '*' = MULTIPLY, '/' = DIVIDE
 * @author g75
 *
 */

public enum Operation {
	ADD ('+'), SUBTRACT('-'), MULTIPLY('*'), DIVIDE('/');
	
	
	private char _relevantSymbol;
	
	private Operation(char symbol) {
		_relevantSymbol = symbol;
	}
	
	public char symbol() {
		return _relevantSymbol;
	}
}
