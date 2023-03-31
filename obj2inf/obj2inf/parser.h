#include <iostream>
#include <vector>
#include <string>
#include <fstream>

namespace obj 
{
	class ObjParser 
	{
	public:
		std::vector<float> vx;
		std::vector<float> vy;
		std::vector<float> vz;
		std::vector<float> vnx;
		std::vector<float> vny;
		std::vector<float> vnz;
		std::vector<float> vtx;
		std::vector<float> vty;
		std::vector<std::string> fx;
		std::vector<std::string> fy;
		std::vector<std::string> fz;

		ObjParser() 
		{
			vx.clear();
			vy.clear();
			vz.clear();
			vnx.clear();
			vny.clear();
			vnz.clear();
			vtx.clear();
			vty.clear();
			fx.clear();
			fy.clear();
			fz.clear();
		}
		~ObjParser()
		{
			vx.clear();
			vy.clear();
			vz.clear();
			vnx.clear();
			vny.clear();
			vnz.clear();
			vtx.clear();
			vty.clear();
			fx.clear();
			fy.clear();
			fz.clear();
		}

		void parse(std::string path)
		{
			std::ifstream obj(path);

			std::string line = "";

			while (getline(obj, line))
			{
				int type = line[0] + line[1];

				switch (type)
				{
					//v
				case 150:
					VandVN(line, vx, vy, vz);
					break;

					//vt
				case 234:
					VT(line, vtx, vty);
					break;

					//vn
				case 228:
					VandVN(line, vnx, vny, vnz);
					break;

					//f
				case 134:
					F(line, fx, fy, fz);
					break;
				}
			}

			obj.close();
		}

		int GetVerticiesNumber() 
		{
			return vx.size();
		}

		int GetTextureVerticiesNumber()
		{
			return vtx.size();
		}

		int GetNormalsNumber()
		{
			return vnx.size();
		}

		int GetFacesNumber()
		{
			return fx.size();
		}

		std::vector<std::string> GetFaces()
		{
			std::vector<std::string> faces;
			for (int i = 0; i < fx.size(); i++)
			{
				faces.push_back(fx[i]);
				faces.push_back(fy[i]);
				faces.push_back(fz[i]);
			}

			return faces;
		}

		std::vector<float> GetVerticies()
		{
			std::vector<float> ver;
			for (int i = 0; i < vx.size(); i++)
			{
				ver.push_back(vx[i]);
				ver.push_back(vy[i]);
				ver.push_back(vz[i]);
			}

			return ver;
		}

		std::vector<float> GetTextureVerticies()
		{
			std::vector<float> tex;
			for (int i = 0; i < vtx.size(); i++)
			{
				tex.push_back(vtx[i]);
				tex.push_back(vty[i]);
			}

			return tex;
		}

		std::vector<float> GetNormals()
		{
			std::vector<float> ver;
			for (int i = 0; i < vnx.size(); i++)
			{
				ver.push_back(vnx[i]);
				ver.push_back(vny[i]);
				ver.push_back(vnz[i]);
			}

			return ver;
		}

	private:
		void VandVN(std::string line, std::vector<float>& x, std::vector<float>& y, std::vector<float>& z)
		{
			int num = 1;
			int i = 0;
			std::string number = "";

			if (line[1] == 32)
				i = 2;
			else
				i = 3;

			for (i; i < line.length(); i++)
			{
				switch (num)
				{
				case 1:
					if (line[i] != 32)
						number += line[i];
					else
					{
						num++;
						x.push_back(stof(number));
						number = "";
					}
					break;

				case 2:
					if (line[i] != 32)
						number += line[i];
					else
					{
						num++;
						y.push_back(stof(number));
						number = "";
					}
					break;

				case 3:
					if (i != line.length() - 1)
						number += line[i];
					else
					{
						number += line[i];
						z.push_back(stof(number));
						number = "";
					}
					break;
				}
			}
		}

		void VT(std::string line, std::vector<float>& x, std::vector<float>& y)
		{
			int num = 1;
			std::string number = "";

			for (int i = 3; i < line.length(); i++)
			{
				switch (num)
				{
				case 1:
					if (line[i] != 32)
						number += line[i];
					else
					{
						num++;
						x.push_back(stof(number));
						number = "";
					}
					break;

				case 2:
					if (i != line.length() - 1)
						number += line[i];
					else
					{
						number += line[i];
						y.push_back(stof(number));
						number = "";
					}
					break;
				}
			}
		}

		void F(std::string line, std::vector<std::string>& x, std::vector<std::string>& y, std::vector<std::string>& z)
		{
			int num = 1;
			int sl = 0;
			std::string number = "";

			for (int i = 2; i < line.length(); i++)
			{
				switch (num)
				{
				case 1:
					if (line[i] != 32)
					{
						if (line[i] == 47)
							sl++;

						if (sl < 2)
							number += line[i];
					}
					else
					{
						num++;
						x.push_back(number);
						number = "";
						sl = 0;
					}
					break;

				case 2:
					if (line[i] != 32)
					{
						if (line[i] == 47)
							sl++;

						if (sl < 2)
							number += line[i];
					}
					else
					{
						num++;
						y.push_back(number);
						number = "";
						sl = 0;
					}
					break;

				case 3:
					if (i != line.length() - 1)
					{
						if (line[i] == 47)
							sl++;

						if (sl < 2)
							number += line[i];
					}
					else
					{
						z.push_back(number);
						number = "";
						sl = 0;
					}
					break;
				}
			}
		}
	};
}