/**
 * 2020.7.2
 * LZU xuhk
 * 
 *  
 * 实验八 对多条执行语句构造递归下降翻译器
 * 实验九 构造能处理完整程序的递归下降翻译器
 * 
 * 任务介绍:
 * 对递归下降分析器技能型改造，使其能够一遍处理，同时完成语法分析和中间代码翻译
 * 输入：一个完整的源程序
 * 输出：与输入对应的一个四元式序列
 * 题目：对实验六程序进行升级改造，使得程序对于输入的一个完成的源程序，在对其做递归下降分析的同时，生成等价的四元式序列，一遍完成。
 * 
 */

#include <stack>
#include <deque>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <string>
using namespace std;

/* 0    <constant>     ID_CONSTANT
 * 1    main           ID_MAIN
 * 2    int            ID_INT
 * 3    if             ID_IF
 * 4    else           ID_ELSE
 * 5    while          ID_WHILE
 * 6    do             ID_HHHH
 * 7    +              ID_PLUS
 * 8    -              ID_MINUS
 * 9    *              ID_MUL
 * 10   /              ID_DIV
 * 11   <              ID_LT
 * 12   >              ID_GT
 * 13   !=             ID_NE
 * 14   >=             ID_GE
 * 15   <=             ID_LE
 * 16   ==             ID_EQ
 * 17   ,              ID_COMMA
 * 18   ;              ID_SEMICOLON
 * 19   (              ID_LPAREN
 * 20   )              ID_RPAREN
 * 21   {              ID_LBRACE
 * 22   }              ID_RBRACE
 * 23   =              ID_GIVE
 * 24   <Identifiers>  ID_IDENTIFIER
 * */


#define ID_CONSTANT 0
#define ID_MAIN 1
#define ID_INT 2
#define ID_IF 3
#define ID_ELSE 4
#define ID_WHILE 5
#define ID_PLUS 6
#define ID_MINUS 7
#define ID_MUL 8
#define ID_DIV 9
#define ID_LT 10
#define ID_GT 11
#define ID_NE 12
#define ID_GE 13
#define ID_LE 14
#define ID_EQ 15
#define ID_COMMA 16 
#define ID_SEMICOLON 17 
#define ID_LPAREN 18
#define ID_RPAREN 19
#define ID_LBRACE 20
#define ID_RBRACE 21
#define ID_GIVE 22
#define ID_IDENTIFIER 23

// 设定5个保留字
string reserved_words[5] = {"main", "int", "if", "else", "while"};
int reserved_nums[5] = {1, 2, 3, 4, 5};
// 运算符与界符
string symbols[17] = {"+", "-", "*", "/", "<", ">", "!=", ">=", "<=", "==", ",", ";", "(", ")", "{", "}", "="};
int symbol_nums[17] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
// 自定义关键词，用于判断单词类型
string error_check[24] = {"<Constant>", "main", "int", "if", "else", "while", "+", "-", "*", "/", "<", ">", "!=", ">=", "<=", "==", ",", ";", "(", ")", "{", "}", "=", "<Identifier>"};

// 单词类
struct word {
    string value;
    int type;        // type有三种：标识符1，数字2，运算符界符3
    int line;
};

word next_word;

//四元式
struct quaternary {
    string op;
    string arg1;
    string arg2;
    string res;
}qua[100];

int qua_num = 0;
string letters[5000];
int cur;                // 当前指针位置
int cur_line = 1;       // 当前所在行数
int file_length;        // 文件长度（总行数）
string cur_word;        // 当前单词
int lookahead_type;     // 当前单词的类型

string var[200];
int var_num = 0;
int tmpvar_num = 0;
int follow_loop = 0;
bool have_loop = false;
stack<int> condition_stack;
stack<int> else_stack;
stack<int> loop_stack;
stack<int> loop_start_stack;
bool have_error = false;


/************************* 词法分析 *************************/

// 判断是否为保留字
int is_reserved(string s) {
    int i;
    for (i = 0; i < 5; i++) {
        if (s == reserved_words[i])
            return reserved_nums[i];
    }
    return 0;
}

