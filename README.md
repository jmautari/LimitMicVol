## Limit Microphone Volume

Very simple tool to prevent programs from increasing the default
microphone volume over a configurable maximum value.

[Download x64 version](https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LimitMicVol.exe)

MD5: 5bba3631b428c9580be96924e117751b  
SHA1: 683C97DA3618FC42AC53A15A0075E0748EAD88FA  
SHA-256: 863C194D8E82A5B1BF952275D47DB14201599BEEA35ED51B450D21F994BB84B4

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

### The program looks ugly!

Yes, the functionality is there and that is what matters to me.
Ok, just kidding... I may make it prettier later when I have time
and inspiration for it. For now, that's what it is!