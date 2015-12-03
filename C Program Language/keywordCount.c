#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key))
//#define NKEYS (sizeof keytab / sizeof keytab[0])
#define BUFSIZE 100
char buf[BUFSIZE];	//ungetch的缓冲区
int bufp = 0;	//buf中下一个空闲位置

int getword(char *, int);
struct key * binsearch(char *, struct key *, int);

struct key{
	char *word;
	int count;
} keytab[] = {
	"auto", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"continue", 0,
	"default", 0,
	/* .... */
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0,
};

//统计输入中C语言关键字的出现次数
main(){
	int n;
	char word[MAXWORD];
	struct key * p;

	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			if((p = binsearch(word, keytab, NKEYS)) != NULL)
				p->count++;
	for(p = keytab;p < keytab + NKEYS;p++)
		if(p->count > 0)
			printf("%4d %s\n", p->count, p->word);
	return 0;
}

//binsearch函数：在tab[0]到tab[n-1]中查找单词
struct key *
binsearch(char *word, struct key *tab, int n){
	int cond;
	struct key *low = &tab[0];
	struct key *high = &tab[n];
	struct key *mid;

	while(low < high){
		mid = low + (high - low)/2;		//指针间的加法没有意义，指针间的减法才有意义，所以求中间元素不能直接相加/2
		if((cond = strcmp(word, mid->word)) < 0)
			high = mid;
		else if(cond > 0)
			low = mid + 1;
		else 
			return mid;
	}
	return NULL;
}

//getword函数:从输入中读取下一个单词或字符
int getword(char *word, int lim){
	int getch(void);
	void ungetch(char);
	char *w = word;

	while(isspace(c = getch()))
		;
	if(c!=EOF)
		*w++ = c;
	if(!isalpha(c)){
		*w = '\0';
		return c;
	}
	for( ;--lim > 0;w++)
		if(!isalnum(*w = getch())){
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}

int getch(void){		//取一个字符，可能是压回字符
	return (bufp > 0)?buf[--bufp]:getchar();
}

void ungetch(char c){
	if(bufp>=BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
