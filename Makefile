# Makefile for the SWT code, different compilation flags and modes
# for Mac and Linux 

# Compiler
CC=g++
LINUX_FLAGS= -lopencv_core -lopencv_highgui -lopencv_imgproc
MAC_FLAGS= `pkg-config --cflags --libs opencv`
OPTIM_FLAGS= -O3
COMP_FILES= src/FeaturesMain.cpp

# Commands
linux_swt: $(COMP_FILES)
	@$(CC) $(COMP_FILES) -o ./bin/SWT $(MAC_FLAGS)

mac_swt: $(COMP_FILES)
	@$(CC) $(MAC_FLAGS) $(COMP_FILES) -o ./bin/SWT

clean:
	@rm ./bin/SWT