program()
   var arg, ret, max, isprime, primes
   max = 100

   isprime = function()
      var i
      ret = 1
      i = 2
      while i < arg do
         if arg % i == 0 then
            ret = 0
            break
            i = arg
         end
         i = i + 1
      end
   end

   primes = function()
      arg = 2
      while arg < max do
         call isprime()
         if ret == 1 then print(arg) end
         arg = arg + 1
      end
   end

   call primes()
end