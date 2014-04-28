# Dynamic makefile
CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR = ../../src
SRCDIR2 = src
BUILDDIR := ../../build
BUILDDIR2 := build2
TARGET := dynamixel

LIBDIRS		= -L../../lib
INCLUDEDIRS	= -I../../include
LDFLAGS		= $(LIBDIRS) -lm -ldxl -lpthread -lcurl -ljansson -lX11

VPATH := SRCDIR:SRCDIR2
SRCEXT := cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
SOURCES = $(shell find $(SRCDIR2) -type f -name *.$(SRCEXT))
OBJECTS += $(patsubst $(SRCDIR2)/%,$(BUILDDIR2)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := $(INCLUDEDIRS) -g -W -Wall -O2

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LDFLAGS)"; $(CC) $^ -o $(TARGET) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INCLUDEDIRS) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCLUDEDIRS) -c -o $@ $<
	
$(BUILDDIR2)/%.o: $(SRCDIR2)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR2)
	@echo " $(CC) $(CFLAGS) $(INCLUDEDIRS) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCLUDEDIRS) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BUILDDIR2) $(TARGET)"; $(RM) -r $(BUILDDIR) $(BUILDDIR2) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