// 判断是否为运算符或界符
int is_symbol(string s) {
    int i;
    for (i = 0; i < 17; i++) {
        if (s == symbols[i])
            return symbol_nums[i];
    }
    return 0;
}

// 判断数字
bool is_digit(string s) {
    if ("0" <= s && s <= "9")
        return true;
    return false;
}

// 判断是否是标识符
bool is_letter(string s) {
    if ("a" <= s && s <= "z")
        return true;
    return false;
}

// 判断字符类型：标识符为1；数字为2；运算符或界符为3；
int char_type(string s) {
    if (is_letter(s))
        return 1;
    if (is_digit(s))
        return 2;
    if (s == ">" || s == "=" || s == "<" || s == "!" || 
        s == "," || s == ";" || s == "(" || s == ")" || 
        s == "{" || s == "}" || s == "+" || s == "-" || 
        s == "*" || s == "/")
        return 3;
    return 0;
}

// 取连续标识符作为一个整体标识符
string concat_reserved(string s, int n) {
    int i = n + 1;
    int flag = 1;

    while (flag) {
        if (is_digit(letters[i]) || is_letter(letters[i])) {
            s = (s + letters[i]).c_str();
            if (is_reserved(s)) {
                i++;
                cur = i;
                return s;
            }
            i++;
        }else{
            flag = 0;
        }
    }
    cur = i;
    return s;
}

// 取连续运算符作为一个整体运算符
string concat_symbol(string s, int n) {
    int i = n + 1;
    string ss = letters[i];
    if (ss == ">" || ss == "=" || ss == "<" || ss == "!") {
        s = (s + letters[i]).c_str();
        i++;
    }
    cur = i;
    return s;
}

// 取连续数字作为一个整体数字
string concat_digit(string s, int n) {
    int i = n + 1;
    int flag = 1;

    while (flag) {
        if (is_digit(letters[i])) {
            s = (s + letters[i]).c_str();
            i++;
        }else{
            flag = 0;
        }
    }
    cur = i;
    return s;
}

// 取当前未识别的连续词，并确认该词的类型（标识符/数字/运算符界符）
void take_word() {
    int ty;
    if (cur < file_length) {
        string s1, s2;
        s1 = letters[cur];

        // 排除空格、换行、制表符的影响
        while (s1 == " " || s1 == "\n" || s1 == "\t") {
            if (s1 == "\n") {
                cur_line++;
            }
            if (cur + 1 >= file_length)
                return;
            s1 = letters[++cur];
        }

        // 针对当前字符的类型，进行连续取同类型字符成一个整体词
        ty = char_type(s1);
        switch (ty) {

            // 若为标识符
            case 1:
                s2 = concat_reserved(s1, cur);
                next_word.value = s2;
                next_word.line = cur_line;
                if (is_reserved(s2)) {
                    next_word.type = is_reserved(s2);
                }else{
                    if (next_word.type == ID_CONSTANT) {
                        cout << "******************** 错误 *******************" << endl;
                        cout << "错误行 " << cur_line << ": " << endl;
                        cout << "数字不能作为标识符开头!" << endl;
                        have_error = true;
                        cout << "*********************************************" << endl;
                    }
                    next_word.type = 23;
                }
                break;
            
            // 若为数字
            case 2:
                s2 = concat_digit(s1, cur);
                next_word.value = s2;
                next_word.line = cur_line;
                next_word.type = 0;
                break;
            case 3:
                s2 = concat_symbol(s1, cur);
                next_word.value = s2;
                next_word.line = cur_line;
                next_word.type = is_symbol(s2);
                break;
            default:
                // exit(0);
                have_error = true;
                break;
            
            // 对于运算符，合法情况下最多也就两个组合，故在下列具体函数中进行讨论
        }
    }
}


/************************* 语法分析 *************************/

