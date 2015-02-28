#pragma once

struct interrupt {
	unsigned char master;
	unsigned char enable;
	unsigned char flags;
} extern interrupt;
