/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * 实验七 对算术表达式构造递归下降翻译器
 * 任务介绍:
 * 对算术表达式做递归下降分析，同时将其翻译为中间代码
 * 输入：算术表达式
 * 输出：四元式序列
 * 题目：对实验四的程序进行升级改造，使得程序对于输入的任意一个算术表达式，在对其做递归下降分析的同时，生成等价的中间代码，一遍完成。要求：
 *       1.基础文法：同实验四
 *       2.语法分析：沿用实验四的程序框架
 *       3.语义处理：生成四元式
 *       4.一遍处理：把语义处理的代码插入到语法分析的代码中。设计要点有两个：
 *                  1.语法分析走到哪里的时候应该执行语义动作；
 *                  2.语义动作（这里指的是生成四元式）应该怎么做
 *       5.为简化问题，不考虑输入有错误的情况，不考虑语义检查
 * 
 */

#include<bits/stdc++.h>
using namespace std;
ifstream infile("result.txt");// 文件流
ifstream infile2("analysis.txt");// 文件流
ofstream outfile("result.txt");// 文件输出流
map<string,string> word;// 应用map数据结构形成一个string->string的对应
std::map<string,string>::iterator it;// 用来遍历整个对应关系的迭代器
string str;// 读入的字符串
string sym;// 用来指示读入的符号
string sym2;// 用来指示读入的符号
int count1=0,k=0;
// 读入的sym是否有错误；记录错误；记录左括号；
int flag=0,conterr=0,lpnum=0;

string expressionAnalysis();// 表达式分析，表达式的中间代码表示
string termAnalysis();// 项分析，表达式的中间代码表示
string factorAnalysis();// 因子分析，表达式的中间代码表示
int expressionAnalysis2();// 表达式分析，算数表达式的语义计算
int termAnalysis2();// 项分析，算数表达式的语义计算
int factorAnalysis2();// 因子分析，算数表达式的语义计算

// 定义四元式
struct quad{
     string result;
     string arg1;
     string arg2;
     string op;
};
struct quad quad[50];

// 对应关系进行初始化，如下只包括了表达式的相关符号
void map_init(){
    word["+"]="plus";
    word["-"]="minus";
    word["*"]="times";
    word["/"]="slash";
    word["="]="eql";
    word["("]="lparen";
    word[")"]="rparen";
}

// 词法分析
void lexanalysis(){
    char ch;
    char a;
    string word1;// string变量识别单词
    string str;// string变量进行字符识别
    ostringstream buf;

    // 将文件中的字符串全部读出来
    while(buf&&infile2.get(ch)){
        buf.put(ch);
    }
    str= buf.str();
    cout<<str;     
    int csize=str.length();

    // 对整个字符串进行遍历
    for(int i=0;i<csize;i++){
        while(str[i]==' '||str[i]=='\n') i++;// 若识别到空格或换行符，识别位后移

        // 识别标识符
        if(isalpha(str[i])){// 调用库函数isalpha()，判断首字符是否为字母
            word1=str[i++];
            while(isalpha(str[i])||isdigit(str[i])){   // 判断标识符命名是否合法（字母开头+字母/数字）
                word1+=str[i++];
            }

            it=word.find(word1);
            // 判断该标识是不是基本字，若不是基本字则输出标识符二元组
            if(it!=word.end()){
                outfile<<"("<<word[word1]<<","<<word1<<")"<<endl;
            }else{
                // 否则直接输出字符
                outfile<<"(ident"<<","<<word1<<")"<<endl;
            }
            i--;
        }

        // 识别数字
        else if(isdigit(str[i])){// 判断是不是常数，调用库函数isdigit()
            word1=str[i++];
            while(isdigit(str[i])){
                word1+=str[i++];
            }
             if(isalpha(str[i])){
                outfile<<"error"<<endl;
                break;
            }
            else{
                outfile<<"(number"<<","<<word1<<")"<<endl;
            }
            i--;
        }
        
        // 对其他的基本字依次进行判断
        else{
            word1=str[i];
            it=word.find(word1);
            if(it!=word.end()){
                outfile<<"("<<word[word1]<<","<<word1<<")"<<endl;
            }else{
                outfile<<"error"<<endl;
                break;
            }
        }
    }
    infile2.close();
}

// 读入字符sym的移动
int advance(){
    int found1,found2;
    if(!getline(infile,str)){
        return 0;
    }
    found1=str.find(',',0);
    if(found1 == -1){
        conterr++;
        cout<<"错误：语法错误 识别字符错误"<<endl;
        return -1;
    }
    found2=str.length();
    sym=str.substr(1,found1-1);
    sym2=str.substr(found1+1,found2-found1-2);
    return 1;
}

