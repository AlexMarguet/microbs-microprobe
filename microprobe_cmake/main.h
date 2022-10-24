// microprobe_cmake.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <filesystem>

#include "test_class.h"
#include "sensoray/826api.h"
#include "sensoray/826.h"
#include "tools/data_save.h"

#endif // #ifndef MAIN_H