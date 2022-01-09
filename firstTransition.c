#include <stdlib.h>
#include <string.h>
#include "firstTransition.h"

void first_transition(char *fileNameAs,FILE *file,bool *errorState,int *IC,int *DC,pSymbolTable * symbolTable,pImage * imageTableCode,pImage * imageTableData){
	int	lineNumber=0,L=0,i=0,state=BEGIN,num=0;
	bool labelSetting=FALSE;
	code_word codeWord;
	unsigned int opcodeIn=0,functIn=0,destinationAddrIn=0,sourceAddrIn=0,tempDestination,tempSource;
	char fixLine[SIZE_OF_LINE];/*the line after we reduced all the spaces.*/
	char *token=NULL;
	char data[SIZE_OF_LINE],command[SIZE_OF_LINE],operands[SIZE_OF_LINE],firstWord[LEGAL_LENGTH_LABEL+1],secondWord[LEGAL_LENGTH_LABEL+1],thirdWord[LEGAL_LENGTH_LABEL+1],buffer[SIZE_OF_LINE];
	char firstOperand[SIZE_OF_LINE],secondOperand[SIZE_OF_LINE];
	char * readLine=(char *)allocation(sizeof(char)*SIZE_OF_LINE);
	for(i=0;state!=READ_ALL;i++){/*as long as the file is not over. */
		switch(state){
			case BEGIN:/*initializing IC and DC. */
				*IC=START_IC;
				*DC=START_DC;
				state=READ_NEXT;
				break;
			case READ_NEXT:/*read the next line*/
				lineNumber++;
				if(labelSetting==TRUE)
					labelSetting=FALSE;
				if(fgets(readLine,SIZE_OF_LINE,file)!=NULL){
					if(is_legal_line(readLine)==FALSE){
						*errorState=TRUE;
						print_error(WRONG_LENGTH_LINE,lineNumber,readLine,fileNameAs);
						break;
					}
					remove_space(readLine,fixLine);/*fix the line by remove all the spaces that we don't need.*/
					if(check_empty_line(fixLine)==TRUE){/*check if that is empty line*/
						state=READ_NEXT;
						break;
					}
					else if(words_on_line(fixLine)>THIRD_WORD){/*if the line with more from three word after the fix that is error.*/
						*errorState=TRUE;
						print_error(TOO_MUCH_WORD,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
						break;
					}
					else if(fixLine[0]==';'){/*if the line begin with ; read the next line*/
						state=READ_NEXT;
						break;
					}
					else{/*we check how much word we have exactly every word we put to firstWord,secondWord and thirdWord.*/
						state=IS_LABEL;
						if(words_on_line(fixLine)==FIRST_WORD){
							selected_word_line(fixLine,firstWord,FIRST_WORD);

						}
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
				}
				else
					state=READ_ALL;
				break;
			case IS_LABEL:/*check if the first word is symbol*/
				if(is_label_line(fixLine)==FALSE)/*check if it is label line.*/
					state=IS_STORAGE;
				else if(is_word_label(fixLine)==FALSE){/*we check if the : in the true place.*/
					*errorState=TRUE;
					print_error(WRONG_PLACE_LABEL,lineNumber,readLine,fileNameAs);
					state=READ_NEXT;
				}
				else
					state=DEFINE_LABEL;
				break;
			case DEFINE_LABEL:/*turn on labeSetting*/
				if(words_on_line(fixLine)<SECOND_WORD){
					*errorState=TRUE;
					print_error(LESS_WORD_LABEL,lineNumber,readLine,fileNameAs);
					state=READ_NEXT;
					break;
				}
				labelSetting=TRUE;
				state=IS_STORAGE;
				break;
			case IS_STORAGE:/*check if is storage directive*/
				if(is_storage_directive(secondWord)==TRUE||is_storage_directive(firstWord)==TRUE)
					state=SYMBOL_DEFINE;
				else
					state=ENT_OR_EXT;
				break;
			case SYMBOL_DEFINE:/*if labeSetting on enter the symbol to the symbol table.*/
				if(labelSetting==TRUE){
					if(is_legal_label(firstWord)==FALSE){
						*errorState=TRUE;
						print_error(ILLEGAL_LABEL,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
						break;
					}
					else if(symbol_already_there(*symbolTable,firstWord)==TRUE){/*check if the label is already in the symbol table.*/
						*errorState=TRUE;
						print_error(LABEL_ALREADY_THERE,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
						break;
					}
					else{
						remove_colon_from_label(firstWord);
						add_record(firstWord,*DC,DATA_SYMBOL,symbolTable);
						state=IDENTIFY_DATA;
						break;
					}
				}
				else{
					state=IDENTIFY_DATA;
					break;
				}
			case IDENTIFY_DATA:/*data encoding*/
				if(labelSetting==TRUE){/*if this is label and also string or data directive.*/
					if(strcmp(secondWord,STRING_DIRCTIVE)==EQUAL_STRING){/*check if the second word is string directive*/
						if(is_legal_string(thirdWord)==FALSE){/*check if this is legal string*/
							*errorState=TRUE;
							print_error(ILLEGAL_STRING,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
							break;
						}
						else{/*that is a good string enter the string to image data table.*/
							strcpy(data,thirdWord);
							for(i=1;i<strlen(data)-1;i++){
								char_to_code_word(data[i],&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
								codeWord.funct=functIn;
								codeWord.opcode=opcodeIn;
								codeWord.destinationAddress=destinationAddrIn;
								codeWord.sourceAddress=sourceAddrIn;
								add_image(*DC,fixLine,codeWord,ABSOLUTE,imageTableData);
								*DC=*DC+1;
							}
							char_to_code_word(END_STRING,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.destinationAddress=destinationAddrIn;
							codeWord.sourceAddress=sourceAddrIn;
							add_image(*DC,fixLine,codeWord,ABSOLUTE,imageTableData);
							*DC=*DC+1;
						}
					}
					else{/*this is data directive*/
						if(is_legal_data(thirdWord)==FALSE){/*check if that is a legal data.*/
							printf("the third word is %s\n",thirdWord);
							*errorState=TRUE;
							print_error(ILLEGAL_DATA,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
							break;
						}
						else{/*enter the data to the image data table.*/
							strcpy(data,thirdWord);
							token=strtok(data,COMMA_STRING);
							while(token!=NULL){
								num=atoi(token);
								int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
								codeWord.funct=functIn;
								codeWord.opcode=opcodeIn;
								codeWord.destinationAddress=destinationAddrIn;
								codeWord.sourceAddress=sourceAddrIn;
								add_image(*DC,fixLine,codeWord,ABSOLUTE,imageTableData);
								*DC=*DC+1;
								token=strtok(NULL,COMMA_STRING);
							}
						}
					}
					state=READ_NEXT;
					break;
				}
				else{/*this is not label that is string or data directive*/
					if(strcmp(firstWord,STRING_DIRCTIVE)==EQUAL_STRING){/*check if that is string directive.*/
						if(is_legal_string(secondWord)==FALSE){
							*errorState=TRUE;
							print_error(ILLEGAL_STRING,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
							break;
						}
						else{/*this is legal string enter the string to the image data.*/
							strcpy(data,secondWord);
							for(i=1;i<strlen(data)-1;i++){
								char_to_code_word(data[i],&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
								codeWord.funct=functIn;
								codeWord.opcode=opcodeIn;
								codeWord.destinationAddress=destinationAddrIn;
								codeWord.sourceAddress=sourceAddrIn;
								add_image(*DC,fixLine,codeWord,ABSOLUTE,imageTableData);
								*DC=*DC+1;
							}
							char_to_code_word(END_STRING,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.destinationAddress=destinationAddrIn;
							codeWord.sourceAddress=sourceAddrIn;
							add_image(*DC,fixLine,codeWord,ABSOLUTE,imageTableData);
							*DC=*DC+1;
						}
					}
					else{/*that is data directive*/
						if(is_legal_data(secondWord)==FALSE){
							*errorState=TRUE;
							print_error(ILLEGAL_DATA,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
							break;
						}
						else{/*enter the data to the image data table.*/
							strcpy(data,secondWord);
							token=strtok(data,COMMA_STRING);
							while(token!=NULL){
								num=atoi(token);
								int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
								codeWord.funct=functIn;
								codeWord.opcode=opcodeIn;
								codeWord.destinationAddress=destinationAddrIn;
								codeWord.sourceAddress=sourceAddrIn;
								add_image(*DC,fixLine,codeWord,ABSOLUTE,imageTableData);
								*DC=*DC+1;
								token=strtok(NULL,COMMA_STRING);
							}
						}
					}
					state=READ_NEXT;
					break;
				}
			case ENT_OR_EXT:/*check if is entry or extern directive.*/
				if(labelSetting==TRUE){
					if(strcmp(secondWord,ENTRY_DIRCTIVE)==EQUAL_STRING||strcmp(secondWord,EXTERN_DIRECTIVE)==EQUAL_STRING){
						printf("%s:%d:Warning: we have label before .entry or .extern\n",fileNameAs,lineNumber);
						state=READ_NEXT;
					}
					else
						state=IS_COMMAND_LINE;
				}
				else{
					if(strcmp(firstWord,ENTRY_DIRCTIVE)==EQUAL_STRING)
						state=IF_ENT;
					else if(strcmp(firstWord,EXTERN_DIRECTIVE)==EQUAL_STRING)
						state=IF_EXT;
					else
						state=IS_COMMAND_LINE;
				}
				break;
			case IF_ENT:/*check if entry directive we take care that in the second transition.*/
				state=READ_NEXT;
				break;
			case IF_EXT:/*that is extern directive*/
				if(words_on_line(fixLine)==THIRD_WORD){
					*errorState=TRUE;
					print_error(TOO_MUCH_WORDS_EXT,lineNumber,readLine,fileNameAs);
				}
				else if(words_on_line(fixLine)==FIRST_WORD){
					*errorState=TRUE;
					print_error(LESS_WORDS_EXT,lineNumber,readLine,fileNameAs);
				}
				else if(is_legal_label(secondWord)==FALSE){
					*errorState=TRUE;
					print_error(LESS_WORDS_EXT,lineNumber,readLine,fileNameAs);
				}
				else if(check_symbol_type_and_label(*symbolTable,EXTERNAL_SYMBOL,secondWord)==TRUE){
					*errorState=TRUE;
					print_error(LABEL_ALREADY_THERE_NOT_EXT,lineNumber,readLine,fileNameAs);
				}
				else/*add the label to the symbol table*/
					add_record(secondWord,EXTENAL_VALUE,EXTERNAL_SYMBOL,symbolTable);
				state=READ_NEXT;
				break;
			case IS_COMMAND_LINE:/*this is command line if labeSetting is on enter the symbol to symbol table. */
				if(labelSetting==TRUE){
					remove_colon_from_label(firstWord);
					if(is_legal_label(firstWord)==FALSE){
						*errorState=TRUE;
						print_error(ILLEGAL_LABEL,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
					}
					else if(symbol_already_there(*symbolTable,firstWord)==TRUE){
						*errorState=TRUE;
						print_error(LABEL_ALREADY_THERE,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
					}
					else{
						add_record(firstWord,*IC,CODE_SYMBOL,symbolTable);
						state=SEARCH_ACTION_NAME;
					}
				}
				else
					state=SEARCH_ACTION_NAME;
				break;
			case SEARCH_ACTION_NAME:/*check if is legal action*/
				if(labelSetting==TRUE){
					if(is_legal_command_name(secondWord)==FALSE){
						*errorState=TRUE;
						print_error(ILLEGAL_COMMAND_NAME,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
					}
					else
						state=NUM_OF_WORDS;
				}
				else{
					if(is_legal_command_name(firstWord)==FALSE){
						*errorState=TRUE;
						print_error(ILLEGAL_COMMAND_NAME,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
					}
					else
						state=NUM_OF_WORDS;
				}
				break;
			case NUM_OF_WORDS:/*calculate how much word the instruction need.*/
				if(labelSetting==TRUE){/*calculate how much word the instruction need if that is label*/
					if(words_on_line(fixLine)==SECOND_WORD){
						strcpy(command,secondWord);
						strcpy(operands,EMPTY_STRING);
						if(is_legal_num_of_words_for_command(command,FIRST_WORD)==FALSE){
							*errorState=TRUE;
							print_error(WRONG_NUM_OF_WORDS_FOR_COMMAND,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
						}
						else{
							L=ONE_OPERANDS;
							state=BUILD_FIRST_BIN;
						}
					}
					else{
						L=ONE_OPERANDS+count_operands(thirdWord);
						if(L>THREE_OPERANDS){
							*errorState=TRUE;
							print_error(TOO_MUCH_OPERANDS,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
						}
						else{
							strcpy(command,secondWord);
							strcpy(operands,thirdWord);
							if(is_legal_operand_for_command(count_operands(thirdWord),command)==FALSE){
								*errorState=TRUE;
								print_error(WRONG_NUM_OPPERAND,lineNumber,readLine,fileNameAs);
								state=READ_NEXT;
							}
							else
								state=BUILD_FIRST_BIN;
						}
					}
				}
				else{/*calculate how much word the instruction need if that is not label*/
					if(words_on_line(fixLine)==FIRST_WORD){
						strcpy(command,firstWord);
						strcpy(operands,EMPTY_STRING);
						if(is_legal_num_of_words_for_command(command,FIRST_WORD)==FALSE){
							*errorState=TRUE;
							print_error(WRONG_NUM_OF_WORDS_FOR_COMMAND,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
						}
						else{
							L=ONE_OPERANDS;
							state=BUILD_FIRST_BIN;
						}
					}
					else{
						L=ONE_OPERANDS+count_operands(secondWord);
						if(L>THREE_OPERANDS){
							*errorState=TRUE;
							print_error(TOO_MUCH_OPERANDS,lineNumber,readLine,fileNameAs);
							state=READ_NEXT;
						}
						else{
							strcpy(command,firstWord);
							strcpy(operands,secondWord);
							if(is_legal_operand_for_command(count_operands(secondWord),command)==FALSE){
								*errorState=TRUE;
								print_error(WRONG_NUM_OPPERAND,lineNumber,readLine,fileNameAs);
								state=READ_NEXT;
							}
							else
								state=BUILD_FIRST_BIN;
						}
					}
				}
				break;
			case BUILD_FIRST_BIN:/*build the first word and all the word that we can to encoding if we cant we cant feature A.R.E will be question mark */
				if(L==ONE_OPERANDS){/*we have one operand enter*/
					get_opcode_func(command,&opcodeIn,&functIn);
					codeWord.funct=functIn;
					codeWord.opcode=opcodeIn;
					codeWord.destinationAddress=NONE_ADDR;
					codeWord.sourceAddress=NONE_ADDR;
					add_image(*IC,fixLine,codeWord,ABSOLUTE,imageTableCode);/*enter the operand to the image table code.*/
				}
				else{
					if(is_legal_address(command,operands,L)==FALSE){
						*errorState=TRUE;
						print_error(OPERANDS_NOT_MATCH,lineNumber,readLine,fileNameAs);
						state=READ_NEXT;
						break;
					}
					else if(L==TWO_OPERANDS){/*we have two operand*/
						build_addresses(L,&sourceAddrIn,&destinationAddrIn,operands);
						get_opcode_func(command,&opcodeIn,&functIn);
						codeWord.funct=functIn;
						codeWord.opcode=opcodeIn;
						codeWord.sourceAddress=sourceAddrIn;
						codeWord.destinationAddress=destinationAddrIn;
						add_image(*IC,fixLine,codeWord,ABSOLUTE,imageTableCode);/*add the first word */
						if(destinationAddrIn==DIRECT_ADDR||destinationAddrIn==RELATIVE_ADDR)/*add the second if we cant we add ? to the image data table.*/
							add_image(*IC+1,operands,codeWord,UNKNOWN_ARE,imageTableCode);
						else if(destinationAddrIn==IMMEDIATE_ADDR){
							token=strtok(operands,"#");
							strcpy(buffer,token);
							num=atoi(buffer);
							int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.sourceAddress=sourceAddrIn;
							codeWord.destinationAddress=destinationAddrIn;
							add_image(*IC+1,fixLine,codeWord,ABSOLUTE,imageTableCode);L=2;
						}
						else{
							functIn=0;
							opcodeIn=0;
							sourceAddrIn=0;
							destinationAddrIn=0;
							register_to_code_word(operands,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.sourceAddress=sourceAddrIn;
							codeWord.destinationAddress=destinationAddrIn;
							add_image(*IC+1,fixLine,codeWord,ABSOLUTE,imageTableCode);/*L=2*/
						}
					}
					else{/*we have three operand*/
						strcpy(buffer,operands);
						token=strtok(buffer,COMMA_STRING);
						strcpy(firstOperand,token);
						token=strtok(NULL,COMMA_STRING);
						strcpy(secondOperand,token);
						build_addresses(L,&sourceAddrIn,&destinationAddrIn,operands);
						tempDestination=destinationAddrIn;
						tempSource=sourceAddrIn;
						get_opcode_func(command,&opcodeIn,&functIn);
						codeWord.funct=functIn;
						codeWord.opcode=opcodeIn;
						codeWord.sourceAddress=sourceAddrIn;
						codeWord.destinationAddress=destinationAddrIn;
						add_image(*IC,fixLine,codeWord,ABSOLUTE,imageTableCode);/*add the first word to the image data table*/
						if(tempSource==DIRECT_ADDR||tempSource==RELATIVE_ADDR)/*add the second if we cant we add ? to the image data table.*/
							add_image(*IC+1,firstOperand,codeWord,UNKNOWN_ARE,imageTableCode);
						else if(tempSource==IMMEDIATE_ADDR){
							token=strtok(firstOperand,"#");
							num=atoi(token);
							int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.sourceAddress=sourceAddrIn;
							codeWord.destinationAddress=destinationAddrIn;
							add_image(*IC+1,firstOperand,codeWord,ABSOLUTE,imageTableCode);
						}
						else{
							functIn=0;
							opcodeIn=0;
							sourceAddrIn=0;
							destinationAddrIn=0;
							register_to_code_word(firstOperand,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.sourceAddress=sourceAddrIn;
							codeWord.destinationAddress=destinationAddrIn;
							add_image(*IC+1,fixLine,codeWord,ABSOLUTE,imageTableCode);
						}
						if(tempDestination==DIRECT_ADDR||tempDestination==RELATIVE_ADDR)/*add the third word if we cant we add ? to the image data table.*/
							add_image(*IC+2,secondOperand,codeWord,UNKNOWN_ARE,imageTableCode);
						else if(tempDestination==IMMEDIATE_ADDR){
							token=strtok(secondOperand,"#");
							num=atoi(token);
							int_to_code_word(num,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.sourceAddress=sourceAddrIn;
							codeWord.destinationAddress=destinationAddrIn;
							add_image(*IC+2,secondOperand,codeWord,ABSOLUTE,imageTableCode);
						}
						else{/*is register address*/
							functIn=0;
							opcodeIn=0;
							sourceAddrIn=0;
							destinationAddrIn=0;
							register_to_code_word(secondOperand,&opcodeIn,&functIn,&sourceAddrIn,&destinationAddrIn);
							codeWord.funct=functIn;
							codeWord.opcode=opcodeIn;
							codeWord.sourceAddress=sourceAddrIn;
							codeWord.destinationAddress=destinationAddrIn;
							add_image(*IC+2,fixLine,codeWord,ABSOLUTE,imageTableCode);
						}
					}
				}
				state=SAVE_IC_L;
				break;
			case SAVE_IC_L:
				state=UPDATE_IC;
				break;
			case UPDATE_IC:
				*IC+=L;
				L=0;
				state=READ_NEXT;
				break;

		}
	}
	free(readLine);
}
