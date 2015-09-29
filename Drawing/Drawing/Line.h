#ifndef LINE_H
#define LINE_H

#include "Matrix.h"
#include "UtilDefs.h"

struct Line
{
public:
	Vector3 A;
	Vector3 B;

	void operator *=(const Matrix& m);

	Line(const Vector3& a, const Vector3& b);

	/* Multiplies the given line pointers by the givven specified matrix. */
	static void Multiply(const Matrix& m, int length, Line* lines);
	/*	Check is the lines are visible to the screen and change or remove them is they are not. 
		Returns the new length of the line pointer. */
	static int CheckVisiblity(Line* lines, int length);
	/* Cohen-Sutherland clipping algorithm clips a line from P0 = (x0, y0) to P1 = (x1, y1) agains a rectangle with diagonal from (xMin, YMin) to (xMax, yMax). */
	static bool CohenSutherlandLineClip(Line& l);
	/* Get the length of the line. */
	float Get2DLength() const;
private:
	/* Compute the bit code for a point (x, y) using clip rectangle bounded diagonally by (xMin, yMin), and (yMax, yMax). */
	static int ComputeOutCode(float x, float y);
};

#endif