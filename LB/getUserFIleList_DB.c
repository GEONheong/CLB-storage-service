#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>
#include<string.h>

char *getUserFileList_DB(char* user_id)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserFileList";

    static char buf[BUFSIZ];
    char tempbuf[256];
    char query[256];

    sprintf(buf,"|%-15s|%-20s|%-20s|\n","FileName","First-Touch","Last-Touch");
    strcat(buf,"+---------------+--------------------+--------------------+\n");

    if (!(conn = mysql_init((MYSQL *)NULL)))
    { 
        printf("init fail");
        return "fail";
    }

    printf("mysql_inti success.\n");

    if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0))
    {
        printf("connect error.\n"); //DB connect (MYSQL*, host, id, pw, null, port, 0)
        return "fail";
    }

    printf("mysql_real_connect suc.\n");

    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return "fail";
    }
    printf("select mydb suc.\n");

  
    sprintf(query,"SELECT file_name,first_touch,last_touch from %s",user_id);
    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("insert item fail.\n");
        return "fail";
    }

    res=mysql_store_result(conn);
    int fildes = mysql_num_fields(res);

    while(row=mysql_fetch_row(res)){
        for(int cnt=0; cnt<fildes ;cnt++){
            if(cnt == 2){
                sprintf(tempbuf,"|%-15s|%-20s|%-20s|\n",row[0],row[1],row[2]);
                strcat(buf,tempbuf);
            }
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return buf;
}