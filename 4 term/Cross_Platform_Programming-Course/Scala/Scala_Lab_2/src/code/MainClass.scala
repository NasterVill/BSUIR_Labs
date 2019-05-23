package code

import scala.collection.immutable.List

object MainClass {
  def listToString (elements: List[Int]): List[String] = {
    elements.foldRight(List[String]())((element, list) => element.toString :: list)
  }

  def matrixEvenRowsMultiplication(elements: List[List[Int]]): List[Int] = {
    elements.zipWithIndex.filter(tuple => tuple._2 % 2 == 0).map(tuple => tuple._1.product)
  }

  def concatStudentsLists(first: Map[Int, String], second: Map[Int, String]): Map[Int, String] = {
    val tempOne = first.map{case (_,value) => value}.toList
    val tempTwo = second.map{case (_,value) => value}.toList
    val orderList = List.range(1, first.size + second.size + 1)

    orderList.zip((tempOne ::: tempTwo).sortWith(_ < _)).toMap
  }
}
