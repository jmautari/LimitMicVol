## Limit Microphone Volume

Very simple tool to prevent programs from increasing the default
microphone volume over a configurable maximum value.

[Download x64 version](https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LimitMicVol.exe)

MD5: bae06ec9676a58ace3f98779812f80f9  
SHA1: 875f709f0673f79e93d71ff891d4bd5ea474eb25  
SHA-256: d9b604280418c5b2588af3b9d1df230557b7cf4acd182891b6502cf1277674c4

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

The easiest way is by using Windows Task Scheduler.
