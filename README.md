## Limit Microphone Volume

Very simple tool to prevent programs from increasing the default
microphone volume over a configurable maximum value.

[Download LimitMicVol.exe x64 version][1]

MD5: 070b1dc73405ebfcee0f55eca2d62749  
SHA1: 7951f27489ae10b5ce66c00e2bf10ea0ac7f7cb6  
SHA-256: 71fa4da5ed45293e8bcfa08ebba92cfb6c8189e22546b5bd16d12ef4619bb9db

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

MD5: 795fd22e2918608c60bec76563a8eb34  
SHA1: 461a97be41621bb225bd232a80e8d9c275af5e7f  
SHA256: bbb5a2ffcab740b3eb33f73e65b794d41873ad3f351619c80f22f30d9c9a141b

[1]: https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LimitMicVol.exe
[2]: https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LaunchAtLogon.exe
