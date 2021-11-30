#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

//this is REMOVE FILE function
int deleteItemInFileList_DB(char *user_id,char *filename){
    printf("--------checkPreUserInfo_DB-----------\n");
    //mariadb var
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    //mariadb server info
    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserFileList";

    //query string
    char query[256];

    //initial
    if (!(conn = mysql_init((MYSQL *)NULL)))
    { 
        printf("init fail");
        return 0;
    }
    printf("mysql_init success.\n");

    // connect mariadb server
    if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0))
    {
        printf("connect error.\n"); //DB connect (MYSQL*, host, id, pw, null, port, 0)
        return 0;
    }
    printf("mysql_real_connect success.\n");

    // select database = (use UserFileList;)
    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return 0;
    }
    printf("select UserList DB success.\n");

    //delete from user 'filename' item
    sprintf(query,"DELETE FROM %s WHERE file_name='%s'",user_id,filename);
    if(mysql_query(conn,query)){ //if error
        mysql_close(conn);
        printf("fail delete file item.\n");
        return 0;
    }

    mysql_close(conn);
    return 1;
}