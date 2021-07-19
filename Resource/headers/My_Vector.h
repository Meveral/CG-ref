#pragma once
//#define NDEBUG
#include<assert.h>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<memory>
#include<vector>

namespace Meveral {
	typedef unsigned int uint;
	typedef unsigned int Index;

	template<int DimRows, int DimCols, typename T> class Mat;

	template<int Dim, typename T>
	class Vec {
	public:
		Vec() { assert(Dim > 0); for (int i = Dim; i--; _data[i] = T()); }
		explicit Vec(const std::vector<T>&v) { assert(v.size() >= Dim && "too few elements to contruct Vector"); for (int i = Dim; i--; _data[i] = v[i]); }
		//		Vector(const Vector<Dim, T>&v) {};
		//		Vector operator=(const Vector<Dim, T>&rhs) {};
		~Vec() {};
	public:
		T& operator[](int i) { assert(i >= 0 && i < Dim&&"Index out of range"); return _data[i]; };
		const T& operator[](int i)const { assert(i >= 0 && i < Dim&&"Index out of range"); return _data[i]; };
	public:
		T* data() { return &_data; };
		template<int n> Vec<n, T> head();
		T& x() { assert(Dim > 0 && "out of range"); return _data[0]; }
		T& y() { assert(Dim > 1 && "out of range"); return _data[1]; }
		T& z() { assert(Dim > 2 && "out of range"); return _data[2]; }
		T& w() { assert(Dim > 3 && "out of range"); return _data[3]; }
		const T& x()const { assert(Dim > 0 && "out of range"); return _data[0]; }
		const T& y()const { assert(Dim > 1 && "out of range"); return _data[1]; }
		const T& z()const { assert(Dim > 2 && "out of range"); return _data[2]; }
		const T& w()const { assert(Dim > 3 && "out of range"); return _data[3]; }
		Vec<Dim, T> cwiseProduct();
		void print(std::ostream&os = std::cout)const {
			for (int i = 0; i < Dim; ++i)
				os << _data[i] << " ";
		}
	private:
		T _data[Dim];
	};

	template<typename T>
	class Vec<2, T> {
	public:
		Vec() :_x(T()), _y(T()) {}
		Vec(T x, T y) :_x(x), _y(y) {}
		~Vec() {}
	public:
		T& operator[](int i) { assert(i >= 0 && i < _dim&& "Index out of range"); return i == 0 ? _x : _y; }
		const T& operator[](int i)const { assert(i >= 0 && i < _dim&& "Index out of range"); return i == 0 ? _x : _y; }
	public:
		T& x() { return _x; };
		const T& x()const { return _x; };
		T& y() { return _y; };
		const T& y()const { return _y; };

		float norm() { return std::sqrt(_x*_x + _y * _y); }
		Vec& normalize() { float tmp = 1.f / this->norm(); _x *= tmp; _y *= tmp; return *this; }
		Vec normalized()const { Vec<_dim, T> ret(_x, _y); ret.normalize(); return ret; }

		void print(std::ostream&os = std::cout) const{ os << _x << " " << _y; }
	private:
		static const int _dim{ 2 };
		T _x;
		T _y;
	};

	template<typename T>
	class Vec <3, T> {
	public:
		Vec() :_x(T()), _y(T()), _z(T()) {};
		Vec(T x, T y, T z) :_x(x), _y(y), _z(z) {};
		Vec(const Vec<3, T>&v) :_x(v._x), _y(v._y), _z(v._z) {};
		~Vec() {};
	public:
		T& operator[](int i) { assert(i >= 0 && i < _dim && "Index out of range"); return i == 0 ? _x : (i == 1 ? _y : _z); };
		const T& operator[](int i)const { assert(i >= 0 && i < _dim && "Index out of range"); return i == 0 ? _x : (i == 1 ? _y : _z); };
	public:
		T& x() { return _x; };
		T& y() { return _y; };
		T& z() { return _z; };
		const T& x()const { return _x; };
		const T& y()const { return _y; };
		const T& z()const { return _z; };

