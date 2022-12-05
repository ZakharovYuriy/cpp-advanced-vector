# cpp-advanced-vector
Улучшенный контейнер - вектор.
Обновленная версия проекта - [simple-vector](https://github.com/ZakharovYuriy/cpp-simple-vector)

Читать на других языках: [English](README.md), [Русский](README.RUS.md)<br>

# Описание программы
Этот проект является продолжением проекта [simple-vector](https://github.com/ZakharovYuriy/cpp-simple-vector) - упрощённого аналога стандартного контейнера **vector**. Проект переработан с учетом улучшения эффективности методов и расширения функционала.

# Сборка при помощи Cmake
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
# Системые требования:
  1. C++17(STL)
  2. GCC (MinG w64) 11.2.0  

# Стек технологий:
  1. Размещающий оператор new<br>
  2. Операторы new и new[], операторы delete и delete[]<br>
  3. Обработка исключений<br>
  4. Три уровня безопасности исключений<br>
  5. Работа с неинициализированной областью памяти : <memory>

## Перед тем как начать:
  0. Установка и настройкка всех требуемых компонентов в среде разработки длля запуска приложения<br>
  1. Вариант использования и тесты показаны в main.cpp.<br>

# Подробное описание проекта:
## Методы, поддерживаемые текущей версией контейнера:
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

`RawMemory` отвечает за хранение буфера, который вмещает заданное количество элементов, и предоставляет доступ к элементам по индексу<br>

## Описание отличий от предыдущей версии
Benchmark предыдущей версии контейнера - `SimpleVector` и `std::vector`:

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

Разница в количестве вызываемых конструкторов и операций присваивания вызвана тем, что при выполнении PushBack у заполненного контейнера `SimpleVector` не просто выделяет вдвое больший объём памяти в куче, но и инициализирует её значениями по умолчанию. Такова особенность работы операции **new[]**. Затем значения массива из прежней области памяти копируются, поэтому элементам присваиваются новые значения.<br>
<br>
В текущей версии контейнера используется **размещающий оператор new** и работа с сырой памятью.<br>
Код **Benchmark** текущей версии есть в файле main.cpp, при запуске программы, после прохождения тестов выводится **Benchmark**.<br>
