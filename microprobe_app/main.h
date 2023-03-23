#pragma once

#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <iomanip>
#include <cstdio>

#include <QApplication>

#include "mainwindow.h"
#include "sensoray/826api.h"
#include "sensoray/826.h"
#include "controller.h"
#include "tools/data_saver.h"

#endif // MAIN_H