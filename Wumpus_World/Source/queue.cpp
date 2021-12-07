#include"function.h"

void Queue::push(Cell c)
{
	data.push_back(c);
}
Cell Queue::pop()
{
	Cell pop_cell = data[0];
	data.erase(data.begin());
	return pop_cell;
}
void Queue::display()
{
	if (data.empty())
		cout << "The queue is empty" << endl;
	else
		for (auto a : data)
		{
			cout << "(" << a.x << "," << a.y << "," << a.value << ") ";
		}
	cout << endl;
}

bool Queue::empty() {
	return data.empty();
}

int Queue::size() {
	return data.size();
}

Cell Queue::at(int i) {
	return data[i];
}

void Queue::swap(Cell* a, Cell* b)
{
	Cell t = *a;
	*a = *b;
	*b = t;
}