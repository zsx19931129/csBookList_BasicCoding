#include<stdio.h>
#include<string.h>

#define MAXTOKEN 100

enum{ NAME, PARENS, BRACKETS};

void dcl(void);
void dirdcl(void);
int gettoken(void);
int tokentype;			//最后一个记号的类型
char token[MAXTOKEN];	//最后一个记号的字符串
char name[MAXTOKEN];	//标识符名
char datatype[MAXTOKEN];//出具类型为char、int等
char out[1000];			//输出串

//将声明转换为文字描述
main(){
	while( gettoken() != EOF ){	//该行的第一个记号是数据类型
		strcpy(datatype, token);
		out[0] = '\0';
		dcl();	//分析该行的其余部分
		if(tokentype != '\n')
			printf("syntax error\n");
		printf("%s: %s %s\n", name, out, datatype);
	}
	return 0;
}

//返回下一个标记
int gettoken(void){
	int c, getch(void);
	void ungetch(int);
	char *p = token;

	while((c = getch()) == ' ' || c == '\t' )
		;
	if( c == '('){
		if((c = getch()) == ')'){
			strcpy(token, "()");
			return tokentype = PARENS;
		}
	}else if( c == '['){
		for(*p++ = c; ( *p++ = getch()) != ']';)
			;
		*p = '\0';
		return tokentype = BRACKETS;
	}else if(isalpha(c)){
		for(*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	}else 
		return tokentype = c; 
}

//dcl函数：对一个声明符进行语法分析
void dcl(void){
	int ns;

	for(ns = 0; gettoken() == '*'; )	//统计字符*的个数
		ns++;
	dirdcl();
	while(ns-- > 0)
		strcat(out, " pointer to");
}

// dirdcl函数：分析一个直接声明
void dirdcl(void){
	int type;

	if(tokentype == '('){				//形式为(dcl)
		dcl();
		if(tokentype != ')')
			printf("error: missing )\n");
	}else if(tokentype == NAME)			//变量名
		strcpy(name, token);
	else
		printf("error:expected name or (dcl)\n");
	while((type=gettoken()) == PARENS || type == BRACKETS)
		if(type == PARENS)
			strcat(out, " function returning");
		else{
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
}

// //undcl函数：将文字转换为声明，将文字简化为x () * [] () char 这种形式作为输入
// //这种方法的一个缺点是会多生成圆括号
// main(){
// 	int type;
// 	char temp[MAXTOKEN];

// 	while(gettoken()!=EOF){
// 		strcpy(out, token);
// 		if(type == PARENS || type == BRACKETS)
// 			strcat(out, token);
// 		else if(type == '*'){
// 			sprintf(temp, "(*%s)", out);
// 			strcpy(out, temp);
// 		}else if(type == NAME){
// 			sprintf(temp, "%s %s", token, out);
// 			strcpy(out, temp);
// 		}else
// 			printf("invalid input at %s\n", token);
// 		printf("%s\n", out);
// 	}
// 	return 0;
// }