#!/bin/bash
# 文件名：fetch_gmail.sh
# 用途:获取Gmail工具

username="PUT_USERNAME_HERE"
password="PUT_PASSWORD_HERE"

SHOW_COUNT=5    # 最近未读的邮件数目

echo

curl -u $username:$password --silent "https://mail.google.com/mail/feed/atom" | \
tr -d '\n' | sed 's:</entry>:\n:g' | \
sed 's/.*<title>\(.*\)<\/title.*<author><name>\([^<]*\)<\/name><email>
\([^<]*\).*/Author: \2 [\3] \nSubject: \1\n/' | \
head -n $( $SHOW_COUNT * 3 ))
