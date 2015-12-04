//模拟实现fopen函数和getc函数

#include <fcnt1.h>
#include "syscalls.h"
#define PERMS 0666	//所有者、所有者组和其他成员都可以读写

int _fillbuf(FILE *fp);

#define NULL 0
#define EOF (-1)
#define BUFSIZ 1024
#define OPEN_MAX 20		//一次最多可打开的文件数

typedef struct _iobuf{
	int cnt;		//剩余的字符数
	char *ptr;		//下一个字符的位置
	char *base;		//缓冲区的位置
	int flag;		//文件访问模式
	int fd;			//文件描述符
} FILE;
//extern FILE _iob[OPEN_MAX];
FILE _iob[OPEN_MAX] = {		
	// stdin, stdout, stderr
	{0, (char *)0, (char *)0, _READ, 0},
	{0, (char *)0, (char *)0, _WRITE, 1},
	{0, (char *)0, (char *)0, _WRITE | _UNBUF, 2}
};


#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum _flags {
	_READ = 01,		//以读方式打开文件
	_WRITE = 02,	//以写方式打开文件
	_UNBUF = 04,	//不对文件进行缓冲
	_EOF = 010,		//已到文件的末尾
	_ERR = 020		//该文件发生错误
};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 \
		? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p) (--(p)->cnt >= 0 \
		? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc((x), stdout)

int _fillbuf(FILE *fp){
	int bufsize;

	if((fp->flag&(_READ | _EOF | _ERR)) != _READ)
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if(fp->base == NULL)		//还未分配缓冲区
		if((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;		//不能分配缓冲区
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if(--fp->cnt < 0){
		if(fp->cnt == -1)
			fp->flag |= _EOF;
		else
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char)*fp->ptr++;
}

// fopen函数：打开文件，并返回文件指针
FILE *fopen(char *name, char *mode){
	int fd;
	FILE *fp;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for(fp = _iob;fp < _iob + OPEN_MAX;fp++)
		if((fp->flag & (_READ | _WRITE)) == 0)
			break;	//寻找一个空闲位
	if(fp >= _iob + OPEN_MAX)	//没有空闲位置
		return NULL;
	
	if(*mode == 'w')
		fd = creat(name, PERMS);
	else if(*mode == 'a'){
		if((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	}else
		fd = open(name, O_RONLY, 0);
	if(fd == -1)
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

