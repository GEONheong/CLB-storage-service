#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>
#include<string.h>


//return user filelist
char *getUserFileList_DB(char* user_id)
{
    //mariadb var
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    //mariadb server info
    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserFileList";


    //return value filelist
    static char filelist[BUFSIZ];
    char tempbuf[256];
    
    //query string
    char query[256];

    //filelist table head
    sprintf(filelist,"|%-15s|%-20s|%-20s|\n","FileName","First-Touch","Last-Touch");
    strcat(filelist,"+---------------+--------------------+--------------------+\n");

    //initial
    if (!(conn = mysql_init((MYSQL *)NULL)))
    { 
        printf("init fail");
        return "fail";
    }
    printf("mysql_inti success.\n");

    // connect mariadb server
    if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0))
    {
        printf("connect error.\n"); //DB connect (MYSQL*, host, id, pw, null, port, 0)
        return "fail";
    }
    printf("mysql_real_connect suc.\n");

    // select database = (use UserFileList;)
    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return "fail";
    }
    printf("select mydb suc.\n");

    //get file list and file information 
    sprintf(query,"SELECT file_name,first_touch,last_touch from %s",user_id);
    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("insert item fail.\n");
        return "fail";
    }

    res=mysql_store_result(conn);   //result query
    int fildes = mysql_num_fields(res); //result item count

    while(row=mysql_fetch_row(res)){ //append filelist val
        for(int cnt=0; cnt<fildes ;cnt++){
            if(cnt == 2){
                sprintf(tempbuf,"|%-15s|%-20s|%-20s|\n",row[0],row[1],row[2]);
                strcat(filelist,tempbuf);
            }
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return filelist;
}