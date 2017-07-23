#pragma once
#include "Shape.h"
#include <iostream>
using namespace std;

class Recangle :public Shape
{
public:
	Recangle();
	~Recangle();
	void draw() {
		cout << "Ve hinh tam giac";
	}
	int square() {
		return (width * height);
	}
};

