#ifndef QXCHANNEL_H
#define QXCHANNEL_H

#include <QtGui>
#include <libusc.h>

class QxChannelStatus : public QObject
{
    Q_OBJECT
public:
    QxChannelStatus(libusc_device* device, int index, QTableWidget* parent);
protected:
    virtual void timerEvent(QTimerEvent * event);
private slots:
    void modeChanged(int);
    void sliderMoved(int value);
private:
    QTableWidget* m_tableWidget;
    libusc_device* m_device;
    int m_index;
    QComboBox* m_mode;
    QSlider* m_slider;
    QSpinBox* m_target;
    QSpinBox* m_speed;
    QSpinBox* m_acceleration;
    QSpinBox* m_position;
};

#endif // QXCHANNEL_H