// 产生新变量名t1,t2...作为四元式标记符号
string newtemp(){
    char *p;
    char m[12];
    p=(char*)malloc(12);
    k++;
    itoa(k,m,10);
    strcpy(p+1,m);
    p[0]='t';
    string s;
    s=p;
    return s;
}

// 产生四元式用于显示
void emit(string op,string arg1,string arg2,string result){
    quad[count1].op=op;
    quad[count1].arg1=arg1;
    quad[count1].arg2=arg2;
    quad[count1].result=result;
    count1++;
    return;
}

// 表达式的递归下降分析程序
string expressionAnalysis(){
    string op,arg1,arg2,result;
    if(conterr){
        return NULL;
	}

    // 通过项分析得到第一个参数的值
	arg1=termAnalysis();
	if(conterr){
        return NULL;
	}
    // +/-运算
	while((sym=="plus")||(sym=="minus")){
        op=sym2;
		flag=advance();
		if(conterr){
            return NULL;
		}
        // sym有错误
		if(flag==0){
            cout<<"错误：语法错误 <加法运算符>后缺项"<<endl;
            conterr++;
			return NULL;
		}
		arg2=termAnalysis();// 通过项分析得到第二个参数的值
		if(conterr){
            return NULL;
		}
		result=newtemp();// 产生中间变量名，相当于对结果进行存储
		emit(op,arg1,arg2,result);// 产生四元式，相当于进行加法或减法运算，得出结果
		arg1=result;// 保存得到的结果到第一个因子
	}
	return arg1;// 返回表达式最终得到的值
}

// 项的递归下降分析程序
string termAnalysis(){
    string op,arg1,arg2,result;
    arg1=factorAnalysis();// 通过因子分析得到第一个参数的值
    if(conterr){
        return NULL;
    }
	while((sym=="times")||(sym=="slash")){
        op=sym2;
		flag=advance();
		if(conterr){
            return NULL;
		}
		if(flag==0){
			conterr++;
			cout<<"错误：语法错误 <乘法运算符>后缺因子"<<endl;
			return NULL;
		}
		if(conterr){
            return NULL;
		}
		arg2=factorAnalysis();// 通过因子分析得到第二个参数的值
		if(conterr){
            return NULL;
		}
		result=newtemp();// 产生中间变量名，相当于对结果进行存储
		emit(op,arg1,arg2,result);// 产生四元式，相当于进行乘法或除法运算，得出结果
		arg1=result;// 保存得到的结果
	}
	return arg1;// 返回项最终得到的值
}

string factorAnalysis(){
    string arg;
    if(sym=="ident"){// 如果是标识符，最终返回标识符的符号
        arg=sym2;
        flag=advance();
        if(conterr){
            return NULL;
		}
		if(lpnum==0&&sym=="rparen"){
            conterr++;
			cout<<"错误：语法错误 ')'不匹配"<<endl;
			return NULL;
        }
    }
    else if(sym=="number"){// 如果是无符号整数，最终返回相应的整数
        arg=sym2;
        flag=advance();
        if(conterr){
            return NULL;
		}
		if(lpnum==0&&sym=="rparen"){
            conterr++;
			cout<<"错误：语法错误 ')'不匹配"<<endl;
			return NULL;
        }
    }
    else if(sym=="lparen"){// 如果是左括号，则要进行右括号匹配，并判断中间是不是表达式，并得出表达式的值进行返回
        lpnum++;
        flag=advance();
        if(conterr){
            return NULL;
		}
		if(flag==0){
			conterr++;
			cout<<"错误：语法错误 '('后缺少表达式"<<endl;
			return NULL;
		}
        arg=expressionAnalysis();
        if(conterr){
            return NULL;
		}
        if(flag==0||sym!="rparen"){
			conterr++;
			cout<<"错误：语法错误 表达式后面缺少')'"<<endl;
			return " ";
		}else{
		    lpnum--;
            flag=advance();
            if(conterr){
                return NULL;
            }
            if(flag==0){
                return arg;
            }
		}
    }else{
		cout<<"错误：语法错误 因子首部不为<标识符>|<无符号整数>|'('"<<endl;
		conterr++;
		return " ";
	}
	return arg;
}

int main(){
    int i=0,num,result;
    // 开始词法分析
    map_init();

    cout<<"~~算术表达式："<<endl;
    lexanalysis();
    cout<<endl;
   
    // 开始语法和语义分析 
    cout<<endl<<"~~算术表达式的中间代码表示:"<<endl;

    flag=advance();
    
    if(flag){
        expressionAnalysis();// 开始进行表达式分析
    }
    if(flag!=-1&&!conterr){// 若表达式正确则输出表达式的中间代码表示
        for(int i=0;i<count1;i++){
            cout<<'('<<quad[i].op<<','<<quad[i].arg1<<','<<quad[i].arg2<<','<<quad[i].result<<')'<<endl;;
        }
    }

    infile.close();
    return 0;
}