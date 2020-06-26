################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bbox.cpp \
../src/graph.cpp \
../src/jsonM.cpp \
../src/main.cpp \
../src/miscellaneous.cpp \
../src/problem.cpp \
../src/problem_arrow.cpp \
../src/problem_arrow_greedy.cpp \
../src/problem_cg.cpp \
../src/problem_rs.cpp \
../src/problem_sl.cpp \
../src/problem_sl_greedy.cpp \
../src/problem_slk.cpp \
../src/problem_slk_greedy.cpp 

OBJS += \
./src/bbox.o \
./src/graph.o \
./src/jsonM.o \
./src/main.o \
./src/miscellaneous.o \
./src/problem.o \
./src/problem_arrow.o \
./src/problem_arrow_greedy.o \
./src/problem_cg.o \
./src/problem_rs.o \
./src/problem_sl.o \
./src/problem_sl_greedy.o \
./src/problem_slk.o \
./src/problem_slk_greedy.o 

CPP_DEPS += \
./src/bbox.d \
./src/graph.d \
./src/jsonM.d \
./src/main.d \
./src/miscellaneous.d \
./src/problem.d \
./src/problem_arrow.d \
./src/problem_arrow_greedy.d \
./src/problem_cg.d \
./src/problem_rs.d \
./src/problem_sl.d \
./src/problem_sl_greedy.d \
./src/problem_slk.d \
./src/problem_slk_greedy.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++17 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" -lCGAL -lgmp -lcairo
	@echo 'Finished building: $<'
	@echo ' '


