#include "../includes/Scop.h"

int		mstrchr(char *s, int a)
{
	int	ret;

	ret = 0;
	while (s && *(s + ret) && *(s + ret) != (char)a )
		++ret;
	return (ret);
}