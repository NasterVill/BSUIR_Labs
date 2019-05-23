package code

import scala.annotation.tailrec
import scala.collection.mutable

object MainClass {

  def recursiveAverage(elements: List[Int]): Double = {
    def innerAverage(elements: List[Int], size: Double): Double = elements match {
      case Nil => 0
      case x :: tail => (x + innerAverage(tail, size - 1) * (size - 1)) / size
    }
    innerAverage(elements, elements.length)
  }

  def tailRecursiveAverage(elements: List[Int]): Double = {
    @tailrec
    def averageIteration(elements: List[Int], accumulator: Double, iterationAmount: Int): Double = {
      elements match {
        case Nil => accumulator / iterationAmount
        case x :: tail => averageIteration(tail, accumulator + x, iterationAmount + 1)
      }
    }
    averageIteration(elements, 0, 0)
  }

  @tailrec
  def makePositiveList (elements: List[Int], newList: mutable.MutableList[Int]): Unit =
    elements match{
      case Nil => Unit
      case value::tail => {
        if (value > 0) newList += value
        makePositiveList(tail, newList)
      }
  }

  def makePositiveListNoRecursion (elements: List[Int]): mutable.MutableList[Int] = {
    val newList = mutable.MutableList[Int]()
    for(i <- elements.indices) {
      if(elements(i) > 0) newList += elements(i)
    }
    newList
  }

}