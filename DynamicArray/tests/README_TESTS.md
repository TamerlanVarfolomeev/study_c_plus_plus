# Модульные тесты для Dynamic Array

## Описание

Модульные тесты на базе Google Test для класса `Array<T>`. Тесты покрывают все требования задания.

## Установка Google Test

### Вариант 1: Через vcpkg (рекомендуется)

```bash
vcpkg install gtest:x64-windows
```

Затем в настройках проекта Visual Studio:
1. Tools → Options → vcpkg → Use vcpkg
2. Убедитесь, что путь к vcpkg указан правильно

### Вариант 2: Через NuGet в Visual Studio

1. Правый клик на проект → Manage NuGet Packages
2. Browse → поиск "Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn"
3. Install

### Вариант 3: Ручная установка

1. Скачать Google Test: https://github.com/google/googletest/releases
2. Собрать библиотеку (CMake)
3. Добавить пути в настройках проекта

## Настройка проекта

### Шаг 1: Добавить пути включения

**Properties → C/C++ → General → Additional Include Directories:**
```
$(SolutionDir)DynamicArray\src
$(VcpkgRoot)installed\x64-windows\include
```

### Шаг 2: Настроить линковку

**Properties → Linker → Input → Additional Dependencies:**
```
gtest.lib
gtest_main.lib
```

**Properties → Linker → General → Additional Library Directories:**
```
$(VcpkgRoot)installed\x64-windows\lib
```

### Шаг 3: Настроить точку входа (если нужно)

**Properties → Linker → System → SubSystem:**
- Console (/SUBSYSTEM:CONSOLE)

## Запуск тестов

### Через Visual Studio:
1. Test → Test Explorer
2. Run All Tests

### Через командную строку:
```bash
cd x64\Debug
ArrayTests.exe
```

## Покрытие тестами

✅ **Конструкторы:**
- Конструктор по умолчанию
- Конструктор с capacity
- Конструктор из initializer_list

✅ **Вставка элементов:**
- insert(const T&) - в конец
- insert(T&&) - в конец с move-семантикой
- insert(size_t, const T&) - по индексу
- insert(size_t, T&&) - по индексу с move-семантикой
- insert(initializer_list) - список элементов
- insert(size_t, initializer_list) - список по индексу

✅ **Удаление:**
- remove(size_t) - удаление элемента

✅ **Доступ к элементам:**
- operator[] - чтение
- operator[] - запись
- operator[] const - константный доступ

✅ **Размер и емкость:**
- size() - текущий размер
- capacity() - рост емкости

✅ **Итераторы:**
- iterator() - прямой обход
- reverseIterator() - обратный обход
- constIterator() - константный прямой обход
- constReverseIterator() - константный обратный обход
- get() - получение значения
- set() - установка значения
- next() - переход к следующему
- hasNext() - проверка наличия следующего

✅ **Копирование и перемещение:**
- Конструктор копирования
- Оператор присваивания (копирование)
- Move-конструктор
- Move-оператор присваивания
- Защита от самоприсваивания

✅ **Дополнительные тесты:**
- Move-семантика для элементов
- Комплексные операции
- Пример из задания
- Большие массивы
- Работа со строками

## Структура тестов

Всего реализовано **35 тестов**, покрывающих:
- Базовую функциональность
- Граничные случаи
- Интеграционные сценарии
- Производительность

## Пример вывода

```
[==========] Running 35 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 35 tests from ArrayTest
[ RUN      ] ArrayTest.DefaultConstructor
[       OK ] ArrayTest.DefaultConstructor (0 ms)
[ RUN      ] ArrayTest.ConstructorWithCapacity
[       OK ] ArrayTest.ConstructorWithCapacity (0 ms)
...
[----------] 35 tests from ArrayTest (15 ms total)
[==========] 35 tests from 1 test suite ran. (16 ms total)
[  PASSED  ] 35 tests.
```

