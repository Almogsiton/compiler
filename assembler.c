/*
 ============================================================================
 Name        : assembler.c
 Author      : Almog Siton
 Date        : 13.03.21
 E-mail      : almogsiton@gmail.com
 Description : maman 14
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "firstTransition.h"
#include "utils.h"
#include "table.h"
#include "secondTransition.h"
#include "buildFiles.h"

/*the main get name of file from the argument line and translate the source file to machine language for assembly language.
 * if we have error we will print appropriate message.
 * @param symbolTable is list of symbol table.
 * @param entTable is the list of entry symbol with this table we will build the ent file.
 * @param extern is the list for extern file.
 * @param imageTableCode is list of the code image.
 * @param imageTableData is list of the image data.*/
int main(int argc, char const **argv){
	int i,IC=START_IC,DC=START_DC,ICF,DCF,sumExt=0,sumEnt=0;
	bool errorState=FALSE;
	pSymbolTable symbolTable=NULL,entTable=NULL,extTable=NULL;
	pImage imageTableCode=NULL,imageTableData=NULL;
	char fileNameAs[SIZE_OF_FILE_NAME],fileNameOutPut[SIZE_OF_FILE_NAME],fileNameEnt[SIZE_OF_FILE_NAME],fileNameExt[SIZE_OF_FILE_NAME];
	FILE * inputFile,*assemblerOutputFile,*entOutputFile,*extOutputFile;
	if(argc <= 1){
		printf("the number of argument is too small we can't run the program\n");
		exit(0);
	}
	for(i=1; i<argc; i++){/*check all the files*/
		errorState=FALSE;
		strcpy(fileNameAs,argv[i]);
		strcat(fileNameAs,ENDING_FILE_AS);
		inputFile = fopen(fileNameAs,"r");
		if (!inputFile)
			printf("Error: The input file %s does not exists.\n",fileNameAs);
		else{/*the file was successfully opened*/
			first_transition(fileNameAs,inputFile,&errorState,&IC,&DC,&symbolTable,&imageTableCode,&imageTableData);
			ICF=IC;
			DCF=DC;
			update_symbol_table(symbolTable,ICF,DATA_SYMBOL);
			update_data_image(imageTableData,ICF);
			if(errorState==TRUE){/* we have error do free for all the table and the image data and image code and go to the next file.*/
				free_table(&symbolTable);
				free_image(&imageTableCode);
				free_image(&imageTableData);
			}
			else{/*start the second transition.*/
				rewind(inputFile);
				second_transition(fileNameAs,inputFile,&errorState,&symbolTable,&imageTableCode,&imageTableData,&entTable,&extTable,&sumExt,&sumEnt);
				printf("%s:the symbol table after the second transition is:\n",fileNameAs);
				print_symbol_table(symbolTable);
				printf("%s:the image data table after the second transition is:\n",fileNameAs);
				print_image(imageTableCode);
				print_image(imageTableData);
				fclose(inputFile);
				if(errorState==TRUE){/* we have error do free for all the table and the image data and image code and go to the next file.*/
					free_table(&symbolTable);
					free_image(&imageTableCode);
					free_image(&imageTableData);
					free_table(&entTable);
					free_table(&extTable);
				}
				else{/*the second transition succeed now we are building the output files.. */
					strcpy(fileNameOutPut,argv[i]);
					strcat(fileNameOutPut,ENDING_FILE_OUTPUT_OB);
					assemblerOutputFile = fopen(fileNameOutPut,"w");
					if (!assemblerOutputFile)/*build the assembler output file*/
						printf("Error: cannot open the file %s.\n",fileNameOutPut);
					else{
						printf("trying to build the file %s...\n",fileNameOutPut);
						build_out_put_file_ob(assemblerOutputFile,&imageTableCode,&imageTableData,ICF,DCF);
						fclose(assemblerOutputFile);
					}
					if(sumEnt!=0){/*if we have record on entry table we make ent file.*/
						strcpy(fileNameEnt,argv[i]);
						strcat(fileNameEnt,ENDING_FILE_ENT);
						entOutputFile = fopen(fileNameEnt,"w");
						if(!entOutputFile)
							printf("Error: cannot open the file %s.\n",fileNameEnt);
						else{
							printf("trying to build the file %s...\n",fileNameEnt);
							build_out_put_file_ent_or_ext(entOutputFile,&entTable);
							fclose(entOutputFile);
						}
					}
					if(sumExt!=0){/*if we have record on external table we make ext file.*/
						strcpy(fileNameExt,argv[i]);
						strcat(fileNameExt,ENDING_FILE_EXT);
						extOutputFile = fopen(fileNameExt,"w");
						if(!extOutputFile)
							printf("Error: cannot open the file %s.\n",fileNameExt);
						else{
							printf("trying to build the file %s...\n",fileNameExt);
							build_out_put_file_ent_or_ext(extOutputFile,&extTable);
							fclose(entOutputFile);
						}
					}
					free_table(&symbolTable);
					free_image(&imageTableCode);
					free_image(&imageTableData);
					free_table(&entTable);
					free_table(&extTable);
				}
			}
		}
		IC=START_IC,DC=START_DC,ICF=0,DCF=0,sumExt=0,sumEnt=0;
		errorState=FALSE;
		symbolTable=NULL,entTable=NULL,extTable=NULL;
		imageTableCode=NULL,imageTableData=NULL;
		inputFile=NULL,assemblerOutputFile=NULL,entOutputFile=NULL,extOutputFile=NULL;
	}
	printf("done!\n");
	return 0;
}
