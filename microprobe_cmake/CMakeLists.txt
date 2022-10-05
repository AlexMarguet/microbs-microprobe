# CMakeList.txt : projet CMake pour microprobe_cmake, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

project ("microprobe_cmake")
# Ajoutez une source à l'exécutable de ce projet.
add_executable (microprobe_cmake "main.cpp" "main.h"
                "sensoray/826api.h" "sensoray/826.h" "sensoray/826.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET microprobe_cmake PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.

add_library(test_class "test_class.cpp" "test_class.h" "sensoray/826.h" "sensoray/826.cpp")
target_link_libraries(microprobe_cmake PUBLIC test_class)


# Import external library

add_library(sensoray826 SHARED IMPORTED)
set_property(TARGET sensoray826 PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/s826.dll")
set_property(TARGET sensoray826 PROPERTY
             IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/s826.lib")
target_link_libraries(microprobe_cmake PRIVATE sensoray826)