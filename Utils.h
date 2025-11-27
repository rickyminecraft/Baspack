#pragma once

enum class Direction : char
{
	Big = 0,
	Small
};

static union Donnees
{
	uint8_t Data_char[4];
	uint16_t Data_short[2];
	uint32_t Data_int;
	float Data_float;
} datas;