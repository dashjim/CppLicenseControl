> A MAC address based license control sample app based on C++ and QT. It uses the MD5 but you can change it to SHA1 or anything you want. It contains two VS 2010 projects, the first is a license generator, the later is a matcher which will read the license file and match it with one of the local MAC.

Usage:    
```
C:\depot\MacMatcher\Win32\Debug>MacToMD5.exe 00:21:CC:C9:55:97 >license

C:\depot\MacMatcher\Win32\Debug>MacMatcher.exe
license: 2c674f4daf43f730a187e74b0a633277
mac: 8C:70:5A:EE:75:D9
mac: 00:FF:40:98:2C:07
mac: 8C:70:5A:EE:75:D8
mac: **00:21:CC:C9:55:97**
matched!
```