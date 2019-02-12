## Limit Microphone Volume

Very simple tool to prevent programs from increasing the default
microphone volume over a configurable maximum value.

[Download LimitMicVol.exe x64 version][1]

MD5: 65c02dd55222f752f0fe93a2aed6db6f  
SHA1: 41e4e072e05286c2ea35bbb0356e082311d30906  
SHA256: 9a368e79aea2cab7d6d1fa70500f24bbe7e01fae715a4fdc573245154431f716

### Why?

If you have a microphone that gets very loud even with a fairly
low volume of 50 or so, you may find yourself yelling at people
over Skype or other Webrtc apps because they think your
microphone volume should be increased, sometimes for absolutely
no reason...

### What does this tool do?

It will run in the background and will query the microphone
volume every 1.5 seconds; whenever the volume is higher than a
specified value, the volume will be reconfigured to the maximum
value you have defined in the program's slider in the main window.
Double click the task tray icon to reveal the program's window.
Any changes made are automatically saved, you can find the
configuration file at `%APPDATA%\LimitMicVol\config.ini`

### How to make the program to start automatically at logon?

Download `LaunchAtLogon.exe` (source code is also part of the
project) at the same directory where `LimitMicVol.exe` is saved and launch it
from a command prompt window.

If everything goes well a new task will be added to Task Scheduler to launch
`LimitMicVol.exe` at user logon time.

[Download LaunchAtLogon.exe x64 version][2]

MD5: 1ed04df5a252d2c37fd67df34b7b041e  
SHA1: b1397d9afc2d3d6d1b8daab9c044758ce10c7393  
SHA256: b5c137c9fbc0e68610e369f22fb374b1fdb7a99dba929b791994f0b87bbd2c94

[1]: https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LimitMicVol.exe
[2]: https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LaunchAtLogon.exe
