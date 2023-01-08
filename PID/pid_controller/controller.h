#include "math.h"

#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


class PID {
public:
    static const float kp;
    static const float ki;
    static const float kd;

    PID();

    void idle();

    void control_loop();

    void setReference(float reference);

private:
    float m_reference = 0;
    float m_val = 0;
    float m_error = 0;
    float m_prev_error = 0;
    float m_d_error = 0;
};