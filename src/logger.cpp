#include "logger.h"

#if JPLATFORM_WINDOWS

#include <cstdio>
#include <cstdarg>
#include <Windows.h>

void joj::log_output(const LogLevel level, const ErrorCode err, const char* message, ...) {
    // Setup for sprintf and then printf
    char out_msg[1000];
    char out_msg2[1000];
    memset(out_msg, 0, sizeof(out_msg));

    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_msg, 1000, message, arg_ptr);

    // Colors (Windows)
    WORD color_attributes[5] = {
        FOREGROUND_RED | FOREGROUND_INTENSITY,  // Bold red
        FOREGROUND_RED,                         // Red
        FOREGROUND_RED | FOREGROUND_GREEN,      // Orange
        FOREGROUND_BLUE,                        // Blue
        FOREGROUND_GREEN                        // Green
    };

    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Bold red, Red, Orange, Blue, Green
    const char* color_strings[5] = {
        "\033[1;31m", "\033[0;31m", "\033[0;33m", "\033[0;34m", "\033[0;32m"
    };

    const char* level_strings[5] = {
        "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[DEBUG]: ", "[INFO]: "
    };

    const char* err_name = err_to_str(err);

    // FATAL log
    if (level == LOG_LEVEL_FATAL)
    {
        sprintf(out_msg2, "%s%s - %s", level_strings[level], err_name, out_msg);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);
        printf("%s\n", out_msg2);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
        return;
    }
    // ERROR log
    if (level == LOG_LEVEL_ERROR)
    {
        sprintf(out_msg2, "%s%s - %s", level_strings[level], err_name, out_msg);
        SetConsoleTextAttribute(hConsole, color_attributes[level]);
        printf("%s\n", out_msg2);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
        return;
    }

    sprintf(out_msg2, "%s%s", level_strings[level], out_msg);
    SetConsoleTextAttribute(hConsole, color_attributes[level]);
    printf("%s\n", out_msg2);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void joj::todo() {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    printf("[TODO] File: %s - Line: %d\n", __FILE__, __LINE__);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

#endif // JPLATFORM_WINDOWS