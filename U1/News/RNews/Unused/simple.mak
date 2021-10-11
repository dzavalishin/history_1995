CFLAGS=-v- -c -I..\..\lib\out\os2

rnews.exe: process.obj main.obj
	bcc -ernews.exe -L..\..\lib\out\os2 main.obj process.obj uu2.lib uucp.lib fido.lib c2i.lib


process.obj: process.c
	bcc $(CFLAGS) process.c

main.obj: main.c
	bcc $(CFLAGS) main.c


