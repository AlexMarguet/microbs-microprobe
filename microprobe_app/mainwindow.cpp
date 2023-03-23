#include "mainwindow.h"
#include "./ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(Sensoray826& board, Controller& controller, DataSaver& data_saver, QWidget *parent)
    : m_board(board),
    m_controller(controller),
    m_data_saver(data_saver),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_board.dioSourceReset();

    /*
    Generate the window setup from mainwindow.ui.
    Link all QObjects to actual members with findChild().
    Connect object signals to slot functions (callbacks) with connect().
    */
    ui->setupUi(this);

    //Control box
    m_probe_fwd_button = findChild<QPushButton*>("pushButton_2");
    m_probe_bwd_button = findChild<QPushButton*>("pushButton");
    m_tendon_u_release_button = findChild<QPushButton*>("pushButton_3");
    m_tendon_u_reel_button = findChild<QPushButton*>("pushButton_4");
    m_tendon_d_release_button = findChild<QPushButton*>("pushButton_8");
    m_tendon_d_reel_button = findChild<QPushButton*>("pushButton_11");
    m_hold_checkbox = findChild<QCheckBox*>("checkBox");
    m_manual_v_p_lineedit = findChild<QLineEdit*>("lineEdit_15");
    m_manual_v_t_lineedit = findChild<QLineEdit*>("lineEdit_16");

    connect(m_probe_fwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_probe_bwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_u_reel_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_u_release_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_d_reel_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_d_release_button, SIGNAL(pressed()), this, SLOT(activateMotor()));

    connect(m_probe_fwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_probe_bwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_u_reel_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_u_release_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_d_reel_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_d_release_button, SIGNAL(released()), this, SLOT(turnOffMotor()));

    connect(m_hold_checkbox, SIGNAL(stateChanged(int)), this, SLOT(holdCheckbox()));

    //Parameters Box
    m_v_probe_nom = findChild<QLineEdit*>("lineEdit_3");
    m_v_tendon_rel_nom = findChild<QLineEdit*>("lineEdit_9");
    m_f_min = findChild<QLineEdit*>("lineEdit_10");
    m_x_probe_max = findChild<QLineEdit*>("lineEdit_11");
    m_k_p = findChild<QLineEdit*>("lineEdit_8");
    m_k_i = findChild<QLineEdit*>("lineEdit_12");
    m_k_d = findChild<QLineEdit*>("lineEdit_13");
    m_parameters_apply_button = findChild<QPushButton*>("pushButton_18");

    connect(m_parameters_apply_button, SIGNAL(pressed()), this, SLOT(applyParameters()));

    connect(m_v_probe_nom, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_v_tendon_rel_nom, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_f_min, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_x_probe_max, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_k_p, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_k_i, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_k_d, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));

    //Insertion box
    m_start_button = findChild<QPushButton*>("pushButton_9");
    m_stop_button = findChild<QPushButton*>("pushButton_10");
    m_reel_back_button = findChild<QPushButton*>("pushButton_21");
    m_f_ref_inc_button = findChild<QPushButton*>("pushButton_14");
    m_f_ref_dec_button = findChild<QPushButton*>("pushButton_15");
    m_f_increment = findChild<QLineEdit*>("lineEdit_7");
    m_f_ref_lineedit = findChild<QLineEdit*>("lineEdit_4");
    m_preset_checkbox = findChild<QCheckBox*>("checkBox_2");

    connect(m_start_button, SIGNAL(pressed()), this, SLOT(insertion()));
    connect(m_stop_button, SIGNAL(pressed()), this, SLOT(insertion()));
    connect(m_reel_back_button, SIGNAL(pressed()), this, SLOT(insertion()));
    connect(m_f_ref_inc_button, SIGNAL(pressed()), this, SLOT(fRefModif()));
    connect(m_f_ref_dec_button, SIGNAL(pressed()), this, SLOT(fRefModif()));

    //Sensors box
    m_load_sensor_u = findChild<QLineEdit*>("lineEdit");
    m_load_sensor_d = findChild<QLineEdit*>("lineEdit_2");
    m_load_sensor_u_to_zero_button = findChild<QPushButton*>("pushButton_16");
    m_load_sensor_d_to_zero_button = findChild<QPushButton*>("pushButton_17");
    m_f_u_ref = findChild<QLineEdit*>("lineEdit_6");
    m_f_d_ref = findChild<QLineEdit*>("lineEdit_5");

    connect(m_load_sensor_u_to_zero_button, SIGNAL(pressed()), this, SLOT(launchScript()));
    connect(m_load_sensor_d_to_zero_button, SIGNAL(pressed()), this, SLOT(launchScript()));

    //Datasave box
    m_record_button = findChild<QPushButton*>("pushButton_19");
    m_file_name_lineedit = findChild<QLineEdit*>("lineEdit_14");

    connect(m_record_button, SIGNAL(pressed()), this, SLOT(dataRecord()));

    m_emergency_stop_button = findChild<QPushButton*>("pushButton_12");
    connect(m_emergency_stop_button, SIGNAL(pressed()), this, SLOT(launchScript()));

    //Preset box
    m_preset_add_button = findChild<QPushButton*>("pushButton_5");
    m_preset_reset_button = findChild<QPushButton*>("pushButton_6");
    m_preset_start_lineedit = findChild<QLineEdit*>("lineEdit_17");
    m_preset_stop_lineedit = findChild<QLineEdit*>("lineEdit_18");
    m_preset_force_start_lineedit = findChild<QLineEdit*>("lineEdit_19");
    m_preset_force_stop_lineedit = findChild<QLineEdit*>("lineEdit_20");
    m_preset_step = findChild<QCheckBox*>("checkBox_3");
    m_preset_ramp = findChild<QCheckBox*>("checkBox_4");

    connect(m_preset_add_button, SIGNAL(pressed()), this, SLOT(presetConfig()));
    connect(m_preset_reset_button, SIGNAL(pressed()), this, SLOT(presetConfig()));
    connect(m_preset_step, SIGNAL(clicked()), this, SLOT(presetConfig()));
    connect(m_preset_ramp, SIGNAL(clicked()), this, SLOT(presetConfig()));

    //Timer
    /*
    One timer is used to periodically refresh sensor value in the GUI.
    One is used as the loop trigger for controller events.
    */
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateValue()));
    m_timer->start(505);
    m_control_loop_timer = new QTimer(this);
    m_control_loop_timer->setTimerType(Qt::PreciseTimer);

    this->applyParameters();
}

