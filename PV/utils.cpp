#include "utils.h"
#include "log.h"
#include <cstdarg>
#include <cstdio>
#include <random>
#include <thread>
#include <chrono>

void work(int min_ms, int max_ms, const char* fmt, ...) {
    // 参数校验
    if (min_ms < 0) min_ms = 0;
    if (max_ms < min_ms) max_ms = min_ms;

    // 生成随机工作时间
    // 使用线程ID + 随机设备来种子，避免多线程同时启动时种子相同
    static thread_local std::mt19937 gen(std::random_device{}() + std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::uniform_int_distribution<> dis(min_ms, max_ms);
    int sleep_ms = dis(gen);

    // 先记录日志（在睡眠前）
    // 格式化用户消息
    char user_msg[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(user_msg, sizeof(user_msg), fmt, args);
    va_end(args);

    // 输出完整日志消息：用户消息 + "，预计xxxms"
    LOG_INFO("%s，预计%dms", user_msg, sleep_ms);

    // 执行睡眠（这段时间内不会输出日志，但其他线程可以继续运行）
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
}
