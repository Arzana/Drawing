#include <cstdarg>
#include <string.h>
#include "Geon.h"

Geon::Geon(size_t len, ...)
	: trglCount(len)
{
	trgls = malloc_s(trgl, len);

	va_list ap;
	va_start(ap, len);
	for (size_t i = 0; i < len; i++)
	{
		trgls[i] = va_arg(ap, trgl);
	}
	va_end(ap);
}

Geon::~Geon(void)
{
	free_s_check(trgls);
}

Geon::Geon(const Geon * g0, const Geon * g1)
	: trglCount(g0->trglCount + g1->trglCount)
{
	int len = g0->trglCount + g1->trglCount;
	if (len > 0)
	{
		trgls = malloc_s(trgl, len);
		memcpy(trgls, g0->trgls, g0->trglCount);
		memcpy(trgls + g0->trglCount, g1->trgls, g1->trglCount);
	}
	else trgls = NULL;
}
