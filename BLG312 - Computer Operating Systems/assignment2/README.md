# Homework 2 : Online Market Concurrency Control System
This is second assignment of Operating System Lecture, Simple Online Market Concurrency Control System in C.

# How to run
Dockerfile and json given in the repsitory, firstly setup your docker environment

To build:
```
make build
```

To run:
```
make run
```

# Repository Structure
**bin** -- include compiled files.<br>
**data** -- include input.txt and info.log.<br>
**include** -- 2 header file, one of the main request struct, and second one for helping to create a retry request.<br>
**src** -- main.c is the runable, main file. readHelper.c help to read the txt file. threadHelper.c especially help to implement retry mechanism. utils.c generally have some log functions.<br>
