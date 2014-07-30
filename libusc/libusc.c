#include "libusc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum libusc_request
{
    REQUEST_GET_PARAMETER = 0x81,
    REQUEST_SET_PARAMETER = 0x82,
    REQUEST_GET_VARIABLES = 0x83,
    REQUEST_SET_SERVO_VARIABLE = 0x84,
    REQUEST_SET_TARGET = 0x85,
    REQUEST_CLEAR_ERRORS = 0x86,
    REQUEST_GET_SERVO_SETTINGS = 0x87,

    // GET STACK and GET CALL STACK are only used on the Mini Maestro.
    REQUEST_GET_STACK = 0x88,
    REQUEST_GET_CALL_STACK = 0x89,
    REQUEST_SET_PWM = 0x8A,

    REQUEST_REINITIALIZE = 0x90,
    REQUEST_ERASE_SCRIPT = 0xA0,
    REQUEST_WRITE_SCRIPT = 0xA1,
    REQUEST_SET_SCRIPT_DONE = 0xA2, // value.low.b is 0 for go, 1 for stop, 2 for single-step
    REQUEST_RESTART_SCRIPT_AT_SUBROUTINE = 0xA3,
    REQUEST_RESTART_SCRIPT_AT_SUBROUTINE_WITH_PARAMETER = 0xA4,
    REQUEST_RESTART_SCRIPT = 0xA5,
    REQUEST_START_BOOTLOADER = 0xFF,
};

enum libusc_parameter
{
    PARAMETER_INITIALIZED = 0, // 1 byte - 0 or 0xFF
    PARAMETER_SERVOS_AVAILABLE = 1, // 1 byte - 0-5
    PARAMETER_SERVO_PERIOD = 2, // 1 byte - ticks allocated to each servo/256
    PARAMETER_SERIAL_MODE = 3, // 1 byte unsigned value. Valid values are SERIAL_MODE_*. Init variable.
    PARAMETER_SERIAL_FIXED_BAUD_RATE = 4, // 2-byte unsigned value; 0 means autodetect. Init parameter.
    PARAMETER_SERIAL_TIMEOUT = 6, // 2-byte unsigned value
    PARAMETER_SERIAL_ENABLE_CRC = 8, // 1 byte boolean value
    PARAMETER_SERIAL_NEVER_SUSPEND = 9, // 1 byte boolean value
    PARAMETER_SERIAL_DEVICE_NUMBER = 10, // 1 byte unsigned value, 0-127
    PARAMETER_SERIAL_BAUD_DETECT_TYPE = 11, // 1 byte value

    PARAMETER_IO_MASK_C = 16, // 1 byte - pins used for I/O instead of servo
    PARAMETER_OUTPUT_MASK_C = 17, // 1 byte - outputs that are enabled

    PARAMETER_CHANNEL_MODES_0_3 = 12, // 1 byte - channel modes 0-3
    PARAMETER_CHANNEL_MODES_4_7 = 13, // 1 byte - channel modes 4-7
    PARAMETER_CHANNEL_MODES_8_11 = 14, // 1 byte - channel modes 8-11
    PARAMETER_CHANNEL_MODES_12_15 = 15, // 1 byte - channel modes 12-15
    PARAMETER_CHANNEL_MODES_16_19 = 16, // 1 byte - channel modes 16-19
    PARAMETER_CHANNEL_MODES_20_23 = 17, // 1 byte - channel modes 20-23
    PARAMETER_MINI_MAESTRO_SERVO_PERIOD_L = 18, // servo period: 3-byte unsigned values, units of quarter microseconds
    PARAMETER_MINI_MAESTRO_SERVO_PERIOD_HU = 19,
    PARAMETER_ENABLE_PULLUPS = 21, // 1 byte: 0 or 1
    PARAMETER_SCRIPT_CRC = 22, // 2 bytes - stores a checksum of the bytecode program, for comparison
    PARAMETER_SCRIPT_DONE = 24, // 1 byte - copied to scriptDone on startup
    PARAMETER_SERIAL_MINI_SSC_OFFSET = 25, // 1 byte (0-254)
    PARAMETER_SERVO_MULTIPLIER = 26, // 1 byte (0-255)

