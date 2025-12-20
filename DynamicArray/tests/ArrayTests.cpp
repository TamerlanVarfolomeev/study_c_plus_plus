#include <gtest/gtest.h>
#include "../src/Array.h"
#include <string>
#include <cmath>

using namespace myStl;

// ============================================================================
// Базовые тесты для всех типов (int, float, string)
// ============================================================================

// Тест 1: Конструктор по умолчанию
template<typename T>
void TestDefaultConstructor()
{
	Array<T> arr;
	EXPECT_EQ(arr.size(), 0);
	EXPECT_GE(arr.capacity(), 8);
}

TEST(ArrayTest, DefaultConstructor_Int) { TestDefaultConstructor<int>(); }
TEST(ArrayTest, DefaultConstructor_Float) { TestDefaultConstructor<float>(); }
TEST(ArrayTest, DefaultConstructor_String) { TestDefaultConstructor<std::string>(); }

// Тест 2: Конструктор с initializer_list
template<typename T>
void TestInitializerListConstructor()
{
	Array<T> arr = {T(1), T(2), T(3), T(4), T(5)};
	EXPECT_EQ(arr.size(), 5);
	EXPECT_EQ(arr[0], T(1));
	EXPECT_EQ(arr[4], T(5));
}

TEST(ArrayTest, InitializerListConstructor_Int) { TestInitializerListConstructor<int>(); }
TEST(ArrayTest, InitializerListConstructor_Float) { TestInitializerListConstructor<float>(); }
TEST(ArrayTest, InitializerListConstructor_String) 
{ 
	Array<std::string> arr = {"one", "two", "three", "four", "five"};
	EXPECT_EQ(arr.size(), 5);
	EXPECT_EQ(arr[0], "one");
	EXPECT_EQ(arr[4], "five");
}

// Тест 3: Вставка элемента в конец
template<typename T>
void TestInsertAtEnd()
{
	Array<T> arr;
	size_t index1 = arr.insert(T(10));
	size_t index2 = arr.insert(T(20));
	EXPECT_EQ(index1, 0);
	EXPECT_EQ(index2, 1);
	EXPECT_EQ(arr.size(), 2);
	EXPECT_EQ(arr[0], T(10));
	EXPECT_EQ(arr[1], T(20));
}

TEST(ArrayTest, InsertAtEnd_Int) { TestInsertAtEnd<int>(); }
TEST(ArrayTest, InsertAtEnd_Float) { TestInsertAtEnd<float>(); }
TEST(ArrayTest, InsertAtEnd_String) 
{ 
	Array<std::string> arr;
	arr.insert("first");
	arr.insert("second");
	EXPECT_EQ(arr.size(), 2);
	EXPECT_EQ(arr[0], "first");
	EXPECT_EQ(arr[1], "second");
}

// Тест 4: Вставка элемента по индексу
template<typename T>
void TestInsertAtIndex()
{
	Array<T> arr = {T(1), T(2), T(4), T(5)};
	size_t index = arr.insert(2, T(3));
	EXPECT_EQ(index, 2);
	EXPECT_EQ(arr.size(), 5);
	EXPECT_EQ(arr[0], T(1));
	EXPECT_EQ(arr[1], T(2));
	EXPECT_EQ(arr[2], T(3));
	EXPECT_EQ(arr[3], T(4));
	EXPECT_EQ(arr[4], T(5));
}

TEST(ArrayTest, InsertAtIndex_Int) { TestInsertAtIndex<int>(); }
TEST(ArrayTest, InsertAtIndex_Float) { TestInsertAtIndex<float>(); }
TEST(ArrayTest, InsertAtIndex_String)
{
	Array<std::string> arr = {"one", "two", "four", "five"};
	arr.insert(2, "three");
	EXPECT_EQ(arr.size(), 5);
	EXPECT_EQ(arr[0], "one");
	EXPECT_EQ(arr[1], "two");
	EXPECT_EQ(arr[2], "three");
	EXPECT_EQ(arr[3], "four");
	EXPECT_EQ(arr[4], "five");
}

// Тест 5: Удаление элемента
template<typename T>
void TestRemove()
{
	Array<T> arr = {T(1), T(2), T(3), T(4), T(5)};
	arr.remove(2);
	EXPECT_EQ(arr.size(), 4);
	EXPECT_EQ(arr[0], T(1));
	EXPECT_EQ(arr[1], T(2));
	EXPECT_EQ(arr[2], T(4));
	EXPECT_EQ(arr[3], T(5));
}

