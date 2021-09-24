# Ubuntu_System_Monitor
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Linux](https://svgshare.com/i/Zhy.svg)](https://svgshare.com/i/Zhy.svg) [![Atom](https://badgen.net/badge/icon/atom?icon=atom&label)](https://atom.io)

Welcome to the System monitor for Ubuntu repository! This is the second project in the [C++ Nanodegree](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

In this project, I will develop a system monitor for a Linux based system (Ubuntu 20), a basic version of [htop](https://htop.dev).

I will regularly update this readme with my progress and system requirements!

<!-- <p align="center"><img src="img/Terminal.png" width="400" height="300" alt="Image of the terminal"/> <img src="img/Results.png" width="300" height="300" alt="Image of the results"/> </p> -->

## Environment
👉 Ubuntu 20.04\
👉 GNU Make 4.2.1\
👉 CMake 3.21.3\
👉 C++ 17\
👉 g++ 9.3.0\
👉 [ncurses library](https://invisible-island.net/ncurses/announce.html) 6.2\

## Installation
You will need to install the ncurses library in Linux with the following command
```
sudo apt install libncurses5-dev libncursesw5-dev
```

## Compiling and Running
### Compile
The [Makefile](Makefile) is already configure. To build the project run
```
make build
```
This command will create the build directory within the project folder and create the executable.

### Running
The executable is within the `build` directory. You can run it as follows
```
./build/monitor
```
