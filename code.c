#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "code.h"

void add_image(int address,char *sorceCode,code_word codeWord,char are,pImage * imagePointer){
	char *tempSorceCode;
	pImage prevImage,curImage,newImage;
	newImage=(pImage)allocation(sizeof(image_memory));
	tempSorceCode=(char *) allocation(strlen(sorceCode) + 1);
	strcpy(tempSorceCode, sorceCode);
	newImage->address=address;
	newImage->sorceCode=tempSorceCode;
	newImage->codeWord=codeWord;
	newImage->are=are;
	if((*imagePointer)==NULL||(*imagePointer)->address>address){
		newImage->next=(*imagePointer);
		(*imagePointer)=newImage;
		return;
	}
	curImage=(*imagePointer)->next;
	prevImage=*imagePointer;
	while(curImage!=NULL&&curImage->address<address){
		prevImage=curImage;
		curImage=curImage->next;
	}
	newImage->next=curImage;
	prevImage->next=newImage;
}

void print_image(pImage imagePointer){
	pImage curRecord;
	char word[WORD_LENGTH+1],line[SIZE_OF_LINE];
	int sum;
	for(curRecord=imagePointer;curRecord!=NULL;curRecord=curRecord->next){
		sum=curRecord->codeWord.destinationAddress+(4*curRecord->codeWord.sourceAddress)+(16*curRecord->codeWord.funct)+(256*curRecord->codeWord.opcode);
		decimal_to_binary(sum,word);
		strcpy(line,curRecord->sorceCode);
		printf("%.4d\t%s\t%c\n",curRecord->address,word,curRecord->are);
	}
}

void free_image(pImage * imagePointer){
	pImage prevImage,curImage=*imagePointer;
	while(curImage){
		prevImage=curImage;
		curImage=curImage->next;
			free(prevImage->sorceCode);
			free(prevImage);
	}
}

