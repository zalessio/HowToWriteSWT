# Makefile for the SWT code, different compilation flags and modes
# for Mac and Linux. Right now every version has a different folder,
# in order to keep things cleaner. The typical command used for compiling the code is:
#		make <command> VERSION=<version_name>
# e.g.  make linux_swt VERSION=DEFAULT
# The same option for the version has to be specified also for clean. The executables
# are going to be put in the folder ./bin, common for all versions.
# It is necessary to keep the same structure (src, include) for every version, so that 
# this Makefile will stay universal.  

# Compiler
CC=gcc
LINUX_FLAGS= -lopencv_core -lopencv_highgui -lopencv_imgproc -lm
MAC_FLAGS= `pkg-config --cflags --libs opencv` -w
OPTIM_FLAGS= -O3
COMP_FILES= versions/$(VERSION)/src/*

# Commands
linux_swt: $(COMP_FILES)
	@$(CC) -std=c99 $(COMP_FILES) -o ./bin/SWT_$(VERSION) $(LINUX_FLAGS) -lm

mac_swt: $(COMP_FILES)
	@$(CC) -std=c99 $(MAC_FLAGS) $(COMP_FILES) -o ./bin/SWT_$(VERSION)

clean:
	@rm ./bin/SWT_$(VERSION)

clean_all:
	@rm ./bin/*