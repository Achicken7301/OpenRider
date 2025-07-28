#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <stdio.h>
#include <stdarg.h>

// Mã ANSI cho màu
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

void log_print(const char *color, const char *level, const char *tag, const char *fmt, ...);

// Các macro log theo mức độ
#define LOGI(TAG, fmt, ...) log_print(ANSI_COLOR_GREEN, "INFO", TAG, fmt, ##__VA_ARGS__)
#define LOGW(TAG, fmt, ...) log_print(ANSI_COLOR_YELLOW, "WARN", TAG, fmt, ##__VA_ARGS__)
#define LOGE(TAG, fmt, ...) log_print(ANSI_COLOR_RED, "ERROR", TAG, fmt, ##__VA_ARGS__)

#endif // SIMPLE_LOG_H
