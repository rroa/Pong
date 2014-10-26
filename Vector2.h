#pragma once
#include <iostream>
#include <cmath>

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

namespace Math
{
	template<typename T>
	class VECTOR2
	{
	public:
		// CTOR
		//
		VECTOR2();
		VECTOR2( T _x, T _y );

		// FUNCTIONS
		//
		float GetDistance();
		float Normalize();
		void Abs();
		T AddFields() const;

		bool operator==(const VECTOR2<T>& other) const;
		bool operator!=(const VECTOR2<T>& other) const;
		T& operator[](int i);
		VECTOR2<T> operator+(const VECTOR2<T>& other) const;
		VECTOR2<T>& operator+=(const VECTOR2<T>& other);
		VECTOR2<T>& operator*=(const VECTOR2<T>& other);
		VECTOR2<T>& operator*=(const T other);
		VECTOR2<T> operator-(const VECTOR2<T>& other) const;

		// DATA
		//
		T x;
		T y;		
	};

	template<typename T>
	VECTOR2<T>& Math::VECTOR2<T>::operator*=( const T other )
	{
		x *= other;
		y *= other;
		return *this;
	}

	template<typename T>
	VECTOR2<T>& Math::VECTOR2<T>::operator*=( const VECTOR2<T>& other )
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template<typename T>
	VECTOR2<T>& Math::VECTOR2<T>::operator+=( const VECTOR2<T>& other )
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template<typename T>
	T& Math::VECTOR2<T>::operator[]( int i )
	{
		return (&x)[i];
	}

	template<typename T>
	VECTOR2<T> Math::VECTOR2<T>::operator-( const VECTOR2<T>& other ) const
	{
		return VECTOR2<T>( x - other.x, y - other.y );
	}

	template<typename T>
	VECTOR2<T> Math::VECTOR2<T>::operator+( const VECTOR2<T>& other ) const
	{
		return VECTOR2<T>( x + other.x, y + other.y );
	}

	template<typename T>
	T Math::VECTOR2<T>::AddFields() const
	{
		return x + y;
	}

	template<typename T>
	void Math::VECTOR2<T>::Abs()
	{
		std::abs(x);
		std::abs(y);
	}

	template<typename T>
	inline std::ostream& operator<< ( std::ostream& stm, const VECTOR2<T>& other )
	{ 
		return stm << '<' << other.x << "," << other.y << '>' ;
	}
	

	template<typename T>
	bool Math::VECTOR2<T>::operator==( const VECTOR2<T>& other ) const
	{
		return this->x == other.x && this->y == other.y; 
	}

	template<typename T>
	bool Math::VECTOR2<T>::operator!=( const VECTOR2<T>& other ) const
	{
		return !(*this == other);
	}

	template<typename T>
	VECTOR2<T>::VECTOR2()
		: x ( 0 )
		, y ( 0 )
	{}

	template<typename T>
	VECTOR2<T>::VECTOR2( T _x, T _y )
		: x ( _x )
		, y ( _y )
	{}

	template<typename T>
	float VECTOR2<T>::GetDistance()
	{ 
		return sqrtf( x * x + y * y ); 
	}

	template<typename T>
	float VECTOR2<T>::Normalize() 
	{
		float length = GetDistance();
		float inverseScale = 1.0f / length;
		x *= inverseScale;
		y *= inverseScale;

		return length;
	}

	typedef VECTOR2<float>	Vector2;
	typedef VECTOR2<double>	Vector2d;
	typedef VECTOR2<float>	Vector2f;
	typedef VECTOR2<int>	Vector2i;	
}

#endif