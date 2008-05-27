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

/**
 * Conver object from one type to another using std::stringstream.
 * @param arg to convert
 * @return converted
 */
template <typename dst, typename src>
inline dst cast(const src& arg) {
	std::stringstream s;
	s << arg;
	dst result;
	s >> result;
	return result;
}

#endif /*UTILS_H_*/
