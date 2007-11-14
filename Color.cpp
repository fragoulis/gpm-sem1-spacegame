#include "Color.h"

void Color::Assign( float rgba[] ) {
	_rgba[0] = rgba[0];
	_rgba[1] = rgba[1];
	_rgba[2] = rgba[2];
	_rgba[3] = rgba[3];
}

void Color::Assign(float r, float g, float b, float a) {
	_rgba[0] = r;
	_rgba[1] = g;
	_rgba[2] = b;
	_rgba[3] = a;
}

Color::Color() { 
	Assign(0.0, 0.0, 0.0, 0.0);
}

Color::Color(float r, float g, float b, float a) {
	Assign(r, g, b, a);
}

Color::Color(float rgba[]) {
	Assign(rgba[0], rgba[1], rgba[2], rgba[3]);
}

const float* Color::rgba() const {
	return _rgba;
}

Color Color::black() {
	return Color(0.0, 0.0, 0.0, 1.0);
}

Color Color::white() {
	return Color(1.0, 1.0, 1.0, 1.0);
}

Color Color::red() {
	return Color(1.0, 0.0, 0.0, 1.0);
}

Color Color::green() {
	return Color(0.0, 1.0, 0.0, 1.0);
}

Color Color::blue() {
	return Color(0.0, 0.0, 1.0, 1.0);
}

Color Color::yellow() {
	return Color(1.0, 1.0, 0.0, 1.0);
}

Color Color::null() {
	return Color(0.0, 0.0, 0.0, 0.0);
}
