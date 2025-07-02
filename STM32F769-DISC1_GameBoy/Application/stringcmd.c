/*
 * stringcmd.c
 *
 *  Created on: Mar 29, 2012
 *      Author: Marcel
 */

#include "stringcmd.h"


// Get a word in a string separated by spaces
// "pos": indicates the position of the word within the string 1,2, ...
void getWord(char dest[], uint8_t pos, char source[]) {
   uint16_t pdest = 0;
   uint16_t psource = 0;
   uint16_t conta = 0;

   // Looking for the start
   conta = 0;
   psource = 0;
   while (pos > 1 && conta < CMD_OF) {
      if (source[psource] == ' ') pos--;
      if (source[psource] == '\0') conta = CMD_OF;
      else conta++;
      psource++;
   }
   if (conta == CMD_OF) { // Not found correct position
      dest[0] = '\0';
      return;
   }

   // Store chars until the next space or end
   conta = 0;
   pdest = 0;
   while (source[psource] != ' ' &&  source[psource] != '\0' && conta < CMD_OF) {
      dest[pdest] = source[psource];
      psource++;
      pdest++;
      conta++;
   }
   dest[pdest] = '\0';
   if (conta == CMD_OF) { // No space or end of string found
      dest[0] = '\0';
      return;
   }
}


// Chain one string with another
void strCat(char d[], char s[]) {
   uint16_t len, it;
   len = strLen(d);
   it=0;
   while(s[it]!='\0' && it < CMD_OF) {
      d[len] = s[it];
      len++;
      it++;
   }
   d[len]='\0';
}


// Check if two strings are the same
uint8_t isStrEqual(char d[], char s[]) {
   uint16_t pos = 0;
   while (pos < CMD_OF) {
      if(d[pos] != s[pos]) return 0;
      if(d[pos] == '\0') return 1;
      pos++;
   }
   return 0;
}


// Check if two strings are the same from start to finish
uint8_t isSubStrEqual(char d[], char s[], uint16_t start, uint16_t end) {
   uint16_t pos = start;
   while (pos <= end) {
      if(d[pos] != s[pos]) return 0;
      pos++;
   }
   return 1;
}


// Copy one string to another
void strCopy(char d[], char s[]) {
   uint16_t pos = 0;
   while (pos < CMD_OF && s[pos]!='\0') {
      d[pos] = s[pos];
      pos++;
   }
   d[pos] = '\0';
}


// Size of a string
uint16_t strLen(char s[]) {
   uint16_t pos = 0;

   while (pos < CMD_OF && s[pos] != '\0') pos++;

   if (pos == CMD_OF) return 0; // Maximum size exceeded.
   return pos;
}


// Check if the characters are numbers
uint8_t AreNumbers(char s[], uint16_t len) {
   while(len > 0) {
      len--;
      if (*(s+len)< '0' || *(s+len) > '9') return 0;
   }
   return 1;
}


// Returns 1 if the string is a number
uint8_t isNumber(char d[]) {
   uint8_t pos = 0;
   while (pos < CMD_OF && d[pos] != '\0') {
      if((d[pos] < '0' || d[pos] > '9') && (d[pos] != '-' || pos != 0)) return 0;
      pos++;
   }
   if (pos == CMD_OF) return 0;
   if (pos == 0) return 0;
   return 1;
}


// Converts a string to a integer number (accept sign)
int32_t str2num(char s[]) {
   int32_t valor, i;
   uint8_t len;

   len = strLen(s);

   i=1;
   valor=0;
   while(len > 0) {
      len--;
      if (len == 0 && *(s+len) == '-')
         valor = valor*(-1);
      else {
         valor = valor + ((char)*(s+len)-'0')*i;
      }
      i=i*10;
   }
   return valor;

}


// Converts a integer number to a string (accept sign)
void num2str(int32_t num, char* s) {
	uint8_t i=0, j=0;
	char tmp_str[128];
	int32_t absnum;

	// Number in absolute value
	absnum = (num>0?num:-num);

	// Get the digits, from lowest to highest weight
	do {
		int lastDigit = absnum%10;
		tmp_str[i++]='0'+lastDigit;
		absnum = absnum/10;
	} while( absnum>0);

	// Check and set the sign
	if (num < 0) tmp_str[i++] = '-';

	// Turn the number upside down
	while(i>0) s[j++]=tmp_str[--i];
	s[j]='\0';
}


// Transforms a byte to binary format represented with int32
uint32_t num2binary(uint8_t dada) {
   uint32_t val = 0;
   if (bit_test(dada,0)) val = 1;
   if (bit_test(dada,1)) val += 10;
   if (bit_test(dada,2)) val += 100;
   if (bit_test(dada,3)) val += 1000;
   if (bit_test(dada,4)) val += 10000;
   if (bit_test(dada,5)) val += 100000;
   if (bit_test(dada,6)) val += 1000000;
   if (bit_test(dada,7)) val += 10000000;

   return val;
}

// Returns if a bit is set to 1 or 0
uint8_t bit_test(uint16_t dada, uint8_t pos) {
	return (dada >> pos) & 0x0001;
}



// Converts a float to string (accept sign)
void float2str(float num, uint8_t precision, char* s) {
	uint8_t i, j;
	uint32_t parteEntera, parteDecimal;
	float mul10=1;
	char tmp_str[128];

	for(i=0;i<precision;i++) mul10*=10;
	parteEntera=ABS(num);
	parteDecimal=ABS((num-(int32_t)num)*mul10);

	// Put integer digits, from lowest to highest weight
	i=0;
	do {
		tmp_str[i++]=(parteEntera%10)+'0';
		parteEntera = parteEntera/10;
	} while(parteEntera>0);

	// Check and put the sign
	if (num < 0) tmp_str[i++] = '-';

	// Turn the number upside down
	j=0;
	while(i>0) s[j++]=tmp_str[--i];
	s[j]='\0';


	// if has precision, put decimal part
	if(precision>0) {
		uint8_t posDecimal = j;

		// Put decimal digits, from lowest to highest weight
		i=0;
		for(j=0;j<precision;j++) {
			tmp_str[i++]=(parteDecimal%10)+'0';
			parteDecimal = parteDecimal/10;
		}

		// Put the dot
		tmp_str[i++] = '.';

		// Store the decimals digits and the dot
		j=posDecimal;
		while(i>0) s[j++]=tmp_str[--i];
		s[j]='\0';
	}

}

