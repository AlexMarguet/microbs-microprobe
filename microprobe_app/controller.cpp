#include "controller.h"

using namespace std;

Controller::Controller() {
    
}

void Controller::setup() {
    m_board.AdcSetup();
    float f = 0.;
    int k = 0;
    
    // while(f < Sensoray826::tendon_f_min && k < 1000) {
        while(k < 1000) {

        f = m_board.getLoadSensor(Sensoray826::load_sensor_r);
        cout << f << endl;
        k++;
        // Sleep(100);
    }
}

void Controller::insertion() {
    float f = 0;
    float f_data[max_iter] = {};
    int k = 0;

    m_board.AdcSetup();
    m_board.MotorOn(Sensoray826::probe);

    while(f < Sensoray826::tendon_f_max && k < 1000) {
        f = m_board.getLoadSensor(Sensoray826::load_sensor_r);
        f_data[k] = f;
        k++;

        if (f <= Sensoray826::tendon_f_min - 3) {
            m_board.SetMotorDirection(Sensoray826::tendon_r, Sensoray826::low);
            m_board.MotorOn(Sensoray826::tendon_r);
        } else if (f >= Sensoray826::tendon_f_min + 3) {
            m_board.SetMotorDirection(Sensoray826::tendon_r, Sensoray826::high);
            m_board.MotorOn(Sensoray826::tendon_r);
        } else {
            m_board.MotorOff(Sensoray826::tendon_r);
        }
    }

    m_board.MotorOff(Sensoray826::probe);
    m_board.MotorOff(Sensoray826::tendon_r);
    // m_board.MotorOff(tendon_l);

    DataSaver data_saver;
    data_saver.createCsv("experiment_insertion.csv");
    data_saver.writeCsv(k, f_data);
    data_saver.closeCsv();
}