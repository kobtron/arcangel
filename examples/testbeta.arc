program()
   var aux, zero, one, two, three, four
   aux = 10 + 20.2
   print(aux)
   if aux then
      print(333)
   else
      print(444)
   end
   zero = 15
   one = 16.3
   print(one + zero)
   two = new(2)
   print(two)
   two[0] = 4
   print(two[0])
   two[1] = 5
   print(two[1])
   three = two[0] + two[1]
   print(three)
   free(two)
end