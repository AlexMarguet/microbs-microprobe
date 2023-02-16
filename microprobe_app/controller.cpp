#include "controller.h"

using namespace std;

Controller::Controller(Sensoray826 board, DataSaver& data_saver)
    : m_board(board),
    m_data_saver(data_saver) {
    
    for (int i = 0; i<m_integration_period ; i++) {
        t_prev_error[0].push_back(0);
        t_prev_error[1].push_back(0);
    }
}

void Controller::startSetup() {
    m_f_u_ref = m_f_min;
    m_f_d_ref = m_f_min;
}

void Controller::start(float control_freq) {
    if (m_v_probe_nom == 0.) {
        m_max_loops = 10;
    } else {
        m_max_loops = (m_x_probe_max/m_v_probe_nom) * control_freq;
    }

    m_board.setMotorDirection(Sensoray826::probe, Sensoray826::forward);
    m_board.motorOn(Sensoray826::probe);
    m_v_probe = m_v_probe_nom;
}

void Controller::startReelBack(float control_freq) {
    if (m_v_probe_nom == 0.) {
        m_max_loops = 10;
    } else {
        m_max_loops = (m_x_probe_max/m_v_probe_nom) * control_freq;
    }
    std::cout << m_max_loops << std::endl;
    m_board.setMotorDirection(Sensoray826::probe, Sensoray826::backward);
    m_board.motorOn(Sensoray826::probe);
    m_v_probe = -m_v_probe_nom;
}

void Controller::stop() {
    m_board.motorOff(Sensoray826::probe);
    m_board.motorOff(Sensoray826::tendon_u);
    m_board.motorOff(Sensoray826::tendon_d);
}

bool Controller::setup() {
    static int k = 0;

    float epsilon = 5.;
    uint tendon_ready = 0;
    bool setup_done = false;


    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    if (m_load[Sensoray826::load_sensor_u]>(m_f_u_ref+epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::release);
        m_board.motorOn(Sensoray826::tendon_u);
    } else if (m_load[Sensoray826::load_sensor_u]<(m_f_u_ref-epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::reel);
        m_board.motorOn(Sensoray826::tendon_u);
    } else {
        m_board.motorOff(Sensoray826::tendon_u);
        tendon_ready += 1;
    }

    m_load[Sensoray826::load_sensor_d] = m_board.getLoadSensor(Sensoray826::load_sensor_d);
    if (m_load[Sensoray826::load_sensor_d]>(m_f_d_ref+epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_d, Sensoray826::release);
        m_board.motorOn(Sensoray826::tendon_d);
    } else if (m_load[Sensoray826::load_sensor_d]<(m_f_d_ref-epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_d, Sensoray826::reel);
        m_board.motorOn(Sensoray826::tendon_d);
    } else {
        m_board.motorOff(Sensoray826::tendon_d);
        tendon_ready += 1;
    }

    setup_done = (tendon_ready == 2);
    k++;

    if(setup_done || k > 100) {
        k = 0;
        return true;
    }
    return false;
}

void Controller::hold() {
    fixedSpeedControl();
}

bool Controller::controlLoop() {
    fixedSpeedControl();

    m_loop_iter++;

    float time = m_loop_iter * 0.1;
    float x_probe = time * m_v_probe;
    float data[] = {time, x_probe, m_f_u_ref, m_f_d_ref, m_load[Sensoray826::load_sensor_u], m_load[Sensoray826::load_sensor_d],
            m_v_probe, m_v_tendon_u, m_v_tendon_d};

    m_data_saver.writeDataLine(data, DataSaver::header_size);

    if(m_loop_iter > m_max_loops) {
        m_loop_iter = 0;
        this->stop();
        return true;
    }
    return false;
}

bool Controller::controlLoopPID() {
    pidControl();

    m_loop_iter++;
    
    float time = m_loop_iter * 0.1;
    float x_probe = time * m_v_probe;
    float data[] = {time, x_probe, m_f_u_ref, m_f_d_ref, m_load[Sensoray826::load_sensor_u], m_load[Sensoray826::load_sensor_d],
            m_v_probe, m_v_tendon_u, m_v_tendon_d};

    m_data_saver.writeDataLine(data, DataSaver::header_size);
    

    if(m_loop_iter > m_max_loops) {
        m_loop_iter = 0;
        this->stop();
        return true;
    }
    return false;
}

