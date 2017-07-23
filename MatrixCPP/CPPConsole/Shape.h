#pragma once
class Shape
{
public:
	Shape();
	~Shape();
	virtual void draw();
	virtual int square();
	void setWidth(int width) {
		this->width = width;
	}
	void setHeight(int height) {
		this->height = height;
	}
protected:
	int width;
	int height;
	
};

