#!/bin/bash
#文件名：read_db.sh
#用途：从数据库中读取数据

USER="user"
PASS="user"

depts=`mysql -u $USER -p$PASS students <<EOF | tail -n +2
SELECT DISTINCT dept FROM students;
EOF`

for d in $depts;
do
    
    echo Department : $d

    result="`mysql -u $USER -p$PASS students <<EOF
    SET @i:=0;
    SELECT @i:=@i+1 as rank,name,mark FROM students WHERE dept="$d" ORDER
    BY mark DESC;
    EOF`"

    echo "$result"
    echo

done
