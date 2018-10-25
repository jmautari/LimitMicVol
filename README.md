## Limit Microphone Volume

Very simple tool to prevent programs from increasing the default
microphone volume over a configurable maximum value.

[Download LimitMicVol.exe x64 version][1]

MD5: 3a191f85cc1fd564a63d325cc95b8a04  
SHA1: 90c07318cda1082ef3bc8af4677525c27ecb8381  
SHA-256: d7c20772c9592a45d497661eb0ba97b40e8ec6300d4c04733bdfe647cac03a1d

### Why?

If you have a microphone that gets very loud even with a fairly
low volume of 50 or so, you may find yourself yelling at people
over Skype or other Webrtc apps because they think your
microphone volume should be increased, sometimes for absolutely
no reason...

### What this tool does?

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

MD5: bffa5be915b18bd505fb8f6e8ec34713  
SHA1: 09ddc6fc9a626acf88ebd1cfeb4db65b1f6b7b76  
SHA256: c93313c31c9c6b19944b4c8056479b1c66a5cd873026e148ddc40ab224c433e9

[1]: https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LimitMicVol.exe
[2]: https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LaunchAtLogon.exe