    // 9 * 24 = 216, so we can safely start at 30
    PARAMETER_SERVO0_HOME = 30, // 2 byte home position (0=off; 1=ignore)
    PARAMETER_SERVO0_MIN = 32, // 1 byte min allowed value (x2^6)
    PARAMETER_SERVO0_MAX = 33, // 1 byte max allowed value (x2^6)
    PARAMETER_SERVO0_NEUTRAL = 34, // 2 byte neutral position
    PARAMETER_SERVO0_RANGE = 36, // 1 byte range
    PARAMETER_SERVO0_SPEED = 37, // 1 byte (5 mantissa,3 exponent) us per 10ms
    PARAMETER_SERVO0_ACCELERATION = 38, // 1 byte (speed changes that much every 10ms)
    PARAMETER_SERVO1_HOME = 39, // 2 byte home position (0=off; 1=ignore)
    PARAMETER_SERVO1_MIN = 41, // 1 byte min allowed value (x2^6)
    PARAMETER_SERVO1_MAX = 42, // 1 byte max allowed value (x2^6)
    PARAMETER_SERVO1_NEUTRAL = 43, // 2 byte neutral position
    PARAMETER_SERVO1_RANGE = 45, // 1 byte range
    PARAMETER_SERVO1_SPEED = 46, // 1 byte (5 mantissa,3 exponent) us per 10ms
    PARAMETER_SERVO1_ACCELERATION = 47, // 1 byte (speed changes that much every 10ms)
    PARAMETER_SERVO2_HOME = 48, // 2 byte home position (0=off; 1=ignore)
    PARAMETER_SERVO2_MIN = 50, // 1 byte min allowed value (x2^6)
    PARAMETER_SERVO2_MAX = 51, // 1 byte max allowed value (x2^6)
    PARAMETER_SERVO2_NEUTRAL = 52, // 2 byte neutral position
    PARAMETER_SERVO2_RANGE = 54, // 1 byte range
    PARAMETER_SERVO2_SPEED = 55, // 1 byte (5 mantissa,3 exponent) us per 10ms
    PARAMETER_SERVO2_ACCELERATION = 56, // 1 byte (speed changes that much every 10ms)
    PARAMETER_SERVO3_HOME = 57, // 2 byte home position (0=off; 1=ignore)
    PARAMETER_SERVO3_MIN = 59, // 1 byte min allowed value (x2^6)
    PARAMETER_SERVO3_MAX = 60, // 1 byte max allowed value (x2^6)
    PARAMETER_SERVO3_NEUTRAL = 61, // 2 byte neutral position
    PARAMETER_SERVO3_RANGE = 63, // 1 byte range
    PARAMETER_SERVO3_SPEED = 64, // 1 byte (5 mantissa,3 exponent) us per 10ms
    PARAMETER_SERVO3_ACCELERATION = 65, // 1 byte (speed changes that much every 10ms)
    PARAMETER_SERVO4_HOME = 66, // 2 byte home position (0=off; 1=ignore)
    PARAMETER_SERVO4_MIN = 68, // 1 byte min allowed value (x2^6)
    PARAMETER_SERVO4_MAX = 69, // 1 byte max allowed value (x2^6)
    PARAMETER_SERVO4_NEUTRAL = 70, // 2 byte neutral position
    PARAMETER_SERVO4_RANGE = 72, // 1 byte range
    PARAMETER_SERVO4_SPEED = 73, // 1 byte (5 mantissa,3 exponent) us per 10ms
    PARAMETER_SERVO4_ACCELERATION = 74, // 1 byte (speed changes that much every 10ms)
    PARAMETER_SERVO5_HOME = 75, // 2 byte home position (0=off; 1=ignore)
    PARAMETER_SERVO5_MIN = 77, // 1 byte min allowed value (x2^6)
    PARAMETER_SERVO5_MAX = 78, // 1 byte max allowed value (x2^6)
    PARAMETER_SERVO5_NEUTRAL = 79, // 2 byte neutral position
    PARAMETER_SERVO5_RANGE = 81, // 1 byte range
    PARAMETER_SERVO5_SPEED = 82, // 1 byte (5 mantissa,3 exponent) us per 10ms
    PARAMETER_SERVO5_ACCELERATION = 83, // 1 byte (speed changes that much every 10ms)
};

typedef struct ServoStatus ServoStatus;

struct libusc_device
{
    // lock protects refcnt, everything else is finalized at initialization time
    //pthread_mutex_t lock;
    int refcnt;
    int channelcnt;

    libusb_device *dev;
    libusb_device_handle *dev_handle;
};

