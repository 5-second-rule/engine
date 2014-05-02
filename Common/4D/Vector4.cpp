#include "Vector4.h"

using namespace Utility;
Vector4::Vector4(float x, float y, float z, float w) {
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
	this->vector[3] = w;
}

Vector4::Vector4() : Vector4::Vector4(0.0f, 0.0f, 0.0f, 0.0f) {}

Vector4::Vector4(float x, float y, float z) : Vector4(x, y, z, 0.0f) {}

Vector4::Vector4(const Vector4& v) : Vector4(v[0], v[1], v[2], v[3]) {}

/*==============================*
*      Setters and Getters     *
*==============================*/

// Set

void Vector4::set(float x, float y, float z, float w) {
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
	this->vector[3] = w;
}

void Vector4::set(float x, float y, float z) {
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
}

void Vector4::set(int index, float value) {
	if (index < 0 || index >= SIZE) throw std::out_of_range(VECTOR4_RANGE_ERR);
	else this->vector[index] = value;
}

// Get

float Vector4::get(int index) const {
	if (index < 0 || index >= SIZE) throw std::out_of_range(VECTOR4_RANGE_ERR);
	else return this->vector[index];
}

float& Vector4::operator [](const int index) {
	if (index < 0 || index >= SIZE) throw std::out_of_range(VECTOR4_RANGE_ERR);
	else return this->vector[index];
}

float Vector4::operator [](const int index) const {
	return this->get(index);
}

// X, Y, Z, W

float Vector4::x() const {
	return this->vector[0];
}

float Vector4::y() const {
	return this->vector[1];
}

float Vector4::z() const {
	return this->vector[2];
}

float Vector4::w() const {
	return this->vector[3];
}

/*==============================*
*          Arithmetic          *
*==============================*/

// Add

Vector4& Vector4::operator+=(const Vector4& v) {
	this->vector[0] += v[0];
	this->vector[1] += v[1];
	this->vector[2] += v[2];
	this->vector[3] += v[3];

	return *this;
}

void Vector4::add(const Vector4& v) {
	(*this) += v;
}

// Subtract

Vector4& Vector4::operator-=(const Vector4& v) {
	this->vector[0] -= v[0];
	this->vector[1] -= v[1];
	this->vector[2] -= v[2];
	this->vector[3] -= v[3];

	return *this;
}

void Vector4::subtract(const Vector4& v) {
	(*this) -= v;
}

// Multiply

Vector4& Vector4::operator*=(const float f) {
	this->vector[0] *= f;
	this->vector[1] *= f;
	this->vector[2] *= f;

	return *this;
}

// Cross Product

static Vector4 cross(Vector4 u, const Vector4& v) {
	u.cross(v);
	return u;
}

void Vector4::cross(const Vector4& v) {
	//calculate cross product
	float s1 = this->vector[1] * v[2] - this->vector[2] * v[1];
	float s2 = this->vector[2] * v[0] - this->vector[0] * v[2];
	float s3 = this->vector[0] * v[1] - this->vector[1] * v[0];

	//clobber
	this->vector[0] = s1;
	this->vector[1] = s2;
	this->vector[2] = s3;
}

// Dot Product

static float dot(const Vector4& u, const Vector4& v) {
	return u.dot(v);
}

float Vector4::dot(const Vector4& u) const {
	return this->vector[0] * u.vector[0] + this->vector[1] * u.vector[1] + this->vector[2] * u.vector[2];
}

// Angle Between

static float angle(const Vector4& u, const Vector4& v) {
	return u.angle(v);
}

float Vector4::angle(const Vector4& u) const {
	return acos(this->dot(u) / (this->length() * u.length()));
}


/*==============================*
*           Utility            *
*==============================*/

static Vector4 dehomogenize(Vector4 v) {
	v.dehomogenize();
	return v;
}

void Vector4::dehomogenize() {
	float factor = this->vector[3];
	this->vector[0] /= factor;
	this->vector[1] /= factor;
	this->vector[2] /= factor;
	this->vector[3] /= factor;
}


static Vector4 normalize(Vector4 v) {
	v.normalize();
	return v;
}

void Vector4::normalize() {
	float mag = this->length();
	if (mag != 0) {
		this->vector[0] /= mag;
		this->vector[1] /= mag;
		this->vector[2] /= mag;
	}
}

// Negation

Vector4 Vector4::negate(Vector4 u) {
	u.negate();
	return u;
}

void Vector4::negate() {
	*this *= -1.0f;
}

float Vector4::length() const {
	return sqrt(this->lengthSquared());
}

float Vector4::lengthSquared() const {
	return this->dot(*this);
}


void Vector4::print() const {
	std::cout << "<" <<
		this->vector[0] << ", " <<
		this->vector[1] << ", " <<
		this->vector[2] << ", " <<
		this->vector[3] << ">" << std::endl;
}

static Vector4 lerp(float f, Vector4 u, Vector4 v) {
	v -= u;
	v *= f;
	u += v;
	return u;
}

Vector4 Utility::operator+(Vector4 u, const Vector4& v) {
	u += v;
	return u;
}

Vector4 Utility::operator-(Vector4 u, const Vector4& v) {
	u -= v;
	return u;
}

Vector4 Utility::operator*(Vector4 u, const float f) {
	u *= f;
	return u;
}

Vector4 Utility::operator-(Vector4 u) {
	u.negate();
	return u;
}

Vector4 Utility::normalize(Vector4 u){
	u.normalize();
	return u;
}
