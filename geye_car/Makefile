# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: CAR_TRACKING 

# Builds project 'CAR_TRACKING'...
.PHONY: CAR_TRACKING
CAR_TRACKING: 
	make --directory="CAR_TRACKING/" --file=CAR_TRACKING.makefile

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="CAR_TRACKING/" --file=CAR_TRACKING.makefile clean

# Install executable file to /usr/local/geye/bin
install:
	cp ./CAR_TRACKING/2010_2_3.txt /usr/local/geye/bin/car_detecter/2010_2_3.txt
	cp ./CAR_TRACKING/car_comp.csv /usr/local/geye/bin/car_detecter/car_comp.csv
	cp ./CAR_TRACKING/car_root.csv /usr/local/geye/bin/car_detecter/car_root.csv
	cp ./CAR_TRACKING/car_part.csv /usr/local/geye/bin/car_detecter/car_part.csv
	cp ./gccRelease/GPU_function.cubin /usr/local/geye/bin/car_detecter/GPU_function.cubin
	cp ./gccRelease/CAR_TRACKING.exe /usr/local/geye/bin/car_detecter/car_detecter
