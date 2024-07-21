#pragma once 
#include "spdlog/spdlog.h"
inline std::ostream& operator<<(std::ostream& out, const wchar_t* str) 
{ 
    size_t len = wcsrtombs(NULL, &str, 0, NULL); 
    char* buf = (char*)malloc(len + 1); 
    buf[len] = 0; 
    wcsrtombs(buf, &str, len, NULL); 
    out << buf; 
    free(buf); 
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const std::wstring& str) 
{ 
    return operator<<(out, str.c_str());
}

extern void __declspec(dllexport) InitLogger(const std::wstring& logFilePath );



enum LogLevel : uint8_t
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};
namespace RLogger
{
    extern spdlog::logger* GLogger;
}
// #define RLOG LOG
template<typename... TArgs>
void fatalCallback(fmt::format_string<TArgs...>  fmt,TArgs&&... args)
{
    // https://stackoverflow.com/questions/68675303/how-to-create-a-function-that-forwards-its-arguments-to-fmtformat-keeping-the
    const std::string format_str = fmt::format(fmt, std::forward<TArgs>(args)...);
    // const std::string format_str = fmt::vformat(fmt, fmt::make_format_args(std::forward<TARGS>(args)...));
    throw std::runtime_error(format_str);
}
// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
template<typename... TArgs>
static void RLOG(LogLevel level,fmt::format_string<TArgs...> fmt, TArgs&&... args)
{
    assert(RLogger::GLogger);
    switch (level)
    {
    case LogLevel::Debug:
        RLogger::GLogger->debug(fmt, std::forward<TArgs>(args)...); 
        break;
    case LogLevel::Info:
        RLogger::GLogger->info(fmt, std::forward<TArgs>(args)...); 
        break;
    case LogLevel::Warning:
        RLogger::GLogger->warn(fmt, std::forward<TArgs>(args)...); 
        break;
    case LogLevel::Error:
        RLogger::GLogger->error(fmt, std::forward<TArgs>(args)...); 
        break;
    case LogLevel::Fatal:
        RLogger::GLogger->critical(fmt, std::forward<TArgs>(args)...); 
        fatalCallback(fmt, std::forward<TArgs>(args)...);
        break;
    default:
        break;
    }
}

#define RCHECK(condition)                                                      \
    do {                                                                      \
        if (!(condition)) {                                                   \
			RLOG(Fatal, "Check failed: {} at {}:{}", #condition, __FILE__, __LINE__); \
            __debugbreak();                                                   \
        }                                                                     \
    } while (0)