void Controller::fixedSpeedControl() {
    float epsilon = 5.;

    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    m_error[Sensoray826::load_sensor_u] = -(m_f_u_ref - m_load[Sensoray826::load_sensor_u]);
    
    if (m_error[Sensoray826::load_sensor_u] >= epsilon) {
        m_v_tendon_u = m_v_tendon_rel_nom;
    } else if (m_error[Sensoray826::load_sensor_u] <= -epsilon) {
        m_v_tendon_u = -m_v_tendon_rel_nom;
    }

    m_v_tendon_u = m_v_probe + m_v_tendon_u;

    // std::cout << "error = "<< m_error[Sensoray826::load_sensor_u]  << "v tendon u = " << m_v_tendon_u;
    
    m_board.motorOn(Sensoray826::tendon_u, m_v_tendon_u);


    m_load[Sensoray826::load_sensor_d] = m_board.getLoadSensor(Sensoray826::load_sensor_d);
    m_error[Sensoray826::load_sensor_d] = -(m_f_d_ref - m_load[Sensoray826::load_sensor_d]);
    
    if (m_error[Sensoray826::load_sensor_d] >= epsilon) {
        m_v_tendon_d = m_v_tendon_rel_nom;
    } else if (m_error[Sensoray826::load_sensor_u] <= -epsilon) {
        m_v_tendon_d = -m_v_tendon_rel_nom;
    }

    m_v_tendon_d = m_v_probe + m_v_tendon_d;

    // std::cout << "error = "<< m_error[Sensoray826::load_sensor_d]  << "v tendon d = " << m_v_tendon_d << std::endl;

    m_board.motorOn(Sensoray826::tendon_d, m_v_tendon_d);
}

void Controller::pidControl() {
    float pid;

    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    m_error[Sensoray826::load_sensor_u] = -(m_f_u_ref - m_load[Sensoray826::load_sensor_u]);

    t_prev_error[Sensoray826::load_sensor_u].push_back(m_error[Sensoray826::load_sensor_u]);
    t_prev_error[Sensoray826::load_sensor_u].erase(t_prev_error[Sensoray826::load_sensor_u].begin());

    pid = m_k_p * m_error[Sensoray826::load_sensor_u] + m_k_i * (std::reduce(t_prev_error[Sensoray826::load_sensor_u].begin(), t_prev_error[Sensoray826::load_sensor_u].end())) / m_integration_period
            + m_k_d * (m_error[Sensoray826::load_sensor_u] - m_prev_error[Sensoray826::load_sensor_u]);

    // m_v_tendon_u += pid;
    m_v_tendon_u = pid;

    m_prev_error[Sensoray826::load_sensor_u] = m_error[Sensoray826::load_sensor_u];

    if (m_v_tendon_u >= m_v_tendon_rel_nom) {
        m_v_tendon_u = m_v_tendon_rel_nom;
    } else if (m_v_tendon_u <= -m_v_tendon_rel_nom) {
        m_v_tendon_u = -m_v_tendon_rel_nom;
    }

    m_v_tendon_u = m_v_probe + m_v_tendon_u;
    
    m_board.motorOn(Sensoray826::tendon_u, m_v_tendon_u);


    m_load[Sensoray826::load_sensor_d] = m_board.getLoadSensor(Sensoray826::load_sensor_d);
    m_error[Sensoray826::load_sensor_d] = -(m_f_d_ref - m_load[Sensoray826::load_sensor_d]);

    t_prev_error[Sensoray826::load_sensor_d].push_back(m_error[Sensoray826::load_sensor_d]);
    t_prev_error[Sensoray826::load_sensor_d].erase(t_prev_error[Sensoray826::load_sensor_d].begin());

    pid = 0.5 * m_k_p * m_error[Sensoray826::load_sensor_d] + m_k_i * (std::reduce(t_prev_error[Sensoray826::load_sensor_d].begin(), t_prev_error[Sensoray826::load_sensor_d].end())) / m_integration_period
            + m_k_d * (m_error[Sensoray826::load_sensor_d] - m_prev_error[Sensoray826::load_sensor_d]);
    
    // m_v_tendon_d += pid;
    m_v_tendon_d = pid;

    if (m_v_tendon_d >= m_v_tendon_rel_nom) {
        m_v_tendon_d = m_v_tendon_rel_nom;
    } else if (m_v_tendon_d <= -m_v_tendon_rel_nom) {
        m_v_tendon_d = -m_v_tendon_rel_nom;
    }

    m_v_tendon_d = m_v_probe + m_v_tendon_d;

    m_prev_error[Sensoray826::load_sensor_d] = m_error[Sensoray826::load_sensor_d];

    m_board.motorOn(Sensoray826::tendon_d, m_v_tendon_d);
}

//-------------GET-SET
void Controller::setVProbeNom(float v_probe_nom) {
    m_v_probe_nom = v_probe_nom;
}

void Controller::setVTendonRelNom(float v_tendon_rel_nom) {
    m_v_tendon_rel_nom = v_tendon_rel_nom;
}

void Controller::setXProbeMax(float x_probe_max) {
    m_x_probe_max = x_probe_max;
}

void Controller::setFMin(float f_min) {
    m_f_min = f_min;
}

void Controller::setFRef(float f_ref) {
    if (f_ref > m_f_min) {
        m_f_u_ref = f_ref;
        m_f_d_ref = m_f_min;
    } else if (f_ref < -m_f_min) {
        m_f_d_ref = -f_ref;
        m_f_u_ref = m_f_min;
    } else {
        m_f_u_ref = m_f_min;
        m_f_d_ref = m_f_min;
    }
}

void Controller::setKP(float k_p) {
    m_k_p = k_p;
}

void Controller::setKI(float k_i) {
    m_k_i = k_i;
}

void Controller::setKD(float k_d) {
    m_k_d = k_d;
}

float Controller::getFURef() {
    return m_f_u_ref;
}

float Controller::getFDRef() {
    return m_f_d_ref;
}