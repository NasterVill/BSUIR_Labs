# Programming-Design-and-Programming-Languages-Assembly
Assembly labs from 4th term.

This repository contains assembly labs, written with the help of emu8086/tasm + DOS-BOX.
Labs 4, 7, 8 can only be launched in a DOS-BOX. You can watch tasm's compilation info in a "TASM_compilation_info" file.

## Tasks

### Laboratory work №2
Reverse the string.

### Laboratory work №3
Enter the array of integer numbers, and find columns with smallest element's sums.

### Laboratory work №4
Game "Tetris"

### Laboratory work №5
Delete words from file, which match the key word.

### Laboratory work №7
The program, which consistently runs other programs, from a given directory

### Laboratory work №8
A TSR program "text-viewer", where keyboard int 09h vector handler is redefined.

# Compilation info
To compile a program wtih TASM, you'll need a DOS-BOX application. After installing it, you need to mount your main drive.

I usually do it by creating a folder in the root of C:\ disk, and then typing in a DOS-BOX commandline:
`mount c c:\name_of_the_folder`

Then you need to open this drive, by typing in a commandline:
`C:`

And now you can compile your program:
`tasm program_name`
`tlink program_name`

If you want to create a COM program, just slightly change tlink command:
`tlink /t program_name.obj`

To run the program, simply type it's name, as a command:
`program_name`

To pass commandline args to a programm, type them after program's name one by one with a space, as a delimeter:
`program_name arg1 arg2 arg3 ... argn`

By the way, files TASM.exe and TLINK.exe must be in a directory, with your program, otherwise, you'll need to
determine their location for DOS-BOX, but I won't be explaining, how it's done, so if you're up to it, then good luck, though
I strongly recommend, to do it the way, it is suggested up there)