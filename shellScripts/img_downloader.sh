#!/bin/bash
# 用途：图片下载工具
# 文件名：img_downloader.sh

if [ $# -ne 3 ];
then
    echo "Usage: $0 URL -d DIRECTORY"
    exit -1
fi

# 当使用一次shift时候，$2的值移动到$1依次类推
for i in {1..4}
do
    case $1 in
    -d) shift; directory=$1; shift;;
     *) url=${url:-$1}; shift;;
    esac
done

echo $url
echo $directory

mkdir -p $directory;
baseurl=$(echo $url | egrep -o "https?://[a-z.]+")

echo $baseurl

curl -s $url | egrep -o "<img src=[^>]*>" | \
sed 's/<img src=\"\([^"]*\).*/\1/g' > /tmp/$$.list

sed -i "" "s|^/|$baseurl/|" /tmp/$$.list

cd $directory;

while read filename;
do
    curl -s -O "$filename" --silent
done < /tmp/$$.list
