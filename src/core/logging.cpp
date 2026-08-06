#include "logging.h"

void logger::init()
{
    if (s_initialized)
        return;

    s_initialized = true;
    LOG_INFO("Logger initialized");
}

void logger::shutdown()
{
    if (!s_initialized)
        return;

    LOG_INFO("Logger shutdown");
    s_initialized = false;
}