TEST(ArrayTest, Remove_Int) { TestRemove<int>(); }
TEST(ArrayTest, Remove_Float) { TestRemove<float>(); }
TEST(ArrayTest, Remove_String)
{
	Array<std::string> arr = {"one", "two", "three", "four", "five"};
	arr.remove(2);
	EXPECT_EQ(arr.size(), 4);
	EXPECT_EQ(arr[0], "one");
	EXPECT_EQ(arr[1], "two");
	EXPECT_EQ(arr[2], "four");
	EXPECT_EQ(arr[3], "five");
}

// Тест 6: Оператор [] - чтение
template<typename T>
void TestOperatorIndexRead()
{
	Array<T> arr = {T(10), T(20), T(30)};
	EXPECT_EQ(arr[0], T(10));
	EXPECT_EQ(arr[1], T(20));
	EXPECT_EQ(arr[2], T(30));
}

TEST(ArrayTest, OperatorIndexRead_Int) { TestOperatorIndexRead<int>(); }
TEST(ArrayTest, OperatorIndexRead_Float) { TestOperatorIndexRead<float>(); }
TEST(ArrayTest, OperatorIndexRead_String)
{
	Array<std::string> arr = {"first", "second", "third"};
	EXPECT_EQ(arr[0], "first");
	EXPECT_EQ(arr[1], "second");
	EXPECT_EQ(arr[2], "third");
}

// Тест 7: Оператор [] - запись
template<typename T>
void TestOperatorIndexWrite()
{
	Array<T> arr = {T(1), T(2), T(3)};
	arr[0] = T(100);
	arr[1] = T(200);
	EXPECT_EQ(arr[0], T(100));
	EXPECT_EQ(arr[1], T(200));
	EXPECT_EQ(arr[2], T(3));
}

TEST(ArrayTest, OperatorIndexWrite_Int) { TestOperatorIndexWrite<int>(); }
TEST(ArrayTest, OperatorIndexWrite_Float) { TestOperatorIndexWrite<float>(); }
TEST(ArrayTest, OperatorIndexWrite_String)
{
	Array<std::string> arr = {"one", "two", "three"};
	arr[0] = "modified_one";
	arr[1] = "modified_two";
	EXPECT_EQ(arr[0], "modified_one");
	EXPECT_EQ(arr[1], "modified_two");
	EXPECT_EQ(arr[2], "three");
}

// Тест 8: Конструктор копирования
template<typename T>
void TestCopyConstructor()
{
	Array<T> arr1 = {T(1), T(2), T(3), T(4), T(5)};
	Array<T> arr2(arr1);
	EXPECT_EQ(arr2.size(), arr1.size());
	for (size_t i = 0; i < arr1.size(); ++i)
	{
		EXPECT_EQ(arr1[i], arr2[i]);
	}
	// Изменение arr2 не должно влиять на arr1
	arr2[0] = T(999);
	EXPECT_EQ(arr1[0], T(1));
	EXPECT_EQ(arr2[0], T(999));
}

TEST(ArrayTest, CopyConstructor_Int) { TestCopyConstructor<int>(); }
TEST(ArrayTest, CopyConstructor_Float) { TestCopyConstructor<float>(); }
TEST(ArrayTest, CopyConstructor_String)
{
	Array<std::string> arr1 = {"one", "two", "three"};
	Array<std::string> arr2(arr1);
	EXPECT_EQ(arr2.size(), arr1.size());
	EXPECT_EQ(arr2[0], "one");
	EXPECT_EQ(arr2[1], "two");
	EXPECT_EQ(arr2[2], "three");
	// Изменение arr2 не должно влиять на arr1
	arr2[0] = "modified";
	EXPECT_EQ(arr1[0], "one");
	EXPECT_EQ(arr2[0], "modified");
}

// Тест 9: Оператор присваивания (копирование)
template<typename T>
void TestCopyAssignment()
{
	Array<T> arr1 = {T(1), T(2), T(3)};
	Array<T> arr2 = {T(4), T(5), T(6), T(7)};
	arr2 = arr1;
	EXPECT_EQ(arr2.size(), arr1.size());
	for (size_t i = 0; i < arr1.size(); ++i)
	{
		EXPECT_EQ(arr1[i], arr2[i]);
	}
	// Проверка независимости
	arr2[0] = T(999);
	EXPECT_EQ(arr1[0], T(1));
}

