package code

object SecondTask {

  def absNumber(number: Number): Double = {
    number match {
      case Complex(_, _) => number.abs()
      case Scalar(_) => number.abs()
    }
  }

  abstract class Number {
    def abs(): Double
  }

  case class Scalar(value: Double) extends Number {
    def abs(): Double = if (value < 0) value * (-1) else value
  }

  case class Complex(real: Double, imaginary: Double) extends Number {
    def abs(): Double = math.sqrt((real * real) + (imaginary * imaginary))
  }

}