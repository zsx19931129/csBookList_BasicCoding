#include<stdio.h>
#define MAXLINE 1000 		//允许的输入行的最大长度

int getline(char line[], int lim);
void copy(char to[], char from[]);

//打印最长的输入行
main(){
	int len;		//当前的行长度
	int max;		//目前为止发现的最长行的长度
	char line[MAXLINE];	//当前的输入行
	char longest[MAXLINE]; //用于保存最长的行

	max = 0;

	while( (len = getline(line, MAXLINE)) > 0 ){
		if(len > max){
			max = len;
			copy(longest, line);
		}
	}

	if(max > 0) //存在这样的行
		printf("%s", longest);	//行本身包含换行符
	return 0;
}

int getline(char line[], int lim){
	int i;
	int c;
	for(i = 0; i<lim-1 && (c = getchar()) != EOF && c != '\n'; i++ ){
		line[i] = c;
	}

	if(c == '\n'){
		line[i] = c;
		i++;
	}

	line[i] = '\0';
	return i;
}

void copy(char to[], char from[]){
	int i;

	i = 0;
	while( (to[i] = from[i]) != '\0' )
		i++;
}
