/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * ʵ���� ����������ĵݹ��½�����
 * �������:
 * �ݹ��½����﷨����
 * ���룺һ��������Դ����
 * �������ȷ�����
 * ��Ŀ�����һ�����������ַ�����ʽ��Դ��������ó����Ƿ��������﷨���д���ʱ�������
 *       Ҫ��
 *       1.Դ���Լ����﷨���򣺿��Բ��ո�¼A��Ҳ�����Զ���
 *       2.����Ϊ�ַ�����ʽ��Դ������ˣ���Ҫ����ǰ��ʵ�������Ĵʷ���������Ϊ�﷨�������ṩ���ʷ���
 *       3.Ӧ��ָ������ľ���λ�ã��磺��xx����֮��/֮ǰ���ִ��󣬷�����ֹ
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

char const *keyword[5] = { "if", "else", "while", "int", "main"}; //�ؼ���

//���ʱ���
class Word          
{
	public:
	char value[20];//���������ֵ
	int type;//���ʵ��ֱ�10��ʶ����20������30�ؼ��֣�4x�����(�Ӽ������Ϊ41���˳�Ϊ42����ϵ�����43)��50�����
	int line;//�к�
};

FILE *fin;//�ļ���
int line=1;	//������������
char buff[10];//��ŵ��ʵ�����
int flag=0;//�ʷ������ж��Ƿ��Ѿ����ļ����л�ȡ�ַ�
Word new_w;

int e;//�������
char ch;//�ַ����л�ȡ���ַ�
int flag1,flag2=0;
string ys[100];//�Ѿ������ı���
int y_num=0;//�Ѿ����������ĸ���
int f=0; //���ʽ��������ĸ���
string a[200];
string a2;
string a3;
int a_num=1;

void P();//����
int A(); //(){�ֳ���}
void B();//�ֳ���
void C();//����˵������
int F();//��ʶ����
int F1();//��ʶ��
void D();//��䲿��
int D1();//����䲿��
int H();//���
int I();//��ֵ���
int Q();//���ʽ
int X();//��
int X1();//����
int Q1(); //�ӱ��ʽ
int Y();//����
int J();//�������
void K();//ѭ�����
int H1();//���1
int TJ();//����

void error(char doc[])//������λ��������
{
        cout<<"�� "<<new_w.line<<" �г���!\t";
		cout<<doc<<"\n";
		e++;
}

void ise(string a)  //�������Ƿ�����
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
		cout<<"�� "<<new_w.line<<" �г���\t";
		cout<<new_w.value<<" û�б����� !"<<endl;
		e++;
	}
}

int getWord()//�ʷ�����
{
	int i=0; //buff��ǰ�ַ���λ��
	int j=0; //new_w��ǰ�ַ���λ��
	if(flag==0)
	{
		ch=getc(fin);
	}
	while(ch==' '||ch=='\n'||ch=='\t')//ʶ��ո񣬻��У���tab
	{
		if(ch=='\n')
		{
			line++;//��+1
		}
		ch=getc(fin);
		flag=1;
	}

	// ʶ���ʶ��
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))//��ʶ��ͷΪ��ĸ
	{
		for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}i=0;
		//��ʶ����������ĸ������
		do{
			buff[i++]=ch;  //���ַ����е��ַ��ŵ�buff��
			ch=getc(fin);
			flag=1;
		}while((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9') || ch=='_');

		strcpy(new_w.value,buff);//��buff��ֵ�ŵ����ʱ�
		new_w.line=line;//����

		int n;
		//�ж��ǲ��ǹؼ���
		for(n=0;n<5;n++)
		{
			if(strcmp(new_w.value,keyword[n])==0)
			break;
		}
		if(n<5)//�ǹؼ���
		{
			new_w.type=30;//�ؼ�������Ϊ30
		}else{
			new_w.type=10;//��ʶ������Ϊ10
		}
	return 0;
	}
    
    // ʶ������
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
		new_w.type=20;//��������Ϊ20
		new_w.line=line;
	return 0;
	}

	// ʶ��+-�����
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
		new_w.type=41;	//�Ӽ������
		new_w.line=line;
	return 0;
	}

	// ʶ��*/�����
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
		new_w.type=42;	//�˳������
		new_w.line=line;
	return 0;
	}

	// ʶ���ϵ�����
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
		new_w.type=43;	//��ϵ�����
		new_w.line=line;
	return 0;
	}

	// ʶ����
	else if(ch==','||ch==';'||ch=='('||ch==')'||ch=='{'||ch=='}')
	{
	   	for(i=0;i<10;i++)
		{
			buff[i]=NULL;
		}
		i=0;
	   	buff[i++]=ch;
	   	strcpy(new_w.value,buff);
	   	new_w.type=50;//���
	   	ch=getc(fin);
	   	flag=1;
	   	return 0;
	}
	else
	{
	   	new_w.type=00;//����ĵ�������
	   	strcat(new_w.value," ");
		return 0;
	}
}

