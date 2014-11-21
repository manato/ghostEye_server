CC  = gcc
CXX = g++
CSC = gmcs

# OTHERINCLUDE	= -I./externals/include
# OTHERLDLIBS	= -L/externals/x64

INCLUDE  = -I. -I/usr/include -I/usr/include/libxml2
LDLIBS   = -L/usr/lib $(OTHERLDLIBS)



LFLAGS = #-finput-charset=cp932
CFLAGS = -W -Wall -O2 -fopenmp -DNDEBUG
CXXFLAGS = -W -Wall -O2 -fopenmp -DNDEBUG
CSCFLAGS = -warn:4 -optimize

RELEASEFLAGS = $(CXXFLAGS) -DNDEBUG
DEBUGFLAGS   = $(CXXFLAGS) -DDEBUG

LIBS = -lxml2 -lpthread -lz -lm -ljpeg -lpng -ltiff
MLIBS = -llapack -lblas

# Be quiet when building...
#.SILENT:

# Build commands and filename extensions...
.SUFFIXES: .c .cpp .cxx .h .o .obj

.c.o:
	echo Compiling $<...
	$(CC) $(CFLAGS) $(LFLAGS) -c $< $(INCLUDE) $(OTHERINCLUDE)

.cxx.o:
	echo Compiling $<...
	$(CXX) $(CXXFLAGS) $(LFLAGS) -c -xc++ $< $(INCLUDE) $(OTHERINCLUDE)

.cpp.o:
	echo Compiling $<...
	$(CXX) $(CXXFLAGS) $(LFLAGS) -c -xc++ $< $(INCLUDE) $(OTHERINCLUDE)

ALL = \
	AdaTrainer \
	AdaDetector \
	AdaSampler \

COMMON = \
	common.h \
	utility.h \

CLASSIFIERS = \
	feature.h \
	classifier.h \
	classifier_base.h \
	classifier_io.h \
	discrete_adaboost.h \
	real_adaboost.h \

FEATURES = \
	HAAR.h \
	LRP.h \
	MLBP.h \

VALIDATION = \
	validation.h \
	clustering.h \

CPP = \
	trainer.cpp \
	detector.cpp \
	sampler.cpp \

all: $(ALL)

trainer.o: $(COMMON) $(CLASSIFIERS) $(FEATURES)
detector.o: $(COMMON) $(CLASSIFIERS) $(FEATURES) $(VALIDATION)
sampler.o: $(COMMON)

AdaTrainer: trainer.o version.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

AdaDetector: detector.o version.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

AdaSampler: sampler.o version.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

AdaPathGen.exe: pathgen.cs
	$(CSC) $(CSCFLAGS) -out:$@ $^

clean:
	for binary in $(ALL); do\
		echo "=== deleting $$binary ===";\
		( cd ./; rm -f $$binary; rm -f $$binary.exe ) || break;\
	done
	rm -f ./*.o
	rm -f ./*.obj
	rm -f ./*.exe
	rm -f ./*~
	rm -f ./version

install:
	cp ./classifier.xml /usr/local/geye/bin/sign_detecter/classifier.xml
	cp ./sample.png /usr/local/geye/bin/sign_detecter/sample.png
	cp ./AdaDetector /usr/local/geye/bin/sign_detecter/AdaDetector
