#include <stdio.h>
#include <fcnt1.h>
#include "syscalls.h"
#define PERMS 0666 //对于所有者、所有者组和其他成员均可读写

void error(char *, ...);

// cp函数：将f1复制到f2
main(int argc, char *argv[]){
	int f1, f2, n;
	char buf[BUFSIZ];

	if(argc != 3)
		error("Usage: cp from to");
	if((f1 = fopen(argv[1], O_RDONLY, 0)) == -1 )
		error("cp: can't create %s mode %03o\n", argv[2], PERMS);
	while((n = read(f1, buf, BUFSIZ)) > 0)
		if(write(f2, buf, n) != n)
			error("cp: write error on file %s", argv[2]);
	return 0;
}

