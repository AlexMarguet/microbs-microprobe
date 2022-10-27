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
    Sensoray826 m_board;

    float m_data_load[max_iter] = {};

};

#endif // CONTROLLER_H