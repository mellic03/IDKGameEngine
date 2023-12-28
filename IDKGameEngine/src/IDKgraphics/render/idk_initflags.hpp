#pragma once


namespace idk::InitFlag
{
    enum idk_init_flag: uint32_t
    {
        NONE          = 0,
        INIT_HEADLESS = 1 << 0
    };
}

