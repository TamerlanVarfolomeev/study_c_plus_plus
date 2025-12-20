# Dynamic Array - Подробное объяснение кода

## Структура проекта

Проект состоит из двух основных файлов:
- `src/Array.h` - заголовочный файл с объявлениями классов и структур
- `src/Array.ipp` - файл с реализацией шаблонных методов (подключается в конце .h файла)

---

## Array.h - Заголовочный файл

### Строка 1: `#pragma once`
Директива препроцессора, которая гарантирует, что этот файл будет включен только один раз в единицу компиляции. Предотвращает множественное включение и ошибки переопределения.

### Строки 3-5: Подключение стандартных заголовков
```cpp
#include <cstddef>
#include <initializer_list>
#include <iterator>
```
- `<cstddef>` - для типов `size_t`, `ptrdiff_t`
- `<initializer_list>` - для поддержки инициализации списком `{1, 2, 3}`
- `<iterator>` - для типов итераторов STL (`random_access_iterator_tag`)

### Строки 7-41: Обертки над malloc/free

#### Строка 7: `#ifdef CHECK_ALLOCATIONS`
Условная компиляция. Если определен макрос `CHECK_ALLOCATIONS`, компилируется код со статистикой выделения памяти.

#### Строки 8-31: Блок со статистикой
```cpp
namespace
{
    struct Stats { ... } g_Stats;
    void* my_malloc(size_t size) { ... }
    void my_free(void* block) { ... }
}
```

**Анонимное пространство имен** (`namespace` без имени) - делает все внутри видимым только в текущем файле компиляции, предотвращая конфликты имен.

**Структура Stats** (строки 10-20):
- `size_t allocations{ 0 }` - счетчик выделений памяти, инициализируется нулем
- `size_t deallocations{ 0 }` - счетчик освобождений памяти
- `g_Stats` - глобальная переменная для хранения статистики
- `friend std::ostream& operator<<` - перегрузка оператора вывода для удобного вывода статистики

**Функция my_malloc** (строки 21-25):
- `g_Stats.allocations++` - увеличиваем счетчик выделений
- `return malloc(size)` - вызываем стандартную функцию выделения памяти

**Функция my_free** (строки 26-30):
- `g_Stats.deallocations++` - увеличиваем счетчик освобождений
- `free(block)` - вызываем стандартную функцию освобождения памяти

#### Строки 32-41: Блок без статистики
Если `CHECK_ALLOCATIONS` не определен, используются простые обертки:
- `my_malloc` просто вызывает `malloc`
- `my_free` просто вызывает `free`

### Строка 43: `namespace myStl`
Создание пространства имен `myStl` для изоляции нашего класса от стандартной библиотеки.

### Строки 46-47: Объявление шаблонного класса
```cpp
template<typename T>
class Array final
```
- `template<typename T>` - шаблон класса, `T` - тип элементов массива
- `final` - запрещает наследование от этого класса

### Строки 49-100: Структура Iterator

#### Строки 52-56: Типы итератора (type traits)
```cpp
using iterator_category = std::random_access_iterator_tag;
using difference_type = std::ptrdiff_t;
using value_type = T;
using pointer = T*;
using reference = T&;
```
Определяют характеристики итератора для совместимости со STL алгоритмами:
- `iterator_category` - категория итератора (random access позволяет `+`, `-`, `[]`)
- `difference_type` - тип для разности итераторов
- `value_type` - тип значения
- `pointer` - тип указателя
- `reference` - тип ссылки

#### Строка 59: Конструктор итератора
```cpp
Iterator(Array<T>* arr, int64_t pos = 0, int direction = 1)
    : m_pArr(arr), m_Position(pos), m_direction(direction) {}
```
- `arr` - указатель на массив
- `pos` - начальная позиция (по умолчанию 0)
- `direction` - направление обхода (1 - вперед, -1 - назад)
- Список инициализации инициализирует поля перед телом конструктора

#### Строка 61: `operator*()`
```cpp
reference operator*() const { return m_pArr->m_data[m_Position]; }
```
Оператор разыменования. Возвращает ссылку на элемент в текущей позиции.

#### Строка 62: `operator->()`
```cpp
pointer operator->() { return &m_pArr->m_data[m_Position]; }
```
Оператор доступа к членам. Возвращает указатель на элемент.

#### Строки 66-69: Инкремент и декремент
```cpp
Iterator& operator++() { m_Position += m_direction; return *this; }
Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
```
- Префиксный `++` (строка 66): увеличивает позицию и возвращает ссылку на себя
- Постфиксный `++` (строка 67): создает копию, увеличивает позицию, возвращает копию
- Аналогично для `--` (строки 68-69)

