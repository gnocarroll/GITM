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

	constexpr Vec<T, nCols> operator[](int idx) const { return rows[idx]; }
	constexpr Vec<T, nCols>& operator[](int idx) { return rows[idx]; }

	static consteval Mat<T, nRows, nCols> Identity() requires (nRows == nCols) {
		Mat<T, nRows, nCols> ret;

		for (size_t i = 0; i < nRows; i++) {
			ret[i][i] = 1.0f;
		}

		return ret;
	}
};

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