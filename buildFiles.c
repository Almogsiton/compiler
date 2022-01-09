#include "buildFiles.h"

void build_out_put_file_ob(FILE * assemblerOutputFile,pImage *imageTableCode,pImage *imageTableData,int ICF,int DCF){
	pImage currentCode=NULL;
	ICF=ICF-START_IC;
	fprintf(assemblerOutputFile,"  %d  %d\n",ICF,DCF);
	for(currentCode=*imageTableCode;currentCode!=NULL;currentCode=currentCode->next){
		fprintf(assemblerOutputFile,"%.4d %.1X%.1X%.1X %c\n",currentCode->address,currentCode->codeWord.opcode,currentCode->codeWord.funct,(4*currentCode->codeWord.sourceAddress)+currentCode->codeWord.destinationAddress,currentCode->are);
	}
	for(currentCode=*imageTableData;currentCode!=NULL;currentCode=currentCode->next){
		fprintf(assemblerOutputFile,"%.4d %.1X%.1X%.1X %c\n",currentCode->address,currentCode->codeWord.opcode,currentCode->codeWord.funct,(4*currentCode->codeWord.sourceAddress)+currentCode->codeWord.destinationAddress,currentCode->are);
	}
}

void build_out_put_file_ent_or_ext(FILE * entOutputFile,pSymbolTable *entTable){
	pSymbolTable curRecord=NULL;
	for(curRecord=*entTable;curRecord!=NULL;curRecord=curRecord->next){
		fprintf(entOutputFile,"%s %.4d\n",curRecord->symbolName,curRecord->value);
	}
}
