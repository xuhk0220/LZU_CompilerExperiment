/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * 实验六 对完整程序的递归下降分析
 * 任务介绍:
 * 递归下降的语法分析
 * 输入：一个完整的源程序
 * 输出：正确或错误
 * 题目：设计一个程序，输入字符串形式的源程序，输出该程序是否符合相关语法，有错误时报告错误。
 *       要求：
 *       1.源语言及其语法规则：可以参照附录A，也可以自定义
 *       2.输入为字符串形式的源程序，因此，需要调用前面实验做过的词法分析器，为语法分析器提供单词符号
 *       3.应该指出错误的具体位置，如：在xx单词之后/之前发现错误，分析中止
 *             
 */


#include<stdio.h>
#include<sstream>
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string.h>
#include<math.h>
#include<cstring>
#include <vector>
#include<iomanip>

using namespace std;

char const *keyword[5] = { "if", "else", "while", "int", "main"}; //关键字

//单词表类
class Word          
{
	public:
	char value[20];//单词自身的值
	int type;//单词的种别（10标识符，20常数，30关键字，4x运算符(加减运算符为41，乘除为42，关系运算符43)，50界符）
	int line;//行号
};

FILE *fin;//文件流
int line=1;	//单词所在行数
char buff[10];//存放单词的数组
int flag=0;//词法分析判断是否已经从文件流中获取字符
Word new_w;

int e;//错误次数
char ch;//字符流中获取的字符
int flag1,flag2=0;
string ys[100];//已经声明的变量
int y_num=0;//已经声明变量的个数
int f=0; //表达式中运算符的个数
string a[200];
string a2;
string a3;
int a_num=1;

void P();//程序
int A(); //(){分程序}
void B();//分程序
void C();//变量说明部分
int F();//标识符表
int F1();//标识符
void D();//语句部分
int D1();//子语句部分
int H();//语句
int I();//赋值语句
int Q();//表达式
int X();//项
int X1();//子项
int Q1(); //子表达式
int Y();//因子
int J();//条件语句
void K();//循环语句
int H1();//语句1
int TJ();//条件

void error(char doc[])//报错，定位到错误行
{
        cout<<"第 "<<new_w.line<<" 行出错!\t";
		cout<<doc<<"\n";
		e++;
}

void ise(string a)  //检查变量是否被声明
{
	int i;
    for(i=1;i<=y_num;i++)
    {
    	if(a.compare(ys[i])==0)
		{
			break;
		}
	}
	if(i>y_num)
	{
		cout<<"第 "<<new_w.line<<" 行出错！\t";
		cout<<new_w.value<<" 没有被声明 !"<<endl;
		e++;
	}
}

