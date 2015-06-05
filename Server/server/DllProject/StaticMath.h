#pragma once

#define DLLEXPORT __declspec(dllexport)

DLLEXPORT double Test2(double a);

#ifndef LIB_H
#define LIB_H
extern "C" int __declspec(dllexport)add2(int x, int y);
#endif

/* ÎÄ¼þÃû£ºlib.cpp¡¡*/




class StaticMath
{
public:
	DLLEXPORT StaticMath();
	DLLEXPORT ~StaticMath();

	static DLLEXPORT double add(double a, double b);
	static DLLEXPORT double sub(double a, double b);
	static DLLEXPORT double mul(double a, double b);
	static DLLEXPORT double div(double a, double b);

};

