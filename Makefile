all: msh

msh: *.c *.h
	gcc -o msh msh.c commands.c exec.c utils.c
