#ifndef TRANSITIONS_H
#define TRANSITIONS_H
#include <stdio.h>
#include "utils.h"
#include "table.h"
#include "code.h"
#include "textConvert.h"

/*that is the status numbers in the first transition*/
enum statusFirstTransiton{BEGIN=1,READ_NEXT,IS_LABEL,DEFINE_LABEL,IS_STORAGE,SYMBOL_DEFINE,IDENTIFY_DATA,ENT_OR_EXT,IF_ENT,IF_EXT,
IS_COMMAND_LINE,SEARCH_ACTION_NAME,NUM_OF_WORDS,BUILD_FIRST_BIN,SAVE_IC_L,UPDATE_IC,READ_ALL,SAVE_VAL_IC,UPDATE_TABLE,BEGIN_SECOND};

/*this function do the first transition:
 * we try to build the symbol table.
 * set how much words every line need in the memory image.
 * data encoding.
 * we will take care of .entry in the second transition.
 * word with direct address we build in the second transition.
 * word with relative address we build in the second transition.
 * @param fileNameAs is the name of the file.
 * @param file is pointer to the file that we want to convert to machine language.
 * @param errorState is a flag if we have have error in the file,if we have error be TRUE else FALSE.
 * @param IC is counter instructions.
 * @param DC is data counter.
 * @param symbolTable is pointer to symbol table.
 * @param imageTableData is pointer to image data table.
 * @param imageTableCode is pointer the image code table.*/
void first_transition(char *fileNameAs,FILE *file,bool *errorState,int *IC,int *DC,pSymbolTable * symbolTable,pImage * imageTableData,pImage * imageTableCode);
#endif