// 匹配自定义关键词
void match_word(int expected_type) {
    if (lookahead_type != expected_type) {
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << cur_line << ": " << endl;
        cout << "语法错误! 应为 ";
        if (expected_type == 0 || expected_type == 23) {
            cout << error_check[expected_type] << " \"" << next_word.value << "\"";
        }else{
            cout << "\"" << error_check[expected_type] << "\"";
        }
        cout << ", 实际为 \"" << next_word.value << "\"." << endl;
        // exit(0);
        have_error = true;
        cout << "******************** 错误 *******************" << endl;
        take_word();
        lookahead_type = next_word.type;
    }else {
        take_word();
        lookahead_type = next_word.type;
    }
}

bool is_exist_var(string query) {
    int i;
    for (i = 0; i < var_num; i++) {
        if (var[i] == query)
            return true;
    }
    return false;
}

void parse_IDENTIFIER();
void parse_RESERVED();
void parse_LOOP_SEN();
void parse_CONDITION_SEN();
void parse_SEN_ONE();
void parse_MULTISEN();
string parse_FACTOR();
string parse_ITEM1(string arg1);
string parse_ITEM();
string parse_EXPRESSION1(string arg1);
string parse_EXPRESSION();
int parse_RELATION_OPT();
pair<int, pair<string, string>> parse_CONDITION();
void parse_ASSIGN_SEN();
void parse_SEN();
void parse_SEN_PART1();
void parse_SEN_PART();
void parse_IDENTIFIER_TAB1();
void parse_IDENTIFIER_TAB();
void parse_VAR_DECLARATION();
void parse_VAR_DECLARATION_PART();
void parse_MAIN_FUNC();
void parse_PROGRAM();


void parse_RESERVED() {
    if (lookahead_type == ID_MAIN || 
        lookahead_type == ID_IF || 
        lookahead_type == ID_ELSE || 
        lookahead_type == ID_WHILE || 
        lookahead_type == ID_INT) {
        take_word();
        lookahead_type = next_word.type;
    }
}

// 解析标识符（作为表达式左边，最先被识别）
void parse_IDENTIFIER() {
    if (lookahead_type == ID_IDENTIFIER) {
        var[var_num++] = next_word.value;
        take_word();
        lookahead_type = next_word.type;
    }else{
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << next_word.line << ": " << endl;
        cout << "错误标识符 " << next_word.value << endl;
        // exit(0);
        have_error = true;
        cout << "*********************************************" << endl;
    }
}


// 解析while循环语句
void parse_LOOP_SEN() {

    // 识别while(...)判断部分
    match_word(ID_WHILE);
    match_word(ID_LPAREN);
    loop_start_stack.push(qua_num);
    pair<int, pair<string, string>> args = parse_CONDITION();

    // 识别判断表达式中左边第一个运算符
    switch (args.first) {
        case ID_EQ:
            qua[qua_num].op = "!=";
            break;
        case ID_NE:
            qua[qua_num].op = "==";
            break;
        case ID_GT:
            qua[qua_num].op = "<=";
            break;
        case ID_GE:
            qua[qua_num].op = "<";
            break;
        case ID_LT:
            qua[qua_num].op = ">=";
            break;
        case ID_LE:
            qua[qua_num].op = ">";
            break;
        default:
            cout << "******************** 错误 *******************" << endl;
            cout << "运算符不合法!" << endl;
            // exit(0);
            have_error = true;
            cout << "*********************************************" << endl;
            break;
    }
    qua[qua_num].arg1 = args.second.first;
    qua[qua_num].arg2 = args.second.second;
    loop_stack.push(qua_num);            // 四元式序列压栈
    qua_num++;
    match_word(ID_RPAREN);

    // 识别{...}部分
    parse_SEN_ONE();
    int loop_pos = loop_stack.top();
    loop_stack.pop();         // 回填，出栈
    qua[loop_pos].res = "(" + to_string(qua_num+2) + ")";
    qua[qua_num].op = "GOTO";
    qua[qua_num].arg1 = "";
    qua[qua_num].arg2 = "";
    int loop_start_pos = loop_start_stack.top();
    loop_start_stack.pop();
    qua[qua_num].res = "(" + to_string(loop_start_pos+1) + ")";
    qua_num++;
    qua[qua_num].op = "";
    qua[qua_num].arg1 = "";
    qua[qua_num].arg2 = "";
    qua[qua_num].res = "";
    follow_loop = qua_num;
    have_loop = true;
}

