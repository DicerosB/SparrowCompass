This Directory contains all files reqiured for setting up the Adruino IDE in order to use Sparrow Compass. The following Steps describes the installation of the library and the board variant

# Prerequisites
- Arduino IDE 2.0+ [Download Page](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/)
- Stm32CubeProgrammer [Download Page](https://www.st.com/en/development-tools/stm32cubeprog.html)

# Installation

## Adding the STM32 Boards Manager
Open the Arduino IDE, select File -> Preferences.\
Paste the following link to "Additional boards manager URLs"
~~~
https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
~~~
Wait until Download finishes. Then open Boards Manager Tab (Sidebar) and Install "STM32 MCU based boards".

If installation succeeded, a new option should appear under Tools -> Board

## Extending the Boards Manager with Sparrow Compass variant

Copy the content of the "variant" folder in this directory.

Locate the Arduino installation directory. Most often it is `C:\\Users\<USERNAME>\Appdata\Local\Arduino15`

Navigate to `packages\STM32Microelectronics\hardware\stm32\<VERSION>\`

Paste the content into the "variant" folder.

Last step is to add the menu entries to `boards.txt`.\
For that, just copy the content of the file `boards_entry.txt` (located in the previously pasted folder) at the top of the `boards.txt`

In order to make the changes visible, the Arduino IDE cache must be cleared. This can be archeived by deleting `C:\\Users\<USERNAME>\Appdata\Roaming\arduino-ide\LocalStorage` and restarting the IDE.

Now a "Sparrow Compass" option should appear under Tools -> Board -> STM32 MCU based boards

## Installing the Sparrow Compass Library

Copy the content of the "libraries" folder in this directory.

Paste the content into the Arduino Library folder, which is most often located under `Documents\Arduino\libraries`

If the installation was successful, an example script should appear under File -> Example -> Examples from Custom Libraries

If the library is still missing, try to clear the IDE cache again like described [before](#extending-the-boards-manager-with-sparrow-compass-variant)

# Usage

There are two opions for flashing the microcontroller. One is flashing via SWD (Serial Wire Debug), the other is via USB and the DFU protocol. SWD is faster and allows for a debuggung functionality but it also requires to hook up a swd probe to the exposed swd pins on the board. DFU flashing is possible with the custom dfu bootloader using the usb-c port of the device.

## Flasing via SWD

Connect the debug probe to the GND (G), DIO(D) and SCK(C) Pins located on left side of the board.

Plug the probe into your PC and connect power to the device.

In the Arduino IDE, select Tools -> Upload method -> STM32CubeProgrammer (SWD).

Now, the device can be flashed via the upload icon.

## Flashing via USB

Connect the device via USB to your PC.

Open a serial terminal (Tools -> Serial Monitor) and connect to the VCOM Port, which has just been connected. The Baud Rate can be ignored, as the VCOM connection has no underlying UART. 

In order to enter bootloader (DFU) mode, the keyword "deadbeef" must be sent via serial terminal. In bootloader mode, the LED near the Microcontroller is blinking every 100 ms. 

In the Arduino IDE, select Tools -> Upload method -> STM32CubeProgrammer (DFU).

Now, the device can be flashed via the upload icon. After flashing, the device automatically resets and enters normal mode again.

## Debugging via SWD

Enable debug symbols: Tools -> Debug symbols and core logs.

Optimize for Debug: Tools -> Optimize -> Debug (Og).

Make sure you flash your code before debugging.

# Development
Arduino IDE is loading libraries only form the default library directory (Most often `Documents/Arduino/libraries`). As this library is a part of a bigger (git) project, it's inconvenient to develop and debug code inside this directory. A good workaround is the use of symbolic links.

Windows does not allow the creation of symbolic direcory links by default, so this needs to be enabled in Windows Settings by activating the Developer mode under System->Advanced->Developer mode.

Now a symbolic link can be created via cmd (not Powershell) in the Arduino libraries directory:
~~~
mklink /D <LINK NAME> <PATH-TO-sparrow_compass_lib>
~~~