void print_error(error_mode mode,int lineNumber,char * line,char *fileNameAs){
	char buffer[SIZE_OF_LINE];
	char bufferName[SIZE_OF_LINE];
	strcpy(bufferName,fileNameAs);
	strcpy(buffer,line);
	if(mode==WRONG_PLACE_LABEL)
		printf("%s:%d:error:the label is not define well in line %d :%s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==WRONG_LENGTH_LINE)
		printf("%s:%d:error:the length of the line %d is more from 80 :%s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==LESS_WORD_LABEL)
		printf("%s:%d:error:we don't have enough words for label in line %d:%s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==TOO_MUCH_WORD)
		printf("%s:%d:error:we have too much words in line %d: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ILLEGAL_LABEL)
		printf("%s:%d:error:the label in line %d is illegal : %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==LABEL_ALREADY_THERE)
		printf("%s:%d:error:the label in line %d is already exist: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ILLEGAL_STRING)
		printf("%s:%d:error:the string in line %d is illegal string: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ILLEGAL_DATA)
		printf("%s:%d:error:the data in line %d is illegal data: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==TOO_MUCH_WORDS_EXT)
		printf("%s:%d:error:we have too much words in line %d for .extern: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==LESS_WORDS_EXT)
		printf("%s:%d:error:we don't have enough words for .extern in line %d: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ILLEGAL_LABEL_IN_EXT)
		printf("%s:%d:error:the label after .extern in line %d is illegal: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==LABEL_ALREADY_THERE_NOT_EXT)
		printf("%s:%d:error:the label after .extern in line %d is already exist but the symbol type is not external: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ILLEGAL_COMMAND_NAME)
		printf("%s:%d:error:the command name in line %d is unknown in the table command: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==WRONG_NUM_OF_WORDS_FOR_COMMAND)
		printf("%s:%d:error:the number of words in line %d is incorrect according to command: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==TOO_MUCH_OPERANDS)
		printf("%s:%d:error:we have too much operands in line %d: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==WRONG_NUM_OPPERAND)
		printf("%s:%d:error:we have wrong numbers of operands in line %d: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==OPERANDS_NOT_MATCH)
		printf("%s:%d:error:the command and the operands in line %d are not match: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ENTRY_NOT_EXIST)
		printf("%s:%d:error:that is .entry guidance in line %d but the symbol is not exist in the symbol table: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==ENTRY_WITH_EXTERNAL)
		printf("%s:%d:error:that is .entry guidance in line %d but the symbol already .extern guidance  in the symbol table: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==RELATIVE_ADDRESS_EXT)
		printf("%s:%d:error:address %d is relative address and the label is external this is illegal: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==RELATIVE_ADDRESS_SYMBOL_NOT_EXIST)
		printf("%s:%d:error:address %d is relative address but the label is not exist: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==DIRECT_ADDRESS_LEBEL_NOT_EXIST)
		printf("%s:%d:error:address %d is direct address but the label is not exist: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==LESS_WORD_ENT)
		printf("%s:%d:error:we don't have enough words for .entry in line %d: %s\n",bufferName,lineNumber,lineNumber,buffer);
	else if(mode==TOO_MUCH_WORD_ENT)
		printf("%s:%d:error:we  have too much words for .entry in line %d: %s\n",bufferName,lineNumber,lineNumber,buffer);
}
/*this is struct of command with name,command,opcode,number of operand.*/
struct command_table {
	char *commandName;
	opcode op;
	funct fun;
	int numOfOprand;
};

/* A lookup table for opcode & function by command name.*/
static struct command_table search_table[] = {
		{"mov\0", MOV_OPCODE,NONE_FUNCT,TWO_OPERANDS},
		{"cmp\0",CMP_OPCODE,NONE_FUNCT,TWO_OPERANDS},
		{"add\0",ADD_OPCODE,ADD_FUNCT,TWO_OPERANDS},
		{"sub\0",SUB_OPCODE,SUB_FUNCT,TWO_OPERANDS},
		{"lea\0",LEA_OPCODE,NONE_FUNCT,TWO_OPERANDS},
		{"clr\0",CLR_OPCODE,CLR_FUNCT,ONE_OPERANDS},
		{"not\0",NOT_OPCODE,NOT_FUNCT,ONE_OPERANDS},
		{"inc\0",INC_OPCODE,INC_FUNCT,ONE_OPERANDS},
		{"dec\0",DEC_OPCODE,DEC_FUNCT,ONE_OPERANDS},
		{"jmp\0",JMP_OPCODE,JMP_FUNCT,ONE_OPERANDS},
		{"bne\0",BNE_OPCODE,BNE_FUNCT,ONE_OPERANDS},
		{"jsr\0",JSR_OPCODE,JSR_FUNCT,ONE_OPERANDS},
		{"red\0",RED_OPCODE,NONE_FUNCT,ONE_OPERANDS},
		{"prn\0",PRN_OPCODE,NONE_FUNCT,ONE_OPERANDS},
		{"rts\0",RTS_OPCODE,NONE_FUNCT,ZERO_OPERANDS},
		{"stop\0",STOP_OPCODE,NONE_FUNCT,ZERO_OPERANDS},
		{NULL, NONE_OPCODE,NONE_FUNCT,ZERO_OPERANDS}
};

void get_opcode_func(char *name, opcode *opcodeOut, funct *functOut) {
	struct command_table *ptable;
	*opcodeOut = NONE_OPCODE;
	*functOut = NONE_FUNCT;
	for (ptable = search_table; ptable->commandName != NULL; ptable++) {
		if (strcmp(ptable->commandName,name) == 0) {
			*opcodeOut = ptable->op;
			*functOut = ptable->fun;
			return;
		}
	}
}

bool is_legal_command_name(char * name){
	struct command_table *ptable;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,name);
	for (ptable = search_table; ptable->commandName != NULL; ptable++){
		if (strcmp(ptable->commandName,buffer) == EQUAL_STRING)
			return TRUE;
	}
	return FALSE;
}

bool is_legal_num_of_words_for_command(char *commandName,number_of_words num){
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,commandName);
	if(num==FIRST_WORD){
		if(strcmp(buffer,"rts")==EQUAL_STRING||strcmp(buffer,"stop")==EQUAL_STRING)
			return TRUE;
		else
			return FALSE;
	}
	else{
		if(strcmp(buffer,"rts")!=EQUAL_STRING&&strcmp(buffer,"stop")==EQUAL_STRING)
			return TRUE;
		else
			return FALSE;
	}
}

bool is_legal_operand_for_command(int num,char * name){
	struct command_table *ptable;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,name);
	for (ptable = search_table; ptable->commandName != NULL; ptable++){
		if (strcmp(ptable->commandName,buffer) == EQUAL_STRING&&num==ptable->numOfOprand)
			return TRUE;
	}
	return FALSE;
}

bool is_legal_address(char *command,char *operands,int L){
	char * token;
	char buffer1[SIZE_OF_LINE];
	char buffer2[SIZE_OF_LINE];
	strcpy(buffer1,command);
	strcpy(buffer2,operands);
	if(L==TWO_OPERANDS){
		if(buffer2[0]=='#'&&is_legal_immediate_addr(buffer2)==FALSE)
			return FALSE;
		if(buffer2[0]=='#'&&strcmp(buffer1,"prn")!=EQUAL_STRING)
			return FALSE;
		else if(buffer2[0]=='%'){
			if(strcmp(buffer1,"jsr")==EQUAL_STRING)
				return TRUE;
			else if(strcmp(buffer1,"bne")==EQUAL_STRING)
				return TRUE;
			else if(strcmp(buffer1,"jmp")==EQUAL_STRING)
				return TRUE;
			else
				return FALSE;
		}
		else{
			if(strcmp(buffer1,"prn")!=EQUAL_STRING&&strcmp(buffer1,"dec")!=EQUAL_STRING&&strcmp(buffer1,"inc")!=EQUAL_STRING&&strcmp(buffer1,"not")!=EQUAL_STRING&&strcmp(buffer1,"clr")!=EQUAL_STRING){
				if(is_register(buffer2)==TRUE)
					return FALSE;
				return TRUE;
			}
			else
				return TRUE;
		}
	}
	else{
		token=strtok(buffer2,COMMA_STRING);
		if(token[0]=='#'&&is_legal_immediate_addr(buffer2)==FALSE)
			return FALSE;
		if(token[0]=='#'&&strcmp(buffer1,"lea")==EQUAL_STRING)
			return FALSE;
		if(token[0]=='%')
			return FALSE;
		if(strcmp(buffer1,"lea")==EQUAL_STRING&&is_register(token)==TRUE)
			return FALSE;
		token=strtok(NULL,COMMA_STRING);
		if(token[0]=='#'&&is_legal_immediate_addr(token)==FALSE)
			return FALSE;
		if(token[0]=='#'&&strcmp(buffer1,"cmp")!=EQUAL_STRING)
			return FALSE;
		if(token[0]=='%')
			return FALSE;
	}
	return TRUE;
}

void build_addresses(int L,unsigned int * sourceAddrOut,unsigned int * destinationAddrOut,char *operands){
	char * token;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,operands);
	if(L==TWO_OPERANDS){
		*sourceAddrOut=NONE_ADDR;
		if(buffer[0]=='#')
			*destinationAddrOut=IMMEDIATE_ADDR;
		else if(buffer[0]=='%')
			*destinationAddrOut=RELATIVE_ADDR;
		else if(is_register(buffer)==TRUE)
			*destinationAddrOut=REGISTER_ADDR;
		else
			*destinationAddrOut=DIRECT_ADDR;
	}
	else{
		token=strtok(buffer,COMMA_STRING);
		if(token[0]=='#')
			*sourceAddrOut=IMMEDIATE_ADDR;
		else if(is_register(token)==TRUE)
			*sourceAddrOut=REGISTER_ADDR;
		else
			*sourceAddrOut=DIRECT_ADDR;
		token=strtok(NULL,COMMA_STRING);
		if(token[0]=='#')
			*destinationAddrOut=IMMEDIATE_ADDR;
		else if(is_register(token)==TRUE)
			*destinationAddrOut=REGISTER_ADDR;
		else
			*destinationAddrOut=DIRECT_ADDR;
	}
}

void update_data_image(image_memory * pmemory,int ICF){
	pImage curRecord;
	for(curRecord=pmemory;curRecord!=NULL;curRecord=curRecord->next){
		curRecord->address+=ICF;
	}
}

void char_to_code_word(char c,unsigned int * opcodeIn,unsigned int *functIn,unsigned int *sourceAddrIn,unsigned int *destinationAddrIn){
	unsigned int num=c;
		*opcodeIn=num/256;
		num=num-*opcodeIn*256;
		*functIn=num/16;
		num=num-*functIn*16;
		*sourceAddrIn=num/4;
		num=num-*sourceAddrIn*4;
		*destinationAddrIn=num;
}

void int_to_code_word(int num,unsigned int * opcodeIn,unsigned int *functIn,unsigned int *sourceAddrIn,unsigned int *destinationAddrIn){
	int i;
	char buffer[13];
	int sum=0;
	decimal_to_binary(num,buffer);
	for(i=0;i<4;i++){
		if(buffer[i]=='1')
			sum=sum+pow(2,3-i);
	}
	*opcodeIn=sum;
	sum=0;
	for(i=4;i<8;i++){
		if(buffer[i]=='1')
			sum=sum+pow(2,7-i);
	}
	*functIn=sum;
	sum=0;
	for(i=8;i<10;i++){
		if(buffer[i]=='1')
			sum=sum+pow(2,9-i);
	}
	*sourceAddrIn=sum;
	sum=0;
	for(i=10;i<12;i++){
		if(buffer[i]=='1')
			sum=sum+pow(2,11-i);
	}
	*destinationAddrIn=sum;
}

void register_to_code_word(char *regi,unsigned int * opcodeIn,unsigned int *functIn,unsigned int *sourceAddrIn,unsigned int *destinationAddrIn){
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,regi);
	if(strcmp(buffer,"r0")==EQUAL_STRING)
		*destinationAddrIn=1;
	else if(strcmp(buffer,"r1")==EQUAL_STRING)
		*destinationAddrIn=2;
	else if(strcmp(buffer,"r2")==EQUAL_STRING)
		*sourceAddrIn=1;
	else if(strcmp(buffer,"r3")==EQUAL_STRING)
		*sourceAddrIn=2;
	else if(strcmp(buffer,"r4")==EQUAL_STRING)
		*functIn=1;
	else if(strcmp(buffer,"r5")==EQUAL_STRING)
		*functIn=2;
	else if(strcmp(buffer,"r6")==EQUAL_STRING)
		*functIn=4;
	else if(strcmp(buffer,"r7")==EQUAL_STRING)
		*functIn=8;
}