// 解析if(...)...else(...)...选择语句
void parse_CONDITION_SEN() {

    // 识别if(...)判断部分
    match_word(ID_IF);
    match_word(ID_LPAREN);
    pair<int, pair<string, string>> args = parse_CONDITION();
    // 同上
    switch (args.first) {
        case ID_EQ:
            qua[qua_num].op = "!=";
            break;
        case ID_NE:
            qua[qua_num].op = "==";
            break;
        case ID_GT:
            qua[qua_num].op = "<=";
            break;
        case ID_GE:
            qua[qua_num].op = "<";
            break;
        case ID_LT:
            qua[qua_num].op = ">=";
            break;
        case ID_LE:
            qua[qua_num].op = ">";
            break;
        default:
            cout << "******************** 错误 *******************" << endl;
            cout << "未知错误!" << endl;
            // exit(0);
            have_error = true;
            cout << "*********************************************" << endl;
            break;
    }
    qua[qua_num].arg1 = args.second.first;
    qua[qua_num].arg2 = args.second.second;
    condition_stack.push(qua_num);
    qua_num++;
    match_word(ID_RPAREN);
    parse_SEN_ONE();
    match_word(ID_SEMICOLON);
    qua[qua_num].op = "GOTO";
    qua[qua_num].arg1 = "";
    qua[qua_num].arg2 = "";
    else_stack.push(qua_num);
    qua_num++;

    // 识别else{}部分
    match_word(ID_ELSE);
    int condition_pos = condition_stack.top();
    condition_stack.pop();
    qua[condition_pos].res = "(" + to_string(qua_num+1) + ")";
    parse_SEN_ONE();
    int else_num = else_stack.top();
    else_stack.pop();
    qua[else_num].res = "(" + to_string(qua_num+1) + ")";
    have_loop = true;
    follow_loop = qua_num;
}

// 识别{...}符号
void parse_MULTISEN(){
    match_word(ID_LBRACE);
    parse_SEN_PART();
    match_word(ID_RBRACE);
}

// 作为从句，判断接下去是{}代码块还是普通标识符开头的表达式/if选择语句/while循环语句
void parse_SEN_ONE() {
    if (lookahead_type == ID_LBRACE) {
        parse_MULTISEN();
    }else if(lookahead_type == ID_IDENTIFIER || 
             lookahead_type == ID_IF || 
             lookahead_type == ID_WHILE) {
        parse_SEN();
    }else{
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << cur_line << ": " << endl;
        cout << "不是合法句!" << endl;
        // exit(0);
        have_error = true;
        cout << "*********************************************" << endl;
    }
}

// 识别表达式中的因子
string parse_FACTOR() {
    string ret;
    if (lookahead_type == ID_IDENTIFIER) {
        if (!is_exist_var(next_word.value)) {
            cout << "******************** 错误 *******************" << endl;
            cout << "错误行 " << next_word.line << ": " << endl;
            cout << "该标识符未定义" << next_word.value << endl;
            // exit(0);
            have_error = true;
            cout << "*********************************************" << endl;
        }
        ret = next_word.value;
        take_word();
        lookahead_type = next_word.type;
    }else if (lookahead_type == ID_CONSTANT) {
        ret = next_word.value;
        take_word();
        lookahead_type = next_word.type;
    }else if(lookahead_type == ID_LPAREN) {
        match_word(ID_LPAREN);
        ret = parse_EXPRESSION();
        match_word(ID_RPAREN);
    }else{
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << cur_line << ": " << endl;
        cout << "不能作为一个因子! 因子须是标识符/表达式!" << endl;
        // exit(0);
        have_error = true;
        cout << "*********************************************" << endl;
    }
    return ret;
}

