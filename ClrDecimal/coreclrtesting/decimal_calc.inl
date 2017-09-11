#pragma once

// overloaded assigned 
#undef COPYDEC
inline void COPYDEC(DECIMAL &to, const DECIMAL &from)
{
#define COPYDEC_IMPL 1
#if COPYDEC_IMPL == 1
	(to).signscale = (from).signscale; 
	(to).Hi32 = (from).Hi32;
	(to).Lo64 = (from).Lo64;
#elif  COPYDEC_IMPL == 2
	// More stores, but everything seems to be from registers
	(to).scale = (from).scale;
	(to).sign = (from).sign;
	(to).Hi32 = (from).Hi32;
	(to).Lo64 = (from).Lo64;
#endif
}

#undef DECIMAL_SETZERO
inline void DECIMAL_SETZERO(DECIMAL &dec)
{
	memset(&dec, 0, sizeof(DECIMAL));
}