# file: Makefile
# author:
#
# Project: IFJ compiler
# Date: 

all:
	
	gcc main.c parser.c dll.c enum_list.c error.c generator.c psa_stack.c psa.c scanner.c symtable.c stack.c -o ifj_compiler 