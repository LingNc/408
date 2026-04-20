#include "expr.h"
#include <cctype>
#include <cstring>
#include <cmath>
#include <stdexcept>

// 获取当前线程ID（从 thread.cpp 导入）
extern "C" int get_current_tid();

// 获取运算符优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

// 1. 词法分析：将字符串拆分为 Token
std::vector<Token> tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < expr.size()) {
        if (std::isspace(expr[i])) { i++; continue; }

        if (std::isdigit(expr[i]) || expr[i] == '.') {
            size_t start = i;
            while (i < expr.size() && (std::isdigit(expr[i]) || expr[i] == '.')) i++;
            tokens.push_back({NUM, std::stod(expr.substr(start, i - start)), "", 0});
        }
        else if (std::isalpha(expr[i]) || expr[i] == '_') {
            size_t start = i;
            while (i < expr.size() && (std::isalnum(expr[i]) || expr[i] == '_')) i++;
            tokens.push_back({IDENT, 0, expr.substr(start, i - start), 0});
        }
        else if (strchr("+-*/%()", expr[i])) {
            TokenKind tk = (expr[i] == '(') ? LPAREN : (expr[i] == ')' ? RPAREN : OP);
            tokens.push_back({tk, 0, "", expr[i]});
            i++;
        }
        else {
            throw std::runtime_error(std::string("非法字符: ") + expr[i]);
        }
    }
    tokens.push_back({END, 0, "", 0});
    return tokens;
}

// 2. 语法解析：调度场算法（中缀转后缀）
std::vector<Token> shuntingYard(const std::vector<Token>& tokens) {
    std::vector<Token> output;
    std::stack<char> ops;

    for (const auto& t : tokens) {
        if (t.kind == NUM || t.kind == IDENT) {
            output.push_back(t);
        } else if (t.kind == OP) {
            while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(t.op)) {
                output.push_back({OP, 0, "", ops.top()});
                ops.pop();
            }
            ops.push(t.op);
        } else if (t.kind == LPAREN) {
            ops.push('(');
        } else if (t.kind == RPAREN) {
            while (!ops.empty() && ops.top() != '(') {
                output.push_back({OP, 0, "", ops.top()});
                ops.pop();
            }
            if (ops.empty()) throw std::runtime_error("括号不匹配");
            ops.pop(); // 弹出 '('
        } else if (t.kind == END) {
            break;
        }
    }
    while (!ops.empty()) {
        if (ops.top() == '(') throw std::runtime_error("括号不匹配");
        output.push_back({OP, 0, "", ops.top()});
        ops.pop();
    }
    return output;
}

// 3. 逆波兰求值：结合变量解析回调
double evaluate(const std::vector<Token>& postfix,
                std::function<double(const std::string&)> var_resolver) {
    std::stack<double> st;
    for (const auto& t : postfix) {
        if (t.kind == NUM) {
            st.push(t.num_val);
        } else if (t.kind == IDENT) {
            st.push(var_resolver(t.str_val)); // 动态调用 func()
        } else if (t.kind == OP) {
            if (st.size() < 2) throw std::runtime_error("表达式格式错误");
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            double res = 0;
            switch (t.op) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (b == 0) throw std::runtime_error("除数不能为0");
                    res = a / b; break;
                case '%':
                    if (b == 0) throw std::runtime_error("取模数不能为0");
                    res = std::fmod(a, b); break;
                default: throw std::runtime_error("未知运算符");
            }
            st.push(res);
        }
    }
    if (st.size() != 1) throw std::runtime_error("表达式不完整");
    return st.top();
}

// 封装调用入口
double calc(const std::string& expr,
            std::function<double(const std::string&)> var_resolver) {
    auto tokens = tokenize(expr);
    auto postfix = shuntingYard(tokens);
    return evaluate(postfix, var_resolver);
}

// 从信号量名称中提取方括号中的表达式并计算
// 例如："chopstick[i]" -> 计算表达式 "i"，其中 i = current_tid - 1（tid从1开始，i从0开始）
//       "chopstick[(i+1)%5]" -> 计算表达式 "(i+1)%5"
// 如果名称中不包含方括号，返回 -1
int resolve_index_expr(const char* sem_name) {
    if (!sem_name) return -1;

    // 查找左方括号
    const char* lbracket = strchr(sem_name, '[');
    if (!lbracket) return -1;

    // 查找右方括号
    const char* rbracket = strchr(lbracket, ']');
    if (!rbracket) return -1;

    // 提取方括号中的表达式
    std::string expr(lbracket + 1, rbracket - lbracket - 1);
    if (expr.empty()) return -1;

    try {
        // 变量解析器：i 映射到当前线程ID
        auto resolver = [](const std::string& name) -> double {
            if (name == "i") return static_cast<double>(get_current_tid() - 1); // tid 从1开始，i 从0开始
            throw std::runtime_error("未定义变量: " + name);
        };

        double result = calc(expr, resolver);
        return static_cast<int>(result);
    } catch (const std::exception& e) {
        // 表达式计算失败，返回 -1
        return -1;
    }
}

// 格式化信号量名称：将 "chopstick[i]" 转换为 "chopstick[0]"（其中 i=0 是当前线程ID）
// 结果存储在静态缓冲区中，非线程安全但适用于当前场景
// 如果名称中不包含方括号或表达式解析失败，返回原名称
const char* format_sem_name(const char* name) {
    static thread_local char buffer[128];  // 每个线程独立的缓冲区

    // 查找左方括号
    const char* lbracket = strchr(name, '[');
    if (!lbracket) {
        // 没有方括号，直接返回原名称
        return name;
    }

    // 计算索引表达式的值
    int index = resolve_index_expr(name);
    if (index < 0) {
        // 表达式解析失败，返回原名称
        return name;
    }

    // 提取数组名部分（方括号之前的部分）
    size_t array_name_len = lbracket - name;
    if (array_name_len >= sizeof(buffer) - 10) {
        array_name_len = sizeof(buffer) - 10;  // 预留空间给索引
    }

    // 构造格式化后的名称：array_name[index]
    memcpy(buffer, name, array_name_len);
    snprintf(buffer + array_name_len, sizeof(buffer) - array_name_len, "[%d]", index);

    return buffer;
}