// 识别表达式右边的两个因子中的第二个因子
string parse_ITEM1(string arg1) {
    string ret = arg1;
    string to_pass;
    if (lookahead_type == ID_MUL) {
        take_word();
        lookahead_type = next_word.type;
        string arg2 = parse_FACTOR();
        qua[qua_num].op = "*";
        qua[qua_num].arg1 = arg1;
        qua[qua_num].arg2 = arg2;
        string var_name = "tmp" + to_string(tmpvar_num);
        tmpvar_num++;
        qua[qua_num].res = var_name;
        ret = var_name;
        to_pass = ret;
        qua_num++;
        ret = parse_ITEM1(to_pass);
    }else if(lookahead_type == ID_DIV){
        take_word();
        lookahead_type = next_word.type;
        string arg2 = parse_FACTOR();
        qua[qua_num].op = "/";
        qua[qua_num].arg1 = arg1;
        qua[qua_num].arg2 = arg2;
        string var_name = "tmp" + to_string(tmpvar_num);
        tmpvar_num++;
        qua[qua_num].res = var_name;
        ret = var_name;
        to_pass = ret;
        qua_num++;
        ret = parse_ITEM1(to_pass);
    }else{
        // nothing to do
    }
    return ret;
}

// 识别表达式右边的两个因子
string parse_ITEM() {
    string arg1 = parse_FACTOR();     //左因
    string ret = parse_ITEM1(arg1);   //右因
    return ret;
}

// 识别判断表达式
string parse_EXPRESSION1(string arg1) {
    string ret = arg1;
    string to_pass;
    if (lookahead_type == ID_PLUS) {
        take_word();
        lookahead_type = next_word.type;
        string arg2 = parse_ITEM();
        qua[qua_num].op = "+";
        qua[qua_num].arg1 = arg1;
        qua[qua_num].arg2 = arg2;
        string var_name = "tmp" + to_string(tmpvar_num);
        tmpvar_num++;
        qua[qua_num].res = var_name;
        ret = var_name;
        to_pass = ret;
        qua_num++;
        ret = parse_EXPRESSION1(to_pass);
    }else if(lookahead_type == ID_MINUS){
        take_word();
        lookahead_type = next_word.type;
        string arg2 = parse_ITEM();
        qua[qua_num].op = "-";
        qua[qua_num].arg1 = arg1;
        qua[qua_num].arg2 = arg2;
        string var_name = "tmp" + to_string(tmpvar_num);
        tmpvar_num++;
        qua[qua_num].res = var_name;
        ret = var_name;
        to_pass = ret;
        qua_num++;
        ret = parse_EXPRESSION1(to_pass);
    }else{
        // nothing to do
    }
    return ret;
}

// 同上
string parse_EXPRESSION() {
    string arg1 = parse_ITEM();
    string ret = parse_EXPRESSION1(arg1);
    return ret;
}

// 判断复合运算符是否合法正确
int parse_RELATION_OPT() {
    int ropt_type;
    if (lookahead_type != ID_EQ && lookahead_type != ID_NE 
    &&  lookahead_type != ID_GT && lookahead_type != ID_LT
    &&  lookahead_type != ID_GE && lookahead_type != ID_LE) {
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << next_word.line << ": " << endl;
        cout << "运算符不合法!" << endl;
        // exit(0);
        have_error = true;
        cout << "*********************************************" << endl;
    }else{
        ropt_type = lookahead_type;
        take_word();
        lookahead_type = next_word.type;
    }
    return ropt_type;
}

// 整合一条四元式
pair<int, pair<string,string>> parse_CONDITION() {
    string arg1 = parse_EXPRESSION();
    int ropt_type = parse_RELATION_OPT();
    string arg2 = parse_EXPRESSION();
    pair<int, pair<string, string>> args;
    args.first = ropt_type;
    args.second.first = arg1;
    args.second.second = arg2;
    return args;
}

// 开始解析整个表达式
void parse_ASSIGN_SEN() {
    if (!is_exist_var(next_word.value)) {
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << next_word.line << ": " << endl;
        cout << "未声明变量 " << next_word.value << endl;
        // exit(0);
        have_error = true;
        cout << "*********************************************" << endl;
    }
    string temp_identifier = next_word.value;
    parse_IDENTIFIER();
    match_word(ID_GIVE);
    string to_give = parse_EXPRESSION();
    qua[qua_num].op = "=";
    qua[qua_num].arg1 = to_give;
    qua[qua_num].arg2 = "/";
    qua[qua_num].res = temp_identifier;
    qua_num++;
}

