program()
   var square, x, squ
   
   square = function()
      squ = x * x
   end

   x = 1
   
   while x <= 10 do
      call square()
      print(squ)
      x = x + 1
   end
end
