#pragma once

#include <stdexcept>
#include <iostream>
#include <string>

#define VECTOR4_RANGE_ERR "index must be in [0,4)"

namespace Utility {
	class Vector4
	{
	protected:
		float vector[4];

	public:

		static const int SIZE = 4;

		/*==============================*
		*         Constructors         *
		*==============================*/
		Vector4(float x, float y, float z, float w);
		Vector4();
		Vector4(float x, float y, float z);
		Vector4(const Vector4& v);
		/*==============================*
		*      Setters and Getters     *
		*==============================*/

		// Set
		void set(float x, float y, float z, float w);
		void set(float x, float y, float z);
		void set(int index, float value);

		// Get
		float get(int index) const;
		float& operator [](const int index);
		float operator [](const int index) const;

		// X, Y, Z, W
		float x() const;
		float y() const;
		float z() const;
		float w() const;

		/*==============================*
		*          Arithmetic          *
		*==============================*/

		// Add
		Vector4& operator+=(const Vector4& v);
		void add(const Vector4& v);
		// Subtract
		Vector4& operator-=(const Vector4& v);
		void subtract(const Vector4& v);
		// Multiply
		Vector4& operator*=(const float f);
		// Cross Product
		static Vector4 cross(Vector4 u, const Vector4& v);
		void cross(const Vector4& v);
		// Dot Product
		static float dot(const Vector4& u, const Vector4& v);
		float dot(const Vector4& u) const;
		// Angle Between
		static float angle(const Vector4& u, const Vector4& v);
		float angle(const Vector4& u) const;
		/*==============================*
		*           Utility            *
		*==============================*/
		static Vector4 dehomogenize(Vector4 v);
		void dehomogenize();
		static Vector4 normalize(Vector4 v);
		void normalize();
		// Negation
		Vector4 negate(Vector4 u);
		void negate();
		float length() const;
		float lengthSquared() const;
		std::string toString() const;
		void print() const;
		static Vector4 lerp(float f, Vector4 u, Vector4 v);
	};
	Vector4 operator+(Vector4 u, const Vector4& v);
	Vector4 operator-(Vector4 u, const Vector4& v);
	Vector4 operator*(Vector4 u, const float f);
	Vector4 operator-(Vector4 u);
	Vector4 normalize(Vector4 u);
}
