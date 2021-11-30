
CC=gcc
CFLAGC=-c -o
OBJS=./objFile.d/CLBmain.o ./objFile.d/udp_server.o ./objFile.d/tcp_loginServer.o \
./objFile.d/extFuncSet.o ./objFile.d/updateUserFileList_DB.o ./objFile.d/getUserFileList_DB.o \
./objFile.d/delete_filename_userFileList_DB.o ./objFile.d/createNewUserInfo_DB.o \
./objFile.d/checkPreUserInfo_DB.o ./objFile.d/multicasting_send.o


all : startCLB.exe

startCLB.exe : $(OBJS)
	$(CC) -o startCLB.exe $(OBJS) -lmariadb -lpthread

./objFile.d/CLBmain.o: CLBmain.c
	$(CC) $(CFLAGC) ./objFile.d/CLBmain.o CLBmain.c -lpthread

./objFile.d/udp_server.o : udp_server.c
	$(CC) $(CFLAGC) ./objFile.d/udp_server.o udp_server.c

./objFile.d/tcp_loginServer.o : tcp_loginServer.c
	$(CC) $(CFLAGC) ./objFile.d/tcp_loginServer.o tcp_loginServer.c



./objFile.d/createNewUserInfo_DB.o : createNewUserInfo_DB.c
	$(CC) $(CFLAGC) ./objFile.d/createNewUserInfo_DB.o createNewUserInfo_DB.c -lmariadb

./objFile.d/checkPreUserInfo_DB.o : checkPreUserInfo_DB.c
	$(CC) $(CFLAGC) ./objFile.d/checkPreUserInfo_DB.o checkPreUserInfo_DB.c -lmariadb

./objFile.d/extFuncSet.o : extFuncSet.c
	$(CC) $(CFLAGC) ./objFile.d/extFuncSet.o extFuncSet.c -lmariadb

./objFile.d/updateUserFileList_DB.o : updateUserFileList_DB.c
	$(CC) $(CFLAGC) ./objFile.d/updateUserFileList_DB.o updateUserFileList_DB.c -lmariadb

./objFile.d/getUserFileList_DB.o : getUserFileList_DB.c
	$(CC) $(CFLAGC) ./objFile.d/getUserFileList_DB.o getUserFileList_DB.c -lmariadb

./objFile.d/delete_filename_userFileList_DB.o : delete_filename_userFileList_DB.c
	$(CC) $(CFLAGC) ./objFile.d/delete_filename_userFileList_DB.o delete_filename_userFileList_DB.c -lmariadb

./objFile.d/multicasting_send.o : multicasting_send.c
	$(CC) $(CFLAGC) ./objFile.d/multicasting_send.o multicasting_send.c

clean :
	rm ./objFile.d/*.o startCLB.exe