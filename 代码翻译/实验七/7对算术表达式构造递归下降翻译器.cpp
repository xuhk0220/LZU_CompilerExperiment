/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * ʵ���� ���������ʽ����ݹ��½�������
 * �������:
 * ���������ʽ���ݹ��½�������ͬʱ���䷭��Ϊ�м����
 * ���룺�������ʽ
 * �������Ԫʽ����
 * ��Ŀ����ʵ���ĵĳ�������������죬ʹ�ó���������������һ���������ʽ���ڶ������ݹ��½�������ͬʱ�����ɵȼ۵��м���룬һ����ɡ�Ҫ��
 *       1.�����ķ���ͬʵ����
 *       2.�﷨����������ʵ���ĵĳ�����
 *       3.���崦��������Ԫʽ
 *       4.һ�鴦�������崦��Ĵ�����뵽�﷨�����Ĵ����С����Ҫ����������
 *                  1.�﷨�����ߵ������ʱ��Ӧ��ִ�����嶯����
 *                  2.���嶯��������ָ����������Ԫʽ��Ӧ����ô��
 *       5.Ϊ�����⣬�����������д���������������������
 * 
 */

#include<bits/stdc++.h>
using namespace std;
ifstream infile("result.txt");// �ļ���
ifstream infile2("analysis.txt");// �ļ���
ofstream outfile("result.txt");// �ļ������
map<string,string> word;// Ӧ��map���ݽṹ�γ�һ��string->string�Ķ�Ӧ
std::map<string,string>::iterator it;// ��������������Ӧ��ϵ�ĵ�����
string str;// ������ַ���
string sym;// ����ָʾ����ķ���
string sym2;// ����ָʾ����ķ���
int count1=0,k=0;
// �����sym�Ƿ��д��󣻼�¼���󣻼�¼�����ţ�
int flag=0,conterr=0,lpnum=0;

string expressionAnalysis();// ���ʽ���������ʽ���м�����ʾ
string termAnalysis();// ����������ʽ���м�����ʾ
string factorAnalysis();// ���ӷ��������ʽ���м�����ʾ
int expressionAnalysis2();// ���ʽ�������������ʽ���������
int termAnalysis2();// ��������������ʽ���������
int factorAnalysis2();// ���ӷ������������ʽ���������

// ������Ԫʽ
struct quad{
     string result;
     string arg1;
     string arg2;
     string op;
};
struct quad quad[50];

// ��Ӧ��ϵ���г�ʼ��������ֻ�����˱��ʽ����ط���
void map_init(){
    word["+"]="plus";
    word["-"]="minus";
    word["*"]="times";
    word["/"]="slash";
    word["="]="eql";
    word["("]="lparen";
    word[")"]="rparen";
}

