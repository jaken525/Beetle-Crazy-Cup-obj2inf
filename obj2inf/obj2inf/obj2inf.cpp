/*
#############################
	The project was made
			 by
		  JaKeN525
	     obj2inf for
      Beetle Crazy Cup
#############################

 -----------------------------
|   ###    ###    ###    ###  |
|  #   #  #   #  #   #  #   # |
|     #   #   #     #      #  |
|    #    #   #    #    #   # |
|  #####   ###   #####   ###  |
 -----------------------------
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

using namespace std;

int facesNum = 0;
int verNum = 0;
int tverNum = 0;
int norNum = 0;

float* vertices;
float* tvertices;
float* normals;

vector<string> polygons;

string writeShort(int num)
{
	int arr[] = { 0, 0 };
	while (num >= 256)
	{
		arr[1] += 1;
		num -= 256;
	}
	arr[0] = num;

	string hex;
	for (int i = 0; i < 2; i++)
		hex += char(arr[i]);

	return hex;
}

string writeLong(int num) 
{
	int arr[] = { 0, 0, 0, 0 };
	for (int i = 0; num >= 256; i++)
	{
		while (num >= 256) 
		{
			arr[i + 1] += 1;
			num -= 256;
		}
	}
	arr[0] = num;

	string hex = "";
	for (int i = 0; i < 4; i++) 
		hex += char(uint8_t(arr[i]));

	return hex;
}

string writeFloatLong(float num)
{
	string strHEX = "";
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

string changeSymbol(string str, char ch1, char ch2) 
{
	for (int i = 0; i < str.length(); i++)
		if (str[i] == ch1) str[i] = ch2;

	return str;
}

void writeINF() 
{
	// read
	string line;
	ifstream facesFile("Faces.txt");
	ifstream verFile("Vertex.txt");
	ifstream tverFile("Texture.txt");
	ifstream norFile("Normal.txt");

	while (getline(facesFile, line)) 
	{
		polygons.push_back(line);
		facesNum++;
	}
	while (getline(verFile, line))
		verNum++;
	while (getline(tverFile, line))
		tverNum++;
	while (getline(norFile, line))
		norNum++;

	verNum /= 3;
	facesNum /= 3;
	tverNum /= 2;
	norNum /= 3;

	norFile.close();
	facesFile.close();
	verFile.close();
	tverFile.close();

	// write
	ofstream infFile("test.inf", ios_base::binary);

	infFile << writeLong(tverNum) << writeLong(facesNum);
	infFile.close();
}

void writeVER() 
{
	//read
	vertices = new float[verNum * 3];
	tvertices = new float[tverNum * 2];

	ifstream vertexFile("Vertex.txt");
	ifstream textureFile("Texture.txt");
	string str = "";

	for (int i = 0; i < verNum; i++) 
	{
		getline(vertexFile, str);
		vertices[i * 3] = stof(changeSymbol(str, ',', '.'));
		getline(vertexFile, str);
		vertices[(i * 3) + 1] = stof(changeSymbol(str, ',', '.'));
		getline(vertexFile, str);
		vertices[(i * 3) + 2] = stof(changeSymbol(str, ',', '.'));
	}
	for (int i = 0; i < tverNum; i++)
	{
		getline(textureFile, str);
		tvertices[i * 2] = stof(changeSymbol(str, ',', '.'));
		getline(textureFile, str);
		tvertices[(i * 2) + 1] = stof(changeSymbol(str, ',', '.'));
	}
	vertexFile.close();

	//write
	ofstream file("test.ver", ios_base::binary);
	
	for (int i = 0; i < tverNum; i++)
		file << writeFloatLong(vertices[i * 3])
			<< writeFloatLong(vertices[(i * 3) + 1])
			<< writeFloatLong(vertices[(i * 3) + 2])
			<< writeFloatLong(tvertices[i * 2])
			<< writeFloatLong(tvertices[(i * 2) + 1]);
	file.close();
}

void writeTRI(int tex)
{	
	//read
	vector<int> verTriangles;
	vector<int> norTriangles;

	for (int i = 0; i < facesNum * 3; i++)
	{
		string tempFace = "";

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
	ofstream file("test.tri", ios_base::binary);

	for (int i = 0; i < facesNum; i++) 
		file << writeShort(verTriangles[i * 3]) << writeShort(norTriangles[i * 3])
			<< writeShort(verTriangles[(i * 3) + 2]) << writeShort(norTriangles[(i * 3) + 2])
			<< writeShort(verTriangles[(i * 3) + 1]) << writeShort(norTriangles[(i * 3) + 1])
			<< writeShort(tex) << writeShort(256);
	file.close();
}

void writeNOR() 
{
	//read
	normals = new float[norNum * 3];

	ifstream normalsFile("Normal.txt");
	string str = "";

	for (int i = 0; i < norNum; i++)
	{
		getline(normalsFile, str);
		normals[i * 3] = stof(changeSymbol(str, ',', '.'));
		getline(normalsFile, str);
		normals[(i * 3) + 1] = stof(changeSymbol(str, ',', '.'));
		getline(normalsFile, str);
		normals[(i * 3) + 2] = stof(changeSymbol(str, ',', '.'));
	}
	normalsFile.close();

	//write
	ofstream file("test.nor", ios_base::binary);

	for (int i = 0; i < norNum; i++)
		file << writeFloatLong(normals[i * 3])
		<< writeFloatLong(normals[(i * 3) + 1])
		<< writeFloatLong(normals[(i * 3) + 2]);
	file.close();
}

bool FileIsExist(string path)
{
	bool isExist = false;
	ifstream fin(path.c_str());

	if (fin.is_open())
		isExist = true;

	fin.close();
	return isExist;
}

void DeleteAllFiles() 
{
	delete[] normals;
	delete[] vertices;
	delete[] tvertices;

	remove("Vertex.txt");
	remove("Faces.txt");
	remove("Texture.txt");

	if (FileIsExist("Normal.txt"))
		remove("Normal.txt");
}

int main() 
{
	writeINF();
	writeVER();

	int texture;
	cout << "Enter texture number: ";
	cin >> texture;
	writeTRI(texture);

	if (FileIsExist("Normal.txt"))
		writeNOR();

	DeleteAllFiles();

	return 0;
}
