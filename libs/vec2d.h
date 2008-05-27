#ifndef POINT_H_
#define POINT_H_

#include "geometry.h"
#include <cmath>

template <typename T>
struct vec2d {
	
	T x, y;
	
	vec2d() {}
	
	vec2d(const vec2d& p): x(p.x), y(p.y) {}
	
	vec2d(const T& x, const T& y): x(x), y(y) {}
	
	T distans_sqr(const vec2d<T>& p) const { return sqr(x - p.x) + sqr(y - p.y); }
	
	T distans_sqr(const T& px, const T& py) { return sqr(x - px) + sqr(y - py); }
	
	T distans(const vec2d<T>& p) const { return sqrt(distans_sqr(p)); }
	
	T distans(const T& px, const T& py) { return sqrt(distans_sqr(px, py)); }
	
	T length_sqr() const { return x*x + y*y; }
	
	T length() const { return sqrt(length_sqr()); }
	
	void set(const T& x, const T& y) {
		this->x = x;
		this->y = y;
	}
	
	bool in_rect(const T& left, const T& top, const T& right, const T& bottom) const {
        return left <= x && x <= right && bottom <= y && y <= top;
	}
	
    bool operator==(const vec2d<T>& p) const { return p.x == x && p.y == y; }
	
    void move_by(const T& dx, const T& dy) {
    	x += dx;
    	y += dy;
    }
    
    vec2d<T> operator+(const vec2d<T>& to_add) const {
    	return vec2d<T>(x + to_add.x, y + to_add.y);
    }
    
    vec2d<T> operator-(const vec2d<T>& to_sub) const {
    	return vec2d<T>(x - to_sub.x, y - to_sub.y);
    }
    
    vec2d<T> operator*(const T& scale) const {
    	return vec2d<T>(x * scale, y * scale);
    }
    
    vec2d<T> operator/(const T& scalei) const {
    	return vec2d<T>(x / scalei, y / scalei);
    }
    
};

#endif /*POINT_H_*/
