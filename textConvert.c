#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "textConvert.h"
#include "utils.h"

void decimal_to_binary(unsigned int num, char * binShow){
	unsigned int i=0,abs = num;
	    int j;
	    char temp;
	    while(i<WORD_LENGTH){
	    	binShow[i]= abs%2+digit_zero;
	        abs=abs/2;
	        i++;
	    }
	    for (j=0;j<WORD_LENGTH/2;j++){
	        temp = binShow[WORD_LENGTH-1 - j];
	        binShow[WORD_LENGTH-1 - j] = binShow[j];
	        binShow[j] = temp;
	    }
	    binShow[WORD_LENGTH] = END_STRING;
}

int binary_to_decimal(char * binShow){
	int sum=0,i;
	for(i=0;i<WORD_LENGTH;i++)
		sum+=pow(2,WORD_LENGTH-1-i)*(binShow[i]-digit_zero);
	if(binShow[0]==digit_one)
		return 0-(pow(2,12)-sum);
	return sum;
}
