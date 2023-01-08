#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include "sensoray/826.h"
#include "tools/data_saver.h"


class Controller {
public:
    static const uint max_iter = 1000;

    Controller();

    void setup();

    void insertion();
    
private:

    void controlLoop();
    
    Sensoray826 m_board;

    float m_probe_speed = 0;
    float m_tendon_u_speed = 0;
    float m_tendon_d_speed = 0;

    float m_data_load[max_iter] = {};

};

#endif // CONTROLLER_H