#pragma once

#define DLLEXPORT __declspec(dllexport)

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

