#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>


//this is SIGN UP function
//check present User in database 
//if success return 1 fail 0
int createNewUserInfo_DB(char *newUser_id,char *newUser_password){
    printf("--------createNewUserInfo_DB-----------\n");
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

    //flag var if it is not new user 
    int isNotNew;

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

    //check if id exists
    sprintf(query,"select id from allUserList where id='%s'",newUser_id);
    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("fail select id,password.\n");
        return 0;
    }
    else{
        res=mysql_store_result(conn); //result query
        isNotNew = mysql_num_rows(res); //if result is 0 no Info in table, if result is 1 get Info in table
                                        // 0 = empty , 1 = match one
        if(isNotNew){ //find match infomation fail
            mysql_close(conn);
            printf("fail select id,password.\n");
            return 0;
        }
        else{ //do not find match infomation success
            printf("success select is,password\n");
        }
        
    }

    //insert item newUser information
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

    //create newUser filelist table
    sprintf(query,"CREATE TABLE IF NOT EXISTS %s(\
                file_name VARCHAR(100) PRIMARY KEY,\
                location VARCHAR(10) NOT NULL,\
                first_touch DATETIME NOT NULL,\
                last_touch DATETIME NOT NULL\
                )",newUser_id);
    if(mysql_query(conn,query)){
        mysql_close(conn);
        printf("create new table fail.\n");
        return 0;
    }
    printf("create newUser table success\n");
    
    mysql_close(conn);
    return 1;
}