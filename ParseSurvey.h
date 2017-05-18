//
//  ParseSurvey.h
//  VisualizationCode
//
//  Created by Shane Griffith on 6/9/15.
//  Copyright (c) 2015 shane. All rights reserved.
//

#ifndef __ParseSurvey__
#define __ParseSurvey__

#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <vector>
#include <cmath>
#include <math.h>
#include <algorithm>

using namespace std;

class ParseSurvey{
private:
	const static int LINESIZE = 10000;
    //an offset that defines the center of the lake.
    double default_x, default_y;
    
    static const string _auxfile;

	static FILE * OpenFile(std::string filename, const char * op);
	static std::vector<std::string> ParseLine(char * line);
	static std::vector<std::string> ParseLineAdv(char * line, std::string separator);

    std::vector<double> GetRotationMatrix(double X, double Y, double Z);
    std::vector<double> ComposeRotationMatrices(std::vector<double> A, std::vector<double> B);
    std::vector<double> RotationMatrixToRPY(std::vector<double> R);
    vector<double> GetCameraPose(double x, double y, double theta, double camera_pan, double tilt);
protected:
    void ProcessLineEntries(int type, vector<string> lp);
    void ReadDelimitedFile(string file, int type);
public:
    string _base;
    
    vector<vector<double>> boat;
    vector<double> timings;
    vector<double> omega;
    vector<double> cam_pan;
    vector<int> imageno;
    
    ParseSurvey(string base):
    _base(base), default_x(296866.7554855264), default_y(5442698.88922645){
        ReadDelimitedFile(_base + _auxfile, 0);

        if(timings.size() == 0) {
            std::cout << "Survey Error. The AUX file is empty." << std::endl;
        }
    }
    
    static string GetImagePath(string base, int no);
    int GetIndexOfImage(int image);
    double GetAvgAngularVelocity(int sidx, int eidx);
};




#endif /* defined(__ParseSurvey__) */
