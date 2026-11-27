#include "util.h"
#include<cctype>
#include<iostream>
#include<cstdlib>
using namespace std;

int clampInt(int val, int minVal, int maxVal)
{
  if (val<minVal)
    return minVal;
  else if (val>maxVal)
    return maxVal;
  else
    return val; 
}
bool stringEquals(const char* a, const char* b)
{   int i=0;
    for(; a[i]!='\0'&& b[i]!='\0';i++)
     {
        if(a[i]==b[i])
          continue;
        else
         return false;
     }
    if(a[i]=='\0'&& b[i]=='\0')
      return true;
    return false;
}
void trimWhitespace(char* str)
{  int start=0;
   for(int i=0; str[i]!='\0';i++)
    {
       if(isspace(str[i]))
        {start++;}
       else
         { start=i; 
            break;
         }        
    }
   if (str[start]=='\0')
  {
    str[0]='\0';
    return;
  } 
  int end=0;
   for(int i=0; str[i]!='\0';i++)
    {
       if(!isspace(str[i]))
         end=i;
    }
  int i=0;
  for(;start<=end;i++)
   {
     str[i]=str[start];
     start++;
   }
  str[i]='\0';

}
void printHeader(const char* title)
{
  cout<<"====================================="<<endl;
  cout<<title<<endl;
  cout<<"====================================="<<endl;
}
void clearScreen()
{
    #ifdef _WIN32
       system("cls");
    #else
       system("clear");
    #endif
}
// Primitive helper utilities, clamping, and screen control
