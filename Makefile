
#VERSION	: 1.01
#CFLAGS	: -Wall 
#LDFLAGS	: -lm -lncurses
CC    = gcc
OPS   = -lm 


linux:
	gcc -lm glide.c -o glide.bin; ./glide.bin  ; pdflatex graph.tex ; mupdf graph.pdf 

