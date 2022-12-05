# cpp-advanced-vector
Improved vector container.
Updated version of the project - [simple-vector](https://github.com/ZakharovYuriy/cpp-simple-vector)

Read in other languages: [English](README.md), [Русский](README.RUS.md)<br>

# Program Description
This project is a continuation of the project [simple-vector](https://github.com/ZakharovYuriy/cpp-simple-vector) - of a simplified analogue of the standard container **vector**. The project has been redesigned to improve the effectiveness of methods and expand functionality.

# Build using Cmake
To build this project on linux you need:<br>
1) If you don't have Cmake installed, install Cmake<br>
2) If the "Debug" or "Release" folders are not created:<br>

```
mkdir Debug
mkdir Release
```
3) Run the command for Debug and/or Release conf:<br>

```
cmake -E chdir Debug/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Debug
cmake -E chdir Release/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release 
```
4) Go to "Debug" or "Release" folder and build:<br>

```
cmake --build .
```

5) To **Run** program - in the debug or release folder run:<br>

```
./advanced_vector
```
# System requirements:
  1. C++17(STL)
  2. GCC (MinG w64) 11.2.0  

# Stack of technologies:
 1. new placing operator<br>
 2. new and new[] operators, delete and delete[] operators<br>
 3. Exception handling<br>
 4. Three levels of exception security<br>
 5. Working with uninitialized memory area: <memory>

## Before you start:
0. Installation and configuration of all required components in the development environment to run the application<br>
1. The use case and tests are shown in main.cpp .<br>

# Detailed description of the project:
##Methods supported by the current version of the container:
 - Size
 - Capacity
 - operator[]
 - operator=
 - Swap
 - Resize
 - Reserve
 - PushBack
 - PushBack
 - PopBack
 - EmplaceBack
 - begin
 - end
 - cbegin
 - cend
 - Emplace
 - Erase
 - Insert

`RawMemory` is responsible for storing a buffer that holds a specified number of elements, and provides access to elements by index<br>

## Description of differences from the previous version
Benchmark of the previous version of the container- `SimpleVector` и `std::vector`:

```
SimpleVector:
  After construction: Def ctors: 11, Copy ctors: 0, Move ctors: 0, Copy assignments: 10, Move assignments: 0, Dtors: 1
  After push: Def ctors: 31, Copy ctors: 0, Move ctors: 0, Copy assignments: 21, Move assignments: 0, Dtors: 11
  After destruction: Def ctors: 31, Copy ctors: 0, Move ctors: 0, Copy assignments: 21, Move assignments: 0, Dtors: 31
std::vector:
  After construction: Def ctors: 10, Copy ctors: 0, Move ctors: 0, Copy assignments: 0, Move assignments: 0, Dtors: 0
  After push: Def ctors: 10, Copy ctors: 1, Move ctors: 10, Copy assignments: 0, Move assignments: 0, Dtors: 10
  After destruction: Def ctors: 10, Copy ctors: 1, Move ctors: 10, Copy assignments: 0, Move assignments: 0, Dtors: 21
```

The difference in the number of constructors called and assignment operations is caused by the fact that when performing a PushBack on a filled container, SimpleVector not only allocates twice the amount of memory in the heap, but also initializes it with default values. This is a feature of the operation **new[]**. Then the array values from the previous memory area are copied, so new values are assigned to the elements.<br>
<br>
The current version of the container uses the **placing operator new** and working with raw memory.<br>
The code **Benchmark** of the current version is in the file main.cpp , when the program starts, after passing the tests, **Benchmark** is output.<br>
