#! /bin/bash
# filename: filestat.sh

if [ $# -ne 1 ];
then
    echo $0 basepath;
    echo
fi
path=$1

declare -A statarray;

while read line;
do
    ftype=`file -b "$line"`
    let statarray["$ftype"]++;
done< <(find $path -type f -print)    #<(find $path -type f -print 等同于文d件，用子进程输出来代替文件名。)

echo ========== File types and counts =============
for ftype in "${!statarray[@]}";
do
    echo $ftype : ${statarray["$ftype"]}
done

