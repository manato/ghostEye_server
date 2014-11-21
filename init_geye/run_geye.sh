#!/bin/sh
######################################################
# base path definition
######################################################
base_path="/usr/local/geye"

######################################################
# commands initialization
######################################################
# shared memory create program
shm_create="${base_path}/bin/shm_creater/shm_creater"

# car detection program 
car_detect="${base_path}/bin/car_detecter/car_detecter"

# pedestrian detection program 
pds_detect="${base_path}/bin/pedestrian_detecter/pedestrian_detecter"

# sign detection program
sgn_detect="${base_path}/bin/sign_detecter/AdaDetector ${base_path}/bin/sign_detecter/classifier.xml ${base_path}/bin/sign_detecter/sample.png -merge -mindetections 3"

# image receive program 
img_receive="${base_path}/bin/image_receiver/task_observer"

#shared memory delete program 
shm_delete="${base_path}/bin/shm_deleter/shm_deleter"

######################################################
# analyze option and execute commands
######################################################
if [ "$1" = "--terminate" ]        # terminate the running service
then
    # get process id 
    car_pid=`ps | grep car_detecter | awk '{print $1}'`
    pds_pid=`ps | grep pedestrian_dete | awk '{print $1}'`
    sgn_pid=`ps | grep AdaDetector | awk '{print $1}'`
    observer_pid=`ps | grep task_observer | awk '{print $1}'`
    geserver_pid=`ps | grep GhostEye_server | awk '{print $1}'`
    terminate_flag=0    
    
    # if pid != null character string, execute "kill" command
    if [ -n "${car_pid}" ]; then
        kill -KILL ${car_pid}
    fi
    if [ -n "${pds_pid}" ]; then
        kill -KILL ${pds_pid}
    fi
    if [ -n "${sgn_pid}" ]; then
        kill -KILL ${sgn_pid}
    fi
    if [ -n "${observer_pid}" ]; then
        kill -KILL ${observer_pid}
    fi
    if [ -n "${geserver_pid}" ]; then
        kill -KILL ${geserver_pid}
        terminate_flag=1
    fi
    
    # delete shared memory 
    ${shm_delete}
    echo "the service is terminated..."
    
    
elif [ "$1" = "--help" ]          # display help
then
    cat /usr/local/geye/doc/usage.txt
    
else
    # analyze option
    for arg in $@
    do 
        # if option is specified, corresponding detect program will not be booted.
        case ${arg} in
            --nocar) car_detect="";;
            --nopds) pds_detect="";;
            --nosgn) sgn_detect="";;
            *);;
        esac
    done
    # execute comands
    ${shm_create}               # creating shared memory is not back ground job
    ${car_detect}&
    ${pds_detect}&
    ${sgn_detect}&
    ${img_receive}&
    
fi # if[$1 -eq --delete]
