all: msh

msh: *.c *.h
	gcc -o msh msh.c commands.c
