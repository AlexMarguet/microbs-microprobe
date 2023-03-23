# microbs-microprobe
Ultra-flexible probe for soft tissues - Project at Microbs Laboratory, EPFL

## Directory
- microprobe_app
  - libs
    - 826.lib
  - qwidgets
    - qcustomplot.cpp
  - sensoray
    - 826.cpp
    - 826api.cpp
  - tools
    - data_saver.cpp
    - experiment_gen.cpp
  - controller.cpp
  - main.cpp
  - mainwindow.cpp
  - mainwindow.ui
  - CMakeLists.txt
  
## Files
`sensoray/826` contains robotic setup configuration and provides macro functionalities from the low-level api `sensoray/826api`

`tools/data_saver`generates .csv file with experiment variables;
`tools/experiment_gen` is an object containing force references to follow over time

`controller` is a control-loop for tendons to follow the force reference

`mainwindow` retrieves the `ui` configuration to generate the window and links objects to callbacks (manual control, insertion experiments, settings, ...)

(`qcustomplot` is not yet used due to linking issues)

## Run
- Turn on 12[V] power supply, then ground ENABLE pin of motor tendon up
- Open Sensoray's `826demo`and set ADC slot 1 to channel 1 (range -10[V] +10[V])
- Open project in `QtCreator` and run