MainWindow::~MainWindow()
{
    m_controller.stop();
    m_board.dioSourceReset();
    m_board.close();
    delete ui;
}

void MainWindow::activateMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    Sensoray826::Direction direction;
    Sensoray826::Motor motor;
    float speed = 0;

    if (sender_obj == m_tendon_u_reel_button) {
        motor = Sensoray826::tendon_u;
        direction = Sensoray826::reel;
    } else if (sender_obj == m_tendon_u_release_button) {
        motor = Sensoray826::tendon_u;
        direction = Sensoray826::release;
    } else if (sender_obj == m_tendon_d_reel_button) {
        motor = Sensoray826::tendon_d;
        direction = Sensoray826::reel;
    } else if (sender_obj == m_tendon_d_release_button) {
        motor = Sensoray826::tendon_d;
        direction = Sensoray826::release;
    } else if (sender_obj == m_probe_fwd_button) {
        motor = Sensoray826::probe;
        direction = Sensoray826::forward;
    } else if (sender_obj == m_probe_bwd_button) {
        motor = Sensoray826::probe;
        direction = Sensoray826::backward;
    }
    
    if (motor == Sensoray826::tendon_u || motor == Sensoray826::tendon_d) {
        speed = m_manual_v_t_lineedit->text().toFloat();
    } else {
        speed = m_manual_v_p_lineedit->text().toFloat();
    }
    m_board.setMotorDirection(motor, direction);
    m_board.setMotorSpeed(motor, speed);
    m_board.motorOn(motor);
}

void MainWindow::turnOffMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    Sensoray826::Motor motor = Sensoray826::tendon_u;

    if (sender_obj == m_tendon_u_reel_button || sender_obj == m_tendon_u_release_button) {
        motor = Sensoray826::tendon_u;
    } else if (sender_obj == m_tendon_d_reel_button || sender_obj == m_tendon_d_release_button) {
        motor = Sensoray826::tendon_d;
    } else if (sender_obj == m_probe_fwd_button || sender_obj == m_probe_bwd_button) {
        motor = Sensoray826::probe;
    }
    
    m_board.motorOff(motor);
}

