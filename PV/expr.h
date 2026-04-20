#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <vector>
#include <stack>
#include <functional>

// 词法单元类型
enum TokenKind { NUM, IDENT, OP, LPAREN, RPAREN, END };

struct Token {
    TokenKind kind;
    double num_val;
    std::string str_val;
    char op;
};

// 获取运算符优先级
int precedence(char op);

// 1. 词法分析：将字符串拆分为 Token
std::vector<Token> tokenize(const std::string& expr);

// 2. 语法解析：调度场算法（中缀转后缀）
std::vector<Token> shuntingYard(const std::vector<Token>& tokens);

// 3. 逆波兰求值：结合变量解析回调
double evaluate(const std::vector<Token>& postfix,
                std::function<double(const std::string&)> var_resolver);

// 封装调用入口
double calc(const std::string& expr,
            std::function<double(const std::string&)> var_resolver);

// 从信号量名称中提取方括号中的表达式并计算
// 例如："chopstick[i]" -> 计算表达式 "i"，其中 i = current_tid - 1（tid从1开始，i从0开始）
//       "chopstick[(i+1)%5]" -> 计算表达式 "(i+1)%5"
// 如果名称中不包含方括号，返回 -1
int resolve_index_expr(const char* sem_name);

// 格式化信号量名称：将 "chopstick[i]" 转换为 "chopstick[tid-1]"（实际数值）
// 结果存储在 thread_local 静态缓冲区中，每个线程独立，是线程安全的
// 如果名称中不包含方括号或表达式解析失败，返回原名称
const char* format_sem_name(const char* name);

#endif // EXPR_H
