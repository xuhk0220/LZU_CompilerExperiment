/**
 * 2020.7.2
 * LZU xuhk
 * 
 * 
 * ʵ��һ Ԥ����
 * �������:
 * �ڴʷ�����֮ǰ���Գ���Ա�ύ��Դ�������Ԥ�����޳�ע�͵Ȳ���Ҫ���ַ����Լ򻯴ʷ�����
 * ���룺�ַ�����ʽ��Դ����
 * ���������֮����ַ�����ʽ��Դ����
 * ��Ŀ�����һ�����򣬴������ַ������޳�C������ʽ��ע�ͣ�����
 *       1.���磺//...�ĵ���ע�ͣ�
 *       2.���磺/*...*`/�Ķ���ע��
 */

/**
 * ˼·��
 * 1.����������״̬��������ע��//��ע��/**-/
 * 2.��ǰĬ��Ϊ��������״̬��ֱ������/������ж���һ�ַ�
 * 2.ע��//�����н�����ע��/**-/��*-/����
 */

#include <stdio.h>
int main(){
    char name[256];
    printf("please input the filename:");
    gets(name); //�����ļ���
    scan(name); //ʶ���ע������벿��
    return 0;
}

void scan(char *filename){  
    int tag = 0;
    //tag = 0:��ʾ��ȡ���ַ�Ϊ���룻
    //tag = 1:��ʾ��ȡ���ַ�Ϊ//ע��
    //tag = 2:��ʾ��ȡ���ַ�Ϊ/**/ע��

    char temp1, temp2; //���ڶ��룬���
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

        //��ǰΪ������������
        if(tag == 0){
            if(temp1 == '/'){
                temp2 = fgetc(in);

                //��һ���벿��Ϊ/**/ע��
                if(temp2 == '*'){
                    tag = 2;
	            }
                //��һ���벿��Ϊ//ע��
                else if(temp2 == '/'){
                    tag = 1;
                }
                //����Ϊ��������״̬ʱ����һ�������
                else{
                    fputc(temp1, out);
                    fputc(temp2, out);
                }
            }
            else{
                fputc(temp1, out);
            }
        }
        //��ǰΪ����//ע�ͣ�ֱ�����뻻�б�ʾע�ͽ��������¿�ʼ����������
        else if(tag == 1){
            if(temp1 == '\n'){
                tag = 0;
                fputc(temp1,out);
            }
        }
        //��ǰΪ����/**/ע�ͣ�ֱ������*/��ʾע�ͽ��������¿�ʼ����������
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