int getWord()//词法分析
{
	int i=0; //buff当前字符的位置
	int j=0; //new_w当前字符的位置
	if(flag==0)
	{
		ch=getc(fin);
	}
	while(ch==' '||ch=='\n'||ch=='\t')//识别空格，换行，和tab
	{
		if(ch=='\n')
		{
			line++;//行+1
		}
		ch=getc(fin);
		flag=1;
	}

	// 识别标识符
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))//先识别开头为字母
	{
		for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}i=0;
		//标识符后续，字母或数字
		do{
			buff[i++]=ch;  //把字符流中的字符放到buff中
			ch=getc(fin);
			flag=1;
		}while((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9') || ch=='_');

		strcpy(new_w.value,buff);//把buff的值放到单词表
		new_w.line=line;//行数

		int n;
		//判断是不是关键字
		for(n=0;n<5;n++)
		{
			if(strcmp(new_w.value,keyword[n])==0)
			break;
		}
		if(n<5)//是关键字
		{
			new_w.type=30;//关键字类型为30
		}else{
			new_w.type=10;//标识符类型为10
		}
	return 0;
	}
    
    // 识别数字
	else if(ch>='0' && ch<='9')
	{
		for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}i=0;
		do{
			buff[i++]=ch;
			ch=getc(fin);
			flag=1;
		}while(ch>='0' && ch<='9');
		strcpy(new_w.value,buff);
		new_w.type=20;//数字类型为20
		new_w.line=line;
	return 0;
	}

	// 识别+-运算符
	else if(ch=='+' || ch=='-')
	{
		for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}i=0;
		buff[i++]=ch;
		ch=getc(fin);
		flag=1;
		strcpy(new_w.value,buff);
		new_w.type=41;	//加减运算符
		new_w.line=line;
	return 0;
	}

	// 识别*/运算符
	else if(ch=='*'||ch=='/')
	{
		for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}i=0;
		buff[i++]=ch;
		ch=getc(fin);
		flag=1;
		strcpy(new_w.value,buff);
		new_w.type=42;	//乘除运算符
		new_w.line=line;
	return 0;
	}

	// 识别关系运算符
	else if(ch=='='||ch=='<'||ch=='>'||ch=='!')
	{
		for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}
		i=0;
		buff[i++]=ch;
		ch=getc(fin);
		flag=1;
		if(ch=='=')
		{
			buff[i++]=ch;
			ch=getc(fin);
			flag=1;
		}
		strcpy(new_w.value,buff);
		new_w.type=43;	//关系运算符
		new_w.line=line;
	return 0;
	}

	// 识别界符
	else if(ch==','||ch==';'||ch=='('||ch==')'||ch=='{'||ch=='}')
	{
	   	for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}
		i=0;
	   	buff[i++]=ch;
	   	strcpy(new_w.value,buff);
	   	new_w.type=50;//界符
	   	ch=getc(fin);
	   	flag=1;
	   	return 0;
	}
	else
	{
	   	new_w.type=00;//错误的单词类型
	   	strcat(new_w.value," ");
		return 0;
	}
}

void P()//程序
{
	getWord();//(
	A();
}

int A() //(){分程序}
{
	if(strcmp(new_w.value,"{")){ error("缺少'{'");}
	getWord();//int
	B();    // 内部主体语句
	if(strcmp(new_w.value,"} ")){ error("缺少'}'");}
	cout<<"语法检查完成,共发现"<<e<<"个错误。"<<endl;
	return 0;
}

void B()//分程序内部
{
	C();    // 声明定义变量语句
	if(strcmp(new_w.value,";"))
	{ error("缺少';'");}
	getWord();//a=10；中的a
	D();    // 主体语句部分
}

void C()// 变量声明
{
	if(strcmp(new_w.value,"int"))
	{error("缺少'int'");}
		getWord();//a
		ys[++y_num]=new_w.value;
		F();
}

int F()//标识符表
{
	if(new_w.type==10)
	{
		getWord();
	}
	F1();//标识符表，标识符
	return 0;
}

int F1()//标识符
{
	if(strcmp(new_w.value,";")==0)
	{
		return 0;
	}
	if(strcmp(new_w.value,","))
	{error("标识符错误");}

	// 识别第一个变量
	getWord();//b
    ys[++y_num]=new_w.value;
	if(new_w.type!=10)
	{error("标识符错误");}

	// 同时声明多个变量时，识别后续变量声明
	getWord();
	F1();
	return 0;
}

void D()//主体语句部分
{
	H();//语句分类型
	D1();//子语句部分
}

int D1()//子语句部分
{
	if(strcmp(new_w.value,"}")==0)
	{
		getWord();
		if(strcmp(new_w.value,";")==0)
		{
			return 0;
		}
		else{
			H();
	 		D1();
			return 0;
		}
	}
	if(strcmp(new_w.value,";")==0)
	{
	 	getWord();
	 	H();
	 	D1();
	}//else error("语句错误");
	 return 0;
}

int H()//语句，有三种情况：赋值语句/if选择语句/while循环语句
{
	if(new_w.type==10)//如果当前符号为标识符，则为赋值语句
	{
		f=0; //运算符的个数
		flag2=0;
		ise(new_w.value);//判断标识符是否定义
		getWord();
		a[a_num++]=new_w.value;//记录赋值语句=号左边的标识符
		I();//赋值语句
		return 0;
	}
	 else if(strcmp(new_w.value,"if")==0)//判断是否为if条件语句
	 {
	 	flag2=0;
	 	getWord();
		J();//条件语句
		return 0;
	 }
	 else if(strcmp(new_w.value,"while")==0)//判断为while循环语句
	 {
	 	flag2=1;
	 	getWord();
		K();//循环语句
		return 0;
	 }
	 else if(strcmp(new_w.value,";")==0)
	 {
		return 0;
	 }
	 return 0;
}

