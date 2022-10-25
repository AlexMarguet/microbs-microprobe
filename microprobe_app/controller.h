#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include "sensoray/s826.h"
#include "tools/data_saver.h"


class Controller {
public:
    static const uint max_iter = 1000;

    Controller();

    void setup();

    void insertion();
    
private:
    Sensoray826 m_board = 0;

    float[max_iter] m_data_load = {};

};

#endif // CONTROLLER_H