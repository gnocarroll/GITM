#include "src/CircleBuff.hpp"

template <typename T, size_t count>
void CircleBuff<T, count>::Push(T el) {
	if (back == front) {
		front = (front + 1) % count;
	}

	data[back] = el;
	back = (back + 1) % count;
}

template <typename T, size_t count>
T CircleBuff<T, count>::Pop() {
	if (front + 1 % count == back) {
		return T(0);
	}

	front = (front + 1) % count;
	return data[front];
}