struct libusc_range
{
    unsigned char bytes;
    int minimumValue;
    int maximumValue;
};

typedef struct libusc_range libusc_range;

libusc_range* libusc_get_range(enum libusc_parameter parameterId)
{
    //static libusc_range u32 = {4, 0, 0x7FFFFFFF};
    static libusc_range u16 = {2, 0, 0xFFFF};
    //static libusc_range u12 = {2, 0, 0x0FFF};
    //static libusc_range u10 = {2, 0, 0x03FF};
    static libusc_range u8 = {1, 0, 0xFF};
    static libusc_range u7 = {1, 0, 0x7F};
    static libusc_range boolean = {1, 0, 1};
    static libusc_range offset = {1, 0, 254};
    static libusc_range u2 = {1, 0, 3};
    static libusc_range u32440 = {2, 0, 32440};
    static libusc_range u50 = {1, 1, 50};

    switch (parameterId)
    {
    case PARAMETER_INITIALIZED:
    case PARAMETER_SERVOS_AVAILABLE:
    case PARAMETER_SERVO_PERIOD:
    case PARAMETER_MINI_MAESTRO_SERVO_PERIOD_L:
    case PARAMETER_SERVO_MULTIPLIER:
    case PARAMETER_CHANNEL_MODES_0_3:
    case PARAMETER_CHANNEL_MODES_4_7:
    case PARAMETER_CHANNEL_MODES_8_11:
    case PARAMETER_CHANNEL_MODES_12_15:
    case PARAMETER_CHANNEL_MODES_16_19:
    case PARAMETER_CHANNEL_MODES_20_23:
    case PARAMETER_ENABLE_PULLUPS:
        return &u8;
    case PARAMETER_MINI_MAESTRO_SERVO_PERIOD_HU:
    case PARAMETER_SERIAL_TIMEOUT:
    case PARAMETER_SERIAL_FIXED_BAUD_RATE:
    case PARAMETER_SCRIPT_CRC:
        return &u16;
    case PARAMETER_SERIAL_MODE:
        return &u2;
    case PARAMETER_SERIAL_BAUD_DETECT_TYPE:
    case PARAMETER_SERIAL_NEVER_SUSPEND:
    case PARAMETER_SERIAL_ENABLE_CRC:
    case PARAMETER_SCRIPT_DONE:
        return &boolean;
    case PARAMETER_SERIAL_DEVICE_NUMBER:
        return &u7;
    case PARAMETER_SERIAL_MINI_SSC_OFFSET:
        return &offset;
    default: break;
    }
    // must be one of the servo parameters
    switch ((((unsigned char)parameterId- (unsigned char)PARAMETER_SERVO0_HOME) % 9) + (unsigned char)PARAMETER_SERVO0_HOME)
    {
    case (unsigned char)PARAMETER_SERVO0_HOME:
    case (unsigned char)PARAMETER_SERVO0_NEUTRAL:
        return &u32440; // 32640 - 200
    case (unsigned char)PARAMETER_SERVO0_RANGE:
        return &u50; // the upper limit could be adjusted
    case (unsigned char)PARAMETER_SERVO0_SPEED:
    case (unsigned char)PARAMETER_SERVO0_MAX:
    case (unsigned char)PARAMETER_SERVO0_MIN:
    case (unsigned char)PARAMETER_SERVO0_ACCELERATION:
        return &u8;
    }

    return &u8;
}

unsigned int libusc_control_transfer(libusc_device *dev, unsigned char RequestType, unsigned char Request, unsigned short Value, unsigned short Index, unsigned char *data, unsigned short length)
{
    if(!dev)
        return 0;

    if(!dev->dev_handle)
        libusb_open(dev->dev,&dev->dev_handle);

    return libusb_control_transfer(dev->dev_handle, RequestType, Request, Value, Index, data, length, 5000);
}

unsigned short libusc_get_raw_parameter(libusc_device *dev, unsigned short parameter)
{
    libusc_range* range = libusc_get_range(parameter);
    unsigned short value = 0;
    unsigned short buffer;

    libusc_control_transfer(dev, 0xC0, (unsigned char)REQUEST_GET_PARAMETER, 0, parameter, (unsigned char*)&buffer, range->bytes);

    if (range->bytes == 1)
    {
        // read a single byte
        value = *(unsigned char*)&buffer;
    }
    else
    {
        // read two bytes
        value = buffer;
    }

    return value;
}

