This Directory contains all files reqiured for setting up the Adruino IDE in order to use Sparrow Compass. The following Steps describes the installation of the library and the board variant

# Prerquisites
- Arduino IDE 2.0+ [Download Page](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/)
- Stm32CubeProgrammer [Download Page](https://www.st.com/en/development-tools/stm32cubeprog.html)

# Installation

## Adding the STM32 Boards Manager
Open the Arduino IDE, select File -> Preferences.\
Paste the following link to "Additional boards manager URLs"
~~~
https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
~~~
Wait until Download finishes. Then open Boards Manager Tab (Sidebar) and Install "SM32 MCU based boards".

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

Now clear the IDE cache again like described [before](#extending-the-boards-manager-with-sparrow-compass-variant)

If the installation was successful, an example script should appear under File -> Example -> Examples from Custom Libraries

# Usage