void P()//����
{
	getWord();//(
	A();
}

int A() //(){�ֳ���}
{
	if(strcmp(new_w.value,"{")){ error("ȱ��'{'");}
	getWord();//int
	B();    // �ڲ��������
	if(strcmp(new_w.value,"} ")){ error("ȱ��'}'");}
	cout<<"�﷨������,������"<<e<<"������"<<endl;
	return 0;
}

void B()//�ֳ����ڲ�
{
	C();    // ��������������
	if(strcmp(new_w.value,";"))
	{ error("ȱ��';'");}
	getWord();//a=10���е�a
	D();    // ������䲿��
}

void C()// ��������
{
	if(strcmp(new_w.value,"int"))
	{error("ȱ��'int'");}
		getWord();//a
		ys[++y_num]=new_w.value;
		F();
}

int F()//��ʶ����
{
	if(new_w.type==10)
	{
		getWord();
	}
	F1();//��ʶ������ʶ��
	return 0;
}

int F1()//��ʶ��
{
	if(strcmp(new_w.value,";")==0)
	{
		return 0;
	}
	if(strcmp(new_w.value,","))
	{error("��ʶ������");}

	// ʶ���һ������
	getWord();//b
    ys[++y_num]=new_w.value;
	if(new_w.type!=10)
	{error("��ʶ������");}

	// ͬʱ�����������ʱ��ʶ�������������
	getWord();
	F1();
	return 0;
}

void D()//������䲿��
{
	H();//��������
	D1();//����䲿��
}

int D1()//����䲿��
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
	}//else error("������");
	 return 0;
}

int H()//��䣬�������������ֵ���/ifѡ�����/whileѭ�����
{
	if(new_w.type==10)//�����ǰ����Ϊ��ʶ������Ϊ��ֵ���
	{
		f=0; //������ĸ���
		flag2=0;
		ise(new_w.value);//�жϱ�ʶ���Ƿ���
		getWord();
		a[a_num++]=new_w.value;//��¼��ֵ���=����ߵı�ʶ��
		I();//��ֵ���
		return 0;
	}
	 else if(strcmp(new_w.value,"if")==0)//�ж��Ƿ�Ϊif�������
	 {
	 	flag2=0;
	 	getWord();
		J();//�������
		return 0;
	 }
	 else if(strcmp(new_w.value,"while")==0)//�ж�Ϊwhileѭ�����
	 {
	 	flag2=1;
	 	getWord();
		K();//ѭ�����
		return 0;
	 }
	 else if(strcmp(new_w.value,";")==0)
	 {
		return 0;
	 }
	 return 0;
}

int I()//��ֵ���
{
	if(strcmp(new_w.value,"="))
	{error("ȱ��'='");}
	getWord();
	Q();//��ֵ���=���ұ߱��ʽ
	return 0;
}

