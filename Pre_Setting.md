# Setting<사전 준비>
> - CLB로 사용할 머신 사전 작업
>   - `sudo apt update`
>   - `sudo apt install docker.io`
>   - 라즈베리파이인 경우
>       - `sudo docker run --name mariadb_clb -p 3306:3306 -e MYSQL_ROOT_PASSWORD =user -e TZ=Asia/Seoul -d jsurf/rpi-mariadb`
>   - 일반 머신인 경우
>       - `sudo docker run --name mariadb_clb -p 3306:3306 -e MYSQL_ROOT_PASSWORD =user -e TZ=Asia/Seoul -d mariadb`
>   - `/etc/systemd/system/`경로에 `LB/mariadb_clb.service` 파일 복사/넣기
>   - `sudo systemctl enable mairadb_clb`
>   - `sudo systemctl start mairadb_clb` 
>       - 머신이 꺼지더라도 `mariadb_clb` 컨테이너는 자동으로 다시 실행 됩니다.

> - CLB로 사용할 머신 DB Setting (이 부분은 자동화로 변경 예정)
>   - `sudo docker exec -it mariadb_clb bash` 컨테이너로 접속
>   - `mysql -u root -p` 비밀번호 `user` 입력
>``` sql
>   [none]> CREATE DATABASE UserFileList;
>   [none]> CREATE DATABASE UserList;
>   [none]> CREATE DATABASE getLocation;
>```
>``` sql 
>   [none]> USE UserList;
>   [UserList]> CREATE TABLE allUserList (
>               id          VARCHAR(100) NOT NULL PRIMARY KEY,
>               password    VARCHAR(100) NOT NULL,
>               join_date   DATETIME     DEFAULT  now()
>           );
>   [UserList]> USE getLocation;
>   [getLocation]> CREATE TABLE extSet (
>                   ext      VARCHAR(100) NOT NULL PRIMARY KEY,
>                   location VARCHAR(100) NOT NULL
>               );
>```
>
>아래 와 같이 아이템을 삽입 합니다
>```sql
> INSERT INTO extSet (ext,location) VALUES (... ,...); #예시
>+------+----------+
>| ext  | location |
>+------+----------+
>| avi  | video    |
>| doc  | docs     |
>| docx | docs     |
>| flv  | video    |
>| html | docs     |
>| hwp  | docs     |
>| mkv  | video    |
>| mov  | video    |
>| mp3  | audio    |
>| mp4  | video    |
>| ogg  | audio    |
>| pdf  | docs     |
>| raw  | audio    |
>| txt  | docs     |
>| wav  | audio    |
>| wma  | audio    |
>| wmv  | video    |
>+------+----------+
>```
>만약 Storage 가 받는 파일의 종류를 바꾸셨다면<br>
>location에 해당 Storage의 종류를 입력하고, ext에 넣고싶은 확장자를 입력하면 됩니다.



>- Client 사용자 사전 준비
>   - `sudo apt update`
>   - `sudo apt install sshd`
>   - `sudo apt install sshpass`

[돌아가기](./README.md)