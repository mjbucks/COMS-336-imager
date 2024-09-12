# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -O2

# Source files
SRC = main.cpp

# Output executable
OUT = generate_image.exe

# PPM output file
PPM_OUTPUT = output_image.ppm

# Default target
all: $(OUT)

# Compile the C++ file
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Run the program and generate the PPM image
run: $(OUT)
	$(OUT) > $(PPM_OUTPUT)

# Clean the build
clean:
	del /F /Q $(OUT) $(PPM_OUTPUT)
