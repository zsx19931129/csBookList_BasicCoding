#!/bin/bash
#文件名：sighandle.sh
#用途：信号处理程序
function handler()
{
    echo Hey, received signal : SIGINT
}
echo My process ID is $$
# $$是一个特殊变量，它可以返回当前进程的进程ID
trap 'handler' SIGINT
#handler是信号SIGINT的信号处理程序
while true;
do
    sleep 1
done
