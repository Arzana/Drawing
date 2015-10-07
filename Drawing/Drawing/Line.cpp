#include "Line.h"
#include <vector>

typedef int OutCode;
const OutCode INSIDE =	0;	//0000
const OutCode LEFT =	1;	//0001
const OutCode RIGHT =	2;	//0010
const OutCode BOTTOM =	4;	//0100
const OutCode TOP =		8;	//1000

Line::Line(const Vector3& a, const Vector3& b)
{
	A = Vector3(a);
	B = Vector3(b);
}

Line::Line(float x0, float y0, float x1, float y1)
{
	A = Vector3(x0, y0, 0);
	B = Vector3(x1, y1, 0);
}

OutCode Line::ComputeOutCode(float x, float y)
{
	OutCode code = INSIDE;						// Initialised as being inside of the clip window.

	if (x < 0) code |= LEFT;					// To the left of the clip window.
	else if (x > WINDOW_WIDTH) code |= RIGHT;	// To the right of the clip window.
	if (y < 0) code |= BOTTOM;					// Below the clip window. (Actual top see CohenSutherLandLineClip(Line&) function).
	else if (y > WINDOW_HEIGHT) code |= TOP;	// Above the clip window. (Actual bottom see CohenSutherLandLineClip(Line&) function).

	return code;
}

bool Line::CohenSutherlandLineClip(Line& l)
{
	/*	SDL flips the window so that the top left is 0,0 and the bottom right max, max.
		This means that where we check for clipping the top is actualy the bottom and the bottom is the top.
		Left and right remain unchanged. */

	float x0 = l.A.X, x1 = l.B.X, y0 = l.A.Y, y1 = l.B.Y;
	OutCode outcode0 = ComputeOutCode(x0, y0);
	OutCode outcode1 = ComputeOutCode(x1, y1);
	bool accept = false;

	while (true)
	{
		if (!(outcode0 | outcode1))				// Both point are inside the clipping rectangle.
		{
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) break;	// Both points are outside the clipping rectangle.
		else
		{
			float x, y;
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			/*	Find the intersection point;
				use formulas:
				y = y0 + slope * (x - x0),
				x = x0 + (1 / slope) * (y - y0) */
			if (outcodeOut & TOP)			// Point is above the clip rectangle.
			{
				x = x0 + (x1 - x0) * (WINDOW_HEIGHT - y0) / (y1 - y0);
				y = WINDOW_HEIGHT - 1;
			}
			else if (outcodeOut & BOTTOM)	// Point is blow the clip rectangle.
			{
				x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
				y = 0;
			}
			else if (outcodeOut & RIGHT)	// Point is to the right if the clip rectangle.
			{
				y = y0 + (y1 - y0) * (WINDOW_WIDTH - x0) / (x1 - x0);
				x = WINDOW_WIDTH - 1;
			}
			else if (outcodeOut & LEFT)		// Point is to the left of the clip rectangle.
			{
				y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
				x = 0;
			}

			if (outcodeOut == outcode0)
			{
				l.A.X = x0 = x;
				l.A.Y = y0 = y;
				outcode0 = ComputeOutCode(x0, y0);
			}
			else
			{
				l.B.X = x1 = x;
				l.B.Y = y1 = y;
				outcode1 = ComputeOutCode(x1, y1);
			}
		}
	}

	return accept;
}

int Line::CheckVisiblity(Line* lines, int length)
{
	for (int i = 0; i < length;)
	{
		if (!CohenSutherlandLineClip(lines[i]))									// The line is not visible.
		{
			if (--length == 0) return 0;

			lines[i] = lines[length];								
			memmove(lines, lines, length * sizeof(lines));						// Shift the array 1 position to the left.
	
			continue;
		}
	
		i++;
	}
	
	return length;
}

float Line::Get2DLength() const
{
	float diffX = fmax(A.X, B.X) - fmin(A.X, B.X);
	float diffY = fmax(A.Y, B.Y) - fmin(A.Y, B.Y);

	return sqrtf(square(diffX) + square(diffY));
}