/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * 实验四 对算术表达式的递归下降分析
 * 任务介绍:
 * 根据给定的上下文无关文法，分析任意一个算术表达式的语法结构
 * 输入：任意的算术表达式
 * 输出：如果判断是正确，则输出表达式的后序遍历，否则输出错误。
 * 题目：设计一个程序，根据给定的上下文无关文法，能够判断。
 *       1.基础文法：Expr -> Term Expr1
 *                   Expr1 -> AddOp Term Expr1 | empty
 *                   Term -> Factor Term1
 *                   Term1 -> MulOp Factor Term1 | empty
 *                   Factor -> id | number | (Expr)
 *                   AddOp -> + | -
 *                   MulOp -> * | / 
 *       2.语法分析方式采用递归子程序
 *       3.输入：形如 a+b*2/4-(b+c)*3 的算术表达式，有+、-、*、/四种运算符，
 *               运算符的优先级、结合规则和括号的用法遵循惯例，有变量、整数
 *               两种运算对象。为简化问题，变量和整数均为只含有1个字符的单词，
 *               忽略空格等非必要的字符。
 *       4.输出：输入正确时，输出表达式的后序遍历；输入错误时，输出error
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

//先序建立二叉树
BiTNode* afaToBtree(char *afa,int ss,int e){
	//如果只有一个数那就是叶子结点了
	if(e - ss == 1){    
		BiTNode* bn = (struct BiTNode*)malloc( sizeof(struct BiTNode) );
		bn -> Data = afa[ss];
		bn -> LChild = NULL;
		bn -> RChild = NULL;
        return bn;
	}
	/*
	   local_r记录当前要转化的表达式生成二叉树的根节点操作符的位置
	   flag记录是否当前搜索在括号里面
       m_m_p记录当前表达式中括号外面最右边的+、-位置
	   a_s_p记录当前表达式中括号外面最右边的*、/位置
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
         //如果式子整个有括号如(5-2*3+7)，即括号外面没有操作符，则去掉括号找二叉树
         afaToBtree(afa,ss + 1,e - 1);
    }
	else{
		//如果有+或者-，则根节点为最右边的+或-，否则是最右边的*或/
        if(a_s_p > 0){
            local_r = a_s_p;
        }
		else if(m_m_p > 0){
            local_r = m_m_p;
        }

		//确定根节点和根节点的左孩子和右孩子
		BiTNode* b = (struct BiTNode*)malloc( sizeof(struct BiTNode) );
		b -> Data = afa[local_r];
        b -> LChild = afaToBtree(afa,ss,local_r);
		b -> RChild = afaToBtree(afa,local_r + 1,e);
	    return b;
	}
}



//后序遍历输出
void PostOrder(BiTNode* bt){
    if(bt != NULL){
         PostOrder(bt -> LChild);
         PostOrder(bt -> RChild);
         printf("%c\n",bt -> Data);   
    }
}

//文法
void error(){
    printf("Error！\n");
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

    printf("Please input(以#结尾):");
    scanf("%s",&s);

    BiTNode* bt;
    i = 0;
    Expr();
    
    //合法后的创建语法分析树并后序输出
    if(s[i] == '#'){
        printf("语句合法!\n");
        bt = afaToBtree(s,0,strlen(s) - 1);
        printf("后序遍历为:\n");
        PostOrder(bt);
    }
    else{
        error();
    } 
    return 0;
}






