#ifndef GETFUNCTIONS_HPP_
#define GETFUNCTIONS_HPP_

#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#define sensitivity (28.5)

using namespace std;

//My compiler has some problem so I need this int2string. One may simply use std::to_string instead.
string int2string(int a){
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;
}

char * string2charp(string s){
    char * c = new char [s.length()+1]; //change type from string to char*
    std::strcpy (c, s.c_str());
    return c;
}

//This "getFiles" function is copied from the Internet.
//What it does is output all the names of files in current folder to string vector "files"
void getFiles(string path, vector<string>& files ){

    long hFile = 0;
    struct _finddata_t fileinfo;
    string p;

    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1){

        do{

            if((fileinfo.attrib &  _A_SUBDIR)){//<--Do NOT delete any braces ("{" or "}") here.
							//It seems quite dummy to have them here, but the function cannot work without them. I DON'T KNOW WHY.
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    getFiles( p.assign(path).append("\\").append(fileinfo.name), files );

            }

            else{

                files.push_back(fileinfo.name);

            }

        }while(_findnext(hFile, &fileinfo)== 0);

        _findclose(hFile);
    }
}
//This function taken from http://stackoverflow.com/questions/3823921/convert-big-endian-to-little-endian-when-reading-from-a-binary-file
double le2be(double d)
{
   double a;
   unsigned char *dst = (unsigned char *)&a;
   unsigned char *src = (unsigned char *)&d;

   dst[0] = src[7];
   dst[1] = src[6];
   dst[2] = src[5];
   dst[3] = src[4];
   dst[4] = src[3];
   dst[5] = src[2];
   dst[6] = src[1];
   dst[7] = src[0];

   return a;
}
//Following function taken from http://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
//(Assume that a char has 8 bits)
string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

string byte_2_str(char* bytes, int size) {
	static const char* const hex = "0123456789ABCDEF";
  string str;
  for (int i = 0; i < size; ++i) {
    const char ch = bytes[i];
    str.append(&hex[(ch  & 0xF0) >> 4], 1);
    str.append(&hex[ch & 0xF], 1);
  }
  return str;
}

bool checknum(char c){
	if(c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='0')
		return true;
	else
		return false;
}

