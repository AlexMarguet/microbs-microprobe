#include "controller.h"

using namespace std;

Controller::Controller(Sensoray826& board, DataSaver& data_saver)
    : m_board(board),
    m_data_saver(data_saver) {
    
    for (int i = 0; i<m_integration_period ; i++) {
        m_prev_error[0].push_back(0);
        m_prev_error[1].push_back(0);
    }
}

void Controller::startHold() {
    m_f_u_ref = m_f_min;
    m_f_d_ref = m_f_min;
}

//Should be improved by resetting the m_prev_error vector. (On start or stop.)
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

void Controller::hold() {
    fixedSpeedControl();
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

//Should ultimately be removed and use exclusively pid control. Currently only used for "hold".
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
    
    m_board.motorOn(Sensoray826::tendon_u, m_v_tendon_u);


    m_load[Sensoray826::load_sensor_d] = m_board.getLoadSensor(Sensoray826::load_sensor_d);
    m_error[Sensoray826::load_sensor_d] = -(m_f_d_ref - m_load[Sensoray826::load_sensor_d]);
    
    if (m_error[Sensoray826::load_sensor_d] >= epsilon) {
        m_v_tendon_d = m_v_tendon_rel_nom;
    } else if (m_error[Sensoray826::load_sensor_u] <= -epsilon) {
        m_v_tendon_d = -m_v_tendon_rel_nom;
    }

    m_v_tendon_d = m_v_probe + m_v_tendon_d;

    m_board.motorOn(Sensoray826::tendon_d, m_v_tendon_d);
}

void Controller::pidControl() {
    float pid;

    m_load[Sensoray826::load_sensor_u] = m_board.getLoadSensor(Sensoray826::load_sensor_u);
    m_error[Sensoray826::load_sensor_u] = -(m_f_u_ref - m_load[Sensoray826::load_sensor_u]);

    //m_prev_error is a vector holding the n-past error values and actual error value.
    m_prev_error[Sensoray826::load_sensor_u].push_back(m_error[Sensoray826::load_sensor_u]);
    m_prev_error[Sensoray826::load_sensor_u].erase(m_prev_error[Sensoray826::load_sensor_u].begin());

    //reduce() sums the values of the vector, end() is a pointer to the past-the-end element which helps access elements from the back.
    pid = m_k_p * m_error[Sensoray826::load_sensor_u] + m_k_i * (std::reduce(m_prev_error[Sensoray826::load_sensor_u].begin(), m_prev_error[Sensoray826::load_sensor_u].end())) / m_integration_period
            + m_k_d * (m_error[Sensoray826::load_sensor_u] - *(m_prev_error[Sensoray826::load_sensor_u].end()-2));

    m_v_tendon_u = pid;

    //Relative velocity capped at maximum value.
    if (m_v_tendon_u >= m_v_tendon_rel_nom) {
        m_v_tendon_u = m_v_tendon_rel_nom;
    } else if (m_v_tendon_u <= -m_v_tendon_rel_nom) {
        m_v_tendon_u = -m_v_tendon_rel_nom;
    }

    /*
    Output velocity relative to the probe velocity. Requires precise knowledge of the actual probe velocity.
    Could be modified by removing the probe velocity and using tendon previous velocity, but all pid coefficients would require large changes.
    */
    m_v_tendon_u = m_v_probe + m_v_tendon_u;
    
    m_board.motorOn(Sensoray826::tendon_u, m_v_tendon_u);


    m_load[Sensoray826::load_sensor_d] = m_board.getLoadSensor(Sensoray826::load_sensor_d);
    m_error[Sensoray826::load_sensor_d] = -(m_f_d_ref - m_load[Sensoray826::load_sensor_d]);

    m_prev_error[Sensoray826::load_sensor_d].push_back(m_error[Sensoray826::load_sensor_d]);
    m_prev_error[Sensoray826::load_sensor_d].erase(m_prev_error[Sensoray826::load_sensor_d].begin());

    //Additional 0.5 proportional factor to have the tendon down working. As long as the setup is asymetric, different pid values for up and down could be an alternative.
    pid = 0.5 * m_k_p * m_error[Sensoray826::load_sensor_d] + m_k_i * (std::reduce(m_prev_error[Sensoray826::load_sensor_d].begin(), m_prev_error[Sensoray826::load_sensor_d].end())) / m_integration_period
            + m_k_d * (m_error[Sensoray826::load_sensor_d] - *(m_prev_error[Sensoray826::load_sensor_d].end()-2));
    
    m_v_tendon_d = pid;

    if (m_v_tendon_d >= m_v_tendon_rel_nom) {
        m_v_tendon_d = m_v_tendon_rel_nom;
    } else if (m_v_tendon_d <= -m_v_tendon_rel_nom) {
        m_v_tendon_d = -m_v_tendon_rel_nom;
    }

    m_v_tendon_d = m_v_probe + m_v_tendon_d;

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

//Sets the force reference for a tendon while keeping minimum tension on the other.
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