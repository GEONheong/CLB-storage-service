CC=gcc
CFLAGC=-c -o
OBJS = ./objFile.d/storage.o ./objFile.d/multicasting_recv.o \
./objFile.d/getPubkey.o ./objFile.d/parsing_Message.o


all : startStorage.exe

startStorage.exe : $(OBJS)
	$(CC) -o startStorage.exe $(OBJS)

./objFile.d/storage.o : storage.c
	$(CC) $(CFLAGC) ./objFile.d/storage.o storage.c

./objFile.d/multicasting_recv.o : multicasting_recv.c
	$(CC) $(CFLAGC) ./objFile.d/multicasting_recv.o multicasting_recv.c

./objFile.d/getPubkey.o : getPubkey.c 
	$(CC) $(CFLAGC) ./objFile.d/getPubkey.o getPubkey.c

./objFile.d/parsing_Message.o : parsing_Message.c
	$(CC) $(CFLAGC) ./objFile.d/parsing_Message.o parsing_Message.c 

clean :
	rm ./objFile.d/*.o startStorage.exe