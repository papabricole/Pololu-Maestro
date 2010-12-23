/*
* Public libusc header file
* Copyright (C) 2010 Morgan Leborgne <morganleborgne@gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef __LIBUSC_H__
#define __LIBUSC_H__

#include <sys/types.h>
#include <libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

    enum libusc_serial_mode
    {
        /// On the Command Port, user can send commands and receive responses.
        /// TTL port/UART are connected to make a USB-to-serial adapter.</summary>
        SERIAL_MODE_USB_DUAL_PORT = 0,

        /// On the Command Port, user can send commands to Maestro and
        /// simultaneously transmit bytes on the UART TX line, and user
        /// can receive bytes from the Maestro and the UART RX line.
        /// TTL port does not do anything.
        SERIAL_MODE_USB_CHAINED = 1,

        /// On the UART, user can send commands and receive reponses after
        /// sending a 0xAA byte to indicate the baud rate.
        /// Command Port receives bytes from the RX line.
        /// TTL Port does not do anything.
        SERIAL_MODE_UART_DETECT_BAUD_RATE = 2,

        /// On the UART, user can send commands and receive reponses
        /// at a predetermined, fixed baud rate.
        /// Command Port receives bytes from the RX line.
        /// TTL Port does not do anything.
        SERIAL_MODE_UART_FIXED_BAUD_RATE = 3
    };

    /// This struct represents all the settings for a Maestro.
    struct libusc_device_settings
    {
        unsigned char firmwareVersionMinor;
        unsigned char firmwareVersionMajor;

        /// The number of servo ports available (0-5).  This, along with the
        /// servoPeriod, determine the "maximum maximum pulse width".
        unsigned char servosAvailable;

        /// This setting only applies to the Micro Maestro.
        /// For the Mini Maestro, see miniMaestroServoPeriod.
        ///
        /// The total time allotted to each servo channel, in units of
        /// 256/12 = 21.33333 us.  The unit for this one are unusual, because
        /// that is the way it is stored on the device and its unit is not
        /// a multiple of 4, so we would have inevitable rounding errors if we
        /// tried to represent it in quarter-microseconds.
        ///
        /// Default is 156, so with 6 servos available you get ~20ms between
        /// pulses on a given channel.
        unsigned char servoPeriod;

        /// This setting only applies to the Mini Maestro.
        /// For the Micro Maestro, see microMaestroServoPeriod.
        ///
        /// The length of the time period in which the Mini Maestro sends pulses
        /// to all the enabled servos, in units of quarter microseconds.
        ///
        /// Valid values for this parameter are 0 to 16,777,215.  But
        ///
        /// Default is 80000, so each servo receives a pulse every 20 ms (50 Hz).
        unsigned int miniMaestroServoPeriod;

        /// This setting only applied to the Mini Maestro.
        /// The non-multiplied servos have a period specified by miniMaestroServoPeriod.
        /// The multiplied servos have a period specified by miniMaestroServoPeriod*servoMultiplier.
        ///
        /// Valid values for this parameter are 1 to 256.
        unsigned short servoMultiplier;

        /// Determines how serial bytes flow between the two USB COM ports, the TTL port,
        /// and the Maestro's serial command processor.
        enum libusc_serial_mode serialMode;

        /// The fixed baud rate, in units of bits per second.  This gets stored in a
        /// different format on the usc.cs, so there will be rounding errors
        /// which get bigger at higher baud rates, but they will be less than
        /// 1% for baud rates of 120000 or less.
        ///
        /// This parameter only applies if serial mode is USB UART Fixed Baud.
        ///
        /// All values above 184 are valid, but values significantly higher than
        /// 250000 are subject to high rounding errors and the usc firmware might not
        /// be able to keep up with those higher data rates.  If the baud rate is too
        /// high and the firmware can't keep up, the Maestro will indicate this to you
        /// by generating a serial overrun or buffer full error.
        unsigned int fixedBaudRate;

        /// If true, then you must send a 7-bit CRC byte at the end of every serial
        /// command (except the Mini SSC II command).
        unsigned char enableCrc;

        /// If true, then the Maestro will never go to sleep.  This lets you power
        /// the processer off of USB even when the computer has gone to sleep and put
        /// all of its USB devices in the suspend state.
        unsigned char neverSuspend;

        /// The serial device number used to identify this device in Pololu protocol
        /// commands.  Valid values are 0-127, default is 12.
        unsigned char serialDeviceNumber;


        /// The offset used to determine which Mini SSC commands this device will
        /// respond to.  The second byte of the Mini SSC command contains the servo
        /// number; the correspondence between servo number and maestro number (0-5)
        /// is servo# = miniSSCoffset + channel#.  Valid values are 0-254.
        unsigned char miniSscOffset;

        /// The time it takes for a serial timeout error to occur, in units of 10 ms.
        /// A value of 0 means no timeout error will occur.  All values 0-65535 are valid.
        unsigned short serialTimeout;

        /// True if the script should not be started when the device starts up.
        /// False if the script should be started.
        unsigned char scriptDone;

        /// If true, this setting enables pullups for each channel 18-20 which
        /// is configured as an input.  This makes the input value be high by
        /// default, allowing the user to connect a button or switch without
        /// supplying their own pull-up resistor.  Thi setting only applies to
        /// the Mini Maestro 24-Channel Servo Controller.
        unsigned char enablePullups;
    };

    /// An object that represents the settings for one servo,
    /// e.g. the information in the Settings tab.  One of these objects
    /// corresponds to one ServoSettingsControl.
    struct libusc_channel_settings
    {
        /// Type (0=servo, 1=servo multiplied, 2=output, 3=input).
        unsigned char mode;

        /// HomeType (0=off, 1=ignore, 2=goto).
        unsigned char homeMode;

        /// Home position: the place to go on startup.
        /// If type==servo, units are 0.25 us (qus).
        /// If type==output, the threshold between high and low is 1500.
        ///
        /// This value is only saved on the device if homeType == Goto.
        unsigned short home;

        /// Minimum (units of 0.25 us, but stored on the device in units of 16 us).
        unsigned short minimum;

        /// Maximum (units of 0.25 us, but stored on the device in units of 16 us).
        unsigned short maximum;

        /// Neutral: the center of the 8-bit set target command (value at 127).
        /// If type==servo, units are 0.25 us (qus).
        /// If type==output, the threshold between high and low is 1500.
        unsigned short neutral;

        /// Range: the +/- extent of the 8-bit command.
        ///   8-bit(254) = neutral + range,
        ///   8-bit(0) = neutral - range
        /// If type==servo units are 0.25 us (qus) (but stored on device in
        /// units of 127*0.25us = 31.75 us.
        /// Range = 0-127*255 = 0-32385 qus.
        /// Increment = 127 qus
        unsigned short range;

        /// Speed: the maximum change in position (qus) per update.  0 means no limit.
        /// Units depend on your settings.
        /// Stored on device in this format: [0-31]*2^[0-7]
        /// Range = 0-31*2^7 = 0-3968.
        /// Increment = 1.
        ///
        /// Note that the *current speed* is stored on the device in units
        /// of qus, and so it is not subject to the restrictions above!
        /// It can be any value 0-65535.
        unsigned short speed;

        /// Acceleration: the max change in speed every 80 ms.  0 means no limit.
        /// Units depend on your settings.
        /// Range = 0-255.
        /// Increment = 1.
        unsigned char acceleration;
    };

    /// Represents the current status of a channel.
    struct libusc_servo_status
    {
        /// The position in units of quarter-microseconds.
        unsigned short position;

        /// The target position in units of quarter-microseconds.
        unsigned short target;

        /// The speed limit.  Units depends on your settings.
        unsigned short speed;

        /// The acceleration limit.  Units depend on your settings.
        unsigned char acceleration;
    };


    typedef struct libusc_device_settings libusc_device_settings;
    typedef struct libusc_channel_settings libusc_channel_settings;
    typedef struct libusc_servo_status libusc_servo_status;
    typedef struct libusc_device libusc_device;

    ssize_t libusc_get_device_list(libusb_context *ctx, libusc_device ***list);
    void libusc_free_device_list(libusc_device **list, int unref_devices);

    libusc_device *libusc_ref_device(libusc_device *dev);
    void libusc_unref_device(libusc_device *dev);

    int libusc_get_num_channels(libusc_device *dev);

    void libusc_set_target(libusc_device *dev, unsigned char servo, unsigned short value);
    void libusc_set_speed(libusc_device *dev, unsigned char servo, unsigned short value);
    void libusc_set_acceleration(libusc_device *dev, unsigned char servo, unsigned short value);
    int libusc_get_servo_status(libusc_device *dev, unsigned char servo, libusc_servo_status * status);

    void libusc_restore_default_configuration(libusc_device *dev);
    
    int libusc_get_device_settings(libusc_device *dev, libusc_device_settings * settings);
    int libusc_set_device_settings(libusc_device *dev, libusc_device_settings * settings);

    int libusc_get_channel_settings(libusc_device *dev, unsigned char channel, libusc_channel_settings * settings);
    int libusc_set_channel_settings(libusc_device *dev, unsigned char channel, libusc_channel_settings * settings);

    void libusc_erase_script(libusc_device *dev);
    void libusc_restart_script(libusc_device *dev);
    void libusc_set_script_done(libusc_device *dev, unsigned char value);
    void libusc_write_script(libusc_device *dev, const char* bytecode, int length);

#ifdef __cplusplus
}
#endif

#endif
