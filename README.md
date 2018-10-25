## Limit Microphone Volume

Very simple tool to prevent programs from increasing the default
microphone volume over a configurable maximum value.

[Download x64 version](https://github.com/jmautari/LimitMicVol/raw/master/release-x64/LimitMicVol.exe)

MD5: 4bba9aea869ae29e7a6f39e46f3f57a6  
SHA1: dcd1cbdd38f1f5b5a009184c0fe5cecd40cf0c12  
SHA-256: c4c879986c66243917ac587d98461d673aec8f8a0b485a6fd83d1ab9c0311c68

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
