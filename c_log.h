/*
MIT License

Copyright (c) 2024 jarvarvarvis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _C_LOG_HEADER
#define _C_LOG_HEADER

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_LOG_OK                 0
#define C_LOG_ERROR_INVALID_ARGS 1
#define C_LOG_ERROR_FORMAT       2
#define C_LOG_ERROR_FORMAT_TIME  3
#define C_LOG_ERROR_ALLOCATION   4

typedef enum {
    C_LOG_SEVERITY_DEBUG   = 1,
    C_LOG_SEVERITY_INFO    = 2,
    C_LOG_SEVERITY_WARNING = 3,
    C_LOG_SEVERITY_ERROR   = 4,

    C_LOG_SEVERITY_SHOW_NONE = 5,
} CLogSeverity;

// The minimum severity that is logged to the console
#ifndef C_LOG_MIN_SEVERITY
#define C_LOG_MIN_SEVERITY C_LOG_SEVERITY_DEBUG
#endif

// The amount of bytes/chars allocated to the initial buffer into which the message is formatted
// This buffer is allocated on the stack.
#ifndef C_LOG_FORMAT_BUFFER_STACK_BYTES
#define C_LOG_FORMAT_BUFFER_STACK_BYTES 512
#endif


#ifndef C_LOG_NO_ANSI

#define C_LOG_ANSI_RESET       "\033[0m"

#define C_LOG_ANSI_GREY        "\033[90m"

#define C_LOG_ANSI_BOLD_WHITE  "\033[97;1m"
#define C_LOG_ANSI_BOLD_RED    "\033[91;1m"
#define C_LOG_ANSI_BOLD_GREEN  "\033[92;1m"
#define C_LOG_ANSI_BOLD_YELLOW "\033[93;1m"

#else

#define C_LOG_ANSI_RESET

#define C_LOG_ANSI_GREY

#define C_LOG_ANSI_BOLD_WHITE
#define C_LOG_ANSI_BOLD_RED
#define C_LOG_ANSI_BOLD_GREEN
#define C_LOG_ANSI_BOLD_YELLOW

#endif


#ifndef C_LOG_IMPLEMENTATION

bool c_log_should_show_severity(CLogSeverity severity);
int c_log(CLogSeverity severity, const char* format, ...);

#else

bool c_log_should_show_severity(const CLogSeverity severity) {
    return severity >= C_LOG_MIN_SEVERITY;
}

int c_log(const CLogSeverity severity, const char* format, ...) {
    // Don't call c_log with C_LOG_SEVERITY_SHOW_NONE or higher values
    if (severity >= C_LOG_SEVERITY_SHOW_NONE) {
        return C_LOG_ERROR_INVALID_ARGS;
    }

    // Should this severity level be shown? (based on C_LOG_MIN_SEVERITY)
    if (!c_log_should_show_severity(severity)) {
        return C_LOG_OK;
    }

    // Format the message into a local buffer
    char temp[C_LOG_FORMAT_BUFFER_STACK_BYTES];

    va_list args;
    va_start(args, format);
    int length = vsnprintf(temp, sizeof(temp), format, args);
    va_end(args);

    // Encoding error
    if (length < 0) {
        return C_LOG_ERROR_FORMAT;
    }

    bool reformat = false;
    char *message_str = temp;
    if ((unsigned int)length >= sizeof(temp)) {
        // Message was truncated because it didn't fit into the buffer
        // -> Allocate a large enough buffer to fit the entire string
        message_str = (char*) malloc(length + 1);
        reformat = true;
    }

    // Allocation error
    if (!message_str) {
        return C_LOG_ERROR_ALLOCATION;
    }

    if (reformat) {
        // Format the message into the bigger buffer
        va_start(args, format);
        length = vsnprintf(message_str, length + 1, format, args);
        va_end(args);
    }

    // Get current time
    const time_t timer = time(NULL);
    const struct tm *time_info = localtime(&timer);

    char time_str[40] = {0};

    // Format time string
    const size_t format_result = strftime(time_str, 40, C_LOG_ANSI_GREY "%Y-%m-%d %H:%M:%S" C_LOG_ANSI_RESET, time_info);
    if (format_result == 0) {
        return C_LOG_ERROR_FORMAT_TIME;
    }

    // Show log message
    if (severity == C_LOG_SEVERITY_DEBUG) {
        const char *severity_str = C_LOG_ANSI_BOLD_GREEN "debug" C_LOG_ANSI_RESET;
        printf("%s  [%s] %s\n", time_str, severity_str, message_str);
    }

    else if (severity == C_LOG_SEVERITY_INFO) {
        const char *severity_str = C_LOG_ANSI_BOLD_WHITE "info" C_LOG_ANSI_RESET;
        printf("%s  [%s] %s\n", time_str, severity_str, message_str);
    }

    else if (severity == C_LOG_SEVERITY_WARNING) {
        const char *severity_str = C_LOG_ANSI_BOLD_YELLOW "warning" C_LOG_ANSI_RESET;
        printf("%s  [%s] %s\n", time_str, severity_str, message_str);
    }

    else if (severity == C_LOG_SEVERITY_ERROR) {
        const char *severity_str = C_LOG_ANSI_BOLD_RED "error" C_LOG_ANSI_RESET;
        printf("%s  [%s] %s\n", time_str, severity_str, message_str);
    }

    // If the message was reformatted, free the allocated memory
    if (reformat) {
        free(message_str);
    }

    return length;
}

#endif /* C_LOG_IMPLEMENTATION */

#endif /* _C_LOG_HEADER */
