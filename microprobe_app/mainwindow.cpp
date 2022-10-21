#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Sensoray826 board, QWidget *parent)
    : m_board(board),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    
{
    ui->setupUi(this);

    m_fwd_button = findChild<QPushButton*>("pushButton");
    m_bwd_button = findChild<QPushButton*>("pushButton_2");
    m_l_button = findChild<QPushButton*>("pushButton_3");
    m_r_button = findChild<QPushButton*>("pushButton_4");
    
    m_board.DioSourceReset();

    // uint ontime = 1000, offtime = 1000;
	// board.CreatePWM(3, ontime, offtime);
	// board.RouteCounterOutput(3, 11);
	// board.StartPWM(3);


    connect(m_r_button, SIGNAL(pressed()), this, SLOT(ActivateMotor()));
    connect(m_l_button, SIGNAL(pressed()), this, SLOT(ActivateMotor()));
    connect(m_fwd_button, SIGNAL(pressed()), this, SLOT(ActivateMotor()));

}

MainWindow::~MainWindow()
{
    m_board.DioSourceReset();
    m_board.Close();
    delete ui;
}

void MainWindow::ActivateMotor() {
    QObject* sender_obj = sender();
    // QPushButton* button = qobject_cast<QPushButton*>(sender());
    uint dir = 0;
    Motor motor = tendon_r;

    if (sender_obj == m_r_button) {
        std::cout << "right button pressed" << std::endl;
    } else if (sender_obj == m_l_button) {
        std::cout << "left button pressed" << std::endl;
        dir = 1;
    } else if (sender_obj == m_fwd_button) {
        motor = probe;
    }
    
    
    m_board.SetMotorDirection(motor, dir);
    m_board.MotorOn(motor);
    Sleep(60);
    m_board.MotorOff(motor);
}