#ifndef CODE_H
#define CODE_H
#include "utils.h"
#include "textConvert.h"

/*number of word */
typedef enum number_of_words{
	FIRST_WORD=1,SECOND_WORD,THIRD_WORD
}number_of_words;

/*error type*/
typedef enum error_mode{
WRONG_PLACE_LABEL=1,WRONG_LENGTH_LINE,LESS_WORD_LABEL,ILLEGAL_LABEL,TOO_MUCH_WORD,
LABEL_ALREADY_THERE,ILLEGAL_STRING,ILLEGAL_DATA,TOO_MUCH_WORDS_EXT,LESS_WORDS_EXT,
ILLEGAL_LABEL_IN_EXT,LABEL_ALREADY_THERE_NOT_EXT,ILLEGAL_COMMAND_NAME,WRONG_NUM_OF_WORDS_FOR_COMMAND,
TOO_MUCH_OPERANDS,WRONG_NUM_OPPERAND,OPERANDS_NOT_MATCH,ENTRY_NOT_EXIST,ENTRY_WITH_EXTERNAL,
RELATIVE_ADDRESS_EXT,RELATIVE_ADDRESS_SYMBOL_NOT_EXIST,DIRECT_ADDRESS_LEBEL_NOT_EXIST,LESS_WORD_ENT,TOO_MUCH_WORD_ENT
}error_mode;

/*number of operand*/
typedef enum number_of_operand{
	ZERO_OPERANDS=0,ONE_OPERANDS,TWO_OPERANDS,THREE_OPERANDS
}num_of_operand;

/*word on machine language in bit field.*/
typedef struct code_word {
	unsigned int destinationAddress: 2;
	unsigned int sourceAddress: 2;
	unsigned int funct:4;
	unsigned int opcode:4;
} code_word;

/*opcode number for action*/
typedef enum opcodes {
	MOV_OPCODE = 0,NONE_OPCODE=0,
	CMP_OPCODE = 1,
	ADD_OPCODE = 2,SUB_OPCODE= 2,
	LEA_OPCODE = 4,
	CLR_OPCODE = 5,	NOT_OPCODE = 5,	INC_OPCODE = 5,	DEC_OPCODE = 5,
	JMP_OPCODE= 9,	BNE_OPCODE = 9,	JSR_OPCODE = 9,
	RED_OPCODE = 12,
	PRN_OPCODE = 13,
	RTS_OPCODE = 14,
	STOP_OPCODE = 15
} opcode;

/*function number for action*/
typedef enum funct {
	MOV_FUNCT = 0,CMP_FUNCT = 0,LEA_FUNCT = 0,RED_FUNCT = 0,PRN_FUNCT = 0,RTS_FUNCT = 0,STOP_FUNCT = 0,NONE_FUNCT=0,
	ADD_FUNCT = 10,CLR_FUNCT = 10,JMP_FUNCT= 10,
	SUB_FUNCT= 11,NOT_FUNCT = 11,BNE_FUNCT = 11,
	INC_FUNCT = 12,JSR_FUNCT = 12,
	DEC_FUNCT = 13
} funct;

/*addressing type*/
typedef enum addressing_modes {
	IMMEDIATE_ADDR = 0,NONE_ADDR=0,
	DIRECT_ADDR = 1,
	RELATIVE_ADDR = 2,
	REGISTER_ADDR = 3
} addressing_type;

/*list of memory image.*/
typedef struct image_memory * pImage;
typedef struct image_memory{
	int address;
	char *sorceCode;
	code_word codeWord;
	char are;
	pImage next;
}image_memory;


/*this function add to list of image memory record.
 * @param address is memory address.
 * @param sorceCode is line command .
 * @param codeWord is structure of word in binary.
 * @param are is A R or E.
 * @imagePointer is pointer to image memory.*/
void add_image(int address,char *sorceCode,code_word codeWord,char are,pImage * imagePointer);

/*this function print the image data*/
void print_image(pImage imagePointer);

/*this function free all the memory that we allocate for a image memory
 * param imagePointer is pointer to the record in the image memory that we want to free. */
void free_image(pImage * imagePointer);

/*this function get the number of mode error and print why is error in the line and the name of the file.
 * @param mode is error number
 * @param lineNumber is where we have error its can be address or line number.
 * @param line is the string of the line
 * @param fileNameAs is the name of the file.*/
void print_error(error_mode mode,int lineNumber,char * line,char *fileNameAs);

/*this function check the opcode number and function number for the command name.
 * the function update the parameters opcode_out and funct_out to opcode number and function number we need.*/
void get_opcode_func(char *name, opcode *opcodOut, funct *functOut);

/*this function check if the string name is legal command name.
 * @param name is the function name that we want to check.
 * @return TRUE if that is legal name else return FALSE. */
bool is_legal_command_name(char * name);

/*this function check if we enough word for the command
 *@param commandName is the name of command.
 *@param num is how much words we have include the command.
 *@param return TRUE if we have enough word for the command else return FALSE. */
bool is_legal_num_of_words_for_command(char *commandName,number_of_words num);

/*this function check if the number of command in the line is good.
 * @param num is the number operand we that we count in the line
 * @param commandName is the name of command we that we want to check.
 * @return TRUE if the the number of comand is good else return FALSE.*/
bool is_legal_operand_for_command(int num,char * name);

/*this function check if the kind of the operands match to the command according to the methods of addressing.
 * @param command is the name of the command.
 * @param operands is the operand that we want to check.
 * param L is the number of the word that the line take.
 *@return TRUE if the kind of the operands match to the command according to the methods of addressing else return FALSE. */
bool is_legal_address(char *command,char *operands,int L);

/*this function build the source address and the destination address by the operands and by the number of word that the word take from the memory.
 *@param L is the number of word that the word take from the memory.
 *@param  sourceAddrOut is pointer to sourceAddrIn
 *@param  destinationAddrOut is pointer to destinationAddrIn
 *@param  operands is string of operands.*/
void build_addresses(int L,unsigned int * sourceAddrOut,unsigned int * destinationAddrOut,char *operands);

/*this function update the image date memory by add ICF to value address
 * @param pmemory is pointer data image memory.
 * @param ICF is the value we want to add to address.*/
void update_data_image(image_memory * pmemory,int ICF);

/*this function convert char to code_word type
 * @param c is char.
 * @return c converted to code_word
 */
void char_to_code_word(char c,unsigned int * opcodeIn,unsigned int *functIn,unsigned int *sourceAddrIn,unsigned int *destinationAddrIn);

/*this function convert int to code_word type
 * @param num is a integer number.
 * */
void int_to_code_word(int num,unsigned int * opcodeIn,unsigned int *functIn,unsigned int *sourceAddrIn,unsigned int *destinationAddrIn);

/*this function get register and return the binary form registers
 * @param regi is the register
 * return the the binary form register*/
void register_to_code_word(char *regi,unsigned int * opcodeIn,unsigned int *functIn,unsigned int *sourceAddrIn,unsigned int *destinationAddrIn);
#endif
