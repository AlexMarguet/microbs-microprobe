# CMakeList.txt : projet CMake pour microprobe_cmake, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

project ("microprobe_cmake")
# Ajoutez une source à l'exécutable de ce projet.
add_executable (microprobe_cmake "main.cpp" "main.h"
                "826api.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET microprobe_cmake PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.

add_library(test_class "test_class.cpp" "test_class.h")
target_link_libraries(microprobe_cmake PUBLIC test_class)


# Import external library

add_library(sensoray826 SHARED IMPORTED)
# set_property(TARGET sensoray826 PROPERTY
#             IMPORTED_LOCATION "C:/Users/Alexandre/Documents/Cours/PDM/code/microprobe_cmake/libs/s826.dll")
# set_property(TARGET sensoray826 PROPERTY
#             IMPORTED_IMPLIB "C:/Users/Alexandre/Documents/Cours/PDM/code/microprobe_cmake/libs/s826.lib")
set_property(TARGET sensoray826 PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/s826.dll")
set_property(TARGET sensoray826 PROPERTY
             IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/s826.lib")
target_link_libraries(microprobe_cmake PRIVATE sensoray826)