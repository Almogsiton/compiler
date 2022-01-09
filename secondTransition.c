#include <stdlib.h>
#include <string.h>
#include "secondTransition.h"

void second_transition(char *fileNameAs,FILE *file,bool *errorState,pSymbolTable * symbolTable,pImage * imageTableCode,pImage * imageTableData,pSymbolTable *entTable,pSymbolTable *extTable,int *sumExt,int *sumEnt){
	int state=READ_NEXT_LINE,lineNumber=0,num=0,i=0;
	unsigned int opcodeIn=0,functIn=0,destinationAddrIn=0,sourceAddrIn=0;
	pImage currentCode=NULL;
	pSymbolTable curSymbol=NULL;
	char fixLine[SIZE_OF_LINE],firstWord[LEGAL_LENGTH_LABEL+1],secondWord[LEGAL_LENGTH_LABEL+1],thirdWord[LEGAL_LENGTH_LABEL+1],buffer[SIZE_OF_LINE];
	char*token=NULL;
	char operand[SIZE_OF_LINE];
	char readLine[SIZE_OF_LINE];
	for(i=0;state!=READ_ALL_FILE;i++){
		switch(state){
			case READ_NEXT_LINE:/*read the next line*/
				lineNumber++;
				if(fgets(readLine,SIZE_OF_LINE,file)!=NULL){
					remove_space(readLine,fixLine);/*fix the line by remove all the spaces that we don't need.*/
					if(check_empty_line(fixLine)==TRUE){
						printf("line %d is empty line\n",lineNumber);
						state=READ_NEXT_LINE;
						break;
					}
					else if(fixLine[0]==';'){
						state=READ_NEXT_LINE;
						break;
					}
					else{
						state=SKIP_LABEL;
						break;
					}
				}
				else{
					state=READ_ALL_FILE;
					break;
				}

			case SKIP_LABEL:/*if this is label we already take care of that.*/
				if(is_label_line(fixLine)==TRUE){
					if(words_on_line(fixLine)==SECOND_WORD){
						selected_word_line(fixLine,firstWord,SECOND_WORD);
					}
					else{
						selected_word_line(fixLine,firstWord,SECOND_WORD);
						selected_word_line(fixLine,secondWord,THIRD_WORD);
					}
				}
				else{
					if(words_on_line(fixLine)==FIRST_WORD)
						selected_word_line(fixLine,firstWord,FIRST_WORD);
					else if(words_on_line(fixLine)==SECOND_WORD){
						selected_word_line(fixLine,firstWord,FIRST_WORD);
						selected_word_line(fixLine,secondWord,SECOND_WORD);
					}
					else{
						selected_word_line(fixLine,firstWord,FIRST_WORD);
						selected_word_line(fixLine,secondWord,SECOND_WORD);
						selected_word_line(fixLine,thirdWord,THIRD_WORD);
					}
				}
				state=IS_DATA_STRING_EXT;
				break;
			case IS_DATA_STRING_EXT:/*if this is data or string or external we have taken care of that already. */
				if(is_data_str_or_ext(firstWord)==TRUE)
					state=READ_NEXT_LINE;
				else
					state=IS_ENTRY;
				break;
			case IS_ENTRY:/*if this is entry we add to sumEnt one record.*/
				if(is_entry(firstWord)==TRUE){
					state=ADD_ENTRY;
					*sumEnt=*sumEnt+1;
				}
				else
					state=READ_NEXT_LINE;
				break;
			case ADD_ENTRY:/*we update the symbol table for the entry record.*/
				if(words_on_line(fixLine)==FIRST_WORD){
					*errorState=TRUE;
					print_error(LESS_WORD_ENT,lineNumber,readLine,fileNameAs);
					state=READ_NEXT_LINE;
				}
				else if(words_on_line(fixLine)==THIRD_WORD){
					*errorState=TRUE;
					print_error(TOO_MUCH_WORD_ENT,lineNumber,readLine,fileNameAs);
					state=READ_NEXT_LINE;
				}
				else{
					if(symbol_already_there(*symbolTable,secondWord)==FALSE){
						*errorState=TRUE;
		                print_error(ENTRY_NOT_EXIST,lineNumber,readLine,fileNameAs);
		                state=READ_NEXT_LINE;
		            }
					else if(symbol_already_there_with_extern(*symbolTable,secondWord)==TRUE){
						*errorState=TRUE;
						print_error(ENTRY_WITH_EXTERNAL,lineNumber,readLine,fileNameAs);
					    state=READ_NEXT_LINE;
					}
					else{
	                    update_symbol_type_to_ent(*symbolTable,secondWord);
	                    state=READ_NEXT_LINE;
					}
				}
				break;
		}
	}
	for(currentCode=*imageTableCode;currentCode!=NULL;currentCode=currentCode->next){/*we build the image data code for all the record that the feature A.R.E is unknown.*/
		if(currentCode->are==UNKNOWN_ARE){
			strcpy(operand,currentCode->sorceCode);
			if(operand[0]=='%'){
				token=strtok(operand,"%");
				strcpy(buffer,token);
				if(symbol_already_there(*symbolTable,buffer)==FALSE){
					*errorState=TRUE;
					print_error(RELATIVE_ADDRESS_SYMBOL_NOT_EXIST,currentCode->address,currentCode->sorceCode,fileNameAs);
				}
				else if(symbol_already_there_with_extern(*symbolTable,buffer)==TRUE){
					*errorState=TRUE;
					print_error(RELATIVE_ADDRESS_EXT,currentCode->address,currentCode->sorceCode,fileNameAs);
				}
				else{
					num=find_label_value(*symbolTable,buffer)-(currentCode->address);
					int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
					currentCode->are=ABSOLUTE;
					currentCode->codeWord.destinationAddress=destinationAddrIn;
					currentCode->codeWord.sourceAddress=sourceAddrIn;
					currentCode->codeWord.funct=functIn;
					currentCode->codeWord.opcode=opcodeIn;
				}
			}
			else{
				if(symbol_already_there(*symbolTable,operand)==FALSE){
					*errorState=TRUE;
					print_error(DIRECT_ADDRESS_LEBEL_NOT_EXIST,currentCode->address,currentCode->sorceCode,fileNameAs);
				}
				else{
					if(symbol_already_there_with_extern(*symbolTable,operand)==TRUE){
						*sumExt=*sumExt+1;
						add_record(operand,currentCode->address,EXTERNAL_SYMBOL,extTable);
						currentCode->are=EXTERNAL;
						num=find_label_value(*symbolTable,operand);
						int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
						currentCode->codeWord.destinationAddress=destinationAddrIn;
						currentCode->codeWord.sourceAddress=sourceAddrIn;
						currentCode->codeWord.funct=functIn;
						currentCode->codeWord.opcode=opcodeIn;
					}
					else{
						currentCode->are=RELOCATABLE;
						num=find_label_value(*symbolTable,operand);
						int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
						currentCode->codeWord.destinationAddress=destinationAddrIn;
						currentCode->codeWord.sourceAddress=sourceAddrIn;
						currentCode->codeWord.funct=functIn;
						currentCode->codeWord.opcode=opcodeIn;
					}
				}
			}
		}
	}
	for(curSymbol=*symbolTable;curSymbol!=NULL;curSymbol=curSymbol->next){/*we build the entry table.*/
		if(curSymbol->symbolType==ENTRY_SYMBOL||curSymbol->symbolType==CODE_AND_ENTRY||curSymbol->symbolType==DATA_AND_ENTRY){
			strcpy(buffer,curSymbol->symbolName);
			lineNumber=curSymbol->value;
			add_record(buffer,lineNumber,ENTRY_SYMBOL,entTable);
		}
	}

}


