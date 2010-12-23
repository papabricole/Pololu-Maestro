#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H

#include <QtGui>

#include <libusc.h>

class ConfigurationFile
{
public:
    /// Parses a saved configuration file
    static bool load(libusc_device* device, QIODevice* io);

    /// Saves a UscSettings object to a textfile.
    /// <param name="device">The device to read from.</param>
    static bool save(libusc_device* device, QIODevice* io);
private:
    static int periodToMicroseconds(unsigned short period, unsigned char servos_available);
    static int periodInMicroseconds(libusc_device *dev, libusc_device_settings* settings);

    ConfigurationFile();
};

#endif // CONFIGURATIONFILE_H