void MainWindow::applyParameters() {
    QObject* sender_obj = sender();
    if (sender_obj == m_parameters_apply_button || sender_obj == nullptr) {
        float v_probe_nom = m_v_probe_nom->text().toFloat();
        float v_tendon_rel_nom = m_v_tendon_rel_nom->text().toFloat();
        float f_min = m_f_min->text().toFloat();
        float x_probe_max = m_x_probe_max->text().toFloat();
        float k_p = m_k_p->text().toFloat();
        float k_i = m_k_i->text().toFloat();
        float k_d = m_k_d->text().toFloat();
        
        m_controller.setVProbeNom(v_probe_nom);
        m_controller.setVTendonRelNom(v_tendon_rel_nom);
        m_controller.setFMin(f_min);
        m_controller.setFRef(f_min);
        m_controller.setXProbeMax(x_probe_max);
        m_controller.setKP(k_p);
        m_controller.setKI(k_i);
        m_controller.setKD(k_d);

        m_parameters_apply_button->setEnabled(false);
    } else if (sender_obj != nullptr) {
        /*
        Each textline modification triggers this callback and enables the apply button.
        Modification is registered as soon as the value changes, which is why we use an apply button to prevent absurd values during editing.
        */
        m_parameters_apply_button->setEnabled(true);
    }
}

/*
Hold is currently linked to controller's fixedSpeedControl.
This can be changed in the controller.hold() function, just make sure to reset previous force error values.
*/
void MainWindow::holdCheckbox() {
    if (m_hold_checkbox->isChecked()) {
        m_controller.startHold();
        delete m_control_loop_timer;
        m_control_loop_timer = new QTimer(this);
        m_control_loop_timer->setTimerType(Qt::PreciseTimer);
        connect(m_control_loop_timer, SIGNAL(timeout()), this, SLOT(hold()));
        m_control_loop_timer->start(100);
    } else {
        m_control_loop_timer->stop();
        m_controller.stop();
    }
}

void MainWindow::hold() {
    this->m_controller.hold();
}

void MainWindow::fRefModif() {
    QObject* sender_obj = sender();
    if (sender_obj == m_f_ref_inc_button) {
        m_f_ref += m_f_increment->text().toFloat();
        m_controller.setFRef(m_f_ref);
    } else if (sender_obj == m_f_ref_dec_button) {
        m_f_ref -= m_f_increment->text().toFloat();
        m_controller.setFRef(m_f_ref);
    }
    m_f_ref_lineedit->setText(QString::number(m_f_ref));
}

void MainWindow::insertion() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (sender_obj == m_start_button) {
        //We always delete the control loop timer to disconnect from the other slots it may have been linked to.
        delete m_control_loop_timer;
        m_control_loop_timer = new QTimer(this);
        m_control_loop_timer->setTimerType(Qt::PreciseTimer);
        connect(m_control_loop_timer, SIGNAL(timeout()), this, SLOT(controlLoopPID()));
        m_controller.start(10);
        //Measure real execution duration.
        m_insertion_start_time = chrono::steady_clock::now();
        m_last_start_time = m_insertion_start_time;
        m_control_loop_timer->start(100);
        m_stop_button->setEnabled(true);
    } else if (sender_obj == m_stop_button) {
        m_controller.stop();
        m_control_loop_timer->stop();
        m_stop_button->setEnabled(false);
    } else if (sender_obj == m_reel_back_button) {
        delete m_control_loop_timer;
        m_control_loop_timer = new QTimer(this);
        m_control_loop_timer->setTimerType(Qt::PreciseTimer);
        connect(m_control_loop_timer, SIGNAL(timeout()), this, SLOT(controlLoopPID()));
        m_controller.startReelBack(10);
        m_control_loop_timer->start(100);
    }
}

