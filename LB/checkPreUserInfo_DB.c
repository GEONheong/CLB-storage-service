#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>


//this is SIGN IN function
//check present User in database
//if success return 1 fail 0
int checkPreUserInfo_DB(char *preUser_id,char *preUser_password){
    printf("--------checkPreUserInfo_DB-----------\n");
    //mariadb var
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    //mariadb server info
    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserList";
    char *database2 = "UserFileList";

    //query string
    char query[256];

    //flag var present user 
    int isPre;

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

    // select database = (use UserList;)
    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return 0;
    }
    printf("select UserList DB success.\n");

    //get id,password 
    sprintf(query,"select id,password from allUserList where id='%s' and password='%s'",preUser_id,preUser_password);
    if(mysql_query(conn,query)){ //if error
        mysql_close(conn);
        printf("fail select id,password.\n");
        return 0;
    }
    else{
        res=mysql_store_result(conn); //result query
        isPre = mysql_num_rows(res); //if result is 0 no Info in table, if result is 1 get Info in table
                                     // 0 = empty , 1 = match one
        if(isPre){ //success (find match infomation)
            printf("success select is,password\n");
        }
        else{ //fail (do not find match infomation)
            mysql_close(conn);
            printf("fail select id,password.\n");
            return 0;
        }
    }
    mysql_close(conn);
    return 1;
}