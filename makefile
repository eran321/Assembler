Assembler: Assembler.o SecondPass.o FirstPass.o OperationEncoder.o DataEncoder.o EntryEncoder.o ExternEncoder.o SymbolTable.o Helper.o
	gcc -g -ansi -Wall -pedantic Assembler.o SecondPass.o FirstPass.o OperationEncoder.o DataEncoder.o EntryEncoder.o ExternEncoder.o SymbolTable.o Helper.o -o Assembler

Assembler.o: Assembler.c FirstPass.h SecondPass.h Helper.h Consts.h DataEncoder.h SymbolTable.h OperationEncoder.h
	gcc -c -ansi -Wall -pedantic Assembler.c -o Assembler.o

SecondPass.o: SecondPass.c SecondPass.h Consts.h DataEncoder.h EntryEncoder.h ExternEncoder.h Types.h OperationEncoder.h Helper.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic SecondPass.c -o SecondPass.o

FirstPass.o: FirstPass.c FirstPass.h Consts.h DataEncoder.h EntryEncoder.h Enums.h ExternEncoder.h Types.h OperationEncoder.h Helper.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic FirstPass.c -o FirstPass.o

OperationEncoder.o: OperationEncoder.c OperationEncoder.h Types.h Consts.h Helper.h SymbolTable.h ExternEncoder.h
	gcc -c -ansi -Wall -pedantic OperationEncoder.c -o OperationEncoder.o

DataEncoder.o: DataEncoder.c DataEncoder.h Types.h Helper.h Consts.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic DataEncoder.c -o DataEncoder.o

EntryEncoder.o: EntryEncoder.c EntryEncoder.h Types.h Consts.h SymbolTable.h Helper.h
	gcc -c -ansi -Wall -pedantic EntryEncoder.c -o EntryEncoder.o

ExternEncoder.o: ExternEncoder.c ExternEncoder.h Types.h Consts.h SymbolTable.h Helper.h
	gcc -c -ansi -Wall -pedantic ExternEncoder.c -o ExternEncoder.o

SymbolTable.o: SymbolTable.c SymbolTable.h Types.h Consts.h Helper.h 
	gcc -c -ansi -Wall -pedantic SymbolTable.c -o SymbolTable.o

Helper.o: Helper.c Helper.h Types.h Enums.h Consts.h 
	gcc -c -ansi -Wall -pedantic Helper.c -o Helper.o

clean:
	rm *.o
