#CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS	:= -std=c++14 -O3 -Wno-format
# Add more warnings
# CXXFLAGS	+= -Wall -Wextra

# CUDA variables
NVCC		:= nvcc
CUDA_CXXFLAGS	:= -O3 -std=c++14 --compiler-options -fPIC
# Add -gencode for specific architectures if needed, e.g.:
# CUDA_CXXFLAGS	+= -gencode arch=compute_70,code=sm_70
# For CUDA library path, ensure it's found by the linker.
# If /usr/local/cuda/lib64 is not in standard linker paths, add:
# CUDA_LDFLAGS	:= -L/usr/local/cuda/lib64 -lcudart
CUDA_LDFLAGS	:= -lcudart

#vpath %.cpp observables
#vpath %.hpp observables

MAIN	:= cdt2d.x
CPP_SOURCES := $(wildcard *.cpp) $(wildcard observables/*.cpp)
# CUDA sources are expected to be in the observables directory for this rule
CUDA_SOURCES := $(wildcard observables/*.cu)

OBJECTS := $(patsubst %.cpp,%.o,$(CPP_SOURCES))
# CUDA objects will be, e.g., observables/ricci_cuda_kernels.o
CUDA_OBJECTS := $(patsubst %.cu,%.o,$(CUDA_SOURCES))


CPP_DEPENDS := $(patsubst %.cpp,%.d,$(CPP_SOURCES))
# For nvcc, dependency generation.
CUDA_DEPENDS := $(patsubst %.cu,%.d,$(CUDA_SOURCES))


.PHONY: all clean

all: $(MAIN)

clean:
	$(RM) $(OBJECTS) $(CUDA_OBJECTS) $(CPP_DEPENDS) $(CUDA_DEPENDS) $(MAIN)

# Linking the executable from the object files
$(MAIN): $(OBJECTS) $(CUDA_OBJECTS)
	echo "CPP_OBJECTS: $(OBJECTS)"
	echo "CUDA_OBJECTS: $(CUDA_OBJECTS)"
	$(CXX) $(CXXFLAGS) $^ $(CUDA_LDFLAGS) -o $@

# Include dependency files
-include $(CPP_DEPENDS)
-include $(CUDA_DEPENDS)

# Rule for C++ files (handles *.cpp and observables/*.cpp)
# This rule correctly creates .o files in the same directory as .cpp files
# e.g. observables/file.cpp -> observables/file.o
%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Rule for CUDA files in observables directory
# This rule should correctly create .o and .d files in observables/
# e.g. observables/ricci_cuda_kernels.cu -> observables/ricci_cuda_kernels.o, observables/ricci_cuda_kernels.d
observables/%.o: observables/%.cu Makefile
	$(NVCC) $(CUDA_CXXFLAGS) --compiler-options -MMD,-MP -c $< -o $@
