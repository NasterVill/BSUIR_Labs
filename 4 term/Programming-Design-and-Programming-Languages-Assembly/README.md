# Programming-Design-and-Programming-Languages-Assembly (КПиЯП ASM)
Assembly labs from 4th term (Лабораторные работы по ассемблеру за 4 семестр (КПиЯп))

В данном репозитории содержатся лабораторные работы по ассемблеру, написанные с помощью emu8086/tasm + DOS-BOX.
Лабораторные работы 4, 7, 8 можно запустить только под DOS-BOX.

This repository contains assembly labs, written with the help of emu8086/tasm + DOS-BOX.
Labs 4, 7, 8 can only be launched in a DOS-BOX. You can watch tasm's compilation info in a "TASM_compilation_info" file.

## Условия лабораторных работ (Tasks)

### Лабораторная работа №2
Перевернуть строку.
Reverse the string.

### Лабораторная работа №3
Ввести матрицу целых чисел размерностью 5х6 элементов. Найти номера столбцов с минимальной суммой элементов.
Enter the array of integer numbers, and find columns with smallest element's sums.

### Лабораторная работа №4
Игра "Тетрис". 
Game "Tetris"

### Лабораторная работа №5
Удалить в файле слова, совпадающие с ключевым. 
Delete words from file, which match the key word.

### Лабораторная работа №7
Написать программу, последовательно запускающую программы, которые расположены в заданном каталоге. 
The program, which consistently runs other programs, from a given directory

### Лабораторная работа №8
Написать резидентную программу "просмоторщик текстового файла". Переопределение вектора прерывания нажатия клавиш клавиатуры.
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