TEST(ArrayTest, CopyAssignment_Int) { TestCopyAssignment<int>(); }
TEST(ArrayTest, CopyAssignment_Float) { TestCopyAssignment<float>(); }
TEST(ArrayTest, CopyAssignment_String)
{
	Array<std::string> arr1 = {"first", "second", "third"};
	Array<std::string> arr2 = {"old1", "old2"};
	arr2 = arr1;
	EXPECT_EQ(arr2.size(), 3);
	EXPECT_EQ(arr2[0], "first");
	EXPECT_EQ(arr2[1], "second");
	EXPECT_EQ(arr2[2], "third");
}

// Тест 10: Move конструктор
template<typename T>
void TestMoveConstructor()
{
	Array<T> arr1 = {T(1), T(2), T(3), T(4), T(5)};
	size_t size = arr1.size();
	Array<T> arr2(std::move(arr1));
	EXPECT_EQ(arr2.size(), size);
	EXPECT_EQ(arr1.size(), 0);
	EXPECT_EQ(arr2[0], T(1));
	EXPECT_EQ(arr2[4], T(5));
}

TEST(ArrayTest, MoveConstructor_Int) { TestMoveConstructor<int>(); }
TEST(ArrayTest, MoveConstructor_Float) { TestMoveConstructor<float>(); }
TEST(ArrayTest, MoveConstructor_String)
{
	Array<std::string> arr1 = {"one", "two", "three"};
	size_t size = arr1.size();
	Array<std::string> arr2(std::move(arr1));
	EXPECT_EQ(arr2.size(), size);
	EXPECT_EQ(arr1.size(), 0);
	EXPECT_EQ(arr2[0], "one");
	EXPECT_EQ(arr2[2], "three");
}

// Тест 11: Move оператор присваивания
template<typename T>
void TestMoveAssignment()
{
	Array<T> arr1 = {T(1), T(2), T(3)};
	Array<T> arr2 = {T(4), T(5)};
	size_t size1 = arr1.size();
	arr2 = std::move(arr1);
	EXPECT_EQ(arr2.size(), size1);
	EXPECT_EQ(arr1.size(), 0);
	EXPECT_EQ(arr2[0], T(1));
	EXPECT_EQ(arr2[2], T(3));
}

TEST(ArrayTest, MoveAssignment_Int) { TestMoveAssignment<int>(); }
TEST(ArrayTest, MoveAssignment_Float) { TestMoveAssignment<float>(); }
TEST(ArrayTest, MoveAssignment_String)
{
	Array<std::string> arr1 = {"first", "second", "third"};
	Array<std::string> arr2 = {"old"};
	arr2 = std::move(arr1);
	EXPECT_EQ(arr2.size(), 3);
	EXPECT_EQ(arr1.size(), 0);
	EXPECT_EQ(arr2[0], "first");
	EXPECT_EQ(arr2[2], "third");
}

// Тест 12: Размер и capacity
template<typename T>
void TestSizeAndCapacity()
{
	Array<T> arr;
	EXPECT_EQ(arr.size(), 0);
	size_t initialCapacity = arr.capacity();
	
	// Добавляем элементы
	for (int i = 0; i < 20; ++i)
	{
		arr.insert(T(i));
		EXPECT_EQ(arr.size(), i + 1);
		EXPECT_GE(arr.capacity(), arr.size());
	}
	
	// Проверяем, что capacity увеличился
	EXPECT_GT(arr.capacity(), initialCapacity);
	
	// Удаляем элементы
	for (int i = 0; i < 10; ++i)
	{
		arr.remove(0);
	}
	EXPECT_EQ(arr.size(), 10);
	EXPECT_GE(arr.capacity(), arr.size());
}

TEST(ArrayTest, SizeAndCapacity_Int) { TestSizeAndCapacity<int>(); }
TEST(ArrayTest, SizeAndCapacity_Float) { TestSizeAndCapacity<float>(); }
TEST(ArrayTest, SizeAndCapacity_String)
{
	Array<std::string> arr;
	EXPECT_EQ(arr.size(), 0);
	
	for (int i = 0; i < 10; ++i)
	{
		arr.insert("item" + std::to_string(i));
		EXPECT_EQ(arr.size(), i + 1);
	}
	
	EXPECT_EQ(arr.size(), 10);
	EXPECT_GE(arr.capacity(), arr.size());
}
