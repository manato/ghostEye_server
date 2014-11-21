#!/bin/sh
if [ $# != 1 ]
then
    echo "USAGE: sh shm_display.sh <input_file / output_file>  (this message is outputed by shm_display.sh)"
else
#./display_shm /home/hirabayashi/dev/autonomous_driving_system/$1
#    ./display_shm /home/hirabayashi/dev/autonomous_driving_system/$1 /home/hirabayashi/dev/autonomous_driving_system/height_file /home/hirabayashi/dev/autonomous_driving_system/width_file
    ./display_shm /home/hirabayashi/demo/autonomous_driving_system/$1 /home/hirabayashi/demo/autonomous_driving_system/height_file /home/hirabayashi/demo/autonomous_driving_system/width_file
fi