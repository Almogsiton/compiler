assembler : assembler.o buildFiles.o code.o firstTransition.o secondTransition.o table.o textConvert.o utils.o
	gcc -g -ansi -Wall -pedantic assembler.o buildFiles.o code.o firstTransition.o secondTransition.o table.o textConvert.o utils.o -lm -o assembler  
assembler.o: assembler.c code.h firstTransition.h utils.h table.h secondTransition.h buildFiles.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
buildFiles.o: buildFiles.c buildFiles.h code.h table.h
	gcc -c -ansi -Wall -pedantic buildFiles.c -o buildFiles.o
code.o:code.c code.h utils.h textConvert.h
	gcc -c -ansi -Wall -pedantic code.c -o code.o
firstTransition.o:firstTransition.c firstTransition.h utils.h table.h code.h textConvert.h
	gcc -c -ansi -Wall -pedantic firstTransition.c -o firstTransition.o
secondTransition.o:secondTransition.c secondTransition.h utils.h table.h code.h textConvert.h
	gcc -c -ansi -Wall -pedantic secondTransition.c -o secondTransition.o
table.o: table.c table.h utils.h
	gcc -c -ansi -Wall -pedantic table.c -o table.o
textConvert.o:textConvert.c textConvert.h utils.h
	gcc -c -ansi -Wall -pedantic textConvert.c -o textConvert.o
utils.o:utils.c utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
