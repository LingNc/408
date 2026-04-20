#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

// 模拟工作任务：输出日志并随机睡眠一段时间
// min_ms: 最小睡眠时间（毫秒）
// max_ms: 最大睡眠时间（毫秒）
// fmt: 格式化字符串（类似 printf），后面跟可变参数
// 输出示例：LOG_DEBUG("第1个哲学家开始进餐，预计150ms")
// 注意：实际执行时间会有微小偏差（日志输出耗时）
void work(int min_ms, int max_ms, const char* fmt, ...);

#endif // UTILS_H
