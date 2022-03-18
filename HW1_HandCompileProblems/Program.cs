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
        static int iCnt = 0;
        static int oCnt = 0;

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
                oCnt++;
                int j = 0;
                while( j < i )
                {
                    j = j + 1;
                    iCnt++;
                }
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
            n = 4;
            FuncB(n);
            double d1 = Math.Pow( 2, n );
            double d2 = ( 1 - Math.Pow( 2, n ) )/ ( 1 - 2 );

            Console.WriteLine($"P2: n = {n}  FuncB count = {cnt}  2^n +2(1-2^(n))/(1-2) = { Math.Pow( 2, n )+ 2* (1- Math.Pow( 2, n))/ (1-2)} = 3(2^n)-2 = {3*Math.Pow(2,n)-2} =  ( 2^^n+1 + 2^^n  =  {  ( 1- Math.Pow(2,n+1) ) / (1-2)  + (1 - Math.Pow(2, n )) / (1 - 2) } ) ");


            for( n = 1 ; n <= 100 ; n++ )
            {
                oCnt = 0;
                iCnt = 0;
                FunC( n );
                Console.WriteLine( $"P3: n = {n} out count = {oCnt}  in count = {iCnt}  total = {iCnt+oCnt}   log2(n)+2n-2  = {Math.Log(n,2)+2*n-2}" );
            }


            // Smallest void number Problem(3)-2
            // [5, 2, 1, 1, 2, 3, 5, 8]
            int[] array = { 5, 2, 1, 1, 2, 3, 5, 8 };
            int up = array.Length;
            List<int> L = new List<int>( );
            int ans = 1;
            foreach( int item in array )
            {
                if( item > up )
                {
                    up--;
                    continue;
                }
                if( item == ans )
                {
                    ans++;
                    while( ans == L[ 0 ] )
                    {
                        L.RemoveAt( 0 );
                    }
                }
                else if( item > ans )
                {
                    int id = 0;
                    while( id < L.Count && item > L[ id ] )
                    {
                        id++;
                    }
                    if(  id >= L.Count || item != L[ id]  )
                        L.Insert( id, item );
                }
            }

            Console.WriteLine( $"The smallest void number is {ans}" );

            Console.ReadKey();
        }
    }
}