void libusc_set_raw_parameter(libusc_device *dev, unsigned short parameter, unsigned short value)
{
    libusc_range* range = libusc_get_range(parameter);
    //requireArgumentRange(value, range.minimumValue, range.maximumValue, "" /*MLparameter*/);
    int bytes = range->bytes;
    unsigned short index = (unsigned short)((bytes << 8) + parameter); // high bytes = # of bytes

    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_SET_PARAMETER, value, index, NULL, 0);
}

ssize_t libusc_get_device_list(libusb_context *ctx, libusc_device ***list)
{
    const uint16_t vendorID = 0x1ffb;
    const uint16_t productIDArray[] = { 0x0089, 0x008a, 0x008b, 0x008c };

    libusb_device **devs;

    ssize_t cnt;
    ssize_t len = 0;
    int i,j;

    (*list) = calloc(1,sizeof(libusc_device*));

    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0)
        return cnt;

    for(i=0; i<cnt; i++)
    {
        libusb_device* dev = devs[i];
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(devs[i], &desc);
        if (r < 0)
        {
            fprintf(stderr, "failed to get device descriptor");
            return 0;
        }

        if (desc.idVendor == vendorID)
        {
            for (j = 0; j < (int)sizeof(productIDArray); j++)
            {
                if (desc.idProduct == productIDArray[j])
                {
                    struct libusc_device* usc;

                    (*list) = realloc(*list, (len+2)*sizeof(libusc_device*));

                    usc = malloc(sizeof(libusc_device));
                    usc->dev = libusb_ref_device(dev);
                    usc->dev_handle = NULL;

                    (*list)[len] = usc;
                    len++;

                    switch(desc.idProduct)
                    {
                    case 0x89:
                        usc->channelcnt = 6;
                        break;
                    case 0x8A:
                        usc->channelcnt = 12;
                        break;
                    case 0x8B:
                        usc->channelcnt = 18;
                        break;
                    case 0x8C:
                        usc->channelcnt = 24;
                        break;
                    default:
                        break;
                    }

                    break;
                }
            }

        }
    }
    (*list)[len] = NULL;

    libusb_free_device_list(devs, 1);

    return len;
}

void libusc_free_device_list(libusc_device **list, int unref_devices)
{
    if (!list)
        return;

    if (unref_devices) {
        int i = 0;
        struct libusc_device *dev;

        while ((dev = list[i++]) != NULL)
            libusc_unref_device(dev);
    }
    free(list);
}

libusc_device *libusc_ref_device(libusc_device *dev)
{
    //pthread_mutex_lock(&dev->lock);
    dev->refcnt++;
    //pthread_mutex_unlock(&dev->lock);
    return dev;
}

void libusc_unref_device(libusc_device *dev)
{
    int refcnt;

    if (!dev)
        return;

    //pthread_mutex_lock(&dev->lock);
    refcnt = --dev->refcnt;
    //pthread_mutex_unlock(&dev->lock);
#if 0
    if (refcnt == 0) {
        //usbi_dbg("destroy device %d.%d", dev->bus_number, dev->device_address);

        if (usbi_backend->destroy_device)
            usbi_backend->destroy_device(dev);

        //pthread_mutex_lock(&dev->ctx->usb_devs_lock);
        list_del(&dev->list);
        //pthread_mutex_unlock(&dev->ctx->usb_devs_lock);

        free(dev);
    }
#endif
}

int libusc_get_num_channels(libusc_device *dev)
{
    return dev ? dev->channelcnt : 0;
}

void libusc_set_target(libusc_device *dev, unsigned char servo, unsigned short value)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_SET_TARGET, value, servo, (unsigned char*)0, 0);
}

void libusc_set_speed(libusc_device *dev, unsigned char servo, unsigned short value)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_SET_SERVO_VARIABLE, value, servo, (unsigned char*)0, 0);
}

void libusc_set_acceleration(libusc_device *dev, unsigned char servo, unsigned short value)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_SET_SERVO_VARIABLE, value, servo, (unsigned char*)(servo | 0x80), 0);
}

