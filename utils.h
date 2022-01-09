#ifndef UTILS_H
#define UTILS_H

#define SIZE_OF_FILE_NAME 1000
#define SIZE_OF_LINE 80
#define LEGAL_LENGTH_LABEL 31
#define MEMORY_SIZE 4096
#define SPACE ' '
#define SPACE_STRING " "
#define EMPTY_STRING ""
#define TAB '\t'
#define END_STRING '\0'
#define COMMA ','
#define COMMA_STRING ","
#define COLON ':'
#define COLON_STRING ":"
#define ENDING_FILE_AS ".as\0"
#define ENDING_FILE_OUTPUT_OB ".OB\0"
#define ENDING_FILE_ENT ".ent\0"
#define ENDING_FILE_EXT ".ext\0"
#define DATA_DIRECIVE ".data\0"
#define STRING_DIRCTIVE ".string\0"
#define ENTRY_DIRCTIVE ".entry\0"
#define EXTERN_DIRECTIVE ".extern\0"
#define EQUAL_STRING 0
#define START_IC 100
#define START_DC 0
#define NUMBER_0F_REGISTERS 9
#define NUMBER_OF_ACTIONS 16
#define NUMBER_OF_DIRECTIVES 5
#define MINIMUM_FOR_STRING 2
#define QUOTATION_MARKS '"'
#define ABSOLUTE 'A'
#define RELOCATABLE 'R'
#define EXTERNAL 'E'
#define UNKNOWN_ARE '?'
#define DIGIT_PLUS '+'
#define DIGIT_MINUS '-'
#define EXTENAL_VALUE 0
#define BEGIN_IMMEDIATE_ADDR '#'
#define WORD_LENGTH 12
#define HEXA_LENGTH 3
#define digit_zero '0'
#define digit_one '1'

/*this function allocates memory,give as a message if the allocation failed.
 * @param size is the size to allocate in bytes. */
void * allocation(int size);

/*that is typedef enum for booleans variable 0 is FALSE 1 is TRUE.*/
typedef enum booleans{
	FALSE = 0, TRUE = 1
}bool;

/*this function check if there is label in the line.
 * @param line is the line that we what to check.
 * @return return TRUE if there label and FALSE if we don't find label. */
bool is_label_line(char *line);

/*this function check if the first word in the line is a label.
 * @param line is the line that we what to check.
 * @return return TRUE if the word is label and FALSE if the word is not a label.  */
bool is_word_label(char *line);

/*this function check if the length line is 80 or less.
 * @param line is line from the file.
 * @return TRUE if the length line is 80 or less,else return FALSE. */
bool is_legal_line(char *line);

/*this function check how much words we have in line.
 * @param line is the line that we what to check.
 * @return integer number that is the number of words that we have in the line.*/
int words_on_line(char *line);

/*this function get line from the file and remove spaces and tabs.
 * @param line is line before we removed the spaces and tabs.
 * @param tempStr is the fix line.*/
void remove_space(char *line,char * tempStr);

/*this function check if the line is only tabs and spaces
 * @return FALSE if the line is empty space*/
bool check_empty_line(char * line);

/*this function selected the word we want by number of word on the line.
 * @param line is the line that we want to take from there word.
 * @param word we put the word that we chose to this parameter.
 * @param numOfWord is the word number on the line.*/
void selected_word_line(char * line,char *word,int numOfWord);

/*this function check if the word that we get is storage directive
 * @param word is the word that we want to check
 * @return TRUE if that is storage directive(.string or .data) else return FALSE*/
bool is_storage_directive(char * word);

/*this function check if the word label is legal or not.
 * legal label is word with maximum 31 characters it's begin with upper-case letter or lower-case letter.
 * the rest of the word is digit or upper-case letter or lower-case letter.
 * colon is not part of the word
 * legal label is also is that is not save word
 * @param label is the label we want to check
 * @return FALSE if the label is illegal and true if is legal.*/
bool is_legal_label(char * label);

/*this function check if the word is save word.
 * @param directivesWords is a directives words in matrix.
 * @param actionsWords is a actions words in matrix.
 * @param registersWords is a registers words in matrix.
 * @return TRUE if word is save word else return FALSE. */
 bool is_save_word(char * word);

/*this function insert : from the ending label.
 *@param label is the label name.*/
 void remove_colon_from_label(char * label);

/*this function check if the word that it gets is legal string in assembly.
 * legal word is word that begin with quotation marks and end with quotation marks.
 * @param word is the word that we want to check.
 * @return TRUE if this is legal word else return FALSE.*/
bool is_legal_string(char * word);

/*this function check if the word is legal string numbers.
 * legal string numbers is number with comma between the numbers.
 * the word can't to end with comma and can't start with comma.
 * comma can't to be after comma and also not before. */
bool is_legal_data(char *word);

/*this function count how much operands we have in the word.
 * @param word is string of one operand or more.
 * @return how much operand we have in the word.*/
int count_operands(char *word);

/*this function check if word is register.
 * @param word is the word we want to check if it is register.
 * @return TRUE if this is register else return FALSE.*/
bool is_register(char * word);

/*this function check if operand is legal immediate address
 * @param operand is the operand we want to check
 * @return TRUE if it is legal operand else return FALSE.*/
bool is_legal_immediate_addr(char * operand);

/*this function check if the word is .extern or .string or .data.
 * @param word is the word we wont to check.
 * @return TRUE if the word is .extern or .string or .data else false.*/
bool is_data_str_or_ext(char * word);

/*this function check if the word is .entry
 * @param word is the word we want to check
 * @return TRUE if the word is .entry else return FALSE.*/
bool is_entry(char * word);

#endif
