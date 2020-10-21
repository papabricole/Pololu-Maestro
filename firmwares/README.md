# Upgrading Firmware

The firmware is the program that runs on the Maestro. The Maestro has field-upgradeable firmware that can be easily updated with bug fixes or new features. There are four types of Maestros: the Micro Maestro 6-Channel USB Servo Controller (usc02a), the Mini Maestro 12-Channel USB Servo Controller (usc03a), the Mini Maestro 18-Channel USB Servo Controller (usc03b), and the Mini Maestro 24-Channel USB Servo Controller (usc03c). Each type of Maestro has its own firmware and you cannot load the firmware for one type onto another.

# Upgrade Instructions

You can determine the version of your Maestro’s firmware by running the Maestro Control Center, connecting to a Maestro, and looking at the firmware version number which is displayed in the upper left corner next to the “Connected to” drop-down box. If you do not already have the latest version (1.03 for Mini Maestros, 1.04 for the Micro Maestro), you can upgrade by following the instructions below:

All of your settings will be reset to default values during the firmware upgrade.

1. Determine which type of Maestro you have either by counting the number of channels or by looking at the name that appears in your Device Manager. There are four types of Maestro: the Micro Maestro 6-Channel USB Servo Controller (usc02a), the Mini Maestro 12-Channel USB Servo Controller (usc03a), the Mini Maestro 18-Channel USB Servo Controller (usc03b), and the Mini Maestro 24-Channel USB Servo Controller (usc03c).
2. Connect your Maestro to a Windows or Linux computer using a USB cable.
3. Run the Maestro Control Center and connect to the Maestro by selecting its serial number in the “Connected to:” drop-down box in the upper left corner.
4. If you were not able to connect to the Maestro using the Maestro Control Center, double-check your USB connection, make sure all other devices are disconnected from the Maestro, and try plugging it into several different USB ports on your computer. If you are still unable to connect to it, see the instructions in Section 4.f.1 for doing a hard bootloader reset.
5. Go to the Device menu and select “Upgrade firmware…”. You will see a message asking you if you are sure you want to proceed: click OK.
6. If you are using Windows XP and see a Found New Hardware Wizard window appear, then you should follow steps 6–8 from Section 3.a to get the bootloader’s driver working.
7. Once the Maestro is in bootloader mode and the bootloader’s drivers are properly installed, the green LED should be blinking in a double heart-beat pattern, and there should be an entry for the bootloader in the “Ports (COM & LPT)” list of your computer’s Device Manager.
8. Go to the window entitled “Firmware Upgrade” that the Maestro Control Center has opened.
9. Click the “Browse…” button and select the firmware file you downloaded. Make sure that the selected file is the right file for your type of Maestro (see steps 2 and 3).
10. Select the COM port corresponding to the bootloader. If you do not know which COM port to select, go to the Device Manager and look in the “Ports (COM & LPT)” section.
11. Click the “Program” button. You will see a message warning you that your device’s firmware is about to be erased and asking you if you are sure you want to proceed: click Yes.
12. It will take a few seconds to erase the Maestro’s existing firmware and load the new firmware. Do not disconnect the Maestro during the upgrade.
13. Once the upgrade is complete, the Firmware Upgrade window will close, the Maestro will disconnect from your computer, and it will reappear. If there is only one Maestro plugged in to your computer, the Maestro Control Center will connect to it. Check the firmware version number and make sure that it now indicates the latest version of the firmware.

If you have problems during or after the firmware upgrade, then it is possible that you loaded the wrong firmware onto your Maestro or some other problem corrupted the firmware. The solution is to retry the firmware upgrade procedure above. Even if your Maestro is not recognized at all by your computer and you see no sign of life from it, the instructions in step 4 can help you get the Maestro into bootloader mode. 