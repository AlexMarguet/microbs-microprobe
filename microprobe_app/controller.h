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
    Controller(Sensoray826 board);

    void startSetup();
    
    bool setup();

    bool controlLoop();

    void start();

    void stop();

    void setVProbe(float v_probe);
    void setVTendonNom(float v_tendon_nom);
    void setXProbeMax(float x_probe_max);
    void setFMin(float f_min);
    void setFRef(float f_ref);
    void setKP(float k_p);
    void setKI(float k_i);
    void setKD(float k_d);

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

    uint m_loop_iter = 0;

    float m_k_p;
    float m_k_i;
    float m_k_d;

};

#endif // CONTROLLER_H