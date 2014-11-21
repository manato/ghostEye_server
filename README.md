# ghostEye_server

server program working with android's ghostEye application
##  how to install 
1. run `$ init_geye/init.sh`

2. modify GPU architecture number written in `geye_car/CAR_TRACKING/CAR_TRACKING.makefile` and `geye_pds/CAR_TRACKING/CAR_TRACKING.makefile`

3. run `$ make and $ make install under each directory`

## how to use 
run 
```
    $ cd /usr/local/geye/shell/ 
    $ ./run.sh [--nocar, --nopds, --nosgn]
    ##### If you want to terminate program halfway
    $ ./run.sh --terminate  # if this is not executed, program keep on working on background.
```