#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


//  aabc
//  Answer :
//  2
//  cbaabc
//  
//  baaaac
//  Answer :
//  5
//  caaaabaaaac
//  baaaacaaaab
//  
//  ababa
//  Answer :
//  0
//  ababa


char s[10000000];
int length;
unsigned char low = 0x21;
unsigned char up = 0x7E;
int range = 0x7E - 0x21 + 1;

void main()
{
    // char value 0x21 up to 0x7E
    unsigned char c;
    length = 0;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        s[length++] = c - low;
    }

    // Target1+Part1 for suffix palindrome; Target1 start from s[0] and shift to right step by step.
    // Part1 reverses from the last characters.
    long long Target1, Part1 ;
    int t1Start = 0;
    int p1Start = length - 1;

    // Target2+Part2 for prefix palindrome; Target2 reversely starts from the last characters and shift to left step by step.
    // Part2 starts from s[0];
    long long Target2, Part2;
    int t2Start = length - 1;
    int p2Start = 0;

    // Module value set
    long long module = pow(2, 8 * sizeof(long long)) / range;

    // The current active length covering target length, part length, and possible a middle character
    int active = length; 
    // Size is the current character length of targets and parts subject to comparisons.
    int size = active / 2;

    // Prepare the beginning modulus values of Target1, Part1, Target2, and Part2.
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

    // newSize is updated when current matching fails and active length reduced by one.
    // Note that updated newSize is alternatively remaining the same as Size or reduced by one
    int newSize;

    // Flag for signaling that either pre- or suf- palindrome is found; no further comparison is necessary
    int minimalAnswerFound = 0;

    // Try all steps until Size is reduced to zero
    for (int shift = 0; size >= 0; shift++)
    {
        if (size == 0) // No any match is found
        {
            //prefix
            printf("%d\n", shift);
            for (int i = 0, j = length - 1; i < shift; i++, j--) printf("%c", s[j] + low);
            for (int i = 0; i < length; i++) printf("%c", s[i] + low);
            printf("\n");

            // suffix
            for (int i = 0; i < length; i++) printf("%c", s[i] + low);
            for (int i =0, j = shift - 1; i < shift; i++, j--) printf("%c", s[j] + low);
            printf("\n");

            break; // No further search.
        }


        int hit;
        if (Target2 == Part2)
        {
            hit = 1;
            // Check matching
            for (int t = t2Start, p = p2Start, z = 0; z < size; z++, t--, p++)
            {
                if (s[t] != s[p])
                {
                    hit = 0; // not match
                    break;
                }
            }
            if (hit)
            {
                // Prefix s[length-1-d] reversely; print out results
                printf("%d\n", shift);
                for (int i = 0, j = length - 1; i < shift; i++, j--) printf("%c", s[j] + low);
                for (int i = 0; i < length; i++) printf("%c", s[i] + low);
                printf("\n");
                if (shift == 0) break;  // The original is palindrome; just report it and exit
            }
            minimalAnswerFound = 1; // Prefix solution found!
        }

        if (Target1 == Part1)
        {
         hit = 1;
           // Check matching
            for (int t = t1Start, p = p1Start, z = 0; z < size; z++, t++, p--)
            {
                if (s[t] != s[p])
                {
                    hit = 0; // not match
                    break;
                }
            }
            if (hit)
            {
                //  Suffix s[0-d] reversely ; print out results
                printf("%d\n", shift);
                for (int i = 0; i < length; i++) printf("%c", s[i] + low);
                for (int i = 0, j= shift - 1; i < shift; i++, j--) printf("%c", s[j] + low);
                printf("\n");
                minimalAnswerFound = 1; // Suffix solution found!
            }
        }

        // Either prefix or suffix solution found, exit; no larger number palindromes are searched. 
        if(minimalAnswerFound) break;  


        // This value comparison checking (pattern matching) fails; Shift targets, reduce active length, update size to
        // calculate new values of targets and parts.
        active--;
        newSize = active / 2; // newSize updated, alternatively reduced by one or remaining the same.

        // prefix part; target shifts to left one character
        Target2 = (Target2 - ( s[t2Start] * (int)(pow(range, size - 1))) % module )  % module;
        if (newSize == size)
        {
            // add new last term, since size is not reduced
            Target2 += s[t2Start - size];
        }
        else
        {
            // Value of part should be reduced, since newSize is reduced
            Part2 = (Part2 - s[p2Start + size - 1]) / range;
        }
        // Formally shifting to left
        t2Start--;

        // suffix part; target shifts to right one character
        //Target1 backward by 1
        Target1 = (Target1 - ( s[t1Start] * (int)(pow(range, size - 1))) % module ) % module;
        if (newSize == size)
        {
            // Add new last term, since size is not reduced
            Target1 += s[t1Start + size];
        }
        else
        {
            // Value of part should be reduced, since newSize is reduced
            Part1 = (Part1 - s[p1Start - size + 1]) / range;
        }
        // Formally shifting to right
        t1Start++;

        // Update size for next step value comparision
        size = newSize;
    }





}