// �ʷ�����
void lexanalysis(){
    char ch;
    char a;
    string word1;// string����ʶ�𵥴�
    string str;// string���������ַ�ʶ��
    ostringstream buf;

    // ���ļ��е��ַ���ȫ��������
    while(buf&&infile2.get(ch)){
        buf.put(ch);
    }
    str= buf.str();
    cout<<str;     
    int csize=str.length();

    // �������ַ������б���
    for(int i=0;i<csize;i++){
        while(str[i]==' '||str[i]=='\n') i++;// ��ʶ�𵽿ո���з���ʶ��λ����

        // ʶ���ʶ��
        if(isalpha(str[i])){// ���ÿ⺯��isalpha()���ж����ַ��Ƿ�Ϊ��ĸ
            word1=str[i++];
            while(isalpha(str[i])||isdigit(str[i])){   // �жϱ�ʶ�������Ƿ�Ϸ�����ĸ��ͷ+��ĸ/���֣�
                word1+=str[i++];
            }

            it=word.find(word1);
            // �жϸñ�ʶ�ǲ��ǻ����֣������ǻ������������ʶ����Ԫ��
            if(it!=word.end()){
                outfile<<"("<<word[word1]<<","<<word1<<")"<<endl;
            }else{
                // ����ֱ������ַ�
                outfile<<"(ident"<<","<<word1<<")"<<endl;
            }
            i--;
        }

        // ʶ������
        else if(isdigit(str[i])){// �ж��ǲ��ǳ��������ÿ⺯��isdigit()
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
        
        // �������Ļ��������ν����ж�
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

// �����ַ�sym���ƶ�
int advance(){
    int found1,found2;
    if(!getline(infile,str)){
        return 0;
    }
    found1=str.find(',',0);
    if(found1 == -1){
        conterr++;
        cout<<"�����﷨���� ʶ���ַ�����"<<endl;
        return -1;
    }
    found2=str.length();
    sym=str.substr(1,found1-1);
    sym2=str.substr(found1+1,found2-found1-2);
    return 1;
}

// �����±�����t1,t2...��Ϊ��Ԫʽ��Ƿ���
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

// ������Ԫʽ������ʾ
void emit(string op,string arg1,string arg2,string result){
    quad[count1].op=op;
    quad[count1].arg1=arg1;
    quad[count1].arg2=arg2;
    quad[count1].result=result;
    count1++;
    return;
}

// ���ʽ�ĵݹ��½���������
string expressionAnalysis(){
    string op,arg1,arg2,result;
    if(conterr){
        return NULL;
	}

    // ͨ��������õ���һ��������ֵ
	arg1=termAnalysis();
	if(conterr){
        return NULL;
	}
    // +/-����
	while((sym=="plus")||(sym=="minus")){
        op=sym2;
		flag=advance();
		if(conterr){
            return NULL;
		}
        // sym�д���
		if(flag==0){
            cout<<"�����﷨���� <�ӷ������>��ȱ��"<<endl;
            conterr++;
			return NULL;
		}
		arg2=termAnalysis();// ͨ��������õ��ڶ���������ֵ
		if(conterr){
            return NULL;
		}
		result=newtemp();// �����м���������൱�ڶԽ�����д洢
		emit(op,arg1,arg2,result);// ������Ԫʽ���൱�ڽ��мӷ���������㣬�ó����
		arg1=result;// ����õ��Ľ������һ������
	}
	return arg1;// ���ر��ʽ���յõ���ֵ
}

// ��ĵݹ��½���������
string termAnalysis(){
    string op,arg1,arg2,result;
    arg1=factorAnalysis();// ͨ�����ӷ����õ���һ��������ֵ
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
			cout<<"�����﷨���� <�˷������>��ȱ����"<<endl;
			return NULL;
		}
		if(conterr){
            return NULL;
		}
		arg2=factorAnalysis();// ͨ�����ӷ����õ��ڶ���������ֵ
		if(conterr){
            return NULL;
		}
		result=newtemp();// �����м���������൱�ڶԽ�����д洢
		emit(op,arg1,arg2,result);// ������Ԫʽ���൱�ڽ��г˷���������㣬�ó����
		arg1=result;// ����õ��Ľ��
	}
	return arg1;// ���������յõ���ֵ
}

string factorAnalysis(){
    string arg;
    if(sym=="ident"){// ����Ǳ�ʶ�������շ��ر�ʶ���ķ���
        arg=sym2;
        flag=advance();
        if(conterr){
            return NULL;
		}
		if(lpnum==0&&sym=="rparen"){
            conterr++;
			cout<<"�����﷨���� ')'��ƥ��"<<endl;
			return NULL;
        }
    }
    else if(sym=="number"){// ������޷������������շ�����Ӧ������
        arg=sym2;
        flag=advance();
        if(conterr){
            return NULL;
		}
		if(lpnum==0&&sym=="rparen"){
            conterr++;
			cout<<"�����﷨���� ')'��ƥ��"<<endl;
			return NULL;
        }
    }
    else if(sym=="lparen"){// ����������ţ���Ҫ����������ƥ�䣬���ж��м��ǲ��Ǳ��ʽ�����ó����ʽ��ֵ���з���
        lpnum++;
        flag=advance();
        if(conterr){
            return NULL;
		}
		if(flag==0){
			conterr++;
			cout<<"�����﷨���� '('��ȱ�ٱ��ʽ"<<endl;
			return NULL;
		}
        arg=expressionAnalysis();
        if(conterr){
            return NULL;
		}
        if(flag==0||sym!="rparen"){
			conterr++;
			cout<<"�����﷨���� ���ʽ����ȱ��')'"<<endl;
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
		cout<<"�����﷨���� �����ײ���Ϊ<��ʶ��>|<�޷�������>|'('"<<endl;
		conterr++;
		return " ";
	}
	return arg;
}

int main(){
    int i=0,num,result;
    // ��ʼ�ʷ�����
    map_init();

    cout<<"~~�������ʽ��"<<endl;
    lexanalysis();
    cout<<endl;
   
    // ��ʼ�﷨��������� 
    cout<<endl<<"~~�������ʽ���м�����ʾ:"<<endl;

    flag=advance();
    
    if(flag){
        expressionAnalysis();// ��ʼ���б��ʽ����
    }
    if(flag!=-1&&!conterr){// �����ʽ��ȷ��������ʽ���м�����ʾ
        for(int i=0;i<count1;i++){
            cout<<'('<<quad[i].op<<','<<quad[i].arg1<<','<<quad[i].arg2<<','<<quad[i].result<<')'<<endl;;
        }
    }

    infile.close();
    return 0;
}