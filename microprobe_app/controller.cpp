#include "controller.h"

using namespace std;

Controller::Controller(Sensoray826 board)
    : m_board(board) {
    
}

void Controller::startSetup() {
    m_f_u_ref = m_f_min;
    m_f_d_ref = m_f_min;
}

void Controller::start(float control_freq) {
    if (m_v_probe == 0.) {
        m_max_loops = 10;
    } else {
        m_max_loops = (m_x_probe_max/m_v_probe) * control_freq;
    }
    std::cout << m_max_loops << std::endl;
    m_board.setMotorDirection(Sensoray826::probe, Sensoray826::forward);
    m_board.motorOn(Sensoray826::probe);
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
    float epsilon = 5.;

    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    if (m_load[Sensoray826::load_sensor_u]>(m_f_u_ref+epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::release);
        m_board.motorOn(Sensoray826::tendon_u);
    } else if (m_load[Sensoray826::load_sensor_u]<(m_f_u_ref-epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::reel);
        m_board.motorOn(Sensoray826::tendon_u);
    } else {
        m_board.motorOff(Sensoray826::tendon_u);
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
    }
}

// DataSaver data_saver;
//     data_saver.createCsv("experiment_insertion.csv");
//     data_saver.writeCsv(k, f_data);
//     data_saver.closeCsv();

bool Controller::controlLoop() {
    float epsilon = 5.;


    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    if (m_load[Sensoray826::load_sensor_u]>(m_f_u_ref+epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::release);
        m_board.motorOn(Sensoray826::tendon_u);
    } else if (m_load[Sensoray826::load_sensor_u]<(m_f_u_ref-epsilon)) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::reel);
        m_board.motorOn(Sensoray826::tendon_u);
    } else {
        m_board.motorOff(Sensoray826::tendon_u);
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
    }

    m_loop_iter++;

    if(m_loop_iter > m_max_loops) {
        m_loop_iter = 0;
        this->stop();
        return true;
    }
    return false;
}

bool Controller::controlLoopPID() {
    float epsilon = 5., v = 0.;


    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    m_error[Sensoray826::load_sensor_u] = m_f_u_ref - m_load[Sensoray826::load_sensor_u];
    v = m_k_p * m_error[Sensoray826::load_sensor_u] + m_k_i * (m_error[Sensoray826::load_sensor_u] + m_prev_error[Sensoray826::load_sensor_u]) + m_k_d * (m_error[Sensoray826::load_sensor_u] - m_prev_error[Sensoray826::load_sensor_u]);
    if (v >= m_v_tendon_nom) {
        v = m_v_tendon_nom;
    }

    m_prev_error[Sensoray826::load_sensor_u] = m_error[Sensoray826::load_sensor_u];

    // std::cout << "PID out = " << v << std::endl;
    if (m_error[Sensoray826::load_sensor_u] >= epsilon) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::reel);
        m_board.setMotorSpeed(Sensoray826::tendon_u, v);
        m_board.motorOn(Sensoray826::tendon_u);
    } else if (m_error[Sensoray826::load_sensor_u] <= -epsilon) {
        m_board.setMotorDirection(Sensoray826::tendon_u, Sensoray826::release);
        m_board.setMotorSpeed(Sensoray826::tendon_u, v);
        m_board.motorOn(Sensoray826::tendon_u);
    } else {
        m_board.motorOff(Sensoray826::tendon_u);
    }
    

    m_load[Sensoray826::load_sensor_d] = m_board.getLoadSensor(Sensoray826::load_sensor_d);
    m_error[Sensoray826::load_sensor_d] = m_f_u_ref - m_load[Sensoray826::load_sensor_d];
    v = m_k_p * m_error[Sensoray826::load_sensor_d] + m_k_i * (m_error[Sensoray826::load_sensor_d] + m_prev_error[Sensoray826::load_sensor_d]) + m_k_d * (m_error[Sensoray826::load_sensor_d] - m_prev_error[Sensoray826::load_sensor_d]);
    
    if (v >= m_v_tendon_nom) {
        v = m_v_tendon_nom;
    }

    m_prev_error[Sensoray826::load_sensor_d] = m_error[Sensoray826::load_sensor_d];

    if (m_error[Sensoray826::load_sensor_d] >= epsilon) {
        m_board.setMotorDirection(Sensoray826::tendon_d, Sensoray826::reel);
        m_board.setMotorSpeed(Sensoray826::tendon_d, v);
        m_board.motorOn(Sensoray826::tendon_d);
    } else if (m_error[Sensoray826::load_sensor_d] <= -epsilon) {
        m_board.setMotorDirection(Sensoray826::tendon_d, Sensoray826::release);
        m_board.setMotorSpeed(Sensoray826::tendon_d, v);
        m_board.motorOn(Sensoray826::tendon_d);
    } else {
        m_board.motorOff(Sensoray826::tendon_d);
    }

    m_loop_iter++;

    if(m_loop_iter > m_max_loops) {
        m_loop_iter = 0;
        this->stop();
        return true;
    }
    return false;
}

//-------------GET-SET
void Controller::setVProbe(float v_probe) {
    m_v_probe = v_probe;
    m_board.setMotorSpeed(Sensoray826::probe, m_v_probe);
}

void Controller::setVTendonNom(float v_tendon_nom) {
    m_v_tendon_nom = v_tendon_nom;
    m_board.setMotorSpeed(Sensoray826::tendon_u, m_v_tendon_nom);
    m_board.setMotorSpeed(Sensoray826::tendon_d, m_v_tendon_nom);
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
        m_f_d_ref = f_ref;
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