/*
 * new.cpp
 *
 *  Created on: 21 июля 2015 г.
 *      Author: kap
 */

#include "safe_malloc.h"

/*
void __attribute__((constructor(0))) __CCM_Init()
{
	extern char _eidata, _sccmram, _eccmram;
	const char *src = &_eidata;
	char *dst = &_sccmram;

	while (dst < &_eccmram)  *dst++ = *src++;
}
*/

void * operator new (size_t size)
{
	void *p = safe_malloc_ram(size);
	if (p == nullptr) p = safe_malloc_exram(size);
	if (p == nullptr) FWTHROW(Result_OutOfMemory);
    return p;
} // p's default ctor implicitly called here



void operator delete   (void * ptr, std::size_t) { safe_free(ptr); }
void operator delete[](void * ptr, std::size_t) { safe_free(ptr); }


void operator delete (void * p)
{
    safe_free(p);
} // p's default dtor implicitly called here

void * operator new[](size_t size)
{
    void *p = safe_malloc_ram(size);
	if (p == nullptr) p = safe_malloc_exram(size);
	if (p == nullptr) FWTHROW(Result_OutOfMemory);
    return p;
}

void operator delete[](void * p)
{
    safe_free(p);
}
