#ifndef BUILD_FILES_H
#define BUILD_FILES_H
#include <stdio.h>
#include "code.h"
#include "table.h"
/*this function build the out put file in hexadecimal base.*/
void build_out_put_file_ob(FILE * assemblerOutputFile,pImage *imageTableCode,pImage *imageTableData,int ICF,int DCF);

/*this function build the ent file and ext file.*/
void build_out_put_file_ent_or_ext(FILE *,pSymbolTable *);

#endif
