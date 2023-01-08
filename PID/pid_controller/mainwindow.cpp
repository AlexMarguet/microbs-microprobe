#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "controller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    PID controller = PID();
    
    controller.setReference(5.);
    controller.idle();

    // for(int i = 0 ; i<5 ; i++) {
    //     controller.control_loop();
    // }
}

MainWindow::~MainWindow()
{
    delete ui;
}