// 对于一个语句的结束，后面应该为选择语句/循环语句/普通表达式/大括号结束代码块
void parse_SEN() {
    if (lookahead_type == ID_IF) {
        parse_CONDITION_SEN();
    }else if(lookahead_type == ID_WHILE) {
        parse_LOOP_SEN();
    }else if(lookahead_type == ID_IDENTIFIER) {
        parse_ASSIGN_SEN();
    }else if(lookahead_type == ID_RBRACE){
        match_word(ID_RBRACE);
    }else{
        cout << "******************** 错误 *******************" << endl;
        cout << "错误行 " << next_word.line << ": " << endl;
        cout << "语句不合法!" << endl;
        // exit(0);
        have_error = true;
        cout << "*********************************************" << endl;
    }
}

// 一个代码块以｝为结束标志，后可以继续跟语句，也可结束整个代码
void parse_SEN_PART1() {
    if (lookahead_type == ID_SEMICOLON) {
        match_word(ID_SEMICOLON);
        parse_SEN();
        parse_SEN_PART1();
    }else{
        parse_SEN();
    }
}

// 进入主体语句部分
void parse_SEN_PART() {
    parse_SEN();
    parse_SEN_PART1();
}

// 在声明语句中，连续声明变量
void parse_IDENTIFIER_TAB1() {
    // 一个变量名后为逗号，则连续定义多个变量
    if (lookahead_type == ID_COMMA) {
        match_word(ID_COMMA);
        parse_IDENTIFIER();
        parse_IDENTIFIER_TAB1();
    }else{
        // nothing to do
    }
}

// 声明定义变量
void parse_IDENTIFIER_TAB() {
    parse_IDENTIFIER();           // 变量名为一个标识符
    parse_IDENTIFIER_TAB1();      // 多个变量一同声明定义
}

// 先声明变量类型int
void parse_VAR_DECLARATION() {
    match_word(ID_INT);
}

// 初始声明定义变量（并赋值）
void parse_VAR_DECLARATION_PART() {
    parse_VAR_DECLARATION();      // 先声明类型int
    parse_IDENTIFIER_TAB();       // 再定义变量（并进行赋值）
}

// main函数主体
void parse_MAIN_FUNC() {

    // 初始声明变量（并赋值）
    parse_VAR_DECLARATION_PART();
    match_word(ID_SEMICOLON);
    cout << "------------------------------------" << endl;

    // 后续主体语句入口
    parse_SEN_PART();
}

// 程序开始固定的main(){...}
void parse_PROGRAM() {
   match_word(ID_MAIN);
   match_word(ID_LPAREN);
   match_word(ID_RPAREN);
   match_word(ID_LBRACE);
   parse_MAIN_FUNC();      // 主体部分入口
   match_word(ID_RBRACE);
   if (have_error) {
       cout << "==================== 存在错误!!! ====================" << endl;
       exit(0);
   }
   cout << "==================== OK!源程序正确! ===================" << endl;
}


int main() {
    char ch;
    freopen("in.txt", "r", stdin);

    file_length = 0;

    while (~scanf("%c", &ch)) {
        letters[file_length++] = ch;
    }
    cur = 0;
    cur_line = 1;

    take_word();
    lookahead_type = next_word.type;

    // 进入主体函数部分
    parse_PROGRAM();

    int k = 0;
    cout << endl;

    // 输出整个四元式序列表
    cout << "No.\tOP\tARG1\tARG2\tRES" << endl;
    for (k = 0; k < qua_num; k++) {
        cout << "[" << k+1 << "]\t" << qua[k].op << "\t" << qua[k].arg1 << "\t" << qua[k].arg2 << "\t" << qua[k].res << "\t" << endl;
    }
    if (have_loop && follow_loop == k) {
        cout << "[" << k+1 << "]\t" << "" << "\t" << "" << "\t" << "" << "\t" << "" << "\t" << endl;
    }

    return 0;
}
