#pragma once
#include <cstdint>
#include <cstddef>

namespace idk
{
    // https://controllers.fandom.com/wiki/Sony_DS5#FFB_Trigger_Modes
    struct DS5State
    {
        uint8_t EnableBits1;        /* 0 */
        uint8_t EnableBits2;        /* 1 */
        uint8_t RumbleRight;        /* 2 */
        uint8_t RumbleLeft;         /* 3 */
        uint8_t HeadphoneVolume;    /* 4 */
        uint8_t SpeakerVolume;      /* 5 */
        uint8_t MicrophoneVolume;   /* 6 */
        uint8_t AudioEnableBits;    /* 7 */
        uint8_t MicLightMode;       /* 8 */
        uint8_t AudioMuteBits;      /* 9 */
        uint8_t RTriggerFFB[11];    /* 10 */
        uint8_t LTriggerFFB[11];    /* 21 */
        uint8_t Unknown1[6];        /* 32 */
        uint8_t LedFlags;           /* 38 */
        uint8_t Unknown2[2];        /* 39 */
        uint8_t LedAnim;            /* 41 */
        uint8_t LedBrightness;      /* 42 */
        uint8_t PadLights;          /* 43 */
        uint8_t LedRed;             /* 44 */
        uint8_t LedGreen;           /* 45 */
        uint8_t LedBlue;            /* 46 */
    };

    uint8_t DS5StateBitSet( uint8_t byte, uint8_t mask )
    {
        return byte |= mask;
    }

    uint8_t DS5StateBitUnset( uint8_t byte, uint8_t mask )
    {
        return byte &= ~mask;
    }

    uint8_t DS5StateBitToggle( uint8_t byte, uint8_t mask )
    {
        return byte ^= mask;
    }

}


namespace idk::internal
{
    struct DS5_Trigger
    {
        enum type: uint8_t
        {
            LEFT  = 0b0001,
            RIGHT = 0b0010,
            BOTH  = LEFT | RIGHT,
        };
    };


    struct DS5_EnableBits1
    {
        enum type: uint8_t
        {
            BIT_HAPTICS       = 1 << 0,
            BIT_AUDIO_HAPTICS = 1 << 2,
            BIT_RTRIGGER = 1 << 2, // 0100
            BIT_LTRIGGER = 1 << 3, // 1000
            BIT_RUMBLE   = 1 << 4,
            BIT_AUDIO    = 1 << 5,
            BIT_MIC      = 1 << 6,
            BIT_LED      = 1 << 7
        };
    };

    struct DS5_EnableBits2
    {
        enum type: uint8_t
        {
            BIT_TRIGGER_VIBRATION = 1 << 0,
            BIT_AUDIO_HAPTICS     = 1 << 1,
            BIT_RUMBLE            = 1 << 2,
            BIT_AUDIO             = 1 << 3,
            BIT_MIC               = 1 << 4,
            BIT_LED               = 1 << 5
        };
    };

    struct DS5_Property
    {
        enum type: uint8_t
        {
            TRIGGER_MODE,
            START_POSITION,
            INTENSITY,
            END_POSITION,
            VIBRATION_FREQ,
            VIBRATION_AMP,
            PAD0,
            EFFECT_DURATION,
        };
    };

    struct DS5_Value
    {
        enum type: uint8_t
        {
            DISABLED   = 0x0,
            CONTINUOUS = 0x1,
            SECTIONAL  = 0x2,
            VIBRATE    = 0x26
        };
    };
}

