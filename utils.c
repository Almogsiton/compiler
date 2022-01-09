#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void * allocation(int size){
	void *p = malloc(size);
	if (!p) {
		printf("Error Memory allocation failed.");
		exit(0);
	}
	return p;
}

bool is_label_line(char *line){
	int i=0;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,line);
	for(i=0;i<strlen(buffer);i++){
		if(buffer[i]==COLON)
			return TRUE;
	}
	return FALSE;
}

bool is_word_label(char *line){
	char *token;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,line);
	token=strtok(buffer,SPACE_STRING);
	if(token[strlen(token)-1]==COLON)
		return TRUE;
	return FALSE;
}

bool is_legal_line(char*line){
	if(strlen(line)<SIZE_OF_LINE)
		return TRUE;
	return FALSE;
}

int words_on_line(char *line){
	char buffer[SIZE_OF_LINE];
	char *token;
	int numWord=0;
	strcpy(buffer,line);
	token=strtok(buffer,SPACE_STRING);
	while(token!=NULL){
		numWord++;
		token=strtok(NULL,SPACE_STRING);
	}
	return numWord;
}

void remove_space(char *line,char * tempStr){
	bool state=FALSE;
	int i,j=0;
	for(i=0;i<strlen(line);i++){
		while(line[i]!=SPACE && line[i]!=TAB&&i<strlen(line)){
			if(line[i]==COMMA&&tempStr[j-1]==SPACE)
				j=j-1;
			state=TRUE;
			tempStr[j]=line[i];
			j++;
			i++;
		}
		if(state==TRUE){
			if(tempStr[j-1]!=COMMA){
				tempStr[j]=SPACE;
				j++;
				state=FALSE;
			}
		}
	}
	tempStr[j]=END_STRING;
	for(i=strlen(tempStr);i>0;i--){
		if(tempStr[i]=='\n')
			tempStr[i]=END_STRING;
	}
}

bool check_empty_line(char * line){
	int i;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,line);
	for(i=0;i<strlen(buffer);i++){
		if(buffer[i]!=' '&&buffer[i]!='\t'&&buffer[i]!='\n')
			return FALSE;
	}
	return TRUE;
}

void selected_word_line(char * line,char *word,int numOfWord){
	char *token;
	char buffer[SIZE_OF_LINE];
	int indexWord=0;
	strcpy(buffer,line);
	token=strtok(buffer,SPACE_STRING);
	if(token!=NULL)
		indexWord++;
	else
		return;
	while(token!=NULL&&indexWord!=numOfWord){
		token=strtok(NULL,SPACE_STRING);
		indexWord++;
	}
	strcpy(word,token);
}

bool is_storage_directive(char * word){
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,word);
	if(strcmp(buffer,DATA_DIRECIVE)==EQUAL_STRING||strcmp(buffer,STRING_DIRCTIVE)==EQUAL_STRING)
		return TRUE;
	return FALSE;
}

bool is_legal_label(char * label){
	int i=0;
	int lengthLabel;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,label);
	lengthLabel=strlen(buffer);
	if(buffer[lengthLabel-1]==COLON){
		buffer[lengthLabel-1]=END_STRING;
		lengthLabel--;
	}
	if(isalpha(buffer[i])==0 && lengthLabel>LEGAL_LENGTH_LABEL)
		return FALSE;
	else
	{
		for(i=1;i<lengthLabel;i++){
			if(isalnum(buffer[i])==0)
				return FALSE;
		}
	}
	return is_save_word(buffer);
}

