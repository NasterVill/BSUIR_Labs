package tests

import code.MainClass
import org.scalatest._

import scala.collection.mutable

class Test extends FlatSpec with Matchers {

  "Average list value (recursion) 1" should "return 5.0" in {
    val list = List.range(1, 10)
    MainClass.recursiveAverage(list) should be(5.0)
  }

  "Average list value (recursion) 2" should "return 50.0" in {
    val list = List.range(1, 100)
    MainClass.recursiveAverage(list) should be(50.0)
  }

  "Average list value (tail recursion) 1" should "return 5.0" in {
    val list = List.range(1, 10)
    MainClass.tailRecursiveAverage(list) should be (5.0)
  }

  "Average list value (tail recursion) 2" should "return 50.0" in {
    val list = List.range(1, 100)
    MainClass.tailRecursiveAverage(list) should be (50.0)
  }

  "Compose new positive list based on the old one (tail recursion)" should "return list with positive elements" in {
    val list = List.range(-10, 10)
    val newList = mutable.MutableList[Int]()
    val answer = mutable.MutableList.range(1, 10)
    MainClass.makePositiveList(list, newList)
    for(i <- answer.indices){
      assert(newList(i) === answer(i))
    }
  }

  "Compose new positive list based on the old one (no recursion)" should "return list with positive elements" in {
    val list = List.range(-10, 10)
    val answer = mutable.MutableList.range(1, 10)
    val newList = MainClass.makePositiveListNoRecursion(list)
    for(i <- answer.indices){
      assert(newList(i) === answer(i))
    }
  }
}