#pragma once

#include <cmath>
#include <cstddef>
#include <iostream>

#include "src/MyIntDef.hpp"

/*
 *	Template vector class: can use different types and be of different
 *  lengths. Reminiscient of std::arr.
 */

template <typename T = float, size_t count = 3>
struct Vec {
private:
	T data[count];

public:
	consteval Vec() : data{} {}

	template <typename... Args>
	constexpr Vec(Args... _data) : data{ _data... } {}

	constexpr T* begin() const { return const_cast<T*>(data); };
	constexpr T* end() const { return begin() + count; };

	constexpr Vec(T arg) {
		for (T& el : (*this)) {
			el = arg;
		}
	}

	consteval size_t Count() { return count; }

	constexpr T operator[](size_t idx) const { return data[idx]; };
	constexpr T& operator[](size_t idx) { return data[idx]; };

	// Unary +/- (not much of a point in + but whatever)
	constexpr Vec<T, count> operator+() { return *this; }
	constexpr Vec<T, count> operator-() {
		Vec<T, count> ret = *this;

		for (T& el : ret) {
			el = -el;
		}

		return ret;
	}

	// Operations with other vector
	// +=, -=
	constexpr Vec<T, count>& operator+=(const Vec<T, count>& addend) {
		for (size_t i = 0; i < count; i++) {
			data[i] += addend[i];
		}

		return *this;
	}
	constexpr Vec<T, count>& operator-=(const Vec<T, count>& other) {
		for (size_t i = 0; i < count; i++) {
			data[i] -= other[i];
		}

		return *this;
	}

	// +, -
	constexpr Vec<T, count> operator+(const Vec<T, count>& addend) const {
		Vec<T, count> ret = *this;
		return ret += addend;
	}
	constexpr Vec<T, count> operator-(const Vec<T, count>& other) const {
		Vec<T, count> ret = *this;
		return ret -= other;
	}

	// v * v (DOT PRODUCT)
	constexpr T operator*(const Vec<T, count>& other) const {
		T ret = data[0] * other[0];

		for (size_t i = 1; i < count; i++) {
			ret += data[i] * other[i];
		}

		return ret;
	}

	// Operations with scalar below
	// Four functions for compound assignment operators +=, -=, *=, /=
	constexpr Vec<T, count>& operator+=(T addend) {
		for (T& el : *this) {
			el += addend;
		}

		return *this;
	}
	constexpr Vec<T, count>& operator-=(T scalar) {
		for (T& el : *this) {
			el -= scalar;
		}

		return *this;
	}
	constexpr Vec<T, count>& operator*=(T scalar) {
		for (T& el : *this) {
			el *= scalar;
		}

		return *this;
	}
	constexpr Vec<T, count>& operator/=(T divisor) {
		for (T& el : *this) {
			el /= divisor;
		}

		return *this;
	}

	// 3 functions for +, -, / (* will be outside class declaration)
	constexpr Vec<T, count> operator+(T addend) const {
		Vec<T, count> ret = *this;
		return ret += addend;
	}
	constexpr Vec<T, count> operator-(T scalar) const {
		Vec<T, count> ret = *this;
		return ret -= scalar;
	}
	constexpr Vec<T, count> operator/(T divisor) const {
		Vec<T, count> ret = *this;
		return ret /= divisor;
	}

	// Additional useful functionality

	constexpr auto Length() const {
		return std::sqrt((*this) * (*this));
	}

	constexpr Vec<T, count> Normalize() const {
		return (*this) / Length();
	}

	constexpr Mat<T, count, count> CrossProdMat() const
		requires (count == 3) {
		Mat<T, count, count> ret;
		
		ret[0][1] = -data[2];
		ret[0][2] = data[1];

		ret[1][0] = data[2];
		ret[1][2] = -data[0];

		ret[2][0] = -data[1];
		ret[2][1] = data[0];

		return ret;
	}

	template <size_t otherCount>
	constexpr Mat<T, count, otherCount> OuterProd(
		const Vec<T, otherCount>& other) {

		Mat<T, count, otherCount> ret;

		for (size_t i = 0; i < count; i++) {
			for (size_t j = 0; j < otherCount; j++) {
				ret[i][j] = data[i] * other[j];
			}
		}

		return ret;
	}
};

// v * scalar, scalar * v
template <typename T, size_t count>
constexpr Vec<T, count> operator*(const Vec<T, count>& v, T scalar) {
	Vec<T, count> ret = v;
	return ret *= scalar;
}
template <typename T, size_t count>
constexpr Vec<T, count> operator*(T scalar, const Vec<T, count>& v) {
	return v * scalar;
}

/*
 *  Cross product
 *  I am aware that analogous product exists for vectors of other dimensions
 *  but I will just define this for vectors of 3 dimensions.
 */
template <typename T>
constexpr Vec<T, 3> operator^(const Vec<T, 3>& v1, const Vec<T, 3>& v2) {
	return Vec<T, 3>(v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]);
}

// Stream IO
template<typename T, size_t count>
static inline std::istream& operator>>(std::istream& istr, Vec<T, count>& v) {
	for (T& el : v) {
		istr >> el;
	}

	return istr;
}

template<typename T, size_t count>
static inline std::ostream& operator<<(std::ostream& ostr, const Vec<T, count>& v) {
	ostr << v[0];

	for (auto iter = v.begin() + 1; iter < v.end(); iter++) {
		ostr << ' ' << (*iter);
	}

	return ostr;
}

// typedefs for vectors that are likely to be used frequently
typedef Vec<float, 2> V2;
typedef Vec<i32, 2> V2i;
typedef Vec<int, 2> V2int;

typedef Vec<> V3;
typedef Vec<i32> V3i;
typedef Vec<ui32> V3ui;

typedef Vec<> V4;
typedef Vec<i32> V4i;
typedef Vec<ui32> V4ui;