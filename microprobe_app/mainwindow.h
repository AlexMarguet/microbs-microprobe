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
#include <QCheckBox>

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
    MainWindow(Sensoray826 board, Controller controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void activateMotor();

    void turnOffMotor();

    void applyParameters();

    void insertion();

    void fRefModif();

    void setup();

    void controlLoop();

    void launchScript();

    void updateValue();

private:
    Ui::MainWindow *ui;
    Sensoray826 m_board;
    Controller m_controller;
    DataSaver m_data_saver;

    QTimer* m_timer;
    QTimer* m_control_loop_timer;

    //Control box
    QPushButton* m_probe_fwd_button;
    QPushButton* m_probe_bwd_button;
    QPushButton* m_tendon_u_reel_button;
    QPushButton* m_tendon_u_release_button;
    QPushButton* m_tendon_d_reel_button;
    QPushButton* m_tendon_d_release_button;
    QCheckBox* m_mirrored_checkbox;

    //Parameters Box
    QLineEdit* m_v_probe;
    QLineEdit* m_v_tendon_nom;
    QLineEdit* m_f_min;
    QLineEdit* m_x_probe_max;
    QPushButton* m_parameters_apply_button;

    //Sensors box
    QLineEdit* m_load_sensor_u;
    QLineEdit* m_load_sensor_d;
    QPushButton* m_load_sensor_u_to_zero_button;
    QPushButton* m_load_sensor_d_to_zero_button;
    QLineEdit* m_f_u_ref;
    QLineEdit* m_f_d_ref;

    //Insertion box
    QPushButton* m_start_button;
    QPushButton* m_stop_button;
    QPushButton* m_f_ref_inc_button;
    QPushButton* m_f_ref_dec_button;
    QLineEdit* m_f_increment;

    
    
    //Scripts box
    QPushButton* m_setup_button;
    QPushButton* m_insertion_button;
    QPushButton* m_calibration_button;
    
    QPushButton* m_dio_reset_button;

};
#endif // MAINWINDOW_H
