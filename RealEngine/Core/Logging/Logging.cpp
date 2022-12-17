#include "Logging.h"
#include <codecvt>
#include <locale>
#include <windows.h>
#include <iostream>

void InitLogger(const std::wstring& logFilePath)
{
    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    // FLAGS_log_dir = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(logFilePath);
    // google::InitGoogleLogging("");
    // FLAGS_colorlogtostderr = 1;
    // // FLAGS_alsologtostderr = 1;
    // FLAGS_logtostderr = 1;
    std::locale::global(std::locale("zh-CN"));
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
