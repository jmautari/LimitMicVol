md5sum release-x64/LimitMicVol.exe | awk '{print "MD5: " $1}' && sha1sum release-x64/LimitMicVol.exe | awk '{print "SHA1: " $1}' && sha256sum release-x64/LimitMicVol.exe | awk '{print "SHA256: " $1}'
echo .
md5sum release-x64/LaunchAtLogon.exe | awk '{print "MD5: " $1}' && sha1sum release-x64/LaunchAtLogon.exe | awk '{print "SHA1: " $1}' && sha256sum release-x64/LaunchAtLogon.exe | awk '{print "SHA256: " $1}'
