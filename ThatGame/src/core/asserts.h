#pragma once

#include "logging.h"

#ifdef _DEBUG
    #define TG_ASSERT(x, ...) \
        do { \
            if (!(x)) { \
                LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                __debugbreak(); \
            } \
        } while (0)
#else
    #define TG_ASSERT(x, ...)
#endif
