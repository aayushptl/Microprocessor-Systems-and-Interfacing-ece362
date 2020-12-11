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
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/Utilities" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/StdPeriph_Driver/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/CMSIS/device" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/Utilities" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/StdPeriph_Driver/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/CMSIS/device" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/hmwk4/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


