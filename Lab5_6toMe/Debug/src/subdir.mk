################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/lab5.s 

C_SRCS += \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

O_SRCS += \
../src/autotest.o 

OBJS += \
./src/lab5.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/Utilities" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/StdPeriph_Driver/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/CMSIS/device" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/Utilities" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/StdPeriph_Driver/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/inc" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/CMSIS/device" -I"/Users/terrencerandall/Documents/Fall_19/ece362/workspace/Lab5_6toMe/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


