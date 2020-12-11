################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/hw4.s \
../src/stdio.s 

C_SRCS += \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/test.c 

OBJS += \
./src/hw4.o \
./src/stdio.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/test.o 

C_DEPS += \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mthumb -mfloat-abi=soft -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/homework4/Utilities" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/homework4/StdPeriph_Driver/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/homework4/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/homework4/CMSIS/device" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/homework4/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mthumb -mfloat-abi=soft -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


