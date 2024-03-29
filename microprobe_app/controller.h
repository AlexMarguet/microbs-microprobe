#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <numeric>
#include "sensoray/826.h"
#include "tools/data_saver.h"


class Controller {
public:
    Controller(Sensoray826& board, DataSaver& data_saver);

    void startHold();

    void hold();
    
    bool controlLoopPID();

    void start(float control_freq);

    void startReelBack(float control_freq);

    void stop();

    //Get-Set
    void setVProbeNom(float v_probe_nom);
    void setVTendonRelNom(float v_tendon_rel_nom);
    void setXProbeMax(float x_probe_max);
    void setFMin(float f_min);
    void setFRef(float f_ref);
    void setKP(float k_p);
    void setKI(float k_i);
    void setKD(float k_d);
    float getFURef();
    float getFDRef();
    
    
private:
    void fixedSpeedControl();
    void pidControl();

    Sensoray826& m_board;
    
    DataSaver& m_data_saver;

    float m_x_probe_max = 0.; // [mm]
    int m_max_loops = 0.;
    uint m_loop_iter = 0;

    float m_f_min = 0.; // [mN]
    float m_f_u_ref = m_f_min;
    float m_f_d_ref = m_f_min;

    float m_load[2] = {0, 0};

    float m_v_probe_nom = 0.;
    float m_v_probe = 0.;
    float m_v_tendon_rel_nom = 0.;
    float m_v_tendon_u = 0.;
    float m_v_tendon_d = 0.;

    //PID
    float m_k_p;
    float m_k_i;
    float m_k_d;

    float m_error[2] = {0, 0};

    int m_integration_period = 2;
    std::array<std::vector<float>, 2> m_prev_error;
};

#endif // CONTROLLER_H