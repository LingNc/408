#include "utils.h"
#include "log.h"
#include <cstdarg>
#include <cstdio>
#include <random>
#include <thread>
#include <chrono>

// 全局随机种子设置
static unsigned int g_work_seed = 0;
static bool g_work_seed_set = false;

void work_init(unsigned int seed) {
    if (seed == 0) {
        g_work_seed_set = false;
    } else {
        g_work_seed = seed;
        g_work_seed_set = true;
    }
}

void work(int min_ms, int max_ms, const char* fmt, ...) {
    // 参数校验
    if (min_ms < 0) min_ms = 0;
    if (max_ms < min_ms) max_ms = min_ms;

    // 生成随机工作时间
    // 根据是否设置了种子来决定如何初始化
    // 使用 tid-1 作为线程索引（tid从1开始，索引从0开始）
    int thread_idx = get_current_tid() - 1;
    static thread_local std::mt19937 gen = [&]() {
        if (g_work_seed_set) {
            // 使用用户设置的种子 + 线程索引，保证每次运行结果相同
            return std::mt19937(g_work_seed + thread_idx);
        } else {
            // 自动随机种子
            return std::mt19937(std::random_device{}() + thread_idx);
        }
    }();
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
