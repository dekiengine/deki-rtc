#pragma once

// DLL export macro
#ifdef _WIN32
    #ifdef DEKI_RTC_EXPORTS
        #define DEKI_RTC_API __declspec(dllexport)
    #else
        #define DEKI_RTC_API __declspec(dllimport)
    #endif
#else
    #define DEKI_RTC_API __attribute__((visibility("default")))
#endif
