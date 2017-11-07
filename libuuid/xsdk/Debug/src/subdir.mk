################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/deploy/git/util-linux/libuuid/src/clear.c \
/home/deploy/git/util-linux/libuuid/src/compare.c \
/home/deploy/git/util-linux/libuuid/src/copy.c \
/home/deploy/git/util-linux/libuuid/src/gen_uuid.c \
/home/deploy/git/util-linux/libuuid/src/isnull.c \
/home/deploy/git/util-linux/libuuid/src/pack.c \
/home/deploy/git/util-linux/libuuid/src/parse.c \
/home/deploy/git/util-linux/libuuid/src/test_uuid.c \
/home/deploy/git/util-linux/libuuid/src/unpack.c \
/home/deploy/git/util-linux/libuuid/src/unparse.c \
/home/deploy/git/util-linux/libuuid/src/uuid_time.c 

OBJS += \
./src/clear.o \
./src/compare.o \
./src/copy.o \
./src/gen_uuid.o \
./src/isnull.o \
./src/pack.o \
./src/parse.o \
./src/test_uuid.o \
./src/unpack.o \
./src/unparse.o \
./src/uuid_time.o 

C_DEPS += \
./src/clear.d \
./src/compare.d \
./src/copy.d \
./src/gen_uuid.d \
./src/isnull.d \
./src/pack.d \
./src/parse.d \
./src/test_uuid.d \
./src/unpack.d \
./src/unparse.d \
./src/uuid_time.d 


# Each subdirectory must supply rules for building sources it contributes
src/clear.o: /home/deploy/git/util-linux/libuuid/src/clear.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/compare.o: /home/deploy/git/util-linux/libuuid/src/compare.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/copy.o: /home/deploy/git/util-linux/libuuid/src/copy.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gen_uuid.o: /home/deploy/git/util-linux/libuuid/src/gen_uuid.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/isnull.o: /home/deploy/git/util-linux/libuuid/src/isnull.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/pack.o: /home/deploy/git/util-linux/libuuid/src/pack.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/parse.o: /home/deploy/git/util-linux/libuuid/src/parse.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/test_uuid.o: /home/deploy/git/util-linux/libuuid/src/test_uuid.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/unpack.o: /home/deploy/git/util-linux/libuuid/src/unpack.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/unparse.o: /home/deploy/git/util-linux/libuuid/src/unparse.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/uuid_time.o: /home/deploy/git/util-linux/libuuid/src/uuid_time.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 Linux gcc compiler'
	aarch64-linux-gnu-gcc -DHAVE_NANOSLEEP -Wall -O0 -g3 -I../../../include -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


