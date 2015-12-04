#include<ctype.h>

/*
atof函数：把字符串s转换为相应的双精度浮点数，处理可选的符号和小数点，并考虑可能缺少的整数或
	小数部分，改进：考虑e的情况。
*/
double atof(char s[])
{
	double val, power;
	int i,sign;

	for(i=0;isspace(s[i]);i++)	//跳过空白符
		;
	sign = (s[i] == '-') ? -1:1;
	if(s[i] == '+' || s[i] == '-')
		i++;
	for(val = 0.0;isdigit(s[i]); i++)
		val = 10.0*val + (s[i] - '0');
	if(s[i] == '.')
		i++;
	for(power = 1.0; isdigit(s[i]);i++){
		val = 10.0*val + (s[i] - '0');
		power *= 10.0;
	}
	return sign*val/power;
}