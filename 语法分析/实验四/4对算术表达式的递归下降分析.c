/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * ʵ���� ���������ʽ�ĵݹ��½�����
 * �������:
 * ���ݸ������������޹��ķ�����������һ���������ʽ���﷨�ṹ
 * ���룺������������ʽ
 * ���������ж�����ȷ����������ʽ�ĺ�������������������
 * ��Ŀ�����һ�����򣬸��ݸ������������޹��ķ����ܹ��жϡ�
 *       1.�����ķ���Expr -> Term Expr1
 *                   Expr1 -> AddOp Term Expr1 | empty
 *                   Term -> Factor Term1
 *                   Term1 -> MulOp Factor Term1 | empty
 *                   Factor -> id | number | (Expr)
 *                   AddOp -> + | -
 *                   MulOp -> * | / 
 *       2.�﷨������ʽ���õݹ��ӳ���
 *       3.���룺���� a+b*2/4-(b+c)*3 ���������ʽ����+��-��*��/�����������
 *               ����������ȼ�����Ϲ�������ŵ��÷���ѭ�������б���������
 *               �����������Ϊ�����⣬������������Ϊֻ����1���ַ��ĵ��ʣ�
 *               ���Կո�ȷǱ�Ҫ���ַ���
 *       4.�����������ȷʱ��������ʽ�ĺ���������������ʱ�����error
 *             
 */

#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 255
char s[LEN];
int i;

typedef struct BiTNode{
   char Data;
   struct BiTNode *LChild;
   struct BiTNode *RChild;
}BiTNode;

//������������
BiTNode* afaToBtree(char *afa,int ss,int e){
	//���ֻ��һ�����Ǿ���Ҷ�ӽ����
	if(e - ss == 1){    
		BiTNode* bn = (struct BiTNode*)malloc( sizeof(struct BiTNode) );
		bn -> Data = afa[ss];
		bn -> LChild = NULL;
		bn -> RChild = NULL;
        return bn;
	}
	/*
	   local_r��¼��ǰҪת���ı��ʽ���ɶ������ĸ��ڵ��������λ��
	   flag��¼�Ƿ�ǰ��������������
       m_m_p��¼��ǰ���ʽ�������������ұߵ�+��-λ��
	   a_s_p��¼��ǰ���ʽ�������������ұߵ�*��/λ��
	*/
    int local_r = 0,flag = 0; 
	int m_m_p = 0,a_s_p = 0;
	for(int i = ss; i < e; i++){
        if(afa[i] == '('){
            flag ++;
        }
		else if(afa[i] == ')'){
            flag --;
        }
		if(flag == 0){
			if(afa[i] == '*' || afa[i] =='/'){
                m_m_p=i;
            }
			else if(afa[i] =='+'||afa[i] =='-'){
                a_s_p = i;
            }
		}
	}
	if( (m_m_p == 0) && (a_s_p == 0) ){
         //���ʽ��������������(5-2*3+7)������������û�в���������ȥ�������Ҷ�����
         afaToBtree(afa,ss + 1,e - 1);
    }
	else{
		//�����+����-������ڵ�Ϊ���ұߵ�+��-�����������ұߵ�*��/
        if(a_s_p > 0){
            local_r = a_s_p;
        }
		else if(m_m_p > 0){
            local_r = m_m_p;
        }

		//ȷ�����ڵ�͸��ڵ�����Ӻ��Һ���
		BiTNode* b = (struct BiTNode*)malloc( sizeof(struct BiTNode) );
		b -> Data = afa[local_r];
        b -> LChild = afaToBtree(afa,ss,local_r);
		b -> RChild = afaToBtree(afa,local_r + 1,e);
	    return b;
	}
}



//����������
void PostOrder(BiTNode* bt){
    if(bt != NULL){
         PostOrder(bt -> LChild);
         PostOrder(bt -> RChild);
         printf("%c\n",bt -> Data);   
    }
}

//�ķ�
void error(){
    printf("Error��\n");
    exit(0);
}

void Expr(){
    
    Term();
    Expr1();
}

void Expr1(){
    
    if(s[i] == '+' || s[i] == '-'){
        AddOp();
        Term();
        Expr1();
    }
    else if( (s[i] != ')') && (s[i] != '#') ){
        error();
    }
}

void Term(){
    
    Factor();
    Term1();
}

void Term1(){
    
    if( (s[i] == '/') || (s[i] == '*') || (s[i] >= '0' && s[i] <= '9') || ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) ){
        MulOp();
        Factor();
        Term1();
    }
    else if( ( (s[i] != '+') && (s[i] != '-') && (s[i] != ')') ) && (s[i] != '#') ){
        error();
    }
    
}

void Factor(){
    if(s[i] >= '0' && s[i] <= '9'){
        i++;
    }
    else if( (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') ){
        i++;
    }
    else if(s[i] == '('){
        i++;
        Expr();
        if(s[i] == ')'){
            i++;
        } 
    }  
    else{
        error();
    }
}

void AddOp(){
    if(s[i] == '+'){
        i++;
        
    }
    else if(s[i] == '-'){
        i++;
    }
}

void MulOp(){
    if(s[i] == '*'){
        i++;
    }
    else if(s[i] == '/'){
        i++;
    }

}

int main(){

    printf("Please input(��#��β):");
    scanf("%s",&s);

    BiTNode* bt;
    i = 0;
    Expr();
    
    //�Ϸ���Ĵ����﷨���������������
    if(s[i] == '#'){
        printf("���Ϸ�!\n");
        bt = afaToBtree(s,0,strlen(s) - 1);
        printf("�������Ϊ:\n");
        PostOrder(bt);
    }
    else{
        error();
    } 
    return 0;
}






