#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Sensoray826 board, Controller controller, QWidget *parent)
    : m_board(board),
    m_controller(controller),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_board.dioSourceReset();

    ui->setupUi(this);

    //Control box
    m_probe_fwd_button = findChild<QPushButton*>("pushButton_2");
    m_probe_bwd_button = findChild<QPushButton*>("pushButton");
    m_tendon_u_release_button = findChild<QPushButton*>("pushButton_3");
    m_tendon_u_reel_button = findChild<QPushButton*>("pushButton_4");
    m_tendon_d_release_button = findChild<QPushButton*>("pushButton_8");
    m_tendon_d_reel_button = findChild<QPushButton*>("pushButton_11");
    m_hold_checkbox = findChild<QCheckBox*>("checkBox");

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

    connect(m_hold_checkbox, SIGNAL(stateChanged(int)), this, SLOT(hold()));

    //Parameters Box
    m_v_probe = findChild<QLineEdit*>("lineEdit_3");
    m_v_tendon_nom = findChild<QLineEdit*>("lineEdit_9");
    m_f_min = findChild<QLineEdit*>("lineEdit_10");
    m_x_probe_max = findChild<QLineEdit*>("lineEdit_11");
    m_k_p = findChild<QLineEdit*>("lineEdit_8");
    m_k_i = findChild<QLineEdit*>("lineEdit_12");
    m_k_d = findChild<QLineEdit*>("lineEdit_13");
    m_parameters_apply_button = findChild<QPushButton*>("pushButton_18");

    connect(m_parameters_apply_button, SIGNAL(pressed()), this, SLOT(applyParameters()));

    connect(m_v_probe, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_v_tendon_nom, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_f_min, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_x_probe_max, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_k_p, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_k_i, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));
    connect(m_k_d, SIGNAL(textChanged(QString)), this, SLOT(applyParameters()));

    //Insertion box
    m_start_button = findChild<QPushButton*>("pushButton_9");
    m_stop_button = findChild<QPushButton*>("pushButton_10");

    connect(m_start_button, SIGNAL(pressed()), this, SLOT(insertion()));
    connect(m_stop_button, SIGNAL(pressed()), this, SLOT(insertion()));

    //Scripts box
    m_setup_button = findChild<QPushButton*>("pushButton_5");
    m_insertion_button = findChild<QPushButton*>("pushButton_6");
    m_calibration_button = findChild<QPushButton*>("pushButton_7");

    connect(m_setup_button, SIGNAL(pressed()), this, SLOT(launchScript()));
    connect(m_insertion_button, SIGNAL(pressed()), this, SLOT(launchScript()));
    connect(m_calibration_button, SIGNAL(pressed()), this, SLOT(launchScript()));

    //Sensors box
    m_load_sensor_u = findChild<QLineEdit*>("lineEdit");
    m_load_sensor_d = findChild<QLineEdit*>("lineEdit_2");
    m_load_sensor_u_to_zero_button = findChild<QPushButton*>("pushButton_16");
    m_load_sensor_d_to_zero_button = findChild<QPushButton*>("pushButton_17");
    m_f_u_ref = findChild<QLineEdit*>("lineEdit_6");
    m_f_d_ref = findChild<QLineEdit*>("lineEdit_5");

    connect(m_load_sensor_u_to_zero_button, SIGNAL(pressed()), this, SLOT(launchScript()));
    connect(m_load_sensor_d_to_zero_button, SIGNAL(pressed()), this, SLOT(launchScript()));


    m_dio_reset_button = findChild<QPushButton*>("pushButton_12");
    connect(m_dio_reset_button, SIGNAL(pressed()), this, SLOT(launchScript()));

    //Timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateValue()));
    m_timer->start(500);

    this->applyParameters();
}

MainWindow::~MainWindow()
{
    // m_data_saver.closeCsv();
    m_board.dioSourceReset();
    m_board.close();
    delete ui;
}

