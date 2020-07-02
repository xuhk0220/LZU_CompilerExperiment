/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * 实验二 标识符的识别
 * 任务介绍:
 * 根据给定源语言的构词规律，从任意字符串中识别出所有的合法标识符
 * 输入：字符串
 * 输出：单词符号流，一行一个单词
 * 题目：设计一个程序，从任意字符串中识别出所有可视为C语言“名字”的子串。注意：
 *       1.构词结构：以字母打头，后跟任意多个字母、数字的单词；长度 不超过15；不区分大小写；把下划线视为第27个字母
 *       2.关键字保留，即：语言定义中保留了某些单词用作关键字，程序员不可以将这些单词用作“名字”（变量名、常量名、函数名、标记号等等）
 */

/**
 * 思路：
 * 1.判断字符串s第一个字符是否为字母，若是则存入数组ss中，若不是则下一个字符作为第一个字符循环操作1;
 * 2.判断字符串下一个字符，是否为a-z或A-Z或0-9或_，若是则继续存入数组中，
 *   若不是则将数组内容与32个关键字进行比较，若不是关键字则输出数组，否则返回到操作1;
 * 3.以上输出控制输出长度，若长度超15则返回到操作1;
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char str[100];
    printf("Please input the str what you want to indentify:");
    gets(str); //输入字符串
    identify(str);  //识别“名字”
    return 0;
}

void identify(char* s){

    //32个关键字
    char keyword[32][10]= {"void","char","int","float","double","short","long","signed","unsigned","struct",
                       "union","enum","typedef","sizeof","auto","static","register","extern","const","volatile",
                       "return","continue","break","goto","if","else","switch","case","default","for","do","while"
                      };

    int s_len = strlen(s); //字符串长度
    char *ss =(char *) malloc(16);
    int ss_len = 0; //暂存数组长度
    int i = 0,j,k,tag = 0; //i、j用于循环，k、tag用来判断识别的是否为关键字


    //遍历整个字符串
    for(i = 0; i < s_len; i++){
        //判断第一个字符
        if( (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') ){
            ss[0] = s[i];        
            ss_len = ss_len + 1;

            //判断后续字符
            for(j = i + 1; j < s_len; j++){
                //控制输出字符数组的长度
                if(ss_len == 15){
                    j = j - 1; 
                    break; // 跳出循环，直接到输出
                }
                else{
                    if( (s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= 'a' && s[j] <= 'z') || (s[j] >= '0' && s[j] <= '9') || s[j] == '_' ){
                        ss[ss_len] = s[j];
                        ss_len = ss_len + 1;
                    }
                    else {
                        break;
                    }
                } 
            }
            //判断是否为关键字
            for(k = 0; k < 32; k++){
                if( strcmp(ss,keyword[k]) == 0){
                    tag = 1;
                    break;
                }
            }
            if(tag == 0){
                ss[ss_len] = '\0';
                printf("%s",ss);
                printf("\n"); 
            }

            i = j;
            memset(ss, 0, 16); //清空暂存数组
            ss_len = 0;
        }   
        
    }
}


