#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>

#include "sensoray/826.h"
#include "tools/data_saver.h"
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Sensoray826 board, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void activateMotor();

    void turnOffMotor();

    void launchScript();

    void updateValue();

private:
    Ui::MainWindow *ui;
    Sensoray826 m_board;
    Controller m_controller;
    DataSaver m_data_saver;

    QTimer* m_timer;

    QPushButton* m_probe_fwd_button;
    QPushButton* m_probe_bwd_button;
    QPushButton* m_tendon_u_fwd_button;
    QPushButton* m_tendon_u_bwd_button;
    QPushButton* m_tendon_d_fwd_button;
    QPushButton* m_tendon_d_bwd_button;
    
    QPushButton* m_setup_button;
    QPushButton* m_insertion_button;
    QPushButton* m_calibration_button;

    QLineEdit* m_load_sensor_u;
    QLineEdit* m_load_sensor_d;
    
};
#endif // MAINWINDOW_H
