#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>


//this is uploadFile or downloadFile function
//update user filelist
int updateUserFileList_DB(char *user_id,char* filename,char* location){
    printf("--------updateUserFileList_DB-----------\n");
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

    //if new file insert item(file_name..)
    sprintf(query,"INSERT INTO %s (file_name,location,first_touch,last_touch)\
                    VALUES ('%s','%s',now(),now())",user_id,filename,location);

    if(mysql_query(conn,query)){ 

        //if already in update last_touch filed
        sprintf(query,"UPDATE %s SET last_touch = now() WHERE file_name = '%s'",user_id,filename);
        
        if(mysql_query(conn,query)){
            mysql_close(conn);
            printf("insert item fail.\n");
            return 0;
        }        
    }

    printf("insert newUser in allUserList success\n");

    mysql_close(conn);
    return 1;
}