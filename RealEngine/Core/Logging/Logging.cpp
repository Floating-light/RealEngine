#include "Logging.h"
#include <codecvt>
#include <locale>
#include <windows.h>
#include <iostream>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

spdlog::logger*  RLogger::GLogger = nullptr; 

void InitLogger(const std::wstring& logFilePath)
{
    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    // FLAGS_log_dir = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(logFilePath);
    // google::InitGoogleLogging("");
    // FLAGS_colorlogtostderr = 1;
    // // FLAGS_alsologtostderr = 1;
    // FLAGS_logtostderr = 1;
    std::locale::global(std::locale("zh-CN"));

    // init logger
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    auto windebug_sink = std::make_shared<spdlog::sinks::windebug_sink_mt>();
    windebug_sink->set_level(spdlog::level::trace);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);
    file_sink->set_level(spdlog::level::trace);

    RLogger::GLogger = new spdlog::logger("", { console_sink ,windebug_sink ,file_sink });

    // BOOL ret = SetConsoleCP(CP_UTF8);
    RLOG(LogLevel::Info, "init spdlog <<<<<<<<<<<<<<<<< ===========");
    // if(!ret)
    // {
    //     DWORD Er = GetLastError();
    //     RLOG(Error, "SetConsoleCP Ahhhhhhhhh: {}", ret);
    // }
    std::string Mystr("中文 AHHHHHHHHHHHHHH");
    std::cout << Mystr << std::endl;

    RLOG(Info, "中文 Ahhhhhhhhh: {}", 110);
}
