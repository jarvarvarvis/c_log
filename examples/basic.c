#define C_LOG_IMPLEMENTATION
#include "../c_log.h"

int main() {
    c_log(C_LOG_SEVERITY_DEBUG,   "Debug message, 123");
    c_log(C_LOG_SEVERITY_INFO,    "This is a short info message.");
    c_log(C_LOG_SEVERITY_WARNING, "This is a very important warning!");
    c_log(C_LOG_SEVERITY_ERROR,   "This is a normal error.");
    c_log(C_LOG_SEVERITY_FATAL,   "Something went very wrong!");
}
