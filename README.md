# exe-archivator 
Program that make exec-me.exe, which contains additional files which are unpacked when exec-me.exe is run. After compleating unpacking exec-me.exe delete itself 
# Compiling
```sh
make
```
# Usage-to-pack: 
```sh
exe-archivator [files to pack]
``` 
# Usage-to-unpack:
```sh 
exec-me [-s to block self-deleting] [command to run after unpacking] [-h/--help to see info]
```