void MainWindow::controlLoopPID() { // Mean duration: ~0.2 [ms]
    auto start = chrono::steady_clock::now();
    //Uncomment below for loop period.
    // chrono::duration<double, milli> elapsed {start - m_last_start_time};
    // cout << "Loop period: " << elapsed.count() << " ms" << endl;

    chrono::duration<double, milli> elapsed {start - m_insertion_start_time};
    if (m_preset_checkbox->isChecked()) {
        float f = m_preset_experiment.getCurrentFRef(elapsed.count()/1000);
        m_controller.setFRef(f);
        // cout << "Force ref at " << elapsed.count() << " = " << f << endl;
    }

    if(this->m_controller.controlLoopPID()) {
        m_controller.stop();
        m_control_loop_timer->stop();
        m_stop_button->setEnabled(false);
        chrono::duration<double, milli> elapsed {chrono::steady_clock::now() - m_insertion_start_time};
        cout << "Total insertion duration: " << elapsed.count() << " ms" << endl;
    }
    
    m_last_start_time = start;
}

//Small button scripts
void MainWindow::launchScript() {
    QObject* sender_obj = sender();

    if (sender_obj == m_emergency_stop_button) {
        cout << "emergency stop" << endl;
        delete m_control_loop_timer;
        m_control_loop_timer = new QTimer(this);
        m_control_loop_timer->setTimerType(Qt::PreciseTimer);
        m_controller.stop();
        this->m_board.dioSourceReset();
    } else if (sender_obj == m_load_sensor_u_to_zero_button) {
        this->m_board.loadSensorOffsetCalibration(Sensoray826::load_sensor_u);
    } else if (sender_obj == m_load_sensor_d_to_zero_button) {
        this->m_board.loadSensorOffsetCalibration(Sensoray826::load_sensor_d);
    }
}

void MainWindow::updateValue() {
    //Could be improved to call getLoadSensor only in the controller, and display the value without retrieving it from the board again.
    m_load_sensor_u->setText(QString::number(m_board.getLoadSensor(Sensoray826::load_sensor_u)));
    m_load_sensor_d->setText(QString::number(m_board.getLoadSensor(Sensoray826::load_sensor_d)));

    m_f_u_ref->setText(QString::number(m_controller.getFURef()));
    m_f_d_ref->setText(QString::number(m_controller.getFDRef()));  
}

void MainWindow::dataRecord() {
    QObject* sender_obj = sender();

    if (sender_obj == m_record_button) {
        m_data_saver.createCsv(m_file_name_lineedit->text().toStdString());
        //Could be improved to read parameters from the controller to make sure they correspond.
        string params = "kp=" + m_k_p->text().toStdString() + ",ki=" + m_k_i->text().toStdString() + ",kd=" + m_k_d->text().toStdString();
        m_data_saver.writeHeader(params);
    }
}

void MainWindow::presetConfig() {
    QObject* sender_obj = sender();

    if (sender_obj == m_preset_add_button) {
        if (m_preset_step->isChecked()){
            m_preset_experiment.add(m_preset_start_lineedit->text().toFloat(), m_preset_stop_lineedit->text().toFloat(),
                                    m_preset_force_start_lineedit->text().toFloat(), m_preset_force_start_lineedit->text().toFloat(), ExperimentPreset::step);
        } else {
            m_preset_experiment.add(m_preset_start_lineedit->text().toFloat(), m_preset_stop_lineedit->text().toFloat(),
                                    m_preset_force_start_lineedit->text().toFloat(), m_preset_force_stop_lineedit->text().toFloat(), ExperimentPreset::ramp);
        }
    } else if (sender_obj == m_preset_reset_button) {
        m_preset_experiment.reset();
    } else if (sender_obj == m_preset_step) {
        if (m_preset_step->isChecked()) {
            m_preset_ramp->setChecked(false);
            m_preset_force_stop_lineedit->setEnabled(false);
        } else {
            m_preset_ramp->setChecked(true);
            m_preset_force_stop_lineedit->setEnabled(true);
        }

    } else if (sender_obj == m_preset_ramp) {
        if (m_preset_ramp->isChecked()) {
            m_preset_step->setChecked(false);
            m_preset_force_stop_lineedit->setEnabled(true);
        } else {
            m_preset_step->setChecked(true);
            m_preset_force_stop_lineedit->setEnabled(false);
        }
    }
}
