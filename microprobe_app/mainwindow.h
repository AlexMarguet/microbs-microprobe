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
#include "tools/experiment_gen.h"
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Sensoray826& board, Controller& controller, DataSaver& data_saver, QWidget *parent = nullptr);
    ~MainWindow();

// Slots are Qt callback mechanism, callback functions must be registered below
private slots:
    void activateMotor();

    void turnOffMotor();

    void applyParameters();

    void holdCheckbox();

    void hold();

    void insertion();

    void fRefModif();

    void controlLoopPID();

    void launchScript();

    void updateValue();

    void dataRecord();

    void presetConfig();

private:
    Ui::MainWindow *ui;
    Sensoray826& m_board;
    Controller& m_controller;
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
    QLineEdit* m_manual_v_p_lineedit;
    QLineEdit* m_manual_v_t_lineedit;


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
    QCheckBox* m_preset_checkbox;

    float m_f_ref = 0;
    chrono::time_point<chrono::steady_clock> m_last_start_time;
    chrono::time_point<chrono::steady_clock> m_insertion_start_time;

    QPushButton* m_preset_add_button;
    QPushButton* m_preset_reset_button;
    QLineEdit* m_preset_start_lineedit;
    QLineEdit* m_preset_stop_lineedit;
    QLineEdit* m_preset_force_start_lineedit;
    QLineEdit* m_preset_force_stop_lineedit;
    QCheckBox* m_preset_step;
    QCheckBox* m_preset_ramp;
    ExperimentPreset m_preset_experiment;
    
    //Datasave box
    QPushButton* m_record_button;
    QLineEdit* m_file_name_lineedit;
    
    //Out-of-box
    QPushButton* m_emergency_stop_button;
};
#endif // MAINWINDOW_H
