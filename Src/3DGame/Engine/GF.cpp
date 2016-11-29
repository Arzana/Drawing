/*void single_line(arg_pl t)
{
	int i = get<0>(t), j = get<1>(t);

	if ((hbuffer + i)->Clip() + (hbuffer + j)->Clip())
	{
		if (!flags.clip) return;
		Line *l = &Line(vrtxat(i), vrtxat(j));
		if (LineClip(l, port)) GF_Line(l);
	}
	else GF_Line(vrtxat(i), vrtxat(j));
}

void single_triangle(const int i, const int j, const int k)
{
	if ((hbuffer + i)->Clip() + (hbuffer + j)->Clip() + (hbuffer + k)->Clip())
	{
		if (!flags.clip) return;
		trgl *t = &trgl(vrtxat(i), vrtxat(j), vrtxat(k));
		struct poly { vrtx *v; int l; } p;

		if (p.v = TriangleClip(t, &p.l, port))
		{
			for (size_t l = 1, m = 2; l < p.l - 1; l++, m++)
			{
				GF_FullTrgl(p.v, p.v + l, p.v + m);
			}

			free(p.v);
		}
	}
	else GF_FullTrgl(vrtxat(i), vrtxat(j), vrtxat(k));
}

void GF_Line(const int x0, const int y0, const int z0, const Color c0, const int x1, const int y1, const int z1, const Color c1)
{
	int width = x1 - x0;
	int height = y1 - y0;

	int dx0 = width < 0 ? -1 : (width > 0 ? 1 : 0);
	int dy0 = height < 0 ? -1 : (height > 0 ? 1 : 0);
	int dx1 = dx0;
	int dy1 = 0;

	uint lng = abs(width);
	uint shrt = abs(height);
	if (lng <= shrt)
	{
		lng = shrt;
		shrt = abs(width);
		dy1 = height < 0 ? -1 : (height > 0 ? 1 : 0);
		dx1 = 0;
	}

	uint num = lng >> 1;
	uint x = x0;
	uint y = y0;

	get_zxy_plt;
	for (size_t i = 0; i < lng; i++)
	{
		float a = invLerp(0, lng, i);
		plot(x, y, lerp(z0, z1, a), Color::Lerp(c0, c1, a));
		num += shrt;
		if (num >= lng)
		{
			num -= lng;
			x += dx0;
			y += dy0;
		}
		else
		{
			x += dx1;
			y += dy1;
		}
	}
}

void GF_HLine(hline l)
{
	get_zxy_plt;

	if (l.x0 == l.x1)
	{
		if (l.z0 > l.z1) plot(l.x0, l.y, l.z0, l.c0);
		else plot(l.x1, l.y, l.z1, l.c1);
	}
	else if (l.x0 < l.x1)
	{
		for (float x = l.x0; x <= l.x1; ++x)
		{
			float a = invLerp(l.x0, l.x1, x);
			plot(x, l.y, lerp(l.z0, l.z1, a), Color::Lerp(l.c0, l.c1, a));
		}
	}
	else
	{
		for (float x = l.x1; x <= l.x0; ++x)
		{
			float a = invLerp(l.x1, l.x0, x);
			plot(x, l.y, lerp(l.z1, l.z0, a), Color::Lerp(l.c1, l.c0, a));
		}
	}
}

void GF_BFTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
	float invSlp0 = (v1->v.X - v0->v.X) / (v1->v.Y - v0->v.Y);
	float invSlp1 = (v2->v.X - v0->v.X) / (v2->v.Y - v0->v.Y);

	float x0 = v0->v.X;
	float x1 = v0->v.X;

	for (int y = v0->v.Y; y <= v1->v.Y; y++)
	{
		float a = invLerp(v0->v.Y, v1->v.Y, y);
		Color c0 = Color::Lerp(v0->c, v1->c, a);
		Color c1 = Color::Lerp(v0->c, v2->c, a);
		float z0 = lerp(v0->v.Z, v1->v.Z, a);
		float z1 = lerp(v0->v.Z, v2->v.Z, a);

		rndr_t.Add({ x0, x1, (float)y, z0, z1, c0, c1 });

		x0 += invSlp0;
		x1 += invSlp1;
	}
}

void GF_TFTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
	float invSlp0 = (v2->v.X - v0->v.X) / (v2->v.Y - v0->v.Y);
	float invSlp1 = (v2->v.X - v1->v.X) / (v2->v.Y - v1->v.Y);

	float x0 = v2->v.X;
	float x1 = v2->v.X;

	for (int y = v2->v.Y; y > v0->v.Y; y--)
	{
		float a = invLerp(v2->v.Y, v0->v.Y, y);
		Color c0 = Color::Lerp(v2->c, v0->c, a);
		Color c1 = Color::Lerp(v2->c, v1->c, a);
		float z0 = lerp(v2->v.Z, v0->v.Z, a);
		float z1 = lerp(v2->v.Z, v1->v.Z, a);

		rndr_t.Add({ x0, x1, (float)y, z0, z1, c0, c1 });

		x0 -= invSlp0;
		x1 -= invSlp1;
	}
}

void GF_FullTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
	Vertex vt0 = *v0;
	Vertex vt1 = *v1;
	Vertex vt2 = *v2;

	SortVerticesByY(&vt0, &vt1, &vt2);

	if (vt1.v.Y == vt2.v.Y) GF_BFTrgl(&vt0, &vt1, &vt2);
	else if (vt0.v.Y == vt2.v.Y) GF_TFTrgl(&vt0, &vt1, &vt2);
	else
	{
		float x = vt0.v.X + (vt1.v.Y - vt0.v.Y) / (vt2.v.Y - vt0.v.Y) * (vt2.v.X - vt0.v.X);
		float y = vt1.v.Y;
		float a = invLerp(vt0.v.Y, vt2.v.Y, y);
		float z = lerp(vt0.v.Z, vt2.v.Z, a);

		Vertex vt3 = Vertex(vect3(x, y, z), Color::Lerp(vt0.c, vt2.c, a));

		GF_BFTrgl(&vt0, &vt3, &vt1);
		GF_TFTrgl(&vt3, &vt1, &vt2);
	}
}*/