#### Строки 72-77: Арифметические операции
```cpp
Iterator& operator +=(difference_type n) { m_Position += n * m_direction; return *this; }
Iterator operator +(difference_type n) const { return Iterator(m_pArr, m_Position + n * m_direction); }
```
- `+=` - изменяет текущий итератор
- `+` - создает новый итератор со сдвинутой позицией
- `n * m_direction` - учитывает направление обхода

#### Строка 80: `operator[]`
```cpp
reference operator[](difference_type n) const { return m_pArr->m_data[m_Position + n * m_direction]; }
```
Доступ к элементу с отступом от текущей позиции.

#### Строки 82-87: Операторы сравнения
```cpp
friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_pArr == b.m_pArr && a.m_Position == b.m_Position; }
```
- `friend` - функция имеет доступ к приватным членам
- Сравнивает указатель на массив и позицию

#### Строки 89-94: Методы задания
```cpp
const reference get() const { return m_pArr->m_data[m_Position]; }
void set(const reference value) { m_pArr->m_data[m_Position] = value; }
void next() { m_Position += m_direction; }
bool hasNext() const { return m_direction == 1 ? m_Position < m_pArr->m_size : m_Position >= 0; }
```
- `get()` - получить значение
- `set()` - установить значение
- `next()` - перейти к следующему
- `hasNext()` - проверить наличие следующего элемента (тернарный оператор учитывает направление)

### Строки 102-152: ConstIterator
Аналогичен Iterator, но:
- `pointer = const T*` - указатель на константу
- `reference = const T&` - ссылка на константу
- `m_pArr` имеет тип `const Array<T>*` - указатель на константный массив

### Строки 155-162: STL-совместимые итераторы
```cpp
Iterator begin() { return Iterator(this); }
Iterator end() { return Iterator(this, m_size); }
```
- `begin()` - итератор на первый элемент (позиция 0)
- `end()` - итератор за последним элементом (позиция m_size)
- `rbegin()/rend()` - обратные итераторы (начинаются с конца, direction = -1)

### Строки 165-168: Итераторы по заданию
```cpp
Iterator iterator() { return Iterator(this); }
Iterator reverseIterator() { return Iterator(this, m_size - 1, -1); }
```
Методы, требуемые заданием. `reverseIterator` начинается с последнего элемента и идет назад.

### Строки 171-180: Конструкторы и деструктор
```cpp
Array();
Array(size_t capacity);
Array(std::initializer_list<T> initList);
Array(const Array<T>& other);
Array<T>& operator=(const Array<T>& other);
Array(Array<T>&& other);
Array<T>& operator=(Array<T>&& other);
~Array();
```
- Конструктор по умолчанию
- Конструктор с емкостью
- Конструктор из списка инициализации
- Конструктор копирования
- Оператор присваивания (копирование)
- Move-конструктор
- Move-оператор присваивания
- Деструктор

### Строки 183-184: Методы размера
```cpp
size_t size() const { return m_size; }
size_t capacity() const { return m_capacity; }
```
- `size()` - текущее количество элементов
- `capacity()` - выделенная емкость

### Строки 186-192: Методы insert
```cpp
size_t insert(const T& value);
size_t insert(T&& value);
size_t insert(size_t index, const T& value);
size_t insert(size_t index, T&& value);
size_t insert(const std::initializer_list<T>& initList);
size_t insert(size_t index, const std::initializer_list<T>& initList);
```
Перегрузки для вставки:
- В конец или в указанную позицию
- L-value (`const T&`) и R-value (`T&&`) ссылки
- Одиночный элемент или список элементов

### Строки 212-217: Приватные члены
```cpp
void reserve(size_t newCapacity);
T* m_data;
size_t m_size;
size_t m_capacity;
```
- `reserve()` - метод перераспределения памяти
- `m_data` - указатель на выделенную память
- `m_size` - текущий размер
- `m_capacity` - текущая емкость

### Строка 222: `#include "Array.ipp"`
Подключение файла с реализацией. Делается в конце .h файла, так как реализация шаблонная.

---

## Array.ipp - Реализация

### Строка 1: `#pragma once`
Защита от множественного включения.

### Строка 3: `#include "Array.h"`
Подключение заголовочного файла с объявлениями.

### Строка 5: `#include "stdexcept"`
Для исключения `std::bad_alloc`.

### Строка 7: `namespace myStl`
Продолжение пространства имен из .h файла.

### Строки 9-16: Конструктор по умолчанию
```cpp
template<typename T>
inline Array<T>::Array()
    : m_size(0), m_capacity(8)
{
    m_data = (T*)my_malloc(sizeof(T) * m_capacity);
    if (!m_data)
        throw std::bad_alloc();
}
```
- `inline` - подсказка компилятору для встраивания функции
- Список инициализации: `m_size = 0`, `m_capacity = 8`
- `my_malloc(sizeof(T) * m_capacity)` - выделяем память на 8 элементов типа T
- `(T*)` - приведение типа `void*` к `T*`
- Проверка на `nullptr` и выбрасывание исключения при ошибке

