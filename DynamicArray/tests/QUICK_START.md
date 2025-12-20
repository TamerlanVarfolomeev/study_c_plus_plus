# Быстрый запуск тестов

## Шаг 1: Установка Google Test

### Через vcpkg (самый простой способ):
```bash
vcpkg install gtest:x64-windows
```

### Через NuGet в Visual Studio:
1. Правый клик на проект → **Manage NuGet Packages**
2. Browse → поиск: `Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn`
3. **Install**

## Шаг 2: Настройка проекта

### В Visual Studio:

1. **Properties** проекта (Alt+Enter)

2. **C/C++ → General → Additional Include Directories:**
   ```
   $(SolutionDir)DynamicArray\src
   $(VcpkgRoot)installed\x64-windows\include
   ```

3. **Linker → Input → Additional Dependencies:**
   ```
   gtest.lib
   gtest_main.lib
   ```

4. **Linker → General → Additional Library Directories:**
   ```
   $(VcpkgRoot)installed\x64-windows\lib
   ```

## Шаг 3: Запуск

### Вариант 1: Через Visual Studio
- **Test → Test Explorer → Run All Tests**

### Вариант 2: Через командную строку
```bash
cd x64\Debug
ArrayTests.exe
```

## Что тестируется

✅ Все конструкторы  
✅ Все методы insert  
✅ remove  
✅ operator[]  
✅ size()  
✅ Итераторы (прямой и обратный)  
✅ Копирование и перемещение  
✅ Пример из задания  

**Всего: 35 тестов**

