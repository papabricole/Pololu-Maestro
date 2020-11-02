#include <libmaestro.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// clang-format off
PYBIND11_MODULE(maestro, m)
{
    m.attr("__version__") = "1.0.0";
    m.def("getConnectedDevices", &MaestroDevice::getConnectedDevices);

    py::class_<MaestroDevice> device(m, "Device");

    py::enum_<MaestroDevice::SerialMode>(device, "SerialMode")
        .value("SERIAL_MODE_USB_DUAL_PORT", MaestroDevice::SerialMode::SERIAL_MODE_USB_DUAL_PORT)
        .value("SERIAL_MODE_USB_CHAINED", MaestroDevice::SerialMode::SERIAL_MODE_USB_CHAINED)
        .value("SERIAL_MODE_UART_DETECT_BAUD_RATE", MaestroDevice::SerialMode::SERIAL_MODE_UART_DETECT_BAUD_RATE)
        .value("SERIAL_MODE_UART_FIXED_BAUD_RATE", MaestroDevice::SerialMode::SERIAL_MODE_UART_FIXED_BAUD_RATE)
        .export_values();

    py::class_<MaestroDevice::DeviceSettings>(device, "DeviceSettings")
        .def_readwrite("firmwareVersionMinor", &MaestroDevice::DeviceSettings::firmwareVersionMinor)
        .def_readwrite("firmwareVersionMajor", &MaestroDevice::DeviceSettings::firmwareVersionMajor)
        .def_readwrite("servosAvailable", &MaestroDevice::DeviceSettings::servosAvailable)
        .def_readwrite("servoPeriod", &MaestroDevice::DeviceSettings::servoPeriod)
        .def_readwrite("miniMaestroServoPeriod", &MaestroDevice::DeviceSettings::miniMaestroServoPeriod)
        .def_readwrite("servoMultiplier", &MaestroDevice::DeviceSettings::servoMultiplier)
        .def_readwrite("fixedBaudRate", &MaestroDevice::DeviceSettings::fixedBaudRate)
        .def_readwrite("enableCrc", &MaestroDevice::DeviceSettings::enableCrc)
        .def_readwrite("neverSuspend", &MaestroDevice::DeviceSettings::neverSuspend)
        .def_readwrite("serialDeviceNumber", &MaestroDevice::DeviceSettings::serialDeviceNumber)
        .def_readwrite("miniSscOffset", &MaestroDevice::DeviceSettings::miniSscOffset)
        .def_readwrite("serialTimeout", &MaestroDevice::DeviceSettings::serialTimeout)
        .def_readwrite("scriptDone", &MaestroDevice::DeviceSettings::scriptDone)
        .def_readwrite("enablePullups", &MaestroDevice::DeviceSettings::enablePullups)
        ;

    py::enum_<MaestroDevice::ChannelMode>(device, "ChannelMode") 
        .value("SERVO", MaestroDevice::ChannelMode::SERVO)
        .value("SERVO_MULTIPLIED", MaestroDevice::ChannelMode::SERVO_MULTIPLIED)
        .value("OUTPUT", MaestroDevice::ChannelMode::OUTPUT)
        .value("INPUT", MaestroDevice::ChannelMode::INPUT)
        .export_values();

    py::enum_<MaestroDevice::HomeMode>(device, "HomeMode")
        .value("OFF", MaestroDevice::HomeMode::OFF)
        .value("IGNORE", MaestroDevice::HomeMode::IGNORE)
        .value("GOTO", MaestroDevice::HomeMode::GOTO)
        .export_values();

    py::class_<MaestroDevice::ChannelSettings>(device, "ChannelSettings")
        .def_readwrite("mode", &MaestroDevice::ChannelSettings::mode)
        .def_readwrite("homeMode", &MaestroDevice::ChannelSettings::homeMode)
        .def_readwrite("home", &MaestroDevice::ChannelSettings::home)
        .def_readwrite("minimum", &MaestroDevice::ChannelSettings::minimum)
        .def_readwrite("maximum", &MaestroDevice::ChannelSettings::maximum)
        .def_readwrite("neutral", &MaestroDevice::ChannelSettings::neutral)
        .def_readwrite("range", &MaestroDevice::ChannelSettings::range)
        .def_readwrite("speed", &MaestroDevice::ChannelSettings::speed)
        .def_readwrite("acceleration", &MaestroDevice::ChannelSettings::acceleration)
        ;

    py::class_<MaestroDevice::ServoStatus>(device, "ServoStatus")
        .def_readonly("position", &MaestroDevice::ServoStatus::position)
        .def_readonly("target", &MaestroDevice::ServoStatus::target)
        .def_readonly("speed", &MaestroDevice::ServoStatus::speed)
        .def_readonly("acceleration", &MaestroDevice::ServoStatus::acceleration)
        ;

    device.def("getName", &MaestroDevice::getName)
          .def("getNumChannels", &MaestroDevice::getNumChannels)
          .def("setTarget", &MaestroDevice::setTarget)
          .def("setSpeed", &MaestroDevice::setSpeed)
          .def("setAcceleration", &MaestroDevice::setAcceleration)
          .def("getServoStatus", &MaestroDevice::getServoStatus)
          .def("restoreDefaultConfiguration", &MaestroDevice::restoreDefaultConfiguration)
          .def("getDeviceSettings", &MaestroDevice::getDeviceSettings)
          .def("setDeviceSettings", &MaestroDevice::setDeviceSettings)
          .def("getChannelSettings", &MaestroDevice::getChannelSettings)
          .def("setChannelSettings", &MaestroDevice::setChannelSettings)
          .def("eraseScript", &MaestroDevice::eraseScript)
          .def("restartScriptAtSubroutine", &MaestroDevice::restartScriptAtSubroutine)
          .def("restartScriptAtSubroutineWithParameter", &MaestroDevice::restartScriptAtSubroutineWithParameter)
          .def("restartScript", &MaestroDevice::restartScript)
          .def("setScriptDone", &MaestroDevice::setScriptDone)
          .def("startBootloader", &MaestroDevice::startBootloader)
          .def("reinitialize", &MaestroDevice::reinitialize)
          .def("clearErrors", &MaestroDevice::clearErrors)
          .def("writeScript", &MaestroDevice::writeScript)
          .def("setPWM", &MaestroDevice::setPWM)
          .def("disablePWM", &MaestroDevice::disablePWM)
          ;
}
