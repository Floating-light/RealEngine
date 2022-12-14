#pragma once 
#include "glog/logging.h"

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

#define RLOG LOG