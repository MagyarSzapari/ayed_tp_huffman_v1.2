#include "funciones/strings.hpp"

#include <cctype>

using std::string;

int length(string s)
{
   return (int)s.length();
}

int charToInt(char c)
{
   if( !isdigit((unsigned char)c) )
   {
      return -1; // Error: el carácter no es un dígito
   }
   return c-'0';
}

char intToChar(int i)
{
   string s = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   return s[i];
}

int digitCount(int n)
{
   if( n==0 )
   {
      return 1;
   }
   int count = 0;
   while( n!=0 )
   {
      n = n/10;
      count++;
   }
   return count;
}

string reverseStr(string s)
{
   string c = "";
   for( int i = length(s)-1; i>=0; i-- )
   {
      c += s[i];
   }
   return c;
}

string intToString(int i)
{
   string s = "";
   for( int c = digitCount(i); c>0; c-- )
   {
      s += intToChar(i%10);
      i = i/10;
   }
   return reverseStr(s);
}

// Convierte s, escrito en base b, a entero.
int stringToInt(string s, int b)
{
   int nro = 0;
   for( int i = 0; i<length(s); i++ )
   {
      nro = nro*b+charToInt(s[i]);
   }
   return nro;
}

bool endsWith(string mainStr, string toMatch)
{
   if( mainStr.length()>=toMatch.length() )
   {
      return mainStr.compare(mainStr.length()-toMatch.length(),toMatch.length(),toMatch)==0;
   }
   return false;
}
