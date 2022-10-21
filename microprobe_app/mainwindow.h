#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>

#include "sensoray/826.h"

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
    void ActivateMotor();

private:
    Ui::MainWindow *ui;
    Sensoray826 m_board;

    QSignalMapper* m_signal_mapper;

    QPushButton* m_fwd_button;
    QPushButton* m_bwd_button;
    QPushButton* m_r_button;
    QPushButton* m_l_button;
    
};
#endif // MAINWINDOW_H
