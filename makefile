default: all
y86emul:
	gcc -Wall y86emul.c -o y86emul
y86dis:
	gcc -Wall y86dis.c -o y86dis
all: y86dis y86emul
clean:
	rm y86dis
	rm y86emul