		float norm() { return std::sqrt(_x*_x + _y * _y + _z * _z); }
		Vec& normalize() { float tmp = 1.f / this->norm(); _x *= tmp; _y *= tmp; _z *= tmp; return *this; }
		Vec normalized()const { Vec<_dim, T> ret(_x, _y, _z); ret.normalize(); return ret; }
		Vec inverse()const { Vec<_dim, T> ret(1.f / _x, 1.f / _y, 1.f / _z); return ret; }

		Vec<3, T> cross(const Vec<3, T>&v)const;
		T dot(const Vec<3, T>&v)const;
		Vec<3, T> cwiseProduct(const Vec<3, T>&v)const { return Vec<3, T>(_x*v._x,_y*v._y,_z*v._z); };

		void print(std::ostream&os = std::cout)const { os << _x << " " << _y << " " << _z; }

	public:
	private:
		static const int _dim{ 3 };
		T _x;
		T _y;
		T _z;
	};

	// Vector operator overloading
	template<int Dim, typename T>
	Vec<Dim, T> operator+(Vec<Dim, T>lhs, const Vec<Dim, T>&rhs) { for (int i = Dim; i--; lhs[i] += rhs[i]); return lhs; };
	template<int Dim, typename T>
	Vec<Dim, T> operator+(Vec<Dim, T>lhs, T n) { for (int i = Dim; i--; lhs[i] += n); return lhs; };
	template<int Dim,typename T1,typename T2>
	Vec<Dim, T1>& operator+=(Vec<Dim, T1>&lhs, const T2&rhs) { lhs = lhs + rhs; return lhs; };

	template<int Dim, typename T>
	Vec<Dim, T> operator-(Vec<Dim, T>lhs, const Vec<Dim, T>&rhs) { for (int i = Dim; i--; lhs[i] -= rhs[i]); return lhs; };
	template<int Dim, typename T>
	Vec<Dim, T> operator-(Vec<Dim, T>lhs, T n) { for (int i = Dim; i--; lhs[i] -= n); return lhs; };
	template<int Dim, typename T>
	Vec<Dim, T> operator-(Vec<Dim, T>lhs) { for (int i = Dim; i--; lhs[i] = -lhs[i]); return lhs; };

	

	template<int Dim, typename T>
	T operator*(const Vec<Dim, T>lhs, const Vec<Dim, T>&rhs) { T ret{}; for (int i = Dim; i--; ret += lhs[i] * rhs[i]); return ret; };
	template<int Dim, typename T1,typename T2>
	Vec<Dim, T1> operator*(Vec<Dim, T1>v, T2 t) { for (int i = Dim; i--; v[i] = v[i] * t); return v; }
	template<int Dim,typename T1,typename T2>
	Vec<Dim, T1>& operator*=(Vec<Dim, T1>&lhs, const T2&rhs) { lhs = lhs * rhs; return lhs; };



	template<int Dim, typename T>
	Vec<Dim, T> operator/(Vec<Dim, T>lhs, const Vec<Dim, T>&rhs) { for (int i = Dim; i--; lhs[i] /= rhs[i]); return lhs; };
	template<int Dim, typename T>
	Vec<Dim, T> operator/(Vec<Dim, T>lhs, T n) { for (int i = Dim; i--; lhs[i] /= n); return lhs; };

	template<int Dim, typename T>
	Vec<Dim, T>& operator+=(Vec<Dim, T>&lhs, const Vec<Dim, T>&rhs) { lhs = lhs + rhs; return lhs; };

	template<int Dim, typename T>
	std::ostream& operator<<( std::ostream&os, Vec<Dim, T>&lhs ) { lhs.print(os); return os; }


	template<int Dim, typename T>
	struct dt {
		static T det(const Mat<Dim, Dim, T>&m)
		{
			T ret{};
			for (int i = Dim; i--; ret += m[0][i] * m.cofactor(0, i));
			return ret;
		}
	};

	template<typename T>
	struct  dt<1, T> {
		static T det(const Mat<1, 1, T>&m)
		{
			return m[0][0];
		}
	};

