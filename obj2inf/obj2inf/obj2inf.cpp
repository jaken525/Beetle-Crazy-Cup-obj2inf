/*
################################
The project was made by JaKeN525
obj2inf for
Beetle Crazy Cup/Kaefer Total/
Beetle Buggin'/Radical Drive/
Larry Ragland's 4x4 Challenge
################################

 ------------------------------
|   ###    ###    ###    ###   |
|  #   #  #   #  #   #  #   #  |
|     #   #   #     #      #   |
|    #    #   #    #    #   #  |
|  #####   ###   #####   ###   |
 ------------------------------
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <windows.h>
#include <shlwapi.h>
#include <vector>
#include <filesystem>
#include "parser.h"

namespace fs = std::filesystem;
using namespace obj;

int facesNum = 0;
int verNum = 0;
int tverNum = 0;
int norNum = 0;

std::vector<float> vertices;
std::vector<float> tvertices;
std::vector<float> normals;

std::vector<std::string> polygons;
std::string path = fs::current_path().string();

std::string openfilename(HWND owner = NULL, uint32_t flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY)
{
	std::string filename(MAX_PATH, '\0');
	OPENFILENAME ofn = { };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = "Wavefront Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = &filename[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = flags;

	if (!GetOpenFileName(&ofn))
		return "";
	return filename;
}

std::string getFilenameFile(const std::string str)
{
	size_t found;
	std::string strt;
	found = str.find_last_of("/\\");
	if (found < str.size())
	{
		strt = str.substr(found + 1, -1);
		found = strt.find(".");
		if (found < strt.size())
			strt = strt.substr(0, found);
	}
	else strt = str;
	size_t lastdot = strt.find_last_of(".");
	if (lastdot == std::string::npos) return strt;
	return strt.substr(0, lastdot);
}

std::string writeShort(int num)
{
	int arr[] = { 0, 0 };
	while (num >= 256)
	{
		arr[1] += 1;
		num -= 256;
	}
	arr[0] = num;

	std::string hex;
	for (int i = 0; i < 2; i++)
		hex += char(arr[i]);

	return hex;
}

std::string writeLong(int num)
{
	std::string Hex = "";
	std::stringstream s;
	s << std::hex << num;

	int zeroes = 8 - size(s.str());
	for (int i = 0; i < zeroes; i++)
		Hex += "0";
	Hex += s.str();

	int arr[] = { 0, 0, 0, 0 };
	int c = 3;
	for (int i = 0; i < Hex.length() - 1; i += 2)
	{
		std::stringstream h;
		h << Hex[i] << Hex[i + 1];
		h >> std::hex >> arr[c];
		c--;
	}

	Hex = "";
	for (int i = 0; i < 4; i++)
		Hex += char(uint8_t(arr[i]));

	return Hex;
}

std::string writeFloatLong(float num)
{
	std::string strHEX = "";
	unsigned long a = 0;
	unsigned long a24 = 0;
	float n = num;

	memcpy(&a, &n, 4);
	strHEX += char(uint8_t(a));
	strHEX += char((uint16_t(a) - uint8_t(a)) / 0x00000100);

	memcpy(&a24, &n, 3);
	strHEX += char((a24 - int(uint16_t(a))) / 0x00010000);
	strHEX += char((a - a24) / 0x01000000);

	return strHEX;
}

std::string changeSymbol(std::string str, char ch1, char ch2)
{
	for (int i = 0; i < str.length(); i++)
		if (str[i] == ch1) str[i] = ch2;

	return str;
}

void writeINF(ObjParser obj, std::string fileName) 
{
	// read
	polygons = obj.GetFaces();
	facesNum = obj.GetFacesNumber();
	verNum = obj.GetVerticiesNumber();
	tverNum = obj.GetTextureVerticiesNumber();
	norNum = obj.GetNormalsNumber();;

	// write
	std::ofstream infFile(path + "\\Converted\\" + fileName + ".inf", std::ios_base::binary);

	infFile << writeLong(tverNum) << writeLong(facesNum);
	infFile.close();
}

void writeVER(ObjParser obj, std::string fileName)
{
	//read
	vertices = obj.GetVerticies();
	tvertices = obj.GetTextureVerticies();

	//write
	std::ofstream file(path + "\\Converted\\" + fileName + ".ver", std::ios_base::binary);
	
	for (int i = 0; i < tverNum; i++)
		file << writeFloatLong(vertices[i * 3])
			<< writeFloatLong(vertices[(i * 3) + 1])
			<< writeFloatLong(vertices[(i * 3) + 2])
			<< writeFloatLong(tvertices[i * 2])
			<< writeFloatLong(tvertices[(i * 2) + 1]);
	file.close();
}

void writeTRI(int tex, std::string fileName)
{	
	//read
	std::vector<int> verTriangles;
	std::vector<int> norTriangles;

	for (int i = 0; i < facesNum * 3; i++)
	{
		std::string tempFace = "";

		for (int k = 0; k < size(polygons[i]); k++)
		{
			if (polygons[i][k] != '/')
				tempFace += polygons[i][k];
			else
			{
				verTriangles.push_back(stoi(tempFace) - 1);

				tempFace = "";
				for (int j = k + 1; j <= size(polygons[i]); j++)
					tempFace += polygons[i][j];

				norTriangles.push_back(stoi(tempFace) - 1);
				tempFace = "";

				break;
			}
		}
	}

	//write
	std::ofstream file(path + "\\Converted\\" + fileName + ".tri", std::ios_base::binary);

	for (int i = 0; i < facesNum; i++) 
		file << writeShort(verTriangles[i * 3]) << writeShort(norTriangles[i * 3])
			<< writeShort(verTriangles[(i * 3) + 2]) << writeShort(norTriangles[(i * 3) + 2])
			<< writeShort(verTriangles[(i * 3) + 1]) << writeShort(norTriangles[(i * 3) + 1])
			<< writeShort(tex) << writeShort(256);
	file.close();
}

void writeNOR(ObjParser obj, std::string fileName)
{
	//write
	std::ofstream file(path + "\\Converted\\" + fileName + ".nor", std::ios_base::binary);

	for (int i = 0; i < norNum; i++)
		file << writeFloatLong(normals[i * 3])
		<< writeFloatLong(normals[(i * 3) + 1])
		<< writeFloatLong(normals[(i * 3) + 2]);
	file.close();
}

bool FileIsExist(std::string path)
{
	bool isExist = false;
	std::ifstream fin(path.c_str());

	if (fin.is_open())
		isExist = true;

	fin.close();
	return isExist;
}

void ClearArrays() 
{
	normals.clear();
	vertices.clear();
	tvertices.clear();
}

int main() 
{
	ClearArrays();
	std::string fileName;
	std::string filePath;

	filePath = openfilename();
	fileName = getFilenameFile(filePath);

	if (!fs::exists(path + "\\Converted"))
		fs::create_directory(path + "\\Converted");

	if (fs::exists(filePath))
	{
		ObjParser obj;
		obj.parse(filePath);

		writeINF(obj, fileName);
		writeVER(obj, fileName);

		int texture;
		std::cout << "Enter texture number: ";
		std::cin >> texture;

		writeTRI(texture, fileName);

		normals = obj.GetNormals();
		if (normals.size() != NULL)
			writeNOR(obj, fileName);

		ClearArrays();
		std::cout << " Model converted successfully!" << std::endl;
	}
	else
		std::cout << " Unable to open the file." << std::endl;

	system("pause");
	return 0;
}