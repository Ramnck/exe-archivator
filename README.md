# exe-archivator 
Program that make exec-me.exe, which contains additional files which are unpacked when exec-me.exe is run. After compleating unpacking exec-me.exe delete itself 
## Compiling
```sh
make
```
## Usage-to-pack: 
```sh
exe-archivator [-c <command> embed command in exec-me.exe] [files to pack] [-h/--help to see info]
``` 
## Usage-to-unpack:
```sh 
exec-me [-s to block self-deleting] [-c <command> task to do after unpacking] 
(or use only -c to block builtin command) [-h/--help to see info]
```
## Direct downloading
> Windows Defender says that it is a trojan, but i swear you, that's not true
```sh
curl https://raw.githubusercontent.com/Ramnck/exe-archivator/master/bin/exe-archivator.exe -o exe-archivator.exe
```
