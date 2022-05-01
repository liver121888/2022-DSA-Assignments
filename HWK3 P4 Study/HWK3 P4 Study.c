#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int length;
unsigned char low = 0x21;
unsigned char up = 0x7E;
int range = 0x7E - 0x21 + 1;

char s[10000000];

void main()
{
    // char value 0x21 up to 0x7E
    unsigned char c;
    length = 0;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        s[length++] = c - low;
    }


    long long Target1, Target2;
    long long Part1, Part2;

    int t1Start = 0;
    int p1Start = length - 1;
    int p2Start = 0;
    int t2Start = length - 1;

    long long module = pow(2, 8 * sizeof(long long)) / range;

    int active = length;
    int size = active / 2;

    Target1 = 0;
    int final = t1Start + size;
    for (int i = t1Start; i < final; i++)
    {
        Target1 = (Target1 * range + s[i]) % module;
    }
    Part2 = Target1;

    Part1 = 0;
    final = p1Start - size;
    for (int i = p1Start; i > final; i--)
    {
        Part1 = (Part1 * range + s[i]) % module;
    }
    Target2 = Part1;
    int newSize;
    for (int d = 0; size >= 0; d++)
    {
        if (size == 0)
        {
            //prefix
            printf("%d\n", d);
            for (int i = 0, j = length - 1; i < d; i++, j--) printf("%c", s[j] + low);
            for (int i = 0; i < length; i++) printf("%c", s[i] + low);
            printf("\n");

            // suffix
            for (int i = 0; i < length; i++) printf("%c", s[i] + low);
            for (int i = d - 1; i >= 0; i--) printf("%c", s[i] + low);
            printf("\n");

            break;
        }

        int hit = 1;
        if (Target2 == Part2)
        {
            // Check matching
            for (int t = t2Start, p = p2Start, z = 0; z < size; z++, t--, p++)
            {
                if (s[t] != s[p])
                {
                    hit = 0;
                    break;
                }
            }
            if (hit)
            {
                // Prefix s[length-1-d] reversely
                // print out results
                printf("%d\n", d);
                for (int i = 0, j = length - 1; i < d; i++, j--) printf("%c", s[j] + low);
                for (int i = 0; i < length; i++) printf("%c", s[i] + low);
                printf("\n");
                break;
            }
        }


        if (Target1 == Part1)
        {
            // Check matching
            for (int t = t1Start, p = p1Start, z = 0; z < size; z++, t++, p--)
            {
                if (s[t] != s[p])
                {
                    hit = 0;
                    break;
                }
            }
            if (hit)
            {
                //  Suffix s[0-d] reversely
                // print out results
                printf("%d\n", d);
                for (int i = 0; i < length; i++) printf("%c", s[i] + low);
                for (int i = d - 1; i >= 0; i--) printf("%c", s[i] + low);
                printf("\n");
                if (d == 0) break;
            }

        }


        active--;
        newSize = active / 2;

        // prefix part
        Part2 = (Part2 - s[p2Start] * (int)(pow(range, size - 1))) % module;
        Part2 = Part2 % module;
        if (newSize == size)
        {
            // Add new last term
            Part2 += s[p2Start + size];
        }
        else
        {
            // size reduced; delete the last term
            Target2 = (Target2 - s[t2Start - size + 1]) / range;
        }

        t2Start--;


        // suffix part
        //Target1 backward by 1
        Target1 = (Target1 - s[t1Start] * (int)(pow(range, size - 1))) % module;
        Target1 = Target1 % module;
        if (newSize == size)
        {
            // Add new last term
            Target1 += s[t1Start + size];
        }
        else
        {
            // size reduced; delete the last term
            Part1 = (Part1 - s[p1Start - size + 1]) / range;
        }

        t1Start++;

        size = newSize;


    }





}