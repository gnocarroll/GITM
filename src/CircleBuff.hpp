#pragma once

template <typename T, size_t count>
class CircleBuff {
private:
	T data[count];
	size_t front;
	size_t back;

public:
	CircleBuff() : front(count - 1), back(0) {}

	void Push(T el);
	T Pop();
};