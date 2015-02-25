#pragma once

#define FLAGS_ZERO 0x80
#define FLAGS_NEGATIVE 0x40
#define FLAGS_HALFCARRY 0x20
#define FLAGS_CARRY 0x10

#define FLAGS_ISZERO		(registers.f & FLAGS_ZERO)
#define FLAGS_ISNEGATIVE	(registers.f & FLAGS_NEGATIVE)
#define FLAGS_ISCARRY		(registers.f & FLAGS_CARRY)
#define FLAGS_ISHALFCARRY	(registers.f & FLAGS_HALFCARRY)

#define FLAGS_ISSET(x)	(registers.f & (x))
#define FLAGS_SET(x)	(registers.f |= (x))
#define FLAGS_CLEAR(x)	(registers.f &= ~(x))

extern const unsigned char instructionTicks[256];

void cpuStep(void);
