#include<stdio.h>

static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

// month_name函数：返回第n个月份的名字
char *month_name(int n){
	//由于没有指定维度，由编译器统计初值个数并进行统计
	static char *name[] = {
		"Illegal month",
		"January", "February", "March",
		"April", "May", "June",
		"July", "August", "September",
		"October", "November", "December"
	};

	return (n<1 || n>12) ? name[0] : name[n];
}

//day_of_year函数：将某月某日的日期表示形式转换为某年中的第几天的表示形式
int day_of_year(int year, int month, int day){
	int i, leap;

	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	for(i=1;i<month;i++)
		day += daytab[leap][i];
	return day; 
}

//month_day函数：将某年中第几天的日期表示形式转换为某月某日的表示形式
void month_day(int year, int yearday, int *pmonth, int *pday){
	int i, leap;

	leap = year%4 == 0 && year%100 !=0 || year%400 == 0;
	for(i=1;yearday > daytab[leap][i];i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;
}

main(){
	// input
	// revoke day_of_year or month_day or month_name
	return 0;
}