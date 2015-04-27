CC = gcc
CFLAGS = -c -Wall 
LDFLAGS = -lm
EXECUTABLE = bin/rapl_measurement
SOURCES= src/rapl_measurement.c src/core_util_api.c src/rapl_read_api.c
OBJECTS = $(SOURCES:.cpp=.o)


all: $(SOURCES) $(EXECUTABLE)

clean:
	@rm -rf bin

$(EXECUTABLE): $(OBJECTS)
	@rm -rf bin
	@mkdir bin
	$(CC)  $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
