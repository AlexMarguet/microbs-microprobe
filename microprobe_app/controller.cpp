#include "controller.h"

using namespace std;

void Controller::Controller() {
    
}

void Controller::setup() {
    m_board.AdcSetup();
    float f = 0.;
    int k = 0;
    
    while(f < Sensoray826::tendon_f_min && k < 1000) {
        f = m_board.getLoad();
        cout << f << endl;
        k++;
    }
}

void Controller::insertion() {
    float f = 0;
    float f_data[max_iter] = {};
    int k = 0;

    m_board.AdcSetup();
    m_board.MotorOn(probe);

    while(f < Sensoray826::tendon_f_max && k < 1000) {
        f = m_board.getLoad();
        f_data[k] = f;
        k++;

        if (f <= Sensoray826::tendon_f_min - 3) {
            m_board.SetMotorDirection(tendon_r, low);
            m_board.MotorOn(tendon_r);
        } else if (f >= Sensoray826::tendon_f_min + 3) {
            m_board.SetMotorDirection(tendon_r, high);
            m_board.MotorOn(tendon_r);
        } else {
            m_board.MotorOff(tendon_r);
        }
    }

    m_board.MotorOff(probe);
    m_board.MotorOff(tendon_r);
    // m_board.MotorOff(tendon_l);

    DataSaver data_saver;
    data_saver.createCsv("experiment_insertion.csv");
    data_saver.writeCsv(k, f_data);
    data_saver.closeCsv();
}