package solve.equ.tests;

import solve.equ.exceptions.InfiniteResultRangeException;
import solve.equ.exceptions.InvalidOperationException;
import solve.equ.logic.Equation;
import solve.equ.logic.Operation;
import static org.junit.Assert.*;
import org.junit.Test;

public class EquationTest {

	@Test(expected = InvalidOperationException.class)
	public void testEquationCharConstructorWithIlligalOperation()
			throws NumberFormatException, InvalidOperationException, NullPointerException {
		new Equation('.', 0,  21 , -1, 0);
	}
	
	@Test(expected = NumberFormatException.class)
	public void testEquationConstructorInvalidArgument()
			throws NumberFormatException, InvalidOperationException, NullPointerException {
		String garbageStr = "fsdjgijsp"; 
		new Equation('+', 0,  Double.parseDouble(garbageStr), -1, 0);
	}
	
	@Test(expected = NullPointerException.class)
	public void testEquationConstructorNull()
			throws NumberFormatException, InvalidOperationException, NullPointerException {
		String garbageStr = null; 
		new Equation('+', 0,  Double.parseDouble(garbageStr), -1, 0);
	}
	
	@Test
	public void testEquationConstructor()
			throws IllegalArgumentException, InvalidOperationException, NullPointerException {
		assertNotNull(new Equation('+', 0,  21 , -1, 0));
		assertNotNull(new Equation(Operation.ADD, 753, 200, 63, 78));
	}

	@Test
	public void testSolve()
			throws ArithmeticException, InfiniteResultRangeException, InvalidOperationException {
		Equation fisrstEquation = new Equation('+', 11, 15, 0, 10);
		Equation secondEquation = new Equation('-', 12, 130, 0, 145);
		Equation thirdEquation = new Equation('*', 13, 91, 0, 200);
		Equation fourthEquation = new Equation('/', 11, 15, 0, 166);
		Equation fithEquation = new Equation('/', 5, 25, 0, 10);
		
		assertTrue(fisrstEquation.solve().value() == 4.0);
		assertEquals(secondEquation.solve().value(), 142, 0);
		assertEquals(thirdEquation.solve().value(), 7, 0);
		assertEquals(fourthEquation.solve().value(), 165, 0);
		assertEquals(fithEquation.solve().isSuitable(), false);
	}
}
