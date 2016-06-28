#include <string>
#include <vector>
#include <fstream>  
#include<sstream>
#ifndef OBJREADER_H
#define OBJREADER_H

using namespace std;


namespace OBJspace{


	void string2char(string str, char c[]){
		int length = str.length();
		int i = 0;
		while (str[i] != 0){
			c[i] = str[i];
			i++;
		}
	}
	void split(const string& src, const string& separator, vector<string>& dest)
	{
		string str = src;
		string substring;
		string::size_type start = 0, index;

		do
		{
			index = str.find_first_of(separator, start);
			if (index != string::npos)
			{
				substring = str.substr(start, index - start);
				dest.push_back(substring);
				start = str.find_first_not_of(separator, index);
				if (start == string::npos) return;
			}
		} while (index != string::npos);

		//the last token
		substring = str.substr(start);
		dest.push_back(substring);
	}

	struct vec3{
		float x;
		float y;
		float z;
		vec3(float a, float b, float c) :x(a), y(b), z(c){}
	};

	struct vec4{
		int q;
		int w;
		int e;
		int r;
		vec4(int q, int w, int e, int r) :q(q), w(w), e(e), r(r){}
	};
	class OBJreader{
		string filename;

	private:
		vector<vec3> vectorv3;
		vector<vec4> vectorf4;
		int nv;
		int nf;
	public:
		class OBJreader(string S) :filename(S){
			construct(filename);
		}
		void OBJreader::construct(string filename);
		vector<vec3>  getvectorv3(){ return vectorv3; }
		vector<vec4>  getvectorf4(){ return vectorf4; }
	};

	void OBJreader::construct(string filename){
		ifstream  myfile("D:\\sore.ob2j");
		if (!myfile){
			return;
		}
		char buffer[256];
		while (!myfile.eof()){
			myfile.getline(buffer, 80);

			if (buffer[0] == '#'){
				continue;
			}
			else if (buffer[0] == 'v'){
				vector<string> vecforv;
				split(buffer, " ", vecforv);
				float coord[3];
				char temp[] = "              ";
				for (int i = 0; i <= 2; i++){
					string2char(vecforv[i + 1], temp);
					sscanf(temp, "%f", &coord[i]);
					memset(temp, 0, sizeof(temp));
				}
				vectorv3.push_back(vec3(coord[0], coord[1], coord[2]));
			}
			else if (buffer[0] == 'f'){
				vector<string> vecforf;
				split(buffer, " ", vecforf);
				float sequence[4];
				char temp[] = "              ";
				for (int i = 0; i <= 3; i++){
					string2char(vecforf[i + 1], temp);
					sscanf(temp, "%f", &sequence[i]);
					memset(temp, 0, sizeof(temp));
				}
				vectorf4.push_back(vec4(int(sequence[0]), int(sequence[1]), int(sequence[2]), int(sequence[3])));
			}
			else
			{
			}
		}

	}
}


#endif

