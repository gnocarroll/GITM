#pragma once

#include <cstddef>
#include <iostream>

#include "src/Vec.hpp"

template <typename T = float, size_t nRows = 3, size_t nCols = nRows>
struct Mat {
private:
	Vec<T, nCols> rows[nRows];

public:
	consteval Mat() : rows{} {};

	template <typename... Args>
	constexpr Mat(Args... _rows) : rows{ _rows... } {}

	constexpr Vec<T, nCols>* begin() const { return const_cast<Vec<T, nCols>*>(rows); }
	constexpr Vec<T, nCols>* end() const { return begin() + nRows; }

	constexpr Vec<T, nCols> operator[](size_t idx) const { return rows[idx]; }
	constexpr Vec<T, nCols>& operator[](size_t idx) { return rows[idx]; }

	static consteval Mat<T, nRows, nCols> Identity() requires (nRows == nCols) {
		Mat<T, nRows, nCols> ret;

		for (size_t i = 0; i < nRows; i++) {
			ret[i][i] = 1.0f;
		}

		return ret;
	}

	constexpr Vec<T, nRows> GetCol(size_t idx) const {
		Vec<T, nRows> ret;

		for (size_t i = 0; i < nRows; i++) {
			ret[i] = rows[i][idx];
		}

		return ret;
	}

	constexpr void SetCol(size_t idx, const Vec<T, nRows>& newCol) {
		for (size_t i = 0; i < nRows; i++) {
			rows[i][idx] = newCol[i];
		}
	}

	constexpr Mat<T, nCols, nRows> Transpose() const {
		Mat<T, nCols, nRows> ret;

		for (size_t i = 0; i < nRows; i++) {
			ret.SetCol(i, rows[i]);
		}

		return ret;
	}

	// Unary +, -

	constexpr Mat<T, nRows, nCols> operator+() const {
		return *this;
	}

	constexpr Mat<T, nRows, nCols> operator-() const {
		for (auto& r : *this) {
			r = -r;
		}
	}

	// Scalar +, -

	constexpr Mat<T, nRows, nCols>& operator+=(T other) {
		for (auto& r : rows)
			r += other;

		return *this;
	}
	constexpr Mat<T, nRows, nCols>& operator-=(T other) {
		for (auto& r : rows)
			r -= other;

		return *this;
	}

	constexpr Mat<T, nRows, nCols> operator+(T other) const {
		auto ret = *this;
		return ret += other;
	}
	constexpr Mat<T, nRows, nCols> operator-(T other) const {
		auto ret = *this;
		return ret -= other;
	}

	// Other vector +, -

	constexpr Mat<T, nRows, nCols>& operator+=(const Mat<T, nRows, nCols>& other) {
		for (size_t i = 0; i < nRows; i++) {
			rows[i] += other[i];
		}

		return *this;
	}
	constexpr Mat<T, nRows, nCols>& operator-=(const Mat<T, nRows, nCols>& other) {
		for (size_t i = 0; i < nRows; i++) {
			rows[i] -= other[i];
		}

		return *this;
	}

	constexpr Mat<T, nRows, nCols> operator+(
		const Mat<T, nRows, nCols>& other) const {
		
		Mat<T, nRows, nCols> ret = *this;
		return ret += other;
	}
	constexpr Mat<T, nRows, nCols> operator-(
		const Mat<T, nRows, nCols>& other) const {

		Mat<T, nRows, nCols> ret = *this;
		return ret -= other;
	}

	// Now will do scalar multiplication, division

	constexpr Mat<T, nRows, nCols>& operator*=(T other) {
		for (auto& r : rows)
			r *= other;

		return *this;
	}
	constexpr Mat<T, nRows, nCols>& operator/=(T other) {
		for (auto& r : rows)
			r /= other;

		return *this;
	}

	// will have symmetric scalar multiplication operator overloads outside
	// the class definition

	constexpr Mat<T, nRows, nCols> operator/(T other) const {
		Mat<T, nRows, nCols> ret = *this;
		return ret /= other;
	}

	// Multiply with column vector

	constexpr Vec<T, nRows> operator*(const Vec<T, nCols>& v) const {
		Vec<T, nRows> ret;

		for (size_t i = 0; i < nRows; i++) {
			ret[i] = rows[i] * v;
		}

		return ret;
	}

	// Matrix multiplication

	template <size_t otherNCols>
	constexpr Mat<T, nRows, otherNCols> operator*(
		const Mat<T, nCols, otherNCols>& other) const {

		Mat<T, nRows, otherNCols> ret;
		auto otherT = other.Transpose();

		for (size_t i = 0; i < nRows; i++) {
			ret[i] = otherT * rows[i];
		}

		return ret;
	}
};

// Scalar multiplication (can be either order)

template <typename T, size_t nRows, size_t nCols>
constexpr Mat<T, nRows, nCols> operator*(const Mat<T, nRows, nCols>& m, T scalar) {
	Mat<T, nRows, nCols> ret = m;
	return ret *= scalar;
}

template <typename T, size_t nRows, size_t nCols>
constexpr Mat<T, nRows, nCols> operator*(T scalar, const Mat<T, nRows, nCols>& m) {
	return m * scalar;
}

// Stream IO
template<typename T, size_t nRows, size_t nCols>
static inline std::istream& operator>>(std::istream& istr, Mat<T, nRows, nCols>& mat) {
	for (auto& row : mat) {
		istr >> row;
	}

	return istr;
}

template<typename T, size_t nRows, size_t nCols>
static inline std::ostream& operator<<(std::ostream& ostr, const Mat<T, nRows, nCols>& mat) {
	ostr << mat[0];

	for (auto iter = mat.begin() + 1; iter < mat.end(); iter++) {
		ostr << '\n' << (*iter);
	}

	return ostr;
}

typedef Mat<> M33;
typedef Mat<float, 4> M44;