void MainWindow::activateMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    Sensoray826::Direction direction;
    Sensoray826::Motor motor;

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
    
    m_board.setMotorDirection(motor, direction);
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
    if (sender_obj == m_parameters_apply_button) {

        float v_probe = m_v_probe->text().toFloat();
        float v_tendon_nom = m_v_tendon_nom->text().toFloat();
        float f_min = m_f_min->text().toFloat();
        float x_probe_max = m_x_probe_max->text().toFloat();
        float k_p = m_k_p->text().toFloat();
        float k_i = m_k_i->text().toFloat();
        float k_d = m_k_d->text().toFloat();
        
        m_controller.setVProbe(v_probe);
        m_controller.setVTendonNom(v_tendon_nom);
        m_controller.setFMin(f_min);
        m_controller.setXProbeMax(x_probe_max);
        m_controller.setKP(k_p);
        m_controller.setKI(k_i);
        m_controller.setKD(k_d);

        m_parameters_apply_button->setEnabled(false);
    } else if (sender_obj != nullptr) {
        m_parameters_apply_button->setEnabled(true);
    }
}

void MainWindow::hold() {
    if (m_hold_checkbox.isChecked()) {
        m_controller.startSetup();
        m_control_loop_timer = new QTimer(this);
        connect(m_control_loop_timer, SIGNAL(timeout()), this, SLOT(setup()));
        m_control_loop_timer->start(100);
    } else {
        m_control_loop_timer->stop();
    }
}

void MainWindow::fRefModif() {
    QObject* sender_obj = sender();
    if (sender_obj == m_f_ref_inc_button) {
        m_f_ref += m_f_increment->text().toFloat();
        m_controller.setFRef(m_f_ref);
    } else if (sender_obj == m_f_ref_inc_button) {
        m_f_ref -= m_f_increment->text().toFloat();
        m_controller.setFRef(m_f_ref);
    }
}

void MainWindow::insertion() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (sender_obj == m_start_button) {
        m_control_loop_timer = new QTimer(this);
        connect(m_control_loop_timer, SIGNAL(timeout()), this, SLOT(controlLoop()));

        m_controller.start();
        m_control_loop_timer->start(100);
    } else if (sender_obj == m_stop_button) {
        m_controller.stop();
        m_control_loop_timer->stop();
    }
}

void MainWindow::setup() {
    if(this->m_controller.setup()) {
        m_control_loop_timer->stop();
    }
}

void MainWindow::controlLoop() {
    if(this->m_controller.controlLoop()) {
        m_controller.stop();
        m_control_loop_timer->stop();
    }
}


void MainWindow::launchScript() {
    QObject* sender_obj = sender();

    if (sender_obj == m_setup_button) {
        std::cout << "setup" << std::endl;
        m_controller.startSetup();
        m_control_loop_timer = new QTimer(this);
        connect(m_control_loop_timer, SIGNAL(timeout()), this, SLOT(setup()));
        m_control_loop_timer->start(100);
    } else if (sender_obj == m_calibration_button) {
        std::cout << "calibration" << std::endl;
        this->m_board.loadSensorCalibration(Sensoray826::load_sensor_u);
    } else if (sender_obj == m_insertion_button) {
        std::cout << "insertion" << std::endl;
        this->m_controller.insertion();
    } else if (sender_obj == m_dio_reset_button) {
        std::cout << "dio reset" << std::endl;
        this->m_board.dioSourceReset();
    } else if (sender_obj == m_load_sensor_u_to_zero_button) {
        this->m_board.loadSensorOffsetCalibration(Sensoray826::load_sensor_u);
    } else if (sender_obj == m_load_sensor_d_to_zero_button) {
        this->m_board.loadSensorOffsetCalibration(Sensoray826::load_sensor_d);
    }
}

void MainWindow::updateValue() {
    m_load_sensor_u->setText(QString::number(m_board.getLoadSensor(Sensoray826::load_sensor_u)));
    m_load_sensor_d->setText(QString::number(m_board.getLoadSensor(Sensoray826::load_sensor_d)));

    m_f_u_ref->setText(QString::number(m_controller.getFURef()));
    m_f_d_ref->setText(QString::number(m_controller.getFDRef()));  
}