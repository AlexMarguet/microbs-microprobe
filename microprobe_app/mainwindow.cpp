#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Sensoray826 board, QWidget *parent)
    : m_board(board),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    
{
    m_board.DioSourceReset();

    ui->setupUi(this);

    m_fwd_button = findChild<QPushButton*>("pushButton");
    m_bwd_button = findChild<QPushButton*>("pushButton_2");
    m_l_button = findChild<QPushButton*>("pushButton_3");
    m_r_button = findChild<QPushButton*>("pushButton_4");

    m_setup_button = findChild<QPushButton*>("pushButton_5");
    m_insertion_button = findChild<QPushButton*>("pushButton_6");
    m_calibration_button = findChild<QPushButton*>("pushButton_7");

    m_load_sensor = findChild<QLineEdit*>("lineEdit");
    m_load_sensor->setText(QString::number(1.5));
    
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(UpdateValue()));
    m_timer->start(1000);

    connect(m_r_button, SIGNAL(pressed()), this, SLOT(ActivateMotor()));
    connect(m_l_button, SIGNAL(pressed()), this, SLOT(ActivateMotor()));
    connect(m_fwd_button, SIGNAL(pressed()), this, SLOT(ActivateMotor()));

    connect(m_setup_button, SIGNAL(pressed()), this, SLOT(LaunchScript()));
    connect(m_insertion_button, SIGNAL(pressed()), this, SLOT(LaunchScript()));
    connect(m_calibration_button, SIGNAL(pressed()), this, SLOT(LaunchScript()));

    m_data_saver.createCsv("test.csv");
}

MainWindow::~MainWindow()
{
    m_data_saver.closeCsv();
    m_board.DioSourceReset();
    m_board.Close();
    delete ui;
}

void MainWindow::ActivateMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    uint dir = 0;
    Sensoray826::Motor motor = Sensoray826::tendon_r;

    if (sender_obj == m_r_button) {
        std::cout << "right button pressed" << std::endl;
    } else if (sender_obj == m_l_button) {
        std::cout << "left button pressed" << std::endl;
        dir = 1;
    } else if (sender_obj == m_fwd_button) {
        motor = Sensoray826::probe;
    }
    
    
    m_board.SetMotorDirection(motor, dir);
    m_board.MotorOn(motor);
    Sleep(60);
    m_board.MotorOff(motor);
}


void MainWindow::LaunchScript() {
    QObject* sender_obj = sender();

    if (sender_obj == m_setup_button) {
        std::cout << "setup" << std::endl;
        this->m_controller.setup();
    } else if (sender_obj == m_calibration_button) {
        std::cout << "calibration" << std::endl;
        this->m_board.loadSensorCalibration(Sensoray826::load_sensor_r);
    }
}

void MainWindow::UpdateValue() {
    float load_r = m_board.getLoadSensor(Sensoray826::load_sensor_r);
    m_load_sensor->setText(QString::number(load_r));
    m_data_saver.writeCsv(1, &load_r);
    // m_load_sensor->setText(QString::number(m_board.AdcIn()));
}