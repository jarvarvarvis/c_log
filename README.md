# c_log

Small header-only logging library for the C programming language

## Basic setup

Define the `C_LOG_IMPLEMENTATION` macro and then include the c_log.h header in any .c file you want (main.c is best suited).
Here is what you need to do exactly:
```
#define C_LOG_IMPLEMENTATION
#include <path to c_log.h>
```

In all other files where you want to use c_log, just include the header file *without* defining the macro again.

## Usage

c_log provides the function `c_log(CLogSeverity severity, const char* format, ...)` to output formatted messages to the console.

The 4 log severities taken as arguments by `c_log` are as follows:
- `C_LOG_SEVERITY_DEBUG`
- `C_LOG_SEVERITY_INFO`
- `C_LOG_SEVERITY_WARNING`
- `C_LOG_SEVERITY_ERROR`

`C_LOG_SEVERITY_DEBUG` is the lowest log level, `C_LOG_SEVERITY_ERROR` is the highest.

To specify the minimum level to be displayed, you can use the `C_LOG_MIN_SEVERITY` macro (by default, it's set to `C_LOG_SEVERITY_DEBUG`, so every log level is shown).

To completely disable logs, set `C_LOG_MIN_SEVERITY` to `C_LOG_SEVERITY_SHOW_NONE`.

(Note that it is *not* a valid argument to the `c_log` function)

## Colored Output

By default, the output of `c_log` is colored using [ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code).

If you want to disable them (e.g. because your terminal does not support them), define `C_LOG_NO_ANSI` before the implementation as follows:
```
#define C_LOG_NO_ANSI
#define C_LOG_IMPLEMENTATION
#include <path to c_log.h>
```

## Memory allocation

Using the `C_LOG_FORMAT_BUFFER_STACK_BYTES` macro, you can set the size of the initial, stack-allocated buffer into which the log message is formatted.
Setting this to a higher value (based on the length of your log messages) can avoid a heap allocation and an additional call to `vsnprintf`.

## General Message format

```
<Date and time>  [<Severity>] <Message>
```