	template<int DimRows, int DimCols, typename T>
	class Mat {
	public:
		Mat() { assert(DimRows > 0 && DimCols > 0); for (int i = DimRows; i--; _data[i] = Vec<DimCols, T>()); }
		~Mat() {};
		//some constructor todo:
	public:
		Vec<DimCols, T>& operator[](int i) { assert(i < DimRows); return _data[i]; }
		const Vec<DimCols, T>& operator[](int i)const { assert(i < DimRows); return _data[i]; }

		Mat& operator<<(T t) { _data[0][0] = t; currentIndex = 1; return *this; }
		Mat& operator,(T t);


	public:
		void print(std::ostream&os = std::cout)const { for (int i = 0; i < DimRows; ++i) { _data[i].print(os); os << std::endl; } }
		Vec<DimCols, T> row(int n) { assert(n >= 0 && n < DimRows&&"out of range"); return _data[n]; }
		const Vec<DimCols, T> row(int n)const { assert(n >= 0 && n < DimRows&&"out of range"); return _data[n]; }
		Vec<DimRows, T> col(int)const;
		Mat<DimCols, DimRows, T> transpose()const;
		Mat<DimRows, DimCols, T> invert()const;
		Mat<DimRows, DimCols, T> invertTranspose()const;
		Mat<DimRows, DimCols, T> adjugate()const;
		Mat<DimRows - 1, DimCols - 1, T> getMinor(int row, int col)const;
		T cofactor(int row, int col)const;
		T det()const;
	public:
		static Mat<DimRows, DimCols, T> Identity();








	private:
		static const uint  _size{ DimRows*DimCols };
		int currentIndex{ 0 };
		Vec<DimCols, T> _data[DimRows];
	};


	template<int Dim, typename T> using Vector = Vec<Dim, T>;
	template<int DimRows, int DimCols, typename T> using Matrix = Mat<DimRows, DimCols, T>;

	typedef Vec<2, int> Vector2i;
	typedef Vec<3, int> Vector3i;
	typedef Vec<4, int> Vector4i;

	typedef Vec<2, float> Vector2f;
	typedef Vec<3, float> Vector3f;
	typedef Vec<4, float> Vector4f;

	typedef Mat<3, 3, int> Matrix3i;
	typedef Mat<4, 4, int> Matrix4i;

	typedef Mat<3, 3, float> Matrix3f;
	typedef Mat<4, 4, float> Matrix4f;

	template<int DimRows, int DimCols, typename T>
	inline Mat<DimRows, DimCols, T> & Mat<DimRows, DimCols, T>::operator,(T t)
	{
		if (currentIndex < _size)
		{
			int r = currentIndex / DimCols;
			int c = currentIndex % DimCols;
			_data[r][c] = t;
			++currentIndex;
		}
		return *this;
	}

	template<int DimRows, int DimCols, typename T>
	inline Vec<DimRows, T> Mat<DimRows, DimCols, T>::col(int n)const
	{
		assert(n >= 0 && n < DimCols&&"out of range");
		Vec<DimRows, T> ret;
		for (int i = DimRows; i--; ret[i] = _data[i][n]);

		return ret;
	}

	template<int DimRows, int DimCols, typename T>
	inline Mat<DimCols, DimRows, T> Mat<DimRows, DimCols, T>::transpose()const
	{
		Mat<DimCols, DimRows, T>ret;
//		for (int i = DimCols; i--; ret[i] = col(i));
		for (int i = 0; i < DimCols; ++i)
		{
			ret[i] = col(i);
		}

		return ret;
	}

	template<int DimRows, int DimCols, typename T>
	inline Mat<DimRows, DimCols, T> Mat<DimRows, DimCols, T>::invert()const
	{
		Mat<DimRows, DimCols, T>ret = this->adjugate();
		T detValue = ret.row(0)*this->col(0);
		T invertDet = 1.f / detValue;
		ret = ret * invertDet;
		return ret;
	}

