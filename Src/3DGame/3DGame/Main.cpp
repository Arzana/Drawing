#include "Main.h"

GameWindow *window;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", WIDTH, HEIGHT);
	window->Draw = Render;
	GF_SetWindow(window);
	window->Run();

	delete window;
	printf("Press any key to continue...");
	getchar();
	return EXIT_SUCCESS;
}

void Render(void)
{
	Vect3 v0 = Vect3(0, -0.5, -2);
	Vect3 v1 = Vect3(-0.5, 0.5, -2);
	Vect3 v2 = Vect3(0.5, 0.5, -2);

	Mtrx4 world = MTRX4_IDENTITY;
	Mtrx4 view = Mtrx4::CreateView(&VECT3_ZERO, &VECT3_FORWARD, &VECT3_UP);
	Mtrx4 pers = Mtrx4::CreateFrustrum(FOV_Y, aspr, DEPTH_NEAR, DEPTH_FAR);
	Mtrx4 mvp = world * view * pers;

	Vect4 v0h = mvp * v0;
	Vect4 v1h = mvp * v1;
	Vect4 v2h = mvp * v2;

	bool clipV0 = v0h.Clip();
	bool clipV1 = v1h.Clip();
	bool clipV2 = v2h.Clip();

	if (!(clipV0 || clipV1 || clipV2))
	{
		GF_StartRender(GF_TRIANGLES);
		GF_SetBufferLength(3);

		Vect3 v0c = v0h.ToNDC();
		Vect3 v1c = v1h.ToNDC();
		Vect3 v2c = v2h.ToNDC();

		Vect3 v0s = ToScreen(&v0c);
		Vect3 v1s = ToScreen(&v1c);
		Vect3 v2s = ToScreen(&v2c);

		GF_AddPoint(v0s, CLR_BLUE);
		GF_AddPoint(v1s, CLR_RED);
		GF_AddPoint(v2s, CLR_GREEN);

		GF_EndRender();
	}
}

Vect3 ToScreen(const Vect3 *ndc)
{
	float x = WIDTH / 2 * ndc->X + WIDTH / 2;
	float y = HEIGHT / 2 * ndc->Y + HEIGHT / 2;
	float z = (DEPTH_FAR - DEPTH_NEAR) / 2 * ndc->Z + (DEPTH_FAR + DEPTH_NEAR) / 2;
	return Vect3(x, y, z);
}