### Строки 19-26: Конструктор с емкостью
```cpp
template<typename T>
inline Array<T>::Array(size_t capacity)
    : m_size(0), m_capacity(capacity)
{
    m_data = (T*)my_malloc(sizeof(T) * m_capacity);
    if (!m_data)
        throw std::bad_alloc();
}
```
Аналогично предыдущему, но емкость задается параметром.

### Строки 29-39: Конструктор из initializer_list
```cpp
template<typename T>
inline Array<T>::Array(std::initializer_list<T> initList)
    : m_size(initList.size()), m_capacity(initList.size())
{
    m_data = (T*)my_malloc(sizeof(T) * m_capacity);
    if (!m_data)
        throw std::bad_alloc();
    int i = 0;
    for (const auto& item : initList)
        new (&m_data[i++]) T(item);
}
```
- `initList.size()` - размер списка инициализации
- Емкость равна размеру (ровно столько, сколько нужно)
- `new (&m_data[i++]) T(item)` - **placement new**: создает объект типа T в уже выделенной памяти по адресу `&m_data[i]`
- `i++` - постфиксный инкремент: использует текущее значение, затем увеличивает

### Строки 42-62: Конструктор копирования
```cpp
template<typename T>
inline Array<T>::Array(const Array<T>& other)
{
    m_capacity = other.m_capacity;
    m_size = other.m_size;

    m_data = (T*)my_malloc(sizeof(T) * m_capacity);
    if (!m_data)
        throw std::bad_alloc();

    for (size_t i = 0; i < m_size; i++)
        new (&m_data[i]) T(other.m_data[i]);
}
```
- Копируем размер и емкость из другого массива
- Выделяем новую память
- Используем конструктор копирования `T(other.m_data[i])`
- Placement new создает объекты в новой памяти

### Строки 65-94: Оператор присваивания (копирование)
```cpp
template<typename T>
inline Array<T>& Array<T>::operator=(const Array<T>& other)
{
    if (this == &other)
        return *this;

    for (size_t i = 0; i < m_size; i++)
        m_data[i].~T();
    my_free(m_data);

    m_capacity = other.m_capacity;
    m_size = other.m_size;

    m_data = (T*)my_malloc(sizeof(T) * m_capacity);
    if (!m_data)
        throw std::bad_alloc();

    for (size_t i = 0; i < m_size; i++)
        new (&m_data[i]) T(other.m_data[i]);

    return *this;
}
```
- `if (this == &other) return *this` - защита от самоприсваивания
- Явно вызываем деструкторы всех элементов: `m_data[i].~T()`
- Освобождаем старую память: `my_free(m_data)`
- Выделяем новую память и копируем элементы
- Возвращаем `*this` для цепочки присваиваний

### Строки 97-107: Move-конструктор
```cpp
template<typename T>
inline Array<T>::Array(Array<T>&& other)
{
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_data = other.m_data;

    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
}
```
- "Крадем" ресурсы у другого объекта
- Копируем указатель на память (не выделяем новую!)
- Обнуляем поля другого объекта, чтобы его деструктор не освободил память

### Строки 110-128: Move-оператор присваивания
```cpp
template<typename T>
inline Array<T>& Array<T>::operator=(Array<T>&& other)
{
    if (this == &other)
        return *this;

    for (size_t i = 0; i < m_size; i++)
        m_data[i].~T();
    my_free(m_data);

    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_data = other.m_data;

    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
    return *this;
}
```
- Сначала освобождаем свои ресурсы
- Затем "крадем" ресурсы у другого объекта
- Обнуляем другой объект

### Строки 131-140: Деструктор
```cpp
template<typename T>
inline Array<T>::~Array()
{
    for (size_t i = 0; i < m_size; i++)
        m_data[i].~T();
    my_free(m_data);
    m_size = 0;
    m_capacity = 0;
    m_data = nullptr;
}
```
- Вызываем деструкторы всех элементов
- Освобождаем память через `my_free`
- Обнуляем поля (хорошая практика)

### Строки 143-147: insert(const T& value)
```cpp
template<typename T>
inline size_t Array<T>::insert(const T& value)
{
    return insert(m_size, value);
}
```
Вставляет элемент в конец, вызывая `insert` с индексом равным текущему размеру.

### Строки 150-154: insert(T&& value)
```cpp
template<typename T>
inline size_t Array<T>::insert(T&& value)
{
    return insert(m_size, std::move(value));
}
```
Вставляет элемент в конец с использованием move-семантики. `std::move` превращает l-value в r-value.

