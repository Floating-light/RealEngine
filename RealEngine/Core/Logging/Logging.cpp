#include "Logging.h"
#include <codecvt>
#include <locale>


void InitLogger(const std::wstring& logFilePath)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    FLAGS_log_dir = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(logFilePath);
    google::InitGoogleLogging("");
    FLAGS_colorlogtostderr = 1;
    FLAGS_alsologtostderr = 1;
    RLOG(INFO) << "Init logger complete" << std::endl;
}
