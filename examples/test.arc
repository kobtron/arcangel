program()
   var hello, fun, funb
   hello = "Hello world\n"
   prints(hello)
   free(hello)
   
   fun = function(a, b, c, d)
      var msg
      msg = "Message!\n"
      prints(msg)
      free(msg)
      print(a)
      print(b)
      print(c)
      print(d)
   end
   
   funb = function(one, two)
      
   end
   
   call fun(4, 3, 2, 5)
   print(fun(1, 1, 1, 1))
   call funb(11, 22)
end