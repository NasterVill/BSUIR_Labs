package tests

import code._
import org.scalatest.FunSuite;

class Test extends FunSuite {
  test("First task test") {
    val string = "MCMXC"
    val answer = 1990

    val stringTwo = "MMVIII"
    val answerTwo = 2008

    assert(FirstTask.parseRomanString(string) === answer)
    assert(FirstTask.parseRomanString(stringTwo) === answerTwo)
  }

  test("Second task scalar test") {
    val scalarNumber = new SecondTask.Scalar(-12)
    val answer = 12
    assert(SecondTask.absNumber(scalarNumber) === answer)
  }

  test("Second task complex test") {
    val complexNamber = new SecondTask.Complex(14, -5)
    val answer = 221
    assert(SecondTask.absNumber(complexNamber) === answer)
  }

}