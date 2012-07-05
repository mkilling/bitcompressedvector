SHELL = /bin/bash
BUILD_DIR=build

CC=g++
CXXFLAGS= -g2  -mtune=native -mssse3 -msse4.1 -funroll-loops

all:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/main main.cpp $(CXXFLAGS)
	$(CC) -O3 -o $(BUILD_DIR)/main_opt main.cpp -g2 -DNDEBUG $(CXXFLAGS)

profile:
	$(CC) -O3 -o $(BUILD_DIR)/main_opt main.cpp -g2 -msse4.1 -DNDEBUG -lprofiler
	CPUPROFILE_FREQUENCY=1000 CPUPROFILE=/tmp/bcv.prof ./$(BUILD_DIR)/main_opt 100000000
	pprof --pdf ./$(BUILD_DIR)/main_opt /tmp/bcv.prof > bcv.pdf

papi:
	g++ -O3 -o $(BUILD_DIR)/main_opt main.cpp -g2 -DNDEBUG -lpapi -DUSE_PAPI_TRACE -lpthread

test: all
	./$(BUILD_DIR)/main_opt 1000000
	./$(BUILD_DIR)/main_opt 10000000
	./$(BUILD_DIR)/main_opt 100000000

release:
	$(RM) -Rf pkg/bcv
	mkdir -p pkg/bcv
	cp bcv_defines.h pkg/bcv
	cp mask.h pkg/bcv
	cp bcv.h pkg/bcv
	tar -C pkg -zcvf bcv.tgz bcv


clean:
	$(RM) -Rf pkg bcv.tgz $(BUILD_DIR)
