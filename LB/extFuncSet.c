#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mariadb/mysql.h>

//get ext function
//ex) filename = test.txt -> return txt
//ex) filename = test -> return NONE
char* getExt(char* filename){ 
	static char buf[256]="";
	char* ext = NULL;

	ext = strrchr(filename,'.');		
	if(ext == NULL)
		return "NONE";

	strcpy(buf,ext+1);
	
	return buf;
}

//return storage location where the file (have ext) can store 
char *compareExt_getStorKind_DB(char *ext){
    printf("--------compareExt_getStorKind_DB-----------\n");
    //mariadb var
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    //mariadb server info
    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "getLocation";

    //query string
    char query[256];

    //return value storage location
    static char storKind[256];

    //is in ext?
    int isIn;

    //initial
    if (!(conn = mysql_init((MYSQL *)NULL)))
    { 
        printf("init fail");
        return "fail";
    }
    printf("mysql_init success.\n");

     // connect mariadb server
    if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0))
    {
        printf("connect error.\n"); //DB connect (MYSQL*, host, id, pw, null, port, 0)
        return "fail";
    }
    printf("mysql_real_connect success.\n");

    // select database = (use getLocation;)
    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return "fail";
    }
    printf("select getLocation DB success.\n");
    
    //check if ext exists
	sprintf(query,"SELECT location FROM extSet WHERE ext = '%s'",ext);
	if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("fail select extSet location.\n");
        return "fail";
    }

	res=mysql_store_result(conn);
	isIn = mysql_num_rows(res);
    if(isIn){
        row=mysql_fetch_row(res);
        strcpy(storKind,row[0]);
        printf("success select extSet location.\n");
    }
    else{
        mysql_close(conn);
        printf("fail select extSet location.\n");
        return "fail";
    }

    mysql_free_result(res);
    mysql_close(conn);

    return storKind;
}
