#pragma once
#include <vector>
#include <fstream>
#include "Utils.h"

class Buffer_file
{
public:
	void Load_buffer(std::ifstream *file) noexcept;
	uint8_t Read_char() noexcept;
	uint16_t Read_short(const Direction Indian = Direction::Big) noexcept;
	uint32_t Read_int(const Direction Indian = Direction::Big) noexcept;
	float Read_float(const Direction Indian = Direction::Big) noexcept;

	void Read_struct(void *Structure, const int Size) noexcept;
	void Seek(const int Position) noexcept;
	uint32_t GetSize() noexcept;

	bool GetErreur(void) noexcept;

	char* GetDirectDataAccess();
	
private:
	uint32_t Get_Filesize(std::ifstream *file) noexcept;

	uint32_t Position = 0;
	uint32_t Size = 0;
	std::vector<char> Buffer;
	bool Erreur = false;
};

