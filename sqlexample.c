#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "127.0.0.1";
    char *user = "root";
    char *password = "user";
    char *database = "UserFileList";

    if (!(conn = mysql_init((MYSQL *)NULL)))
    { 
        printf("init fail");
        exit(1);
    }

    printf("mysql_inti success.\n");

    if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0))
    {
        printf("connect error.\n"); //DB connect (MYSQL*, host, id, pw, null, port, 0)
        exit(1);
    }

    printf("mysql_real_connect suc.\n");

    if (mysql_select_db(conn, database) != 0)
    {
        mysql_close(conn);
        printf("select_db fail.\n");
        exit(1);
    }
    printf("select mydb suc.\n");

    char* sql ="CREATE TABLE user1(\
                    _id INT NOT NULL AUTO_INCREMENT,\
                    file_name VARCHAR(100),\
                    location VARCHAR(10),\
                    first_touch DATETIME,\
                    last_touch DATETIME,\
                    PRIMARY KEY( _id )\
                    )";

    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("create table fail.\n");
        exit(1);
    }

    sql = "INSERT INTO user1 (file_name, location) VALUES ('text.txt','docs1')";
    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("insert item fail.\n");
        exit(1);
    }

    sql = "INSERT INTO user1 (file_name, location, first_touch) \
            VALUES ('ttest.txt','docs2',now())\
            , ('a.video','video2',now())\
            , ('c.audio','audio1',now())";
    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("insert item fail.\n");
        exit(1);
    }

    sql = "DELETE FROM user1 WHERE file_name='ttest.txt'";
    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("delete item fail.\n");
        exit(1);
    }

    sql = "UPDATE user1 SET location = 'video5' WHERE file_name='a.video'";
    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("delete item fail.\n");
        exit(1);
    }

    sql = "UPDATE user1 SET last_touch = now() WHERE file_name='c.audio'";
    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("insert item fail.\n");
        exit(1);
    }

    sql = "SELECT file_name from user1";
    if(mysql_query(conn,sql)){
        mysql_close(conn);
        printf("insert item fail.\n");
        exit(1);
    }

    res=mysql_store_result(conn);
    int fildes = mysql_num_fields(res);

    while(row=mysql_fetch_row(res)){
        for(int cnt=0; cnt<fildes ;cnt++){
            printf("%12s\n",row[cnt]);
        }
    }

    mysql_free_result(res);
    mysql_close(conn);
}