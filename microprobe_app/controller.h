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

    Controller(Sensoray826 board);

    void startSetup();
    
    bool setup();

    void insertion();

    bool controlLoop();

    void start();

    void stop();

    void setVProbe(float v_probe);
    void setVTendonNom(float v_tendon_nom);
    void setXProbeMax(float x_probe_max);
    void setFMin(float f_min);
    void setFRef(float f_ref);

    float getFURef();
    float getFDRef();
    
    
private:
    Sensoray826 m_board;

    float m_x_probe_max = 10.; // [mm]
    int m_max_loops = 100.;

    float m_f_min = 50.; // [mN]
    float m_f_u_ref = m_f_min;
    float m_f_d_ref = m_f_min;

    float m_v_probe = 0.;
    float m_v_tendon_nom = 0.;
    float m_v_tendon_u = 0.;
    float m_v_tendon_d = 0.;

    float m_data_load[max_iter] = {};

};

#endif // CONTROLLER_H