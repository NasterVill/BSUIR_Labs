package code

object FirstTask {
  def parseRomanString(string: String): Int = {
    val arabicNumerals = List("CM" -> 900, "M" -> 1000,
                              "CD" -> 400, "D" -> 500 ,
                              "XC" -> 90 , "C" -> 100 ,
                              "XL" -> 40 , "L" -> 50  ,
                              "IX" -> 9  , "X" -> 10  ,
                              "IV" -> 4  , "V" -> 5   ,
                              "I" -> 1)

    var s = string
    arabicNumerals.foldLeft(0) { (acc: Int, element:(String, Int)) => {
      val length = s.length;
      s = s.replaceAll(element._1, "");
      val frequency = (length - s.length) / element._1.length // getting the frequency of a numeral
      acc + (frequency * element._2) //adding the arabic numerals
    } }
  }
}