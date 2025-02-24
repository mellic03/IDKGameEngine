// #pragma once
// #include <SDL2/SDL.h>
// #include <glm/glm.hpp>

// #include <libidk/idk_assert.hpp>


// namespace idk
// {
//     class Joystick;
// }


// class idk::Joystick
// {
// private:
//     bool _changed = false;

// public:
//     SDL_Joystick  *JS;
//     SDL_JoystickID id;

//     union axes_type
//     {
//         float data[4] = {0, 0, 0, 0};
//         struct { float roll, pitch, yaw, throttle; };
//     };

//     struct bounds_type
//     {
//         axes_type min, max;
//     };

//     axes_type   stick;
//     bounds_type bounds;
//     uint8_t     hat[8];

//     Joystick(): JS(nullptr)
//     {
//         bounds.min = {-1, -1, -1,  0};
//         bounds.max = {+1, +1, +1, +1};
//         clear();
//     }


//     bool changed()
//     {
//         return _changed;
//     }

//     void clear()
//     {
//         for (int i=0; i<sizeof(hat); i++)
//         {
//             hat[i] = SDL_HAT_CENTERED;
//         }
//         _changed = false;
//     }

//     void open( int index )
//     {
//         JS = SDL_JoystickOpen(index);
//         id = SDL_JoystickInstanceID(JS);
//     }

//     void close()
//     {
//         SDL_JoystickClose(JS);
//         JS = nullptr;
//     }

//     void setAxis( uint8_t axis, int16_t value )
//     {
//         _changed = true;
//         static constexpr int16_t axis_min = SDL_JOYSTICK_AXIS_MIN;
//         static constexpr int16_t axis_max = SDL_JOYSTICK_AXIS_MAX;

//         float alpha = float(value-axis_min) / float(axis_max-axis_min);
//         float minv  = bounds.min.data[axis];
//         float maxv  = bounds.max.data[axis];

//         stick.data[axis] = glm::mix(minv, maxv, alpha);
//     }

//     void setHat( uint8_t idx, uint8_t value )
//     {
//         _changed = true;
//         IDK_ASSERT("idx too large", idx < sizeof(hat));
//         hat[idx] = value;
//     }

// };