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
    QPushButton* m_tendon_r_fwd_button;
    QPushButton* m_tendon_r_bwd_button;
    QPushButton* m_tendon_l_fwd_button;
    QPushButton* m_tendon_l_bwd_button;
    
    QPushButton* m_setup_button;
    QPushButton* m_insertion_button;
    QPushButton* m_calibration_button;

    QLineEdit* m_load_sensor;
    
};
#endif // MAINWINDOW_H
