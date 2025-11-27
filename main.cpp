#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <filesystem>
#include "Buffer_file.h"
#include "Utils.h"

//utilisé pour ignorer l'alignement sur les structures
#pragma pack(1)
struct Header
{
	uint8_t FileHeader[7] = {};
	uint32_t Files_pos = 0;
	uint32_t Size = 0;
};

struct File_Node
{
	char File_name[255] = {};
	uint32_t Files_start = 0;
	uint32_t Size = 0;
};

uint8_t FileID[8] = "BASPACK";
uint16_t Taille_noeud = 263; //0x107

int main()
{
	Buffer_file Buffer;
	std::string File_Name = "data.pak";
	std::ifstream File(File_Name, std::ifstream::binary);
	if (!File)
	{
		std::cerr << "file not found" << std::endl;
		exit(EXIT_FAILURE);
	}

	Buffer.Load_buffer(&File);
	File.close();

	Header File_Header;
	File_Node FileNode;

	//recupere le header
	Buffer.Read_struct(&File_Header, sizeof(File_Header));

	//comparaison a faire pour confirmer que le fichier est du bon format
	//if (File_Header.FileHeader != FileID)
	//{
	//	exit(EXIT_FAILURE);
	//}

	Buffer.Seek(File_Header.Files_pos);

	std::string Nom;
	std::string Nom_a_traiter;
	uint16_t Nombre = 0;

	std::string Path = "e:";
	std::string Directory = "datapack";

	//on declare sur quel disque on veut extraire les données
	std::filesystem::current_path(Path);
	
	while (true)
	{
		Buffer.Read_struct(&FileNode, sizeof(FileNode));

		//on convertit l'arborescence en string, plus facile a traiter
		Nom_a_traiter = (char*)FileNode.File_name;

		//on recupere que la partie repertoire en trouvant la derniere occurence du separateur de repertoire
		uint8_t P = Nom_a_traiter.find_last_of(0x5c);
		Nom_a_traiter = Nom_a_traiter.substr(0,P);

		//et on crée les repertoires
		std::filesystem::create_directories(Directory + Nom_a_traiter);

		//on lit la position de depart du fichier dans le gros fichier
		//FileNode.Files_start = Buffer.Read_int();
		//et sa taille
		//FileNode.Size = Buffer.Read_int();

		Nom = Path + "\\" + Directory;

		Nom.append((char*)FileNode.File_name);

		//on demande un accès direct aux données
		char* DataAccess = Buffer.GetDirectDataAccess();
		DataAccess += FileNode.Files_start;
		//ici on boucle sur les données pour les xorer en mémoire
		//rapide a faire
		uint32_t boucle = FileNode.Size;
		while (boucle > 0)
		{
			*DataAccess ^= 0x37;
			DataAccess++;
			boucle--;
		}
		//ici on remet la bonne adresse de départ en memoire
		DataAccess = Buffer.GetDirectDataAccess();
		DataAccess += FileNode.Files_start;

		std::ofstream Output_file(Nom, std::ifstream::binary);
		if (!Output_file)
		{
			std::cerr << "no output file" << std::endl;
			exit(EXIT_FAILURE);
		}

		//on ecrit d'un coup dans le fichier les données
		Output_file.write(DataAccess, FileNode.Size);

		Output_file.close();

		std::cout << Nombre;
		std::cout << std::endl;

		Nombre++;

		uint32_t Next_node = Taille_noeud * (Nombre);

		Buffer.Seek(File_Header.Files_pos+ Next_node);

		//si on est au bout, on quitte
		if (Buffer.GetErreur())
		{
			break;
		}
	}
	return EXIT_SUCCESS;
}