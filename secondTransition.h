#ifndef SECONDTRANSITION_H
#define SECONDTRANSITION_H
#include <stdio.h>
#include "utils.h"
#include "table.h"
#include "code.h"
#include "textConvert.h"

/*that is all the status for the second transition*/
enum statusSecondTransiton{READ_NEXT_LINE=1,SKIP_LABEL,IS_DATA_STRING_EXT,IS_ENTRY,ADD_ENTRY,COMPLETE,READ_ALL_FILE,BULID_OUTPUT};

/*in this function we do the second transition.
 * the take care of entry directive.
 * we build the image data code for all the record that the feature A.R.E is unknown.
 * we build the entry and external table for ent and ext flies.
 * we build the word with direct address.
 * we build the word with relative address.
 * @param file is pointer to the file that we want to convert to machine language.
 * @param symbolTable is pointer to symbol table.
 * @param imageTableData is pointer to image data table.
 * @param imageTableCode is pointer the image code table .
 * @param entTable is pointer to entry table.
 * @param extTable is pointer to exteral table.
 * @param sumExt count how much record we insert to external table.
 * @param sumEnt count how much record we insert to entry table.*/
void second_transition(char *fileNameAs,FILE *file,bool *errorState,pSymbolTable * symbolTable,pImage * imageTableCode,pImage * imageTableData,pSymbolTable *entTable,pSymbolTable *extTable,int *sumExt,int *sumEnt);

#endif
