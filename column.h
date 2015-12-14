#include <iostream>
#include <iomanip>

using namespace std;

class column
{
private:
	vehicle *stackPtr;                      // указатель на стек
	int size;                   // максимальное количество элементов в стеке
	int top;                          // номер текущего элемента стека
public:
	column(int = 10);                  // по умолчанию размер стека равен 10 элементам
	column(const column&);          // конструктор копирования
	~column();                         // деструктор

	inline void push(const vehicle&);     // поместить элемент в вершину стека
	inline vehicle pop();                   // удалить элемент из вершины стека и вернуть его
	inline void printStack();         // вывод стека на экран
	inline vehicle &Peek(int); // n-й элемент от вершины стека
	inline int getStackSize() const;  // получить размер стека
	inline vehicle *getPtr() const;         // получить указатель на стек
	inline int getTop() const;        // получить номер текущего элемента в стеке

	inline void checkSpeed();
	inline float checkDis(int, int);
};

// реализация методов шаблона класса STack
// конструктор Стека
column::column(int maxSize) : size(maxSize) // инициализация константы
{
	stackPtr = new vehicle[size]; // выделить память под стек
	top = 0; // инициализируем текущий элемент нулем;
}

// конструктор копирования
column::column(const column &otherStack) : size(otherStack.getStackSize()) // инициализация константы
{
	stackPtr = new vehicle[size]; // выделить память под новый стек
	top = otherStack.getTop();

	for (int ix = 0; ix < top; ix++)
		stackPtr[ix] = otherStack.getPtr()[ix];
}

// функция деструктора Стека
column::~column()
{
	delete[] stackPtr; // удаляем стек
}

// функция добавления элемента в стек
inline void column::push(const vehicle &value)
{
	// проверяем размер стека
	if (top < size) // номер текущего элемента должен быть меньше размера стека
		stackPtr[top++] = value; // помещаем элемент в стек
}

// функция удаления элемента из стека
inline vehicle column::pop()
{
	// проверяем размер стека
	if (top > 0) // номер текущего элемента должен быть больше 0
		stackPtr[--top]; // удаляем элемент из стека
}

// функция возвращает n-й элемент от вершины стека
inline vehicle& column::Peek(int nom)
{
	if (nom <= top)
		return stackPtr[nom]; // вернуть n-й элемент стека
}

// вывод стека на экран
inline void column::printStack()
{
	for (int ix = top - 1; ix >= 0; ix--)
		cout << "|" << setw(4) << stackPtr[ix].idVehicle << endl;
}

// вернуть размер стека
inline int column::getStackSize() const
{
	return size;
}

// вернуть указатель на стек (для конструктора копирования)
inline vehicle* column::getPtr() const
{
	return stackPtr;
}

// вернуть размер стека
inline int column::getTop() const
{
	return top;
}

inline float column::checkDis(int c2, int c1 = 0)
{
	if (c2 <= getTop())
	{
		vehicle const &car1 = Peek(c1);
		vehicle const &car2 = Peek(c2);

		return sqrt(pow((car1.x - car2.x), 2) + pow((car1.y - car2.y), 2));
	}
	return 0.0;
}

inline void column::checkSpeed()
{
	float dv = 1.4; // v = 0.05
	for (int i = 1; i < 3; i++)
	{
		vehicle car1 = Peek(i - 1);
		vehicle &car2 = Peek(i);
		if (checkDis(i, i - 1) > (car1.height / 2 + car2.height / 2 + 4))
		{
			car2.destination *= car2.acceleration;
			/*
			if (car1.destination.x - dv > car2.destination.x)
			car2.destination.x += dv;
			if (car1.destination.x + dv < car2.destination.x)
			car2.destination.x -= dv;
			if (car1.destination.y - dv > car2.destination.y)
			car2.destination.y += dv;
			if (car1.destination.y + dv < car2.destination.y)
			car2.destination.y -= dv;
			*/
		}
		if (checkDis(i, i - 1) < (car1.height / 2 + car2.height / 2 + 7))
			car2.destination *= car2.deceleration;
	}

}
