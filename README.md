# exe-archivator 
Program that make exec-me.exe, which contains additional files which are unpacked when exec-me.exe is run. After compleating unpacking exec-me.exe delete itself 
## Compiling
```sh
make
```
## Usage-to-pack: 
```sh
exe-archivator [files to pack] [-c <command> embed command in exec-me.exe] [-h/--help to see info]
``` 
## Usage-to-unpack:
```sh 
exec-me [-s to block self-deleting] [-c <command> task to do after unpacking] 
(or use only -c to block builtin command) [-h/--help to see info]
```