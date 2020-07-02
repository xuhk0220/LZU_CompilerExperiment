/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * 实验一 预处理
 * 任务介绍:
 * 在词法分析之前，对程序员提交的源程序进行预处理，剔除注释等不必要的字符，以简化词法分析
 * 输入：字符串形式的源程序
 * 输出：处理之后的字符串形式的源程序
 * 题目：设计一个程序，从任意字符串中剔除C语言形式的注释，包括
 *       1.形如：//...的单行注释；
 *       2.形如：/*...*`/的多行注释
 */

/**
 * 思路：
 * 1.代码有三种状态：正常、注释//、注释/**-/
 * 2.当前默认为正常代码状态，直到读入/后进行判断下一字符
 * 2.注释//到换行结束，注释/**-/到*-/结束
 */

#include <stdio.h>
int main(){
    char name[256];
    printf("please input the filename:");
    gets(name); //输入文件名
    scan(name); //识别除注释外代码部分
    return 0;
}

void scan(char *filename){  
    int tag = 0;
    //tag = 0:表示读取的字符为代码；
    //tag = 1:表示读取的字符为//注释
    //tag = 2:表示读取的字符为/**/注释

    char temp1, temp2; //用于读入，输出
    FILE *in, *out;

    in = fopen(filename,"r");
    if(in == NULL){
        printf("cannot open the input file!");
    }
    
    out = fopen("out.txt", "w");
    if(out == NULL){
        printf("cannot open the output file!");
    }
    
    
    temp1 = fgetc(in);
    while(temp1 != EOF){

        //当前为读入正常代码
        if(tag == 0){
            if(temp1 == '/'){
                temp2 = fgetc(in);

                //下一读入部分为/**/注释
                if(temp2 == '*'){
                    tag = 2;
	            }
                //下一读入部分为//注释
                else if(temp2 == '/'){
                    tag = 1;
                }
                //读入为正常代码状态时，则一个个输出
                else{
                    fputc(temp1, out);
                    fputc(temp2, out);
                }
            }
            else{
                fputc(temp1, out);
            }
        }
        //当前为读入//注释，直到读入换行表示注释结束，重新开始读正常代码
        else if(tag == 1){
            if(temp1 == '\n'){
                tag = 0;
                fputc(temp1,out);
            }
        }
        //当前为读入/**/注释，直到读入*/表示注释结束，重新开始读正常代码
        else{
            if(temp1 == '*'){
                temp2 = fgetc(in);
                if(temp2 == '/'){
                     tag = 0;
                }
            }
        }   
        temp1 = fgetc(in);
    }
    fclose(in);
    fclose(out);
}