int libusc_get_servo_status(libusc_device *dev, unsigned char servo, libusc_servo_status * status)
{
    unsigned int size = dev->channelcnt * 7;
    unsigned char* buffer = (unsigned char*)malloc(size);

    unsigned int bytesRead = libusc_control_transfer(dev, 0xC0, (unsigned char)REQUEST_GET_SERVO_SETTINGS, 0, 0, (unsigned char*)buffer, size);
    if (bytesRead == size)
    {
        status->position = *(unsigned short*)&buffer[servo * 7+0];
        status->target = *(unsigned short*)&buffer[servo * 7+2];
        status->speed = *(unsigned short*)&buffer[servo * 7+4];
        status->acceleration = *(unsigned char*)&buffer[servo * 7+6];
    }

    free(buffer);

    return 1;
}

void libusc_restore_default_configuration(libusc_device *dev)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_REINITIALIZE, 0, 0, NULL, 0);
}


unsigned int libusc_convert_spbrg_to_bps(unsigned short spbrg)
{
    /// Instructions are executed at 12 MHZ
    static const int INSTRUCTION_FREQUENCY = 12000000;

    if (spbrg == 0)
    {
        return 0;
    }

    return (unsigned int)(INSTRUCTION_FREQUENCY + (spbrg + 1) / 2) / (spbrg + 1);
}

/// The converts from bps to SPBRG, so it is the opposite of libusc_convert_spbrg_to_bps.
/// The purse math formula is spbrg = INSTRUCTION_FREQUENCY/Baud - 1.
unsigned short libusc_convert_bps_to_spbrg(unsigned int bps)
{
    /// Instructions are executed at 12 MHZ
    static const int INSTRUCTION_FREQUENCY = 12000000;

    if (bps == 0)
    {
        return 0;
    }

    return (unsigned short)((INSTRUCTION_FREQUENCY - bps / 2) / bps);
}

int libusc_get_device_settings(libusc_device *dev, libusc_device_settings * settings)
{
    unsigned char buffer[14];
    libusc_control_transfer(dev, 0x80, 6, 0x0100, 0x0000, buffer, 14);
    settings->firmwareVersionMinor = (unsigned char)((buffer[12] & 0xF) + (buffer[12] >> 4 & 0xF) * 10);
    settings->firmwareVersionMajor = (unsigned char)((buffer[13] & 0xF) + (buffer[13] >> 4 & 0xF) * 10);

    settings->serialMode = (enum libusc_serial_mode)libusc_get_raw_parameter(dev, PARAMETER_SERIAL_MODE);
    settings->fixedBaudRate = libusc_convert_spbrg_to_bps(libusc_get_raw_parameter(dev, PARAMETER_SERIAL_FIXED_BAUD_RATE));
    settings->enableCrc = libusc_get_raw_parameter(dev, PARAMETER_SERIAL_ENABLE_CRC) != 0;
    settings->neverSuspend = libusc_get_raw_parameter(dev, PARAMETER_SERIAL_NEVER_SUSPEND) != 0;
    settings->serialDeviceNumber = (unsigned char)libusc_get_raw_parameter(dev, PARAMETER_SERIAL_DEVICE_NUMBER);
    settings->miniSscOffset = (unsigned char)libusc_get_raw_parameter(dev, PARAMETER_SERIAL_MINI_SSC_OFFSET);
    settings->serialTimeout = libusc_get_raw_parameter(dev, PARAMETER_SERIAL_TIMEOUT);
    settings->scriptDone = libusc_get_raw_parameter(dev, PARAMETER_SCRIPT_DONE) != 0;

    if (dev->channelcnt == 6)
    {
        settings->servosAvailable = (unsigned char)libusc_get_raw_parameter(dev, PARAMETER_SERVOS_AVAILABLE);
        settings->servoPeriod = (unsigned char)libusc_get_raw_parameter(dev, PARAMETER_SERVO_PERIOD);
    }
    else
    {
        unsigned int tmp = (unsigned int)(libusc_get_raw_parameter(dev, PARAMETER_MINI_MAESTRO_SERVO_PERIOD_HU) << 8);
        tmp |= (unsigned char)libusc_get_raw_parameter(dev, PARAMETER_MINI_MAESTRO_SERVO_PERIOD_L);
        settings->miniMaestroServoPeriod = tmp;

        settings->servoMultiplier = (unsigned short)(libusc_get_raw_parameter(dev, PARAMETER_SERVO_MULTIPLIER) + 1);
    }

    if (dev->channelcnt > 18)
    {
        settings->enablePullups = libusc_get_raw_parameter(dev, PARAMETER_ENABLE_PULLUPS) != 0;
    }

    return 1;
}

