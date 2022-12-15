#pragma once 
#include "glog/logging.h"
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

enum class LogLevel : uint8_t
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};
#define RLOG LOG
template<typename... TArgs>
static void MyRLOG(LogLevel level, TArgs&&... args)
{
    switch (level)
    {
    case LogLevel::Debug:
        spdlog::debug(std::forward<TArgs>(args)...);
        break;
    case LogLevel::Info:
        spdlog::info(std::forward<TArgs>(args)...);
    case LogLevel::Warning:
        spdlog::warn(std::forward<TArgs>(args)...);
    case LogLevel::Error:
        spdlog::error(std::forward<TArgs>(args)...);
    case LogLevel::Fatal:
        stdlog::critical(std::forward<TArgs>(args)...);
        const std::string format_str = fmt::format(std::format<TArgs>(args)...);
        throw std::runtime_error(format_str);
    default:
        break;
    }
}