	template<int DimRows, int DimCols, typename T>
	inline Mat<DimRows, DimCols, T> Mat<DimRows, DimCols, T>::invertTranspose()const
	{
		return this->invert().transpose();
	}

	template<int DimRows, int DimCols, typename T>
	inline Mat<DimRows, DimCols, T> Mat<DimRows, DimCols, T>::adjugate()const
	{
		Mat<DimRows, DimCols, T>ret;
		for (int i = DimRows; i--;)
			for (int j = DimCols; j--; ret[i][j] = cofactor(j, i));
		return ret;
	}

	template<int DimRows, int DimCols, typename T>
	inline Mat<DimRows - 1, DimCols - 1, T> Mat<DimRows, DimCols, T>::getMinor(int row, int col)const
	{
		Mat<DimRows - 1, DimCols - 1, T>ret;
		for (int i = DimRows - 2; i >= 0; --i)
			for (int j = DimCols - 2; j >= 0; --j)
				ret[i][j] = _data[i < row ? i : i + 1][j < col ? j : j + 1];
		return ret;
	}

	template<int DimRows, int DimCols, typename T>
	inline T Mat<DimRows, DimCols, T>::cofactor(int row, int col)const
	{
		assert(DimRows == DimCols);
		T ret = dt<DimRows - 1, T>::det(getMinor(row, col));
		if (ret == 0)
			return ret;
		return ((row + col) & 1 )? -ret : ret;
	}

	template<int DimRows, int DimCols, typename T>
	inline T Mat<DimRows, DimCols, T>::det()const
	{
		assert(DimRows == DimCols);
		T ret{};
		for (int i = DimCols; i--; ret += _data[0][i] * cofactor(i, 0));
		for (int i = 0; i < DimCols; ++i)
			ret += _data[0][i] * cofactor(i, 0);
		return ret;
	}


	template<int DimRows, int DimCols, typename T>
	inline Mat<DimRows, DimCols, T> Mat<DimRows, DimCols, T>::Identity()
	{
		assert(DimRows == DimCols && "no Identity");
		Mat<DimRows, DimCols, T> ret;
		for (int i = DimRows; i--; ret[i][i] = 1);
		return ret;
	}


	// Matrix operator overloading
	template<int R1, int C1, int C2, typename T>
	Mat<R1, C2, T> operator*(const Mat<R1, C1, T>&lhs, const Mat<C1, C2, T>&rhs)
	{
		Mat<R1, C2, T> ret;
		for (int i = R1; i--;)
			for (int j = C1; j--; ret[i][j] = lhs[i] * rhs.col(j));
		return ret;
	}
	template<int DimRows, int DimCols, typename T>
	Mat<DimRows, DimCols, T> operator*(const Mat<DimRows, DimCols, T>&m, T t)
	{
		Mat<DimRows, DimCols, T> ret;
		for (int i = DimRows; i--;)
			for (int j = DimCols; j--; ret[i][j] = m[i][j] * t);
		return ret;
	}

	template<int DimRows, int DimCols, typename T>
	Vec<DimRows, T> operator*(const Mat<DimRows, DimCols, T>&m, const Vec<DimCols, T>&v) { Vec<DimRows, T>ret; for (int i = DimRows; i--; ret[i] = m.row(i) * v); return ret; };


	template<int Dim, typename T>
	template<int n>
	inline Vec<n, T> Vec<Dim, T>::head()
	{
		assert(n > 0 && n < Dim&&"can`t make head.");
		Vec<n, T> ret;
		for (int i = n; i--; ret[i] = _data[i]);
		return ret;
	}

	template<typename T>
	inline Vec<3, T> Vec<3, T>::cross(const Vec<3, T>& v)const
	{
		Vec<3, T> ret;
		ret._x = _y * v._z - _z * v._y;
		ret._y = _z * v._x - _x * v._z;
		ret._z = _x * v._y - _y * v._x;
		return ret;
	}

	template<typename T>
	inline T Vec<3, T>::dot(const Vec<3, T>& v)const
	{
		return (*this)*v;
	}



}
