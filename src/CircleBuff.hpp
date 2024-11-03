#pragma once

template <typename T, size_t count>
class CircleBuff {
private:
	T data[count];
	size_t front;
	size_t back;

public:
	CircleBuff() : data{}, front(count - 1), back(0) {}

	void Push(T el) {
		if (back == front) {
			front = (front + 1) % count;
		}

		data[back] = el;
		back = (back + 1) % count;
	}

	T Pop() {
		if (front + 1 % count == back) {
			return T(0);
		}

		front = (front + 1) % count;
		return data[front];
	}
};