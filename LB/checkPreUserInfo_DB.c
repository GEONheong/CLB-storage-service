#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int checkPreUserInfo_DB(char *preUser_id,char *preUser_password){
    printf("--------checkPreUserInfo_DB-----------\n");
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserList";
    char *database2 = "UserFileList";

    char query[256];
    int isPre;

    if (!(conn = mysql_init((MYSQL *)NULL)))
    { 
        printf("init fail");
        return 0;
    }

    printf("mysql_init success.\n");

    if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0))
    {
        printf("connect error.\n"); //DB connect (MYSQL*, host, id, pw, null, port, 0)
        return 0;
    }

    printf("mysql_real_connect success.\n");

    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return 0;
    }
    printf("select UserList DB success.\n");

    sprintf(query,"select id,password from allUserList where id='%s' and password='%s'",preUser_id,preUser_password);

    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("fail select id,password.\n");
        return 0;
    }
    else{
        res=mysql_store_result(conn); //result query
        isPre = mysql_num_rows(res); //if result is 0 no Info in table, if result is 1 get Info in table

        if(isPre){
            printf("success select is,password\n");
        }
        else{
            mysql_close(conn);
            printf("fail select id,password.\n");
            return 0;
        }
        
    }

    return 1;
}