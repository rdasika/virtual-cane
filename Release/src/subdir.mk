################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/sysinit.c \
../src/virtual_cane.c 

OBJS += \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/sysinit.o \
./src/virtual_cane.o 

C_DEPS += \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/sysinit.d \
./src/virtual_cane.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -D__REDLIB__ -I"/Users/tomdeeds/Documents/LPCXpresso_8.2.2/workspace/virtual_cane/inc" -I"/Users/tomdeeds/Documents/LPCXpresso_8.2.2/workspace/lpc_board_nxp_lpcxpresso_1769/inc" -I"/Users/tomdeeds/Documents/LPCXpresso_8.2.2/workspace/lpc_chip_175x_6x/inc" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