### Строки 157-188: insert(size_t index, const T& value)
```cpp
template<typename T>
inline size_t Array<T>::insert(size_t index, const T& value)
{
    if (m_size == m_capacity)
    {
        size_t newCapacity = m_capacity == 0 ? 1 : (size_t)(m_capacity * 1.6);
        if (newCapacity < m_capacity + 1)
            newCapacity = m_capacity * 2;
        reserve(newCapacity);
    }

    if constexpr (std::is_move_constructible<T>::value)
    {
        for (size_t i = m_size; i > index; i--)
        {
            new (&m_data[i]) T(std::move(m_data[i - 1]));
        }
    }
    else
    {
        for (size_t i = m_size; i > index; i--)
        {
            new (&m_data[i]) T(m_data[i - 1]);
            m_data[i - 1].~T();
        }
    }

    new (&m_data[index]) T(value);
    m_size++;

    return index;
}
```

**Проверка емкости (строки 160-166):**
- Если массив заполнен (`m_size == m_capacity`), увеличиваем емкость
- `m_capacity * 1.6` - увеличиваем в 1.6 раза (как требует задание)
- `(size_t)` - приведение к целому типу (отбрасывание дробной части)
- Если результат меньше чем `m_capacity + 1`, используем удвоение
- Вызываем `reserve()` для перераспределения памяти

**Сдвиг элементов вправо (строки 168-182):**
- Цикл идет от конца (`m_size`) до позиции вставки (`index`)
- `if constexpr` проверяет поддержку move-семантики на этапе компиляции

**Если поддерживается move (строки 169-174):**
- `new (&m_data[i]) T(std::move(m_data[i - 1]))` - перемещаем элемент вправо
- `std::move` превращает l-value в r-value, вызывается move-конструктор
- Старый объект остается в невалидном состоянии (но мы его не используем)

**Если не поддерживается move (строки 175-181):**
- `new (&m_data[i]) T(m_data[i - 1])` - копируем элемент
- `m_data[i - 1].~T()` - явно вызываем деструктор старого объекта
- Это необходимо, так как placement new не вызывает деструктор автоматически

**Вставка нового элемента (строки 184-187):**
- `new (&m_data[index]) T(value)` - создаем новый элемент в освобожденной позиции
- `m_size++` - увеличиваем размер
- Возвращаем индекс вставки

### Строки 191-220: insert(size_t index, T&& value)
Аналогично предыдущему, но принимает r-value ссылку:
```cpp
new (&m_data[index]) T(value);
```
Здесь `value` уже является r-value, поэтому вызовется move-конструктор если он доступен.

---

## Ключевые концепции

### Placement new
`new (address) Type(args)` - создает объект в уже выделенной памяти по адресу `address`. Не выделяет новую память, только вызывает конструктор.

### Move-семантика
Позволяет "перемещать" ресурсы вместо копирования. `std::move` превращает l-value в r-value, что вызывает move-конструктор вместо copy-конструктора.

### if constexpr
Проверка на этапе компиляции. Если условие истинно, компилируется только блок `if`, иначе только блок `else`.

### Коэффициент увеличения 1.6
Золотое сечение в динамических массивах. Балансирует между частыми перераспределениями и избыточным выделением памяти.

---

## Модульные тесты

Проект включает модульные тесты на базе Google Test, расположенные в папке `tests/`.

### Быстрый старт:

1. Установите Google Test через vcpkg:
   ```bash
   vcpkg install gtest:x64-windows
   ```

2. Добавьте пути в настройках проекта:
   - **C/C++ → General → Additional Include Directories:** `$(SolutionDir)DynamicArray\src`
   - **Linker → Input → Additional Dependencies:** `gtest.lib;gtest_main.lib`
   - **Linker → General → Additional Library Directories:** `$(VcpkgRoot)installed\x64-windows\lib`

3. Запустите тесты через Test Explorer в Visual Studio или запустите скомпилированный .exe

### Покрытие тестами:

Тесты покрывают все требования задания:
- ✅ Конструкторы (по умолчанию, с capacity, initializer_list)
- ✅ Деструктор (проверка через отсутствие утечек)
- ✅ insert (все варианты: в конец, по индексу, initializer_list, move-семантика)
- ✅ remove
- ✅ operator[] (чтение и запись)
- ✅ size() и capacity()
- ✅ iterator() и reverseIterator()
- ✅ ConstIterator и constReverseIterator()
- ✅ Копирование (конструктор и присваивание)
- ✅ Перемещение (конструктор и присваивание)
- ✅ Пример из задания

Всего реализовано **35 тестов**.

Подробная инструкция по настройке и запуску: см. `tests/README_TESTS.md`


