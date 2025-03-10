/*
	dprint - macro for debug print. Prints variables with their names.
	Sample usage:
		int x = 4;
		const char* s = "foo";
		dprint(x, s);
	Output:
		x = 4, s = foo
*/
#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>

#define slog std::cerr

#define EXPAND(x) x
#define _NARGS(a1,a2,a3,a4,a5,a6,a7,a8,N,...) N
#define NARGS(...) EXPAND(_NARGS(888,##__VA_ARGS__, 7,6,5,4,3,2,1,0))

#define PRINT_IMPL2(count, ...) EXPAND(dprint ## count (__VA_ARGS__))
#define PRINT_IMPL(count, ...) PRINT_IMPL2(count, __VA_ARGS__)
#define dprint(...) PRINT_IMPL(NARGS(__VA_ARGS__), __VA_ARGS__)

#define dprint1(a) { slog \
	<< #a" = " << a << std::endl; }

#define dprint2(a, b) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << std::endl; }

#define dprint3(a, b, c) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << ", " \
	<< #c" = " << c << std::endl; }

#define dprint4(a, b, c, d) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << ", " \
	<< #c" = " << c << ", " \
	<< #d" = " << d << std::endl; }

#define dprint5(a, b, c, d, e) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << ", " \
	<< #c" = " << c << ", " \
	<< #d" = " << d << ", " \
	<< #e" = " << e << std::endl; }

#define dprint6(a, b, c, d, e, f) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << ", " \
	<< #c" = " << c << ", " \
	<< #d" = " << d << ", " \
	<< #e" = " << e << ", " \
	<< #f" = " << f << std::endl; }

#define dprint7(a, b, c, d, e, f, g) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << ", " \
	<< #c" = " << c << ", " \
	<< #d" = " << d << ", " \
	<< #e" = " << e << ", " \
	<< #f" = " << f << ", " \
	<< #g" = " << g << std::endl; }

#define dprint8(a, b, c, d, e, f, g, h) { slog \
	<< #a" = " << a << ", " \
	<< #b" = " << b << ", " \
	<< #c" = " << c << ", " \
	<< #d" = " << d << ", " \
	<< #e" = " << e << ", " \
	<< #f" = " << f << ", " \
	<< #g" = " << g << ", " \
	<< #h" = " << h << std::endl; }

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& s, std::pair<T1,T2> const &a)
{
	s << "(" << a.first << ", " << a.second << ")";
	return s;
}

template<typename T>
std::ostream& operator<< (std::ostream& o, std::vector<T> const &x)
{
	o << "[" << x.size() << "] {";
	unsigned n=0;
	for(typename std::vector<T>::const_iterator it=x.begin(); it != x.end(); ++it)
		o << (n++?", ":"") << *it;
	o << "}";
	return o;
}

template<typename T>
std::ostream& operator<< (std::ostream& o, std::set<T> const &x)
{
	o << "[" << x.size() << "] {";
	unsigned n=0;
	for(typename std::set<T>::const_iterator it=x.begin(); it != x.end(); ++it)
		o << (n++?", ":"") << *it;
	o << "}";
	return o;
}

template <typename K, typename T, typename C, typename A>
std::ostream& operator<< (std::ostream& o, std::map<K,T,C,A> const &x)
{
	o << "[" << x.size() << "] {\n";
	for(typename std::map<K,T,C,A>::const_iterator it=x.begin(); it != x.end(); ++it)
		o << " " << *it << "\n";
	o << "}";
	return o;
}
