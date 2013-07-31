IGADScheduleICSConverter
========================

Software to download IGAD schedules from the NHTV roster website, and convert them to .ics format (iCalendar) so that they can be read by digital agendas.

How to use
------------------------

You have two options:

- ScheduleICSConverter_CLI.exe
- SchoolScheduleICSConverter_GUI.exe

Both of these can be found in the *dist* folder. To use either one of these, *curl.exe* and *libSICSC.dll* need to be present (by default they are, if you grab *dist* from GitHub).
To run *SchoolScheduleICSConverter_GUI.exe*, you also need to have the ScheduleICSConverter_CLI.exe somewhere on your computer (the program let's you specify where to find the CLI .exe).

### ScheduleICSConverter_CLI.exe

If you've run ScheduleICSConverter_CLI.exe and don't know how to operate it, it's probably not for you and you should be using SchoolScheduleICSConverter_GUI.exe (the one with the fancy white/blue icon). See below.

### SchoolScheduleICSConverter_GUI.exe

If you've read and followed the instructions above, you should be able to run SchoolScheduleICSConverter_GUI.exe by double clicking on it. This will open a window such as this:

![](http://i.imgur.com/2yg4Y2Z.png)

You should first specify where to find the ScheduleICSConverter_CLI.exe program. If it's located in the same folder, simply press "Current":

![](http://i.imgur.com/GgIRxuU.png)

You can now choose which schedules to download by selecting one or multiple years, variations and classes (selecting more than one option from each box is done by holding the *control* key while selecting options).
To download ALL available schedules for all years, all variations and all classes, select AUTO mode (instead of MANUAL).

![](http://i.imgur.com/jTk7rkV.png)

Optionally, you can now choose a starting year for the current schoolyear (should be set correctly by default, but you can change it if you so desire. If you don't know whether you should change this, leave it at what it is by default!).
You can also now choose which range of weeks to download (Again, this should be set correctly by default to the range of weeks 0 thru 52. You won't have to change anything).

Now, you can press "Run" to let the software do its thing. While it's busy you will see black command windows pop up and close, this is supposed to happen so don't freak out ;)
If the "View Output When Done" checkbox is checked, the software will automatically open the output folder containing all generated .ics files. This folder is always located in the same folder as the program, and it's always called 'output'.

![](http://i.imgur.com/4XyEPH8.png)

You can now import the .ics files into your digital agenda!

How to build
------------------------

- Open the solution in Visual Studio and build it.
- Grab a copy of [*curl.exe*](http://curl.haxx.se/) and place it in the output folder (either *dist/Debug* or *dist/Release*).
- Pick your poison:
  - *ScheduleICSConverter_CLI.exe* : This is a command line interface (CLI) program that will download and convert specific schedules based on the command line parameters passed to it. If no parameters are passed, it will open in a 'UI' mode which allows you to manually input the data needed to grab and convert a schedule. This is used by the GUI application.
  - *SchoolScheduleICSConverter_GUI.exe* : This is a graphical user interface (GUI) program (meaning it has buttons and stuff!) that will let you pick which schedules you want to download and convert in a more user-friendly manner. This program invokes the CLI executable.
