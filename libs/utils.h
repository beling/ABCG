#ifndef UTILS_H_
#define UTILS_H_

template <typename T>
inline T& bound(T& value, T& min, T& max) {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

template <typename T>
inline const T& bound(const T& value, const T& min, const T& max) {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

#endif /*UTILS_H_*/
