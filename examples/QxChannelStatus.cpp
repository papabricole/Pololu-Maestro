#include "QxChannelStatus.h"

QxChannelStatus::QxChannelStatus(libusc_device* device, int index, QTableWidget* parent) :
        QObject(parent),
        m_tableWidget(parent),
        m_device(device),
        m_index(index)
{
    m_mode = new QComboBox();
    m_mode->addItem("SERVO", 0);
    m_mode->addItem("SERVOMULTIPLIED", 1);
    m_mode->addItem("OUTPUT", 2);
    m_mode->addItem("INPUT", 3);

    libusc_channel_settings device_settings;
    libusc_get_channel_settings(m_device, m_index, &device_settings);

    m_mode->setCurrentIndex( device_settings.mode);

    connect(m_mode, SIGNAL(activated(int)), this, SLOT(modeChanged(int)));

    m_slider = new QSlider(Qt::Horizontal);

    libusc_servo_status status;
    libusc_get_servo_status(m_device, m_index, &status);

    libusc_channel_settings channel_settings;
    libusc_get_channel_settings(m_device, m_index, &channel_settings);

    m_slider->setRange(channel_settings.minimum, channel_settings.maximum);
    m_slider->setValue( status.position );

    connect(m_slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));

    m_target = new QSpinBox();
    m_target->setRange(channel_settings.minimum, channel_settings.maximum);
    m_target->setValue( status.position );

    connect(m_slider, SIGNAL(sliderMoved(int)), m_target, SLOT(setValue(int)));

    m_speed= new QSpinBox();
    m_speed->setValue(channel_settings.speed);
    m_acceleration= new QSpinBox();
    m_acceleration->setValue(channel_settings.acceleration);
    m_position= new QSpinBox();
    m_position->setRange(channel_settings.minimum, channel_settings.maximum);

    connect(m_position, SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));

    m_tableWidget->setCellWidget(index, 0, m_mode);
    m_tableWidget->setCellWidget(index, 1, m_slider);
    m_tableWidget->setCellWidget(index, 2, m_target);
    m_tableWidget->setCellWidget(index, 3, m_speed);
    m_tableWidget->setCellWidget(index, 4, m_acceleration);
    m_tableWidget->setCellWidget(index, 5, m_position);

    startTimer(100);
}

void QxChannelStatus::timerEvent(QTimerEvent * event)
{
    libusc_servo_status status;
    libusc_get_servo_status(m_device, m_index, &status);

    m_position->setValue( status.position );
}

void QxChannelStatus::modeChanged(int mode)
{
    libusc_channel_settings settings;
    libusc_get_channel_settings(m_device, m_index, &settings);
    settings.mode = mode;
    libusc_set_channel_settings(m_device, m_index, &settings);
}

void QxChannelStatus::sliderMoved(int value)
{
    libusc_set_target(m_device, m_index, value);
}
