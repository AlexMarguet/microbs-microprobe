#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <chrono>

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
    MainWindow(Sensoray826 board, Controller controller, DataSaver& data_saver, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void activateMotor();

    void turnOffMotor();

    void applyParameters();

    void holdCheckbox();

    void hold();

    void insertion();

    void fRefModif();

    void setup();

    void controlLoop();

    void controlLoopPID();

    void launchScript();

    void updateValue();

    void dataRecord();

private:
    Ui::MainWindow *ui;
    Sensoray826 m_board;
    Controller m_controller;
    DataSaver& m_data_saver;

    QTimer* m_timer;
    QTimer* m_control_loop_timer;

    //Control box
    QPushButton* m_probe_fwd_button;
    QPushButton* m_probe_bwd_button;
    QPushButton* m_tendon_u_reel_button;
    QPushButton* m_tendon_u_release_button;
    QPushButton* m_tendon_d_reel_button;
    QPushButton* m_tendon_d_release_button;
    QCheckBox* m_hold_checkbox;

    //Parameters Box
    QLineEdit* m_v_probe_nom;
    QLineEdit* m_v_tendon_rel_nom;
    QLineEdit* m_f_min;
    QLineEdit* m_x_probe_max;
    QLineEdit* m_k_p;
    QLineEdit* m_k_i;
    QLineEdit* m_k_d;
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
    QPushButton* m_reel_back_button;
    QPushButton* m_f_ref_inc_button;
    QPushButton* m_f_ref_dec_button;
    QLineEdit* m_f_increment;
    QLineEdit* m_f_ref_lineedit;
    QCheckBox* m_pid_checkbox;

    float m_f_ref = 0;
    chrono::time_point<chrono::steady_clock> m_last_start_time;
    chrono::time_point<chrono::steady_clock> m_insertion_start_time;

    uint m_k_loop = 0;
    uint m_step_start[] = {20};
    uint m_step_stop[] = {40};
    float m_step_height[] = {100};
    uint m_step_iter = 0;
    uint m_max_steps = 0;

    
    //Datasave box
    QPushButton* m_record_button;
    QLineEdit* m_file_name_lineedit;
    
    //Out-of-box
    QPushButton* m_emergency_stop_button;

    QPushButton* m_script_button;

};
#endif // MAINWINDOW_H
