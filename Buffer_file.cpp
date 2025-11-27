#include "Buffer_file.h"

void Buffer_file::Load_buffer(std::ifstream *file) noexcept
{
	Buffer_file::Size = Get_Filesize(file);
	Buffer_file::Buffer.resize(Buffer_file::Size);
	file->read(Buffer_file::Buffer.data(), Buffer_file::Size);
}

uint8_t Buffer_file::Read_char() noexcept
{
	if (Position < Buffer_file::Size)
	{
		return Buffer_file::Buffer[Buffer_file::Position++];
	}
	else
	{
		Buffer_file::Erreur = true;
		return  0;
	}
}

uint16_t Buffer_file::Read_short(const Direction Indian) noexcept
{
	uint16_t Return_value = 0;
	datas.Data_char[0] = Read_char();
	datas.Data_char[1] = Read_char();

	switch (Indian)
	{
	case Direction::Big: // ->
		Return_value = datas.Data_short[0];
		break;
	case Direction::Small: // <-
		uint8_t temp = {};
		temp = datas.Data_char[0];
		datas.Data_char[0] = datas.Data_char[1];
		datas.Data_char[1] = temp;
		Return_value = datas.Data_short[0];
		break;
	}
	return Return_value;
}

uint32_t Buffer_file::Read_int(const Direction Indian) noexcept
{
	uint32_t Return_value = 0;
	datas.Data_char[0] = Read_char();
	datas.Data_char[1] = Read_char();
	datas.Data_char[2] = Read_char();
	datas.Data_char[3] = Read_char();

	switch (Indian)
	{
	case Direction::Big:
		Return_value = datas.Data_int;
		break;
	case Direction::Small:
		uint8_t temp = {}, temp1 = {};
		temp = datas.Data_char[0];
		temp1 = datas.Data_char[1];
		datas.Data_char[0] = datas.Data_char[3];
		datas.Data_char[1] = datas.Data_char[2];
		datas.Data_char[2] = temp1;
		datas.Data_char[3] = temp;
		Return_value = datas.Data_int;
		break;
	}
	return Return_value;
}

float Buffer_file::Read_float(const Direction Indian) noexcept
{
	float Return_value = 0.0f;
	datas.Data_char[0] = Read_char();
	datas.Data_char[1] = Read_char();
	datas.Data_char[2] = Read_char();
	datas.Data_char[3] = Read_char();

	switch (Indian)
	{
	case Direction::Big:
		Return_value = datas.Data_float;
		break;
	case Direction::Small:
		char temp = {}, temp1 = {};
		temp = datas.Data_char[0];
		temp1 = datas.Data_char[1];
		datas.Data_char[0] = datas.Data_char[3];
		datas.Data_char[1] = datas.Data_char[2];
		datas.Data_char[2] = temp1;
		datas.Data_char[3] = temp;
		Return_value = datas.Data_float;
		break;
	}
	return Return_value;
}

void Buffer_file::Read_struct(void * Structure, const int Size) noexcept
{
	uint8_t* Structure_data = (uint8_t*)Structure;
	for (auto Boucle = 0; Boucle < Size; Boucle++)
	{
		Structure_data[Boucle] = Read_char();
	}
}

void Buffer_file::Seek(const int Position) noexcept
{
	if (Position < Buffer_file::Size)
	{
		Buffer_file::Position = Position;
	}
	else
	{
		Buffer_file::Erreur = true;
	}
}

inline uint32_t Buffer_file::GetSize() noexcept
{
	return Buffer_file::Size;
}

bool Buffer_file::GetErreur(void) noexcept
{
	return Buffer_file::Erreur;
}

char* Buffer_file::GetDirectDataAccess()
{
	return Buffer_file::Buffer.data();
}

uint32_t Buffer_file::Get_Filesize(std::ifstream *file) noexcept
{
	file->seekg(0, file->end);
	std::streamoff File_size = file->tellg();
	file->seekg(0);
	return (uint32_t)File_size;
}