int libusc_set_device_settings(libusc_device *dev, libusc_device_settings * settings)
{
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_MODE, (unsigned char)settings->serialMode);
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_FIXED_BAUD_RATE, libusc_convert_bps_to_spbrg(settings->fixedBaudRate));
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_ENABLE_CRC, (unsigned short)(settings->enableCrc ? 1 : 0));
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_NEVER_SUSPEND, (unsigned short)(settings->neverSuspend ? 1 : 0));
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_DEVICE_NUMBER, settings->serialDeviceNumber);
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_MINI_SSC_OFFSET, settings->miniSscOffset);
    libusc_set_raw_parameter(dev, PARAMETER_SERIAL_TIMEOUT, settings->serialTimeout);
    libusc_set_raw_parameter(dev, PARAMETER_SCRIPT_DONE, (unsigned short)(settings->scriptDone ? 1 : 0));

    if (dev->channelcnt == 6)
    {
        libusc_set_raw_parameter(dev, PARAMETER_SERVOS_AVAILABLE, settings->servosAvailable);
        libusc_set_raw_parameter(dev, PARAMETER_SERVO_PERIOD, settings->servoPeriod);
    }
    else
    {
        libusc_set_raw_parameter(dev, PARAMETER_MINI_MAESTRO_SERVO_PERIOD_L, (unsigned char)(settings->miniMaestroServoPeriod & 0xFF));
        libusc_set_raw_parameter(dev, PARAMETER_MINI_MAESTRO_SERVO_PERIOD_HU, (unsigned short)(settings->miniMaestroServoPeriod >> 8));

        unsigned char multiplier;
        if (settings->servoMultiplier < 1)
        {
            multiplier = 0;
        }
        else if (settings->servoMultiplier > 256)
        {
            multiplier = 255;
        }
        else
        {
            multiplier = (unsigned char)(settings->servoMultiplier - 1);
        }
        libusc_set_raw_parameter(dev, PARAMETER_SERVO_MULTIPLIER, multiplier);
    }

    if (dev->channelcnt > 18)
    {
        libusc_set_raw_parameter(dev, PARAMETER_ENABLE_PULLUPS, (unsigned short)(settings->enablePullups ? 1 : 0));
    }

    return 1;
}

unsigned short specifyServo(unsigned short p, unsigned char servo)
{
    const unsigned char servoParameterBytes = 9;

    return (unsigned short)((unsigned char)(p) + servo * servoParameterBytes);
}

unsigned short exponentialSpeedToNormalSpeed(unsigned char exponentialSpeed)
{
    // Maximum value of normalSpeed is 31*(1<<7)=3968
    int mantissa = exponentialSpeed >> 3;
    int exponent = exponentialSpeed & 7;

    return (unsigned short)(mantissa * (1 << exponent));
}

unsigned char normalSpeedToExponentialSpeed(unsigned short normalSpeed)
{
    unsigned short mantissa = normalSpeed;
    unsigned char exponent = 0;

    while (1)
    {
        if (mantissa < 32)
        {
            // We have reached the correct representation.
            return (unsigned char)(exponent + (mantissa << 3));
        }

        if (exponent == 7)
        {
            // The number is too big to express in this format.
            return 0xFF;
        }

        // Try representing the number with a bigger exponent.
        exponent += 1;
        mantissa >>= 1;
    }
}

int libusc_get_channel_settings(libusc_device *dev, unsigned char channel, libusc_channel_settings * settings)
{
    if (dev->channelcnt == 6)
    {
        /*unsigned char bitmask = (byte)(1 << channelToPort(i));
        if ((ioMask & bitmask) == 0)
        {
            setting.mode = ChannelMode.Servo;
        }
        else if ((outputMask & bitmask) == 0)
        {
            setting.mode = ChannelMode.Input;
        }
        else
        {
            setting.mode = ChannelMode.Output;
        }*/
    }
    else
    {
        unsigned char channelModeBytes = (unsigned char)libusc_get_raw_parameter(dev, (unsigned char)PARAMETER_CHANNEL_MODES_0_3 + (channel >> 2));

        settings->mode = ((channelModeBytes >> ((channel & 3)<<1)) & 3);
    }

    unsigned short home = libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_HOME, channel));
    if (home == 0)
    {
        settings->homeMode = 0;
        settings->home = 0;
    }
    else if (home == 1)
    {
        settings->homeMode = 1;
        settings->home = 0;
    }
    else
    {
        settings->homeMode = 2;
        settings->home = home;
    }

    settings->minimum = (unsigned short)(64 * libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_MIN, channel)));
    settings->maximum = (unsigned short)(64 * libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_MAX, channel)));
    settings->neutral = libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_NEUTRAL, channel));
    settings->range = (unsigned short)(127 * libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_RANGE, channel)));
    settings->speed = exponentialSpeedToNormalSpeed((unsigned char)libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_SPEED, channel)));
    settings->acceleration = (unsigned char)libusc_get_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_ACCELERATION, channel));

    return 1;
}

