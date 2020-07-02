/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * ʵ��� ��ʶ����ʶ��
 * �������:
 * ���ݸ���Դ���ԵĹ��ʹ��ɣ��������ַ�����ʶ������еĺϷ���ʶ��
 * ���룺�ַ���
 * ��������ʷ�������һ��һ������
 * ��Ŀ�����һ�����򣬴������ַ�����ʶ������п���ΪC���ԡ����֡����Ӵ���ע�⣺
 *       1.���ʽṹ������ĸ��ͷ�������������ĸ�����ֵĵ��ʣ����� ������15�������ִ�Сд�����»�����Ϊ��27����ĸ
 *       2.�ؼ��ֱ������������Զ����б�����ĳЩ���������ؼ��֣�����Ա�����Խ���Щ�������������֡�����������������������������Ǻŵȵȣ�
 */

/**
 * ˼·��
 * 1.�ж��ַ���s��һ���ַ��Ƿ�Ϊ��ĸ���������������ss�У�����������һ���ַ���Ϊ��һ���ַ�ѭ������1;
 * 2.�ж��ַ�����һ���ַ����Ƿ�Ϊa-z��A-Z��0-9��_��������������������У�
 *   ������������������32���ؼ��ֽ��бȽϣ������ǹؼ�����������飬���򷵻ص�����1;
 * 3.�����������������ȣ������ȳ�15�򷵻ص�����1;
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char str[100];
    printf("Please input the str what you want to indentify:");
    gets(str); //�����ַ���
    identify(str);  //ʶ�����֡�
    return 0;
}

void identify(char* s){

    //32���ؼ���
    char keyword[32][10]= {"void","char","int","float","double","short","long","signed","unsigned","struct",
                       "union","enum","typedef","sizeof","auto","static","register","extern","const","volatile",
                       "return","continue","break","goto","if","else","switch","case","default","for","do","while"
                      };

    int s_len = strlen(s); //�ַ�������
    char *ss =(char *) malloc(16);
    int ss_len = 0; //�ݴ����鳤��
    int i = 0,j,k,tag = 0; //i��j����ѭ����k��tag�����ж�ʶ����Ƿ�Ϊ�ؼ���


    //���������ַ���
    for(i = 0; i < s_len; i++){
        //�жϵ�һ���ַ�
        if( (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') ){
            ss[0] = s[i];        
            ss_len = ss_len + 1;

            //�жϺ����ַ�
            for(j = i + 1; j < s_len; j++){
                //��������ַ�����ĳ���
                if(ss_len == 15){
                    j = j - 1; 
                    break; // ����ѭ����ֱ�ӵ����
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
            //�ж��Ƿ�Ϊ�ؼ���
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
            memset(ss, 0, 16); //����ݴ�����
            ss_len = 0;
        }   
        
    }
}


