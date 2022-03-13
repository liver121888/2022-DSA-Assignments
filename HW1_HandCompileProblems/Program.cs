using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW1_HandCompileProblems
{
    class Program
    {
        static int cnt = 0;
        static void FuncB(int n)
        {
            cnt++;

            if (n > 0)
            {
                FuncB(n - 1);
                FuncB(n - 1);
                FuncB(0);
            }

        }

        static void FunC(int n)
        {
            int i = n;
            while (i > 1)
            {
                int j = 0;
                while (j < i) j = j + 1;
                i = i / 2;
            }
        }


        // We will simply consider division with real numbers

        static void Main(string[] args)
        {
            int sum = 0, i = 1;
            int n = (int)Math.Pow(2, 20);
            int c = 0;
            while (sum < n)
            {
                sum += i;
                i *= 2;
                c++;
            }
            Console.WriteLine($"P1: n = {n}  count = {c}  log2 n = {Math.Log(n, 2)}");

            // Sn = a(1-r ^ n) / ( 1-r)  + ( 1+2+...+n )
            // f(n) = 2 f(n-1) + 1 = 2 ( 2 f(n-2) + 1 ) + 1 = 2 ( 2 (2 f(n-3) + 1) + 1 ) + 1 = ....
            // geometric sequence factor 2 +  [ 1 + 2 + 4 + 8 + ... ]
            cnt = 0;
            n = 10;
            FuncB(n);
            Console.WriteLine($"P2: n = {n}  FuncB count = {cnt}  ( 2^^n+1 + 2^^n  = {  ( 1- Math.Pow(2,n+1) ) / (1-2)  + (1 - Math.Pow(2, n )) / (1 - 2) } ) ");






            Console.ReadKey();
        }
    }
}
