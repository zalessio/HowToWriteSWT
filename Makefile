# Makefile for the SWT code, different compilation flags and modes
# for Mac and Linux 

# Compiler
CC=g++
LINUX_FLAGS= -lopencv_core -lopencv_highgui -lopencv_imgproc
MAC_FLAGS= `pkg-config --cflags --libs opencv`
OPTIM_FLAGS= -O3
COMP_FILES= versions/$(VERSION)/src/*

# Commands
linux_swt: $(COMP_FILES)
	@$(CC) $(COMP_FILES) -o ./bin/SWT_$(VERSION) $(MAC_FLAGS)

mac_swt: $(COMP_FILES)
	@$(CC) $(MAC_FLAGS) $(COMP_FILES) -o ./bin/SWT_$(VERSION)

clean:
	@rm ./bin/SWT_$(VERSION)

clean_all:
	@rm ./bin/*