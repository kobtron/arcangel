program()
   var calculate, cb, other, globalb
   
   calculate = function()
      var b
      if 0 then
         b = 2
         print(b)
      else
         b = 4
         print(b)
      end
      
      while b do
         b = b - 1
         print(b)
      end
      
      b = 5
      print(b)
      
      repeat
         b = b - 1
         print(b)
      until !(b or 0)
      
      b = 5
      print(b)
      
      while 1 do
         b = b - 1
         if b == 3 then
            continue
         end
         print(b)
         if b == 2 then
            break
         end
      end
   end

   cb = callback()
      var otakar, c, b
      otakar = 2
      if 1 then
         b = ++!1        +        9 * otakar    %    14  /  1 * 2        +        2                - 1.0-1 << 2 >> 1
         print(b)
         if 0 then
            b = 44
            print(b)
         else
            b = 55
            print(b)
            if 0 then
               b = 88
               print(b)
            else
               b = 99
               print(b)
            end
         end
         if 0 then
            b = 66
            print(b)
         else
            b = 77
            print(b)
         end
      else
         b = 33
         print(b)
      end

      if 0 then
         b = 11
         print(b)
      else
         b = 12
         print(b)
      end
      
      b = calculate()
      
      print(10 + 10.0 + 1)
      print(float(3))
      print(int(4.5))
      
      globalb = new(2 + 3)
      globalb[1] = 222
      print(globalb[1])
      free(globalb)
   end
end