#!/bin/bash
#文件名：remove_duplicates.sh
#用途: 查找并删除重复文件，每一个文件只保留一个样本
ls -lS | awk 'BEGIN {
    getline;getline;
    name1=$9; size=$5;
}
{   
    name2=$9;
    cursize=$5;
    if (size==cursize)
    {
        "md5sum "name1 | getline; csum1=$1;
        "md5sum "name2 | getline; csum2=$1;
        if ( csum1==csum2 )
        { print name1; print name2; }
    };
    size=cursize; name1=name2;
}' | sort -u > /tmp/duplicate_files

cat /tmp/duplicate_files | xargs -I {} md5sum {} | sort -u -k1 #| awk ' { print ""$2"" } ' /tmp/duplicate_sample

#echo Removing..
#comm -23 /tmp/duplicate_files /tmp/duplicate_sample | tee /dev/stderr | xargs rm
#echo Removed duplicates files successfully