int Q()//���ʽ
{
	X();//��
	Q1();//�ӱ��ʽ
	return 0;
}

int X()//��
{
	Y();//����
	X1();//����
	return 0;
}

int X1()//����
{
	flag1=0;
	//����һ���ַ�Ϊ;���߹�ϵ�����ʱ�����ʽ����
	if(strcmp(new_w.value,";")==0 || new_w.type==43 || strcmp(new_w.value,")") ==0)
	{
		return 0;
	}

	//����һ���ַ����ǼӼ��˳����ϵ���������ζ�Ÿ����������Ӧ���Ǳ��ʽ����
	else if(new_w.type!=41&&new_w.type!=42&&new_w.type!=43&&strcmp(new_w.value,",")!=0 )
	{
		flag1=1;
		error("ȱ��';'");
		return 0;
	}
	getWord();
	Y(); // ʶ������
	return 0;
}

int Q1() //Ƕ���ӱ��ʽ
{
	//����һ���ַ�Ϊ;���߹�ϵ�����ʱ�����ʽ����
	if(strcmp(new_w.value,";")==0 || new_w.type==43||strcmp(new_w.value,")")==0)
	{
		return 0;
	}
	// ͬǰ
	else if(new_w.type!=41&&new_w.type!=42&&new_w.type!=43&&strcmp(new_w.value,",")!=0)
	{
		if(flag1==0){
			error("ȱ��';'");
		}
		return 0;
	}
	getWord();
	X();     // ����Ƕ���ӱ��ʽ
	return 0;
}

int Y()//����
{
	// ���Ӳ��Ǳ�ʶ�������ǳ�����Ҳ���ǣ������Ϊ����
	if(new_w.type!=10 && new_w.type!=20 && strcmp(new_w.value,"("))
	{
		error("���ʽ����!!!!");
		return 0;
	}

	//��ʶ��������
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

	// ����ΪǶ������
	else if(strcmp(new_w.value,"(")==0)
	{
		getWord();
		Q();  // ������ʽʶ��
		if(strcmp(new_w.value,")")==0)
		{
			return 0;
		}
	}
	return 0;
}

int J()//�������
{
	if(strcmp(new_w.value,"("))
	{
		error("ȱ��'('");
	 }
	 getWord();
	 TJ();   //(...)�ڲ��ж�
	 if(strcmp(new_w.value,")"))
	 {
	 	error("ȱ��')'");
	 }
	  getWord();
	  H1();//���һ
	  if(strcmp(new_w.value,";")==0)
	  {
	   		getWord();
	  }

	  // �����else����
	  if(strcmp(new_w.value,"else"))
	  {
	  	H1();//���һ
	  }
	  else
	  {
	  	getWord();
	  	H1();//���һ
	  }
	return 0;
}

void K()//ѭ�����
{
	if(strcmp(new_w.value,"("))
	{
		error("ȱ��'('");
	}
	getWord();
	TJ();           //(...)�ڲ��жϲ���
	if(strcmp(new_w.value,")"))
	{
		error("ȱ��')'");
	}
	getWord();
	H1();           // ���
}

int H1()//���1
{
	//�У������
	if(strcmp(new_w.value,"{")==0)
	{
		getWord();
		D();//��䲿��
		return 0;
	}
	// �ޣ������
	else
	{
		H();//���
		return 0;
	}
}

int TJ()//����
{
	Q();           // ���ʽʶ��
	if(new_w.type!=43)
	{
		error("ȱ�ٹ�ϵ�����");
	}
	getWord();
	Q();          // ���ʽʶ��
	return 0;
}

int main()
{
	char Infile[100];
	printf("����Ҫ���Ե�Դ�������֣�xx.txt��:");
    scanf("%s", Infile);
    if ((fin = fopen(Infile, "r")) == NULL)
    {
        printf("\n �����ļ����� \n");
        return (1);
    }

	// ���岿��
	P();
	return 0;
}

