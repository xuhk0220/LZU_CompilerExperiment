/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * ʵ���� 
 * �������:
 * ���ݸ���Դ���ԵĹ��ʹ��ɣ��������ַ�����ʶ������еĺϷ����ʷ��ţ����Եȳ��Ķ�Ԫ����ʽ���
 * ���룺�ַ�����ʽ��Դ����
 * ��������ʷ��������ɵĴ�(��)�������Եȳ��Ķ�Ԫ����ʽ����
 * ��Ŀ�����һ�����򣬸��ݸ���Դ���ԵĹ��ʹ��򣬴������ַ�����ʶ������������еĺϷ��ĵ��ʷ��ţ����Եȳ��Ķ�Ԫ����ʽ�����ע�⣺
 *       1.��¼�н�����һ������C�﷨������Ƶ�Դ����LittleC��һ������Palcal�﷨������Ƶ�Դ����LittleP,������Ϊ�ο�
 *       2.ѧ������������ѡ���������Դ���ԣ��Դ�Ϊ��������ɱ�ʵ��ͺ���ʵ�顣�����Ե�ʵ��Ӧ�����㸽¼B��Ҫ��
 *       3.�ó���Ӧ�����Ϊ����2��ģ�飺����ģ��͹���ģ�顣����ģ������˳������ںͳ��ڣ���Ҫ�������롢���������������ģ�飻
 *         ����ģ�鸺�����ķָʶ�𡢹���ȹ������������ĺô���ֻҪģ���Ľӿ�(��Ҫ������Щ���ݣ����ݵĽṹ)��ƺ���
 *         ����ʵ�������﷨������ʱ�Ϳ���ֱ�ӵ��ô˴��Ĺ���ģ�飬������Ҫ�Ķ�̫�����
 */

/**
 * ˼·��
 * 1.��Ϊ5����𣺹ؼ��֡���ʶ�����ָ����������������
 * 2.�����ַ�����ʽ��Դ����󣬰���ÿ���ַ��ж��Ƿ�Ϊ�ָ����������������
 * 3.���ڹؼ��ֺͱ�ʶ������ʶ��Ϊ�Ϸ��ַ��������ж��Ƿ�Ϊ�ؼ��֣���������Ϊ��ʶ��
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//32���ؼ���
char keyword[32][10] = {"void","char","int","float","double","short","long","signed","unsigned","struct",
                       "union","enum","typedef","sizeof","auto","static","register","extern","const","volatile",
                       "return","continue","break","goto","if","else","switch","case","default","for","do","while"
                      };
//�����
char countChar[8] = {'+','-','/','%','*','=','>','<'};
//�ָ��� 
char splitChar[7] = {',',';','"','(',')','}','{'}; 
char name[5][10] = {"�ؼ���","��ʶ��","�ָ���","�����","����",};

int main(){

    char str[100];
    while(gets(str)){
        for(int i = 0; i < strlen(str); i++){
            i = MatchString(str,i); 
            i = MatchNumber(str,i);
            i = MatchCountChar(str,i);
            i = MatchSplitChar(str,i);
        }
    }
    return 0;
}


//�ж�һ���Ϸ��ַ����Ƿ�Ϊ���ؼ��֡�
int matchKeyword(char *str){
    for(int i = 0; i < 32; i++){
        //�ǹؼ���
        if( !strcmp(str,keyword[i]) ){
            return 0;
        } 
    }
    return 1;
}

//�ж��Ƿ�Ϊ����ʶ��������ʶ��Ϊ�Ϸ��ַ��������ų����ؼ��֡��Ŀ���
int MatchString(char *str,int i){
    char str1[30];
    for(int m = 0; m < strlen(str1); m++){
        str1[m] = '\0';
    }

    int j = 0;
    //�ж�����ĸ�Ƿ����
    if( (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i]=='_' ){
        str1[j] = str[i];
        //�жϺ����ַ�
        while( (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_' || (str[i] >= '0' && str[i]<='9' ) ){
            str1[j++] = str[i++];
        } 
        str1[j] = '\0';
    }

    //�ǹؼ���
    if(j && matchKeyword(str1)){
        printf("%s  %s\n",name[1],str1);
    }
    //�Ǳ�ʶ��
    else if(j && !matchKeyword(str1)){
        printf("%s  %s\n",name[0],str1);
    }
    return i;
}

//�ж��Ƿ�Ϊ���������
int MatchCountChar(char *str,int i){
    for(int j = 0; j < 8; j++){
        if(str[i] == countChar[j]){
            printf("%s  %c\n",name[3],str[i]);
        }
    }
    return i;
}

//�ж��Ƿ�Ϊ���ָ�����
int MatchSplitChar(char *str,int i){
    for(int j = 0; j < 7; j++) {
        if(str[i] == splitChar[j]){
            printf("%s  %c\n",name[2],str[i]);
        }
    }
    return i;
}

//�ж��Ƿ�Ϊ��������
int MatchNumber(char *str,int i){
    char str1[30];
    for(int m = 0; m < 30; m++){
        str1[m] = '\0';
    }

    int j = 0;
    if(str[i] >= '0' && str[i] <= '9'){
        str1[j] = str[i];
    while(str[i] >= '0' && str[i] <= '9'){
        str1[j++] = str[i++];
    }
    str1[j] = '\0';
    printf("%s  %s\n",name[4],str1);
    }
    return i;
}