int I()//赋值语句
{
	if(strcmp(new_w.value,"="))
	{error("缺乏'='");}
	getWord();
	Q();//赋值语句=号右边表达式
	return 0;
}

int Q()//表达式
{
	X();//项
	Q1();//子表达式
	return 0;
}

int X()//项
{
	Y();//因子
	X1();//子项
	return 0;
}

int X1()//子项
{
	flag1=0;
	//当下一个字符为;或者关系运算符时，表达式结束
	if(strcmp(new_w.value,";")==0 || new_w.type==43 || strcmp(new_w.value,")") ==0)
	{
		return 0;
	}

	//当下一个字符不是加减乘除或关系运算符，意味着该子项不成立，应该是表达式结束
	else if(new_w.type!=41&&new_w.type!=42&&new_w.type!=43&&strcmp(new_w.value,",")!=0 )
	{
		flag1=1;
		error("缺少';'");
		return 0;
	}
	getWord();
	Y(); // 识别因子
	return 0;
}

int Q1() //嵌套子表达式
{
	//当下一个字符为;或者关系运算符时，表达式结束
	if(strcmp(new_w.value,";")==0 || new_w.type==43||strcmp(new_w.value,")")==0)
	{
		return 0;
	}
	// 同前
	else if(new_w.type!=41&&new_w.type!=42&&new_w.type!=43&&strcmp(new_w.value,",")!=0)
	{
		if(flag1==0){
			error("缺少';'");
		}
		return 0;
	}
	getWord();
	X();     // 不断嵌套子表达式
	return 0;
}

int Y()//因子
{
	// 因子不是标识符，不是常数，也不是（）子项，为出错
	if(new_w.type!=10 && new_w.type!=20 && strcmp(new_w.value,"("))
	{
		error("表达式错误!!!!");
		return 0;
	}

	//标识符和数字
	if(new_w.type==10||new_w.type== 20)
	{
		if(new_w.type==10)
		{
			ise(new_w.value);
		}
		getWord();
		a[a_num++]=new_w.value;
		return 0;
	}

	// 因子为嵌套子项
	else if(strcmp(new_w.value,"(")==0)
	{
		getWord();
		Q();  // 子项表达式识别
		if(strcmp(new_w.value,")")==0)
		{
			return 0;
		}
	}
	return 0;
}

int J()//条件语句
{
	if(strcmp(new_w.value,"("))
	{
		error("缺少'('");
	 }
	 getWord();
	 TJ();   //(...)内部判断
	 if(strcmp(new_w.value,")"))
	 {
	 	error("缺少')'");
	 }
	  getWord();
	  H1();//语句一
	  if(strcmp(new_w.value,";")==0)
	  {
	   		getWord();
	  }

	  // 如果有else部分
	  if(strcmp(new_w.value,"else"))
	  {
	  	H1();//语句一
	  }
	  else
	  {
	  	getWord();
	  	H1();//语句一
	  }
	return 0;
}

void K()//循环语句
{
	if(strcmp(new_w.value,"("))
	{
		error("缺少'('");
	}
	getWord();
	TJ();           //(...)内部判断部分
	if(strcmp(new_w.value,")"))
	{
		error("缺少')'");
	}
	getWord();
	H1();           // 语句
}

int H1()//语句1
{
	//有｛｝情况
	if(strcmp(new_w.value,"{")==0)
	{
		getWord();
		D();//语句部分
		return 0;
	}
	// 无｛｝情况
	else
	{
		H();//语句
		return 0;
	}
}

int TJ()//条件
{
	Q();           // 表达式识别
	if(new_w.type!=43)
	{
		error("缺少关系运算符");
	}
	getWord();
	Q();          // 表达式识别
	return 0;
}

int main()
{
	char Infile[100];
	printf("输入要测试的源程序名字（xx.txt）:");
    scanf("%s", Infile);
    if ((fin = fopen(Infile, "r")) == NULL)
    {
        printf("\n 读入文件错误 \n");
        return (1);
    }

	// 主体部分
	P();
	return 0;
}

