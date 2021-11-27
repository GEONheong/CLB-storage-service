#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int createNewUserInfo_DB(char *newUser_id,char *newUser_password){
    printf("--------createNewUserInfo_DB-----------\n");
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserList";
    char *database2 = "UserFileList";

    char query[256];

    int isNotNew;

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

    sprintf(query,"select id from allUserList where id='%s'",newUser_id);

    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("fail select id,password.\n");
        return 0;
    }
    else{
        res=mysql_store_result(conn); //result query
        isNotNew = mysql_num_rows(res); //if result is 0 no Info in table, if result is 1 get Info in table

        if(isNotNew){
            mysql_close(conn);
            printf("fail select id,password.\n");
            return 0;
        }
        else{
            printf("success select is,password\n");
        }
        
    }

    sprintf(query,"INSERT INTO allUserList (id, password) \
                    SELECT '%s', '%s' FROM DUAL \
                    WHERE NOT EXISTS \
                    (SELECT id FROM allUserList \
                    WHERE id='%s') \
                    ",newUser_id,newUser_password,newUser_id);

    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("insert item fail.\n");
        return 0;
    }
    printf("insert newUser in allUserList success\n");

    //change database UserFileList

    if (mysql_select_db(conn, database2) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        return 0;
    }
    printf("select UserFileList DB success.\n");

    sprintf(query,"CREATE TABLE IF NOT EXISTS %s(\
                file_name VARCHAR(100),\
                location VARCHAR(10),\
                first_touch DATETIME,\
                last_touch DATETIME\
                )",newUser_id);

    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("create new table fail.\n");
        return 0;
    }
    printf("create newUser table success\n");

    return 1;
}