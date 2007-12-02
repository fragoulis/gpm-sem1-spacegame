#pragma once

/** 
 * Color taken directly from the tutorials
 */
class Color {
private:
	float _rgba[4];

public:
	void Assign(float r, float g, float b, float a);
    void Assign(float rgba[]);
    void alpha( float a ) { _rgba[3] = a; }

	Color();

	Color(float r, float g, float b, float a);

	Color(float rgba[]);

	const float* rgba() const;

	static Color black();

	static Color white();

	static Color red();

	static Color green();

	static Color blue();

	static Color yellow();

	static Color null();
};