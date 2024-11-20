#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <gcem.hpp>

#include "src/MathUtil.hpp"
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

	constexpr T Element(size_t idx) const {
		return reinterpret_cast<T*>(this)[idx];
	}
	constexpr T& Element(size_t idx) {
		return reinterpret_cast<T*>(this)[idx];
	}

	constexpr T* Data() {
		return reinterpret_cast<T*>(this);
	}

	constexpr void SetCol(size_t idx, const Vec<T, nRows>& newCol) {
		for (size_t i = 0; i < nRows; i++) {
			rows[i][idx] = newCol[i];
		}
	}

	template <size_t otherRows, size_t otherCols>
	constexpr void CopyIn(size_t row, size_t col,
		const Mat<T, otherRows, otherCols>& other) {
		
		size_t rowEnd = std::min(nRows, row + otherRows);
		size_t colEnd = std::min(nCols, col + otherCols);

		for (size_t i = row; i < rowEnd; i++) {
			for (size_t j = col; j < colEnd; j++) {
				rows[i][j] = other[i - row][j - col];
			}
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

typedef Mat<float, 2> M22;
typedef Mat<> M33;
typedef Mat<float, 4> M44;

constexpr M33 crossProdMat(const V3& v) {
	M33 ret;

	ret[0][1] = -v[2];
	ret[0][2] = v[1];

	ret[1][0] = v[2];
	ret[1][2] = -v[0];

	ret[2][0] = -v[1];
	ret[2][1] = v[0];

	return ret;
}

template <typename T, size_t count, size_t otherCount>
constexpr Mat<T, count, otherCount> outerProd(
	const Vec<T, count>& v, const Vec<T, otherCount>& other) {

	Mat<T, count, otherCount> ret;

	for (size_t i = 0; i < count; i++) {
		for (size_t j = 0; j < otherCount; j++) {
			ret[i][j] = v[i] * other[j];
		}
	}

	return ret;
}

constexpr M22 rotation2D(float degrees) {
	float rad = DEG2RAD * degrees;
	float cosRet = gcem::cos(rad);
	float sinRet = gcem::sin(rad);

	return M22{
		V2{cosRet, sinRet},
		V2{-sinRet, cosRet}
	};
}

template <size_t ax, size_t nRows>
constexpr Mat<float, nRows, nRows> rotationAxis(float degrees) {
	static_assert(ax <= 2);
	static_assert((nRows == 3) || (nRows == 4));

	float rad = DEG2RAD * degrees;
	float cosRet = std::cos(rad);
	float sinRet = std::sin(rad);

	Mat<float, nRows, nRows> ret;

	auto sh = [](size_t idx) consteval {
		return (idx + ax) % 3;
	};

	ret[sh(0)][sh(0)] = 1.0f;

	ret[sh(1)][sh(1)] = cosRet;
	ret[sh(1)][sh(2)] = -sinRet;
	ret[sh(2)][sh(1)] = sinRet;
	ret[sh(2)][sh(2)] = cosRet;

	if constexpr (nRows == 4) {
		ret[3][3] = 1.0f;
	}

	return ret;
}

template <size_t nRows>
constexpr Mat<float, nRows, nRows> rotationX(float degrees) {
	return rotationAxis<0, nRows>(degrees);
}

template <size_t nRows>
constexpr Mat<float, nRows, nRows> rotationY(float degrees) {
	return rotationAxis<1, nRows>(degrees);
}

template <size_t nRows>
constexpr Mat<float, nRows, nRows> rotationZ(float degrees) {
	return rotationAxis<2, nRows>(degrees);
}

template <size_t nRows, size_t axCount>
constexpr Mat<float, nRows, nRows> rotation(float degrees, Vec<float, axCount> ax) {
	static_assert((nRows == 3) || (nRows == 4));
	static_assert((axCount == 3) || (axCount == 4));

	// Want first 3 entries of ax to be normalized V3
	V3& axRef = *((V3*)&ax);
	axRef = axRef.Normalize();

	float rad = DEG2RAD * degrees;
	float sinRet = gcem::sin(rad);
	float cosRet = gcem::cos(rad);

	M33 topLeft = cosRet * M33::Identity() +
		sinRet * crossProdMat(axRef) +
		(1.0f - cosRet) * outerProd(axRef, axRef);

	Mat<float, nRows, nRows> ret;

	if constexpr (nRows == 3) {
		ret = topLeft;
		return ret;
	}

	ret.CopyIn(0, 0, topLeft);
	ret[3][3] = 1.0f;

	return ret;
}

// Useful for OpenGL

// assumes we want camera in middle aligned with planes (fine for current
// purposes), provide perspective projection frustum for OpenGL
constexpr M44 perspective(float fov, float aspRatio, float near, float far) {
	float right = near * gcem::tan(fov / 2.0f * DEG2RAD);
	float top = right / aspRatio;
	float dist = far - near;

	M44 ret;
	float* data = ret.Data();

	data[0] = near / right;
	data[5] = near / top;
	data[10] = -(far + near) / dist;
	data[11] = -(2.0f * far * near) / dist;
	data[14] = -1.0f;

	return ret;
}

// now instead for orthographic projection (not sure I will actually use)
constexpr M44 ortho(float fov, float aspRatio, float near, float far) {
	float right = near * gcem::tan(fov / 2.0f * DEG2RAD);
	float top = right / aspRatio;
	float dist = far - near;

	M44 ret;
	float* data = ret.Data();

	data[0] = 1.0f / right;
	data[5] = 1.0f / top;
	data[10] = -2.0f / dist;
	data[11] = -(far + near) / dist;
	data[15] = 1.0f;

	return ret;
}