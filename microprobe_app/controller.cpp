#include "controller.h"

using namespace std;

Controller::Controller() {
    
}

void Controller::setup() {
    float f = 0.;
    int k = 0;
    
    // while(f < Sensoray826::tendon_f_min && k < 1000) {
    while(k < 1000) {

        f = m_board.getLoadSensor(Sensoray826::load_sensor_u);
        k++;
        // Sleep(100);
    }
}

void Controller::insertion() {
    float f = 0;
    float epsilon = 1.;
    float f_data[max_iter] = {};
    int k = 0;

    int forward = 0, backward = 1;

    // m_board.MotorOn(Sensoray826::probe);

    while(f > Sensoray826::tendon_f_max && k < 1000) {
        f = m_board.getLoadSensor(Sensoray826::load_sensor_u);
        f_data[k] = f;
        k++;

        if (f >= Sensoray826::tendon_f_min + epsilon) {
            m_board.setMotorDirection(Sensoray826::tendon_u, forward);
            m_board.motorOn(Sensoray826::tendon_u);
        } else if (f <= Sensoray826::tendon_f_min - epsilon) {
            m_board.setMotorDirection(Sensoray826::tendon_u, backward);
            m_board.motorOn(Sensoray826::tendon_u);
        } else {
            m_board.motorOff(Sensoray826::tendon_u);
        }
        Sleep(20);
        cout << "loop: " << k << endl;
    }

    m_board.motorOff(Sensoray826::probe);
    m_board.motorOff(Sensoray826::tendon_u);
    // m_board.MotorOff(tendon_l);

    DataSaver data_saver;
    data_saver.createCsv("experiment_insertion.csv");
    data_saver.writeCsv(k, f_data);
    data_saver.closeCsv();
}

void Controller::controlLoop() {

    // float v_fwd, angle;
    // if (mouse) {
    //     get3dMousePosition(&v_fwd, &angle);
    // }  
    // if (gui) {
    //     getGuiTrajectory(&v_fwd, &angle);
    // }

    // m_probe_speed = v_fwd;
    // float differential_load = angleToMotorSpeed(angle);
    // m_load_u_ref = base_load + differential_load;
    // m_load_d_ref = base_load - differential_load;

    // load_u, load_d = m_board.getLoadSensor(both);

    // v_motor = pidCalc(load, ref);
    // m_board.setMotorSpeed(v_motor);

}