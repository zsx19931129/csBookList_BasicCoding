#include <stdio.h>
#include <string.h>
#include "syscalls.h"		//该头文件在之前的demo中有实现
#include <fcnt1.h>			//读写标志
#include <sys/types.h>		//类型定义
#include <sys/stat.h>		//stat返回的结构
#include <sys/dir.h>		//本地目录结构

#define NAME_MAX 14			//最长文件名：由具体的系统决定

typedef struct {			//可移植的目录项
	long ino;				//i节点编号
	char name[NAME_MAX + 1];
} Dirent;

typedef struct {			//最小的DIR:无缓冲等特性
	int fd;
	Dirent d;				//目录项
} DIR;

//为实现OS无关特性独立出来的几个函数
DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);

void fsize(char *);

//打印文件长度
main(int argc, char **argv){
	if(argc == 1)		//默认为当前目录
		fsize(".");
	else
		while(--argc > 0)
			fsize(*++argv);
	return 0;
}

//调用stat函数获取文件信息
int stat(char *, struct stat *);
void dirwalk(char *, void (*fcn)(char *));

//fsize函数：打印文件name的长度
void fsize(char *name){
	struct stat stbuf;

	if(stat(name, &stbuf) == -1){
		fprintf(stderr, "fsize: can't access %s\n", name);
		return ;
	}
	if((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fsize);
	printf("%8ld %s\n", stbuf.st_size, name);
}

#define MAX_PATH 1024

//dirwalk函数：对dir中的所有文件调用函数fcn
void dirwalk(char *dir, void (*fcn)(char *)){
	char name[MAX_PATH];
	Dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir)) == NULL){
		fprintf(stderr, "dirwalk:can't open %s\n", dir);
		return ;
	}
	while((dp = readdir(dfd)) != NULL){
		if(strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
			continue;	//跳过自身目录和父目录
		if(strlen(dir) + strlen(dp->name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->name);
		else{
			fprintf(name, "%s/%s", dir, dp->name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}

//opendir函数
DIR *opendir(char *dirname){
	int fd;
	struct stat stbuf;
	DIR *dp;

	if((fd = open(dirname, O_RDONLY, 0)) == -1 ||
		 fstat(fd, &stbuf) == -1 || (stbuf.st_mode & S_IFMT) != S_IFDIR ||
		 (dp = (DIR *) malloc(sizeof(DIR))) == NULL)
		return NULL;
	dp->fd = fd;
	return dp;
}

//closedir函数：关闭由opendir打开的目录
void closedir(DIR *dp){
	if(dp){
		close(dp->fd);
		free(dp);
	}
}

//readdir函数：按顺序读取目录项
Dirent *readdir(DIR *dp){
	struct direct dirbuf;		//本地目录结构
	static Dirent d;			//返回：可移植结构

	while(read(dp->fd, (char *) &dirbuf, sizeof(dirbuf)) == sizeof(dirbuf)){
		if(dirbuf.d_ino == 0)	//目录未使用
			continue;
		d.ino = dirbuf.d_ino;
		strncpy(d.name, dirbuf.d_name, DIRSIZ);
		d.name[DIRSIZ] = '\0';	//添加终止符
		return &d;
	}
	return NULL;
}



