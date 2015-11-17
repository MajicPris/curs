

#include <cassert> // для assert
#include <iostream>

#include <iomanip> // для setw


class column
{
private:
	column *columnPtr;                      // указатель на стек
	const int size;                   // максимальное количество элементов в стеке
	int top;                          // номер текущего элемента стека
public:
	column(int = 10);                  // по умолчанию размер стека равен 10 элементам
	column(const column&);          // конструктор копирования
	~column();                         // деструктор

	inline void push(const column &);     // поместить элемент в вершину стека
	inline column pop();                   // удалить элемент из вершины стека и вернуть его
	inline const column &Peek(int) const; // n-й элемент от вершины стека
	inline int getcolumnSize() const;  // получить размер стека
	inline int getTop() const;        // получить номер текущего элемента в стеке
};

// реализация методов шаблона класса column

// конструктор Стека
column::column(int maxSize) :
size(maxSize) // инициализация константы
{
	columnPtr = new column[size]; // выделить память под стек
	top = 0; // инициализируем текущий элемент нулем;
}

// конструктор копирования

column::column(const column & othercolumn) :
size(othercolumn.getcolumnSize()) // инициализация константы
{
	columnPtr = new column[size]; // выделить память под новый стек
	top = othercolumn.getTop();

	for (int ix = 0; ix < top; ix++)
		columnPtr[ix] = othercolumn.getPtr()[ix];
}

// функция деструктора Стека
column::~column()
{
	delete[] columnPtr; // удаляем стек
}

// функция добавления элемента в стек
inline void column::push(const column &value)
{
	// проверяем размер стека
	assert(top < size); // номер текущего элемента должен быть меньше размера стека

	columnPtr[top++] = value; // помещаем элемент в стек
}

// функция удаления элемента из стека
inline column column::pop()
{
	// проверяем размер стека
	assert(top > 0); // номер текущего элемента должен быть больше 0

	columnPtr[--top]; // удаляем элемент из стека
}

// функция возвращает n-й элемент от вершины стека
inline const column &column::Peek(int nom) const
{
	//
	assert(nom <= top);

	return columnPtr[top - nom]; // вернуть n-й элемент стека
}

// вернуть размер стека
inline int column::getcolumnSize() const
{
	return size;
}

// вернуть размер стека
inline int column::getTop() const
{
	return top;
}
