#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Sensoray826 board, QWidget *parent)
    : m_board(board),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    
{
    m_board.dioSourceReset();

    ui->setupUi(this);

    m_probe_fwd_button = findChild<QPushButton*>("pushButton");
    m_probe_bwd_button = findChild<QPushButton*>("pushButton_2");
    m_tendon_r_bwd_button = findChild<QPushButton*>("pushButton_3");
    m_tendon_r_fwd_button = findChild<QPushButton*>("pushButton_4");
    m_tendon_l_bwd_button = findChild<QPushButton*>("pushButton_8");
    m_tendon_l_fwd_button = findChild<QPushButton*>("pushButton_11");

    m_setup_button = findChild<QPushButton*>("pushButton_5");
    m_insertion_button = findChild<QPushButton*>("pushButton_6");
    m_calibration_button = findChild<QPushButton*>("pushButton_7");

    m_load_sensor = findChild<QLineEdit*>("lineEdit");
    m_load_sensor->setText(QString::number(1.5));
    
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateValue()));
    m_timer->start(200);

    connect(m_probe_fwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_probe_bwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_r_fwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_r_bwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_l_fwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));
    connect(m_tendon_l_bwd_button, SIGNAL(pressed()), this, SLOT(activateMotor()));

    connect(m_probe_fwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_probe_bwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_r_fwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_r_bwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_l_fwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));
    connect(m_tendon_l_bwd_button, SIGNAL(released()), this, SLOT(turnOffMotor()));

    connect(m_setup_button, SIGNAL(pressed()), this, SLOT(launchScript()));
    connect(m_insertion_button, SIGNAL(pressed()), this, SLOT(launchScript()));
    connect(m_calibration_button, SIGNAL(pressed()), this, SLOT(launchScript()));
}

MainWindow::~MainWindow()
{
    m_data_saver.closeCsv();
    m_board.dioSourceReset();
    m_board.close();
    delete ui;
}

void MainWindow::activateMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    uint dir = 0;
    Sensoray826::Motor motor = Sensoray826::tendon_r;

    if (sender_obj == m_tendon_r_fwd_button) {
        motor = Sensoray826::tendon_r;
        dir = 0;
    } else if (sender_obj == m_tendon_r_bwd_button) {
        motor = Sensoray826::tendon_r;
        dir = 1;
    } else if (sender_obj == m_tendon_l_fwd_button) {
        motor = Sensoray826::tendon_l;
        dir = 0;
    } else if (sender_obj == m_tendon_l_bwd_button) {
        motor = Sensoray826::tendon_l;
        dir = 1;
    } else if (sender_obj == m_probe_fwd_button) {
        motor = Sensoray826::probe;
        dir = 1;
    } else if (sender_obj == m_probe_bwd_button) {
        motor = Sensoray826::probe;
        dir = 0;
    }
    
    m_board.setMotorDirection(motor, dir);
    m_board.motorOn(motor);
}

void MainWindow::turnOffMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    Sensoray826::Motor motor = Sensoray826::tendon_r;

    if (sender_obj == m_tendon_r_fwd_button || sender_obj == m_tendon_r_bwd_button) {
        motor = Sensoray826::tendon_r;
    } else if (sender_obj == m_tendon_l_fwd_button || sender_obj == m_tendon_l_bwd_button) {
        motor = Sensoray826::tendon_l;
    } else if (sender_obj == m_probe_fwd_button || sender_obj == m_probe_bwd_button) {
        motor = Sensoray826::probe;
    }
    
    m_board.motorOff(motor);
}


void MainWindow::launchScript() {
    QObject* sender_obj = sender();

    if (sender_obj == m_setup_button) {
        std::cout << "setup" << std::endl;
        this->m_controller.setup();
    } else if (sender_obj == m_calibration_button) {
        std::cout << "calibration" << std::endl;
        this->m_board.loadSensorCalibration(Sensoray826::load_sensor_r);
    } else if (sender_obj == m_insertion_button) {
        std::cout << "insertion" << std::endl;
        this->m_controller.insertion();
    }
}

void MainWindow::updateValue() {
    float load_r = m_board.getLoadSensor(Sensoray826::load_sensor_r);
    m_load_sensor->setText(QString::number(load_r));
}