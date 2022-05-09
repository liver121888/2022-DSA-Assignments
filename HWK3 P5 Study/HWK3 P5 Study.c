#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


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


char s[10000010];
int length;
unsigned char low = 0x21;
unsigned char up = 0x7E;
int range = 0x7E - 0x21 + 1;

void main()
{
}