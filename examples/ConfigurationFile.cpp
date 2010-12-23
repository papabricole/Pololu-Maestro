#include "ConfigurationFile.h"

#include <QtGui>
#include <QXmlStreamWriter>
#include <QXmlSimpleReader>

ConfigurationFile::ConfigurationFile()
{
}

class Handler : public QXmlDefaultHandler
{
public:
    Handler(libusc_device* device) :
            QXmlDefaultHandler(),
            channel(0),
            m_device(device){}

    virtual bool startElement(const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
    {
        elementName = qName;

        if(elementName == "Channels")
        {
            for (int i=0; i<atts.count(); i++)
            {
                if (atts.qName(i) == "MiniMaestroServoPeriod")
                {
                    device_settings.miniMaestroServoPeriod = atts.value(i).toInt();
                }
                if (atts.qName(i) == "ServoMultiplier")
                {
                    device_settings.servoMultiplier = atts.value(i).toInt();
                }
            }
        }

        if(elementName == "Channel")
        {
            libusc_channel_settings channel_settings;
            for (int i=0; i<atts.count(); i++)
            {
                if (atts.qName(i) == "mode")
                {
                    if (atts.value(i) == "Servo") channel_settings.mode = 0;
                    else if (atts.value(i) == "Servo Multiplied") channel_settings.mode = 1;
                    else if (atts.value(i) == "Output") channel_settings.mode = 2;
                    else if (atts.value(i) == "Input") channel_settings.mode = 3;
                }
                if (atts.qName(i) == "min")
                {
                    channel_settings.minimum = atts.value(i).toInt();
                }
                if (atts.qName(i) == "max")
                {
                    channel_settings.maximum = atts.value(i).toInt();
                }

                if (atts.qName(i) == "homemode")
                {
                    if (atts.value(i) == "Off")  channel_settings.homeMode = 0;
                    else if (atts.value(i) == "Ignore")  channel_settings.homeMode = 1;
                    else if (atts.value(i) == "Goto")  channel_settings.homeMode = 2;
                }
                if (atts.qName(i) == "speed")
                {
                    channel_settings.speed = atts.value(i).toInt();
                }
                if (atts.qName(i) == "acceleration")
                {
                    channel_settings.acceleration = atts.value(i).toInt();
                }
                if (atts.qName(i) == "neutral")
                {
                    channel_settings.neutral = atts.value(i).toInt();
                }
                if (atts.qName(i) == "range")
                {
                    channel_settings.range = atts.value(i).toInt();
                }
            }
            libusc_set_channel_settings(m_device, channel++, &channel_settings);
        }
        return true;
    }

    virtual bool characters(const QString & ch)
    {
        if(elementName == "NeverSuspend")
        {
            device_settings.neverSuspend = (ch == "true") ? 1 : 0;
        }

        if(elementName == "SerialMode")
        {
            if (ch == "USB_DUAL_PORT") device_settings.serialMode = SERIAL_MODE_USB_DUAL_PORT;
            else if (ch == "USB_CHAINED") device_settings.serialMode = SERIAL_MODE_USB_CHAINED;
            else if (ch == "UART_DETECT_BAUD_RATE") device_settings.serialMode = SERIAL_MODE_UART_DETECT_BAUD_RATE;
            else if (ch == "UART_FIXED_BAUD_RATE") device_settings.serialMode = SERIAL_MODE_UART_FIXED_BAUD_RATE;
        }

        if(elementName == "FixedBaudRate")
        {
            device_settings.fixedBaudRate = ch.toInt();
        }
        if(elementName == "SerialTimeout")
        {
            device_settings.serialTimeout = ch.toInt();
        }
        if(elementName == "EnableCrc")
        {
            device_settings.enableCrc = (ch == "true") ? 1 : 0;
        }
        if(elementName == "SerialDeviceNumber")
        {
            device_settings.serialDeviceNumber = ch.toInt();
        }
        if(elementName == "SerialMiniSscOffset")
        {
            device_settings.miniSscOffset = ch.toInt();
        }
        if(elementName == "Script ScriptDone")
        {
            device_settings.scriptDone = (ch == "true") ? 1 : 0;
        }

        return true;
    }

    virtual bool endDocument ()
    {
        libusc_set_device_settings(m_device, &device_settings);

        return true;
    }

    int channel;
    QString elementName;

    libusc_device_settings device_settings;

    libusc_device* m_device;
};

bool ConfigurationFile::load(libusc_device* device, QIODevice* io)
{
    QXmlSimpleReader xmlReader;
    QXmlInputSource source(io);
    Handler handler(device);
    xmlReader.setContentHandler(&handler);
    xmlReader.setErrorHandler(&handler);

    return xmlReader.parse(&source);
}

/// The approximate number of microseconds represented by the servo
/// period when PARAMETER_SERVO_PERIOD is set to this value.
int ConfigurationFile::periodToMicroseconds(unsigned short period, unsigned char servos_available)
{
    return (int)period * 256 * servos_available / 12;
}

int ConfigurationFile::periodInMicroseconds(libusc_device *dev, libusc_device_settings* settings)
{
    if (libusc_get_num_channels(dev) == 6)
    {
        return periodToMicroseconds(settings->servoPeriod, settings->servosAvailable);
    }
    else
    {
        return settings->miniMaestroServoPeriod / 4;
    }
}

bool ConfigurationFile::save(libusc_device* device, QIODevice* io)
{
    libusc_device_settings device_settings;
    libusc_get_device_settings(device, &device_settings);

    QXmlStreamWriter stream(io);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeComment("Pololu Maestro servo controller settings file, http://www.pololu.com/catalog/product/1350");

    stream.writeStartElement("UscSettings");
    stream.writeAttribute("version", "1");

    stream.writeTextElement("NeverSuspend", device_settings.neverSuspend ? "true" : "false");
    switch(device_settings.serialMode)
    {
    case SERIAL_MODE_USB_DUAL_PORT:         stream.writeTextElement("SerialMode", "USB_DUAL_PORT");         break;
    case SERIAL_MODE_USB_CHAINED:           stream.writeTextElement("SerialMode", "USB_CHAINED");           break;
    case SERIAL_MODE_UART_DETECT_BAUD_RATE: stream.writeTextElement("SerialMode", "UART_DETECT_BAUD_RATE"); break;
    case SERIAL_MODE_UART_FIXED_BAUD_RATE:  stream.writeTextElement("SerialMode", "UART_FIXED_BAUD_RATE");  break;
    }
    stream.writeTextElement("FixedBaudRate", QString::number(device_settings.fixedBaudRate));
    stream.writeTextElement("SerialTimeout", QString::number(device_settings.serialTimeout));
    stream.writeTextElement("EnableCrc", device_settings.enableCrc ? "true" : "false");
    stream.writeTextElement("SerialDeviceNumber", QString::number(device_settings.serialDeviceNumber));
    stream.writeTextElement("SerialMiniSscOffset", QString::number(device_settings.miniSscOffset));

    stream.writeStartElement("Channels");
    stream.writeAttribute("MiniMaestroServoPeriod", QString::number(device_settings.miniMaestroServoPeriod));
    stream.writeAttribute("ServoMultiplier", QString::number(device_settings.servoMultiplier));
    stream.writeComment(QString("Period = %1 ms").arg(periodInMicroseconds(device, &device_settings) / 1000));
    for (int i=0; i<libusc_get_num_channels(device); i++)
    {
        libusc_channel_settings channel_settings;
        libusc_get_channel_settings(device, i, &channel_settings);

        stream.writeComment(QString("Channel %1").arg(i));
        stream.writeStartElement("Channel");
        stream.writeAttribute("name", "");

        switch(channel_settings.mode)
        {
        case 0: stream.writeAttribute("mode", "Servo");            break;
        case 1: stream.writeAttribute("mode", "Servo Multiplied"); break;
        case 2: stream.writeAttribute("mode", "Output");           break;
        case 3: stream.writeAttribute("mode", "Input");            break;
        }

        stream.writeAttribute("min", QString::number(channel_settings.minimum));
        stream.writeAttribute("max", QString::number(channel_settings.maximum));

        switch(channel_settings.homeMode)
        {
        case 0: stream.writeAttribute("homemode", "Off");    break;
        case 1: stream.writeAttribute("homemode", "Ignore"); break;
        case 2: stream.writeAttribute("homemode", "Goto");   break;
        }

        stream.writeAttribute("home", QString::number(channel_settings.home));
        stream.writeAttribute("speed", QString::number(channel_settings.speed));
        stream.writeAttribute("acceleration", QString::number(channel_settings.acceleration));
        stream.writeAttribute("neutral", QString::number(channel_settings.neutral));
        stream.writeAttribute("range", QString::number(channel_settings.range));

        stream.writeEndElement(); // Channel
    }

    stream.writeEndElement(); // Channels

    stream.writeTextElement("Script ScriptDone", device_settings.scriptDone ? "true" : "false");

    stream.writeEndElement(); // UscSettings
    stream.writeEndDocument();

    return true;
}