int libusc_set_channel_settings(libusc_device *dev, unsigned char channel, libusc_channel_settings * settings)
{
    if (dev->channelcnt == 6)
    {
        /*
        if (setting.mode == ChannelMode.Input || setting.mode == ChannelMode.Output)
        {
            ioMask |= (byte)(1 << channelToPort(i));
        }

        if (setting.mode == ChannelMode.Output)
        {
            outputMask |= (byte)(1 << channelToPort(i));
        }*/
    }
    else
    {
        unsigned char parameter = PARAMETER_CHANNEL_MODES_0_3 + (channel >> 2);
        unsigned char channelModeBytes = (unsigned char)libusc_get_raw_parameter(dev, parameter);

        channelModeBytes &= (unsigned char)~(3 << ((channel & 3) << 1));
        channelModeBytes |= (unsigned char)(settings->mode << ((channel & 3) << 1));

        libusc_set_raw_parameter(dev, parameter, channelModeBytes);
    }

    // Make sure that HomeMode is "Ignore" for inputs.
    unsigned char correctedHomeMode = settings->homeMode;
    if (settings->mode == 3)
    {
        correctedHomeMode = 1;
    }

    // Compute the raw value of the "home" parameter.
    unsigned short home;
    if (correctedHomeMode == 0) home = 0;
    else if (correctedHomeMode == 1) home = 1;
    else home = settings->home;
    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_HOME, channel), home);

    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_MIN, channel), (unsigned short)(settings->minimum / 64));
    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_MAX, channel), (unsigned short)(settings->maximum / 64));
    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_NEUTRAL, channel), settings->neutral);
    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_RANGE, channel), (unsigned short)(settings->range / 127));
    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_SPEED, channel), normalSpeedToExponentialSpeed(settings->speed));
    libusc_set_raw_parameter(dev, specifyServo(PARAMETER_SERVO0_ACCELERATION, channel), settings->acceleration);

    return 1;
}

/// Erases the entire script and subroutine address table from the devices.
void libusc_erase_script(libusc_device *dev)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_ERASE_SCRIPT, 0, 0, NULL, 0);
}

void libusc_restart_script(libusc_device *dev)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_RESTART_SCRIPT, 0, 0, NULL, 0);
}

void libusc_set_script_done(libusc_device *dev, unsigned char value)
{
    libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_SET_SCRIPT_DONE, value, 0, NULL, 0);
}

void libusc_write_script(libusc_device *dev, const char* bytecode, int length)
{
    libusc_set_script_done(dev, 1); // stop the script

    /*
                // load the new script
                BytecodeProgram program = settings.bytecodeProgram;
                List<byte> byteList = program.getByteList();
                if (byteList.Count > maxScriptLength)
                {
                    throw new Exception("Script too long for device (" + byteList.Count + " bytes)");
                }
                if (byteList.Count < maxScriptLength)
                {
                    // if possible, add QUIT to the end to prevent mysterious problems with
                    // unterminated scripts
                    byteList.Add((byte)Opcode.QUIT);
                }
                eraseScript();
                setSubroutines(program.subroutineAddresses, program.subroutineCommands);
     */
    unsigned short block;
    for (block = 0; block < (length + 15) / 16; block++)
    {
        // write each block in a separate request
        unsigned char block_bytes[16];

        unsigned short j;
        for (j = 0; j < 16; j++)
        {
            if (block * 16 + j < length)
                block_bytes[j] = bytecode[block * 16 + j];
            else
                block_bytes[j] = (unsigned char)0xFF; // don't change flash if it is not necessary
        }

        libusc_control_transfer(dev, 0x40, (unsigned char)REQUEST_WRITE_SCRIPT, 0, block, block_bytes, 16);
    }


    //setRawParameter(uscParameter.PARAMETER_SCRIPT_CRC, program.getCRC());
}