bool is_save_word(char * word){
	int i;
	char buffer[SIZE_OF_LINE];
	char *directivesWords[NUMBER_OF_DIRECTIVES] = {"data\0","string\0","struct\0","entry\0","extern\0"};
	char *actionsWords[NUMBER_OF_ACTIONS] = {"mov\0","cmp\0","add\0","sub\0","lea\0","clr\0","not\0","inc\0","dec\0","jmp\0","bne\0","red\0","prn\0","jsr\0","rts\0","stop\0"};
	char *registersWords[NUMBER_0F_REGISTERS]={"r0\0","r1\0","r2\0","r3\0","r4\0","r5\0","r6\0","r7\0","PSW\0"};
	strcpy(buffer,word);
	for(i=0;i<NUMBER_OF_DIRECTIVES;i++){
		if(strcmp(directivesWords[i],buffer)==EQUAL_STRING)
			return FALSE;
	}
	for(i=0;i<NUMBER_OF_ACTIONS;i++){
		if(strcmp(actionsWords[i],buffer)==EQUAL_STRING)
			return FALSE;
	}
	for(i=0;i<NUMBER_0F_REGISTERS;i++){
			if(strcmp(registersWords[i],buffer)==EQUAL_STRING)
				return FALSE;
	}
	return TRUE;
}

void remove_colon_from_label(char * label){
		if(label[strlen(label)-1]==COLON)
			label[strlen(label)-1]=END_STRING;
}

bool is_legal_string(char * word){
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,word);
	if(strlen(buffer)<MINIMUM_FOR_STRING)
		return FALSE;
	else if(buffer[0]!=QUOTATION_MARKS||buffer[strlen(buffer)-1]!=QUOTATION_MARKS)
		return FALSE;
	else
		return TRUE;
}

bool is_legal_data(char *word){
	int i;
	char c;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,word);
	if(isdigit(buffer[0])==0&&buffer[0]!=DIGIT_PLUS&&buffer[0]!=DIGIT_MINUS)
		return FALSE;
	else if(buffer[strlen(buffer)-1]==COMMA)
		return FALSE;
	else{
		c=buffer[0];
		for(i=1;i<strlen(buffer);i++){
			if(buffer[i]!=COMMA&&isdigit(buffer[i])==0&&buffer[i]!=DIGIT_PLUS&&buffer[i]!=DIGIT_MINUS)
				return FALSE;
			else if(c==COMMA&&buffer[i]==COMMA)
				return FALSE;
			c=buffer[i];
		}
		return TRUE;
	}
}

int count_operands(char *word){
	char *token;
	char buffer[SIZE_OF_LINE];
	int numOperands=0;
	strcpy(buffer,word);
	token=strtok(buffer,COMMA_STRING);
	while(token!=NULL){
		numOperands++;
		token=strtok(NULL,COMMA_STRING);
	}
	return numOperands;
}

bool is_register(char * word){
	int i;
	char *registersWords[NUMBER_0F_REGISTERS]={"r0\0","r1\0","r2\0","r3\0","r4\0","r5\0","r6\0","r7\0","PSW\0"};
	for(i=0;i<NUMBER_0F_REGISTERS;i++){
			if(strcmp(registersWords[i],word)==EQUAL_STRING)
				return TRUE;
	}
	return FALSE;
}

bool is_legal_immediate_addr(char * operand){
	int i=0;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,operand);
	if(buffer[i]!=BEGIN_IMMEDIATE_ADDR)
		return FALSE;
	i++;
	if(strlen(buffer)>i){
		if(isdigit(buffer[i])==0&&buffer[i]!=DIGIT_PLUS&&buffer[i]!=DIGIT_MINUS)
			return FALSE;
		for(i=2;i<strlen(buffer);i++){
			if(isdigit(buffer[i])==0)
				return FALSE;
		}
		return TRUE;
	}
	else
		return FALSE;
}

bool is_data_str_or_ext(char * word){
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,word);
	if(is_storage_directive(buffer)==TRUE)
		return TRUE;
	else if(strcmp(buffer,EXTERN_DIRECTIVE)==EQUAL_STRING)
		return TRUE;
	else
		return FALSE;
}

bool is_entry(char * word){
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,word);
	if(strcmp(buffer,ENTRY_DIRCTIVE)==EQUAL_STRING)
		return TRUE;
	else
		return FALSE;
}
