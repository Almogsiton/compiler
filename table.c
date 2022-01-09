#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

void add_record(char *nameOfSymbol,int value,symbol_type symbolType,pSymbolTable * ptable){
	char *tempSymbolName;
	pSymbolTable prevRecord,curRecord,newRecord;
	newRecord=(pSymbolTable)allocation(sizeof(symbol_table));
	tempSymbolName=(char *) allocation(strlen(nameOfSymbol) + 1);
	strcpy(tempSymbolName, nameOfSymbol);
	newRecord->symbolName=tempSymbolName;
	newRecord->value=value;
	newRecord->symbolType=symbolType;
	if((*ptable==NULL)||((*ptable)->value>value)){
		newRecord->next=*ptable;
		*ptable=newRecord;
		return;
	}
	curRecord=(*ptable)->next;
	prevRecord=*ptable;
	while(curRecord!=NULL&&curRecord->value<value){
		prevRecord=curRecord;
		curRecord=curRecord->next;
	}
	newRecord->next=curRecord;
	prevRecord->next=newRecord;
}

void free_table(pSymbolTable * ptable){
	pSymbolTable prevRecord=NULL,curRecord=*ptable;
	while(curRecord){
		prevRecord=curRecord;
		curRecord=curRecord->next;
		free(prevRecord->symbolName);
		free(prevRecord);
	}
}

void print_symbol_table(pSymbolTable ptable){
	pSymbolTable curRecord=NULL;
	for(curRecord=ptable;curRecord!=NULL;curRecord=curRecord->next){
		printf("%s\t%d\t",curRecord->symbolName,curRecord->value);
		if(curRecord->symbolType==CODE_SYMBOL)
			printf("code\n");
		else if(curRecord->symbolType==DATA_SYMBOL)
			printf("data\n");
		else if(curRecord->symbolType==EXTERNAL_SYMBOL)
			printf("external\n");
		else if(curRecord->symbolType==ENTRY_SYMBOL)
			printf("entry\n");
		else if(curRecord->symbolType==DATA_AND_EXTERNAL)
			printf("data,external\n");
		else if(curRecord->symbolType==CODE_AND_ENTRY)
			printf("code,entry\n");
		else if(curRecord->symbolType==DATA_AND_ENTRY)
			printf("data,entry\n");
	}
}

bool symbol_already_there(symbol_table *table,char *label){
	pSymbolTable curRecord=NULL;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,label);
	remove_colon_from_label(buffer);
	for(curRecord=table;curRecord!=NULL;curRecord=curRecord->next){
		if(strcmp(curRecord->symbolName,buffer)==EQUAL_STRING)
			return TRUE;
	}
	return FALSE;
}

bool check_symbol_type_and_label(symbol_table *table,symbol_type symbolType,char *label){
	pSymbolTable curRecord=NULL;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,label);
	remove_colon_from_label(buffer);
	for(curRecord=table;curRecord!=NULL;curRecord=curRecord->next){
		if(symbolType!=curRecord->symbolType&&strcmp(curRecord->symbolName,buffer)==EQUAL_STRING)
			return TRUE;
	}
	return FALSE;
}

void update_symbol_table(symbol_table * ptable,int ICF,symbol_type symbolType){
	pSymbolTable curRecord=NULL;
	for(curRecord=ptable;curRecord!=NULL;curRecord=curRecord->next){
		if(curRecord->symbolType==symbolType)
			curRecord->value+=ICF;
	}
}

bool symbol_already_there_with_extern(symbol_table *table,char *label){
	pSymbolTable curRecord=NULL;
	char buffer[SIZE_OF_LINE];
	strcpy(buffer,label);
	for(curRecord=table;curRecord!=NULL;curRecord=curRecord->next){
		if(strcmp(curRecord->symbolName,buffer)==EQUAL_STRING&&curRecord->symbolType==EXTERNAL_SYMBOL)
			return TRUE;
	}
	return FALSE;
}

void update_symbol_type_to_ent(symbol_table * ptable,char *label){
	pSymbolTable curRecord=NULL;
	for(curRecord=ptable;curRecord!=NULL;curRecord=curRecord->next){
		if(strcmp(curRecord->symbolName,label)==EQUAL_STRING){
			if(curRecord->symbolType==DATA_SYMBOL){
				curRecord->symbolType=DATA_AND_ENTRY;
				return;
			}
			else if(curRecord->symbolType==CODE_SYMBOL){
				curRecord->symbolType=CODE_AND_ENTRY;
				return;
			}
		}
	}
}

int find_label_value(symbol_table * ptable,char *label){
	pSymbolTable curRecord=NULL;
	for(curRecord=ptable;curRecord!=NULL;curRecord=curRecord->next){
		if(strcmp(curRecord->symbolName,label)==EQUAL_STRING)
			return curRecord->value;
	}
	return -1;
}