double getEnergy(string filename){

	double energy;
	const char * cfilename = filename.c_str();

	char cenergy[8];

	//Use ifstream is very crucial here. Don't replace with fstream
	ifstream file(cfilename,ifstream::binary); //"ios::binary": Open with binary mode to avoid ending at 0x1a
	int readn = 0;

	for(int i = 0; i < 144; i++){

		char c[1] = {NULL};

		file.read(c,1);
		if(i > 135){
			cenergy[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cenergy)/sizeof(cenergy[0]);
    string scenergy(cenergy,len);

    string hscenergy = string_to_hex(scenergy);
    sscanf(hscenergy.c_str(),"%x",&energy);

	return le2be(energy);
}

double getxyDist(string filename){

	double xyDist;
	const char * cfilename = filename.c_str();

	char cxyDist[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 96; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 87){
			cxyDist[readn] = c[0];
			//cout<<readn<<"  "<<byte_2_str(cxyDist,readn)<<endl;
			readn++;
		}
	}
    file.close();

    int len = sizeof(cxyDist)/sizeof(cxyDist[0]);
    string scxyDist(cxyDist,len);

    string hscxyDist = string_to_hex(scxyDist);
    sscanf(hscxyDist.c_str(),"%x",&xyDist);
	return le2be(xyDist);
}

double getAngle(string filename){

	double Angle;
	const char * cfilename = filename.c_str();

	char cAngle[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 120; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 111){
			cAngle[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cAngle)/sizeof(cAngle[0]);
    string scAngle(cAngle,len);

    string hscAngle = string_to_hex(scAngle);
    sscanf(hscAngle.c_str(),"%x",&Angle);
	return le2be(Angle);
}

double getGT(string filename){

	double GT;
	const char * cfilename = filename.c_str();

	char cGT[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 88; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 79){
			cGT[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cGT)/sizeof(cGT[0]);
    string scGT(cGT,len);

    string hscGT = string_to_hex(scGT);
    sscanf(hscGT.c_str(),"%x",&GT);
	return le2be(GT);
}

double getXo(string filename){

	double Xo;
	const char * cfilename = filename.c_str();

	char cXo[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 72; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 63){
			cXo[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cXo)/sizeof(cXo[0]);
    string scXo(cXo,len);

    string hscXo = string_to_hex(scXo);

    if(hscXo=="0000000000000000")
    	return 0.;

    sscanf(hscXo.c_str(),"%x",&Xo);
	return le2be(Xo);
}

double getYo(string filename){

	double Yo;
	const char * cfilename = filename.c_str();

	char cYo[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 80; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 71){
			cYo[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cYo)/sizeof(cYo[0]);
    string scYo(cYo,len);

    string hscYo = string_to_hex(scYo);

    if(hscYo=="0000000000000000")
    	return 0.;

    sscanf(hscYo.c_str(),"%x",&Yo);
	return le2be(Yo);
}

double getWidth(string filename){

	double energy = getEnergy(filename);
	double xyDist = getxyDist(filename);

	double width = xyDist * sensitivity / sqrt(energy);

	return width;
}

string getDate(string filename){

	char Date[16];
	char cDate[8];
	cDate[2] = '/';
	cDate[5] = '/';
	const char * cfilename = filename.c_str();

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 224; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 212){
			Date[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int firdn;
    for(int i = 0; i < 16; i++){

    	if(checknum(char(Date[i]))){
    		firdn = i;
    		break;
    	}
    }

    if(checknum(char(Date[firdn+1]))){
    	cDate[3] = char(Date[firdn]);
    	cDate[4] = char(Date[firdn+1]);
    	if(checknum(char(Date[firdn+4]))){
        	cDate[0] = char(Date[firdn+3]);
        	cDate[1] = char(Date[firdn+4]);
        	//cDate[6] = char(Date[firdn+6]);
        	//cDate[7] = char(Date[firdn+7]);
        	cDate[6] = char(Date[firdn+8]);
        	cDate[7] = char(Date[firdn+9]);
    	}
    	else if(char(Date[firdn+4])=='.'){
        	cDate[0] = '0';
        	cDate[1] = char(Date[firdn+3]);
        	//cDate[6] = char(Date[firdn+5]);
        	//cDate[7] = char(Date[firdn+6]);
        	cDate[6] = char(Date[firdn+7]);
        	cDate[7] = char(Date[firdn+8]);
    	}
    }
    else if(char(Date[firdn+1])=='.'){
    	cDate[3] = '0';
    	cDate[4] = char(Date[firdn]);
    	if(checknum(char(Date[firdn+3]))){
        	cDate[0] = char(Date[firdn+2]);
        	cDate[1] = char(Date[firdn+3]);
        	//cDate[6] = char(Date[firdn+5]);
        	//cDate[7] = char(Date[firdn+6]);
        	cDate[6] = char(Date[firdn+7]);
        	cDate[7] = char(Date[firdn+8]);
    	}
    	else if(char(Date[firdn+3])=='.'){
        	cDate[0] = '0';
        	cDate[1] = char(Date[firdn+2]);
        	//cDate[6] = char(Date[firdn+4]);
        	//cDate[7] = char(Date[firdn+5]);
        	cDate[6] = char(Date[firdn+6]);
        	cDate[7] = char(Date[firdn+7]);
    	}
    }
/*
    struct stat *fileinfomation;
	stat(filename.c_str(), fileinfomation);
    const time_t tt = fileinfomation->st_mtime;

    if(checknum(char(cDate[9]))==false){
    	string stime = ctime(&tt);
    	string::iterator it=stime.end();
  		it = it - 2;
    	cDate[9] = *it;
    }
*/
    int len = sizeof(cDate)/sizeof(cDate[0]);
    string scDate(cDate,len);

	return scDate;
}

double getEL(string filename){

	double EL;
	const char * cfilename = filename.c_str();

	char cEL[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 128; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 119){
			cEL[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cEL)/sizeof(cEL[0]);
    string scEL(cEL,len);

    string hscEL = string_to_hex(scEL);

    if(hscEL=="0000000000000000")
    	return 0.;

    sscanf(hscEL.c_str(),"%x",&EL);
	return le2be(EL);
}

double getL2(string filename){

	double L2;
	const char * cfilename = filename.c_str();

	char cL2[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 168; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 159){
			cL2[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cL2)/sizeof(cL2[0]);
    string scL2(cL2,len);

    string hscL2 = string_to_hex(scL2);

    if(hscL2=="0000000000000000")
    	return 0.;

    sscanf(hscL2.c_str(),"%x",&L2);
	return le2be(L2);
}

double getL13(string filename){

	double L13;
	const char * cfilename = filename.c_str();

	char cL13[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 176; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 167){
			cL13[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cL13)/sizeof(cL13[0]);
    string scL13(cL13,len);

    string hscL13 = string_to_hex(scL13);

    if(hscL13=="0000000000000000")
    	return 0.;

    sscanf(hscL13.c_str(),"%x",&L13);
	return le2be(L13);
}

double getW(string filename){

	double W;
	const char * cfilename = filename.c_str();

	char cW[8];

	ifstream file(cfilename,ifstream::binary);
	int readn = 0;
	for(int i = 0; i < 184; i++){
		char c[1] = {NULL};

		file.read(c,1);
		if(i > 175){
			cW[readn] = c[0];
			readn++;
		}
	}
    file.close();

    int len = sizeof(cW)/sizeof(cW[0]);
    string scW(cW,len);

    string hscW = string_to_hex(scW);

    if(hscW=="0000000000000000")
    	return 0.;

    sscanf(hscW.c_str(),"%x",&W);
	return le2be(W);
}

#endif /* GETFUNCTIONS_HPP_ */
