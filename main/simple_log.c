#include "simple_log.h"

// Hàm log cốt lõi
void log_print(const char *color, const char *level, const char *tag, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  printf("%s[%s] ", color, tag); // Màu + level + tag
  vprintf(fmt, args);                       // Nội dung như printf
  printf("%s", ANSI_COLOR_RESET);           // Reset màu
  printf("\n");                             // Xuống dòng
  va_end(args);
}
