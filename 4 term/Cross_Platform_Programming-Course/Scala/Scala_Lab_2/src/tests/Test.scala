package tests

import code.MainClass
import org.scalatest._

class Test extends FlatSpec with Matchers {
  val list = List.range(-10, 10)
  val listTwo = MainClass.listToString(list)
  val firstMap = Map(1 -> "Andrew",
    2 ->"Boris",
    3 ->"Dmitriy",
    4 ->"Ivan",
  )

  val secondMap = Map(1 ->"Anya",
    2 ->"Igor",
    3 ->"Polina",
    4 ->"Sergei",
  )

  println(MainClass.matrixEvenRowsMultiplication(List[List[Int]](List[Int](1,2,3),
    List[Int](4,5,6),
    List[Int](7,8,9),
    List[Int](10,11,12)
  )))
  println(listTwo)
  MainClass.concatStudentsLists(firstMap, secondMap).foreach(tuple => println(tuple._1 + " " + tuple._2))
}