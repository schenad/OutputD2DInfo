#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include "BasicExcel.hpp"
#include "GetFunctions.hpp"
#define MAX_NUM_OF_FILES (1000) //Max number of files this program can handle
#define sensitivity (28.5)

using namespace std;

int main(){

    char cwd[1000];
    getcwd(cwd, sizeof(cwd));             //Find current working directory

    vector<string> files;
    getFiles(cwd, files);                 //Get file names of all files in current folder
    printf("Remaining following files in the current directory: \n%s\n\n",cwd);

    string d2dfiles[MAX_NUM_OF_FILES];    //Filter out all the .d2d files
    int numofd2d = 0;
	for(unsigned int i = 0; i < files.size(); i++){
		if(files[i].find(".d2d")!=std::string::npos||files[i].find(".D2D")!=std::string::npos){
			d2dfiles[numofd2d] = (files[i]);
			numofd2d++;
		}
	}

	if(numofd2d == 0){                    //If there is no .d2d file, exit directly.
		cout<<"No D2D file found. Press any key to exit."<<endl;
		while(getchar())
			return -1;
	}

	for(int i = 0; i < numofd2d; i++)     //List out all the files.
		cout<<d2dfiles[i]<<endl;
	cout<<endl<<endl;

	cout<<"Output information:"<<endl;

	 for(int i = 0; i < numofd2d; i++)
		cout<<d2dfiles[i]<<endl<<"Date = "<<getDate(d2dfiles[i])<<endl<<"Xo = "<<getXo(d2dfiles[i])<<endl<<"Yo = "<<getYo(d2dfiles[i])<<endl<<"Range = "<<round(getWidth(d2dfiles[i]))<<endl<<"Angle = "<<getAngle(d2dfiles[i])<<endl<<"Energy = "<<round(getEnergy(d2dfiles[i]))<<endl<<"Gate Time = "<<getGT(d2dfiles[i])<<endl<<"EL = "<<getEL(d2dfiles[i])<<endl<<"L2 = "<<getL2(d2dfiles[i])<<endl<<"L13 = "<<getL13(d2dfiles[i])<<endl<<"W = "<<getW(d2dfiles[i])<<endl<<endl;
	cout<<endl;

	char lastdofyear;
	for(int i = 0; i < numofd2d; i++)
		if(checknum(*getDate(d2dfiles[i]).end())==false){
			cerr<<"Error: detection of years failed. Input last digit of year manually:"<<endl;
			cin>>lastdofyear;
			break;
		}

	YExcel::BasicExcel OutInfo;
	OutInfo.New(1);

	YExcel::BasicExcelWorksheet* sheet = OutInfo.GetWorksheet("Sheet1");

	//Write title
	sheet->Cell(0,0)->SetString("Type");  //Type
	sheet->Cell(0,1)->SetString("Filename");  //Filename
	sheet->Cell(0,2)->SetString("Date");  //Date
	sheet->Cell(0,3)->SetString("Xo");  //Xo
	sheet->Cell(0,4)->SetString("Yo");  //Yo
	sheet->Cell(0,5)->SetString("Range");  //Range
	sheet->Cell(0,6)->SetString("Angle");  //Angle
	sheet->Cell(0,7)->SetString("Energy");  //Energy
	sheet->Cell(0,8)->SetString("Rounded Energy");  //Rounded energy
	sheet->Cell(0,9)->SetString("Gate Time");  //Gate time
	sheet->Cell(0,10)->SetString("EL");  //Entrance Lens
	sheet->Cell(0,11)->SetString("L2");  //L2
	sheet->Cell(0,12)->SetString("L13");  //L13
	sheet->Cell(0,13)->SetString("W");  //Wehnelt
	//Write in data
	if (sheet){
		for(int i = 0; i < numofd2d; i++){
			sheet->Cell(i+1,0)->SetString("D2D");  //Type
			sheet->Cell(i+1,1)->SetString(d2dfiles[i].c_str());  //Filename
			string cDate(getDate(d2dfiles[i]));
			if(checknum(*cDate.end())==false)
				cDate[7]=lastdofyear;
			sheet->Cell(i+1,2)->SetString(cDate.c_str());  //Date
			sheet->Cell(i+1,3)->SetDouble(getXo(d2dfiles[i]));  //Xo
			sheet->Cell(i+1,4)->SetDouble(getYo(d2dfiles[i]));  //Yo
			sheet->Cell(i+1,5)->SetDouble(round(getWidth(d2dfiles[i])));  //Range
			sheet->Cell(i+1,6)->SetDouble(getAngle(d2dfiles[i]));  //Angle
			sheet->Cell(i+1,7)->SetDouble(getEnergy(d2dfiles[i]));  //Energy
			sheet->Cell(i+1,8)->SetDouble(round(getEnergy(d2dfiles[i])));  //Rounded energy
			sheet->Cell(i+1,9)->SetDouble((getGT(d2dfiles[i])));  //Gate time
			sheet->Cell(i+1,10)->SetDouble((getEL(d2dfiles[i])));  //Entrance Lens
			sheet->Cell(i+1,11)->SetDouble((getL2(d2dfiles[i])));  //L2
			sheet->Cell(i+1,12)->SetDouble((getL13(d2dfiles[i])));  //L13
			sheet->Cell(i+1,13)->SetDouble((getW(d2dfiles[i])));  //Wehnelt
		}
	}

	char XLSN[30];
	strcpy(XLSN,"D2DInfo (");
	string d2dFN = d2dfiles[0];
	d2dFN = d2dFN.substr(0,8);
	strcat(XLSN,d2dFN.c_str()); //using c_str convert std::string to char*
	strcat(XLSN,").xls");
	OutInfo.SaveAs(XLSN);

	cout<<"Read completed. Press any key to exit."<<endl;
	while(getchar())
	   	return 0;
}
