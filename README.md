# 408 PV Library - 408 的 C++ 信号量与线程库

单头文件库 (`PV.hpp`)，提供信号量原语、线程管理和日志功能，用于408操作系统课程实验。尽可能遵循408教材的PV操作写法，让教材代码能够直接运行。

## 快速开始

```cpp
#include "PV.hpp"

semaphore mutex = 1;  // 初始化信号量

process task() {
    P(mutex);           // P操作（等待）
    LOG_INFO("LOG");
    work(100, 500, "工作中 线程id=%d", tid); // 模拟工作，tid 可用 i+1 或 _tid
    V(mutex);           // V操作（释放）
}

int main() {
    log_init(2);        // 初始化日志: 0=INFO, 1=DEBUG, 2=SEM

    ThreadManager tm;
    tm.addThread(task, 5, "Worker", 1);  // 5个线程，每个执行1次
    tm.runAll();        // 运行并等待完成
    return 0;
}
```

编译：`g++ -std=c++17 -pthread test.cpp -o test`

## 构建

切换到项目根目录，编译 `PV.hpp` ：

```bash
make
```

## 核心组件

### 信号量 (semaphore)
```cpp
semaphore s;        // 默认value=0
semaphore s = 5;    // 初始化为5
s = 3;              // 赋值
if (s > 0) { ... }  // 比较运算符
LOG_INFO("%d", s);    // 自动转换为s.value
```

### P/V 操作
```cpp
P(sem);              // P操作，信号量名自动解析
V(sem);              // V操作
// 支持数组索引自动计算: P(chopstick[i]) 显示为如 "chopstick[0]"
// i 是当前线程的 0-based 索引，无需定义即可直接使用，如：
P(chopstick[i]);
```

### 线程管理 (ThreadManager)
```cpp
ThreadManager tm;
tm.addThread(func, count, name, repeat);  // 添加线程对应的任务、线程数量、名称和重复次数
tm.runAll();                              // 运行并统计用时
```

### 日志
```cpp
LOG_INFO(fmt, ...)   // 普通信息
LOG_DEBUG(fmt, ...)  // 调试信息
LOG_SEM(fmt, ...)    // 信号量操作日志
// 输出: [时间] [级别] [Txx:名称] 消息
```

### 模拟工作 (work)
```cpp
work_init(123456);                    // 设置随机种子(可选)
work(100, 1000, "日志%d", tid);      // 随机睡眠100-1000ms，输出日志
```

### 线程宏
```cpp
_tid / tid   // 当前线程ID(1-based)，两者等价
_i / i       // 线程索引(0-based) = tid-1，两者等价
_name / name // 线程名称，两者等价
process      // void 的类型别名
```

## 示例程序

| 文件 | 说明 |
|------|------|
| `pd1.cpp` | 哲学家问题 - 限制同时就餐人数 |
| `pd2.cpp` | 哲学家问题 - 奇偶不同顺序拿筷子 |
| `pd3.cpp` | 哲学家问题 - 互斥锁 + 信号量 |
| `producer-consumer.cpp` | 生产者消费者问题 |
| `rw.cpp` | 读者写者问题 |

## 日志级别

- `0` (INFO): 基本信息
- `1` (DEBUG): + 调试信息
- `2` (SEM): + P/V操作详情

## 特点

- 单文件库 (`PV.hpp`)，包含所有实现
- 自动信号量名解析：`P(chopstick[i])` 日志显示实际索引
- 线程安全日志，带时间戳和线程ID
- 可复现随机数（通过 `work_init`）

## 许可证

本项目采用 [MIT 许可证](LICENSE) 开源。