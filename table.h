#ifndef TABLE_H
#define TABLE_H
#include "utils.h"

/*we give to the symbol type number, in this way we know in the continue what is the kind of the symbol type*/
typedef enum symbol_type {
	CODE_SYMBOL,DATA_SYMBOL,EXTERNAL_SYMBOL,ENTRY_SYMBOL,DATA_AND_EXTERNAL,CODE_AND_ENTRY,DATA_AND_ENTRY
} symbol_type;

/*that is typedef struct for symbol table list.*/
typedef struct symbol_table * pSymbolTable;
typedef struct symbol_table{
	char *symbolName;
	int value;
	symbol_type symbolType;
	pSymbolTable next;
}symbol_table;

/*this function add new record to the symbol table in the right place
 * @param nameOfSymbol is the symbol name we want to add to the table.
 * @param value is the value of the record that we want to insert.
 * @param symbolType is the type of the symbol.
 * @param ptable is pointer to the symbol table.*/
void add_record(char *nameOfSymbol,int value,symbol_type symbolType,pSymbolTable * ptable);

/*this function free all the memory that we allocate for a symbol table
 * @param ptable is pointer to the record in the table that we want to free.*/
void free_table(pSymbolTable * ptable);

/*this function print the symbol table */
void print_symbol_table(pSymbolTable ptable);

/*this function check if the label is already in in the table.
 * @param table is the symbol table.
 * @param label is the label that we want to check.
 * @return TRUE if the label exist in the table,FALSE if it is not.*/
bool symbol_already_there(symbol_table *table,char *label);

/*this function check if the label is already in the table and also if the symbol type is not the same.
 *@param table is the symbol table.
 *@param label is the label that we want to check.
 *@param symbol type is the symbol type we want to check. */
bool check_symbol_type_and_label(symbol_table *table,symbol_type symbolType,char *label);

/*this function update the symbol table after the first transition by add ICF to value if the symbol type is data.
 * @param ptable is pointer to the symbol table.
 * @param ICF is the value we need to add to the symbol with the symbol type data
 * @param symbolType is the symbol type.*/
void update_symbol_table(symbol_table * ptable,int ICF,symbol_type symbolType);

/*this function check if there is label with extern guidance with this label name.
 * @param table is pointer to symbol table.
 * @param label is the name of the label we want to check.
 * @return TRUE if the label exist with extern guidance else FALSE */
bool symbol_already_there_with_extern(symbol_table *table,char *label);

/*this function add symbol type entry to label.
 * @param ptable is pointer to the symbol table.
 * @param label is the label name.*/
void update_symbol_type_to_ent(symbol_table * ptable,char *label);

/*this function get label and search what is his value.
 * @param ptable is pointer to the symbol table.
 * @param label is the label that we want to find his value.
 * @return the value of label if the label is not found return -1. */
int find_label_value(symbol_table * ptable,char *label);

#endif
