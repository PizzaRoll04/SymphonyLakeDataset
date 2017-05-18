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

/*A class to parse the data from an aux file of one survey.*/
class ParseSurvey{
private:
    const static int LINESIZE = 10000;
    
    /*The aux file name.*/
    static const std::string _auxfile;
    
    /*An offset that defines the center of the lake, which is subtracted from the
     aux file positions to simplify interpretation of the boat positions.
     */
    double default_x, default_y;
    
    /*Helper functions for opening a file and parsing csv data.*/
	static FILE * OpenFile(std::string filename, const char * op);
    void ProcessLineEntries(int type, std::vector<std::string> lp);
    void ReadDelimitedFile(std::string file, int type);
	static std::vector<std::string> ParseLine(char * line);
	static std::vector<std::string> ParseLineAdv(char * line, std::string separator);

    /*Converts the three orientation angles into a row-major rotation matrix.*/
    std::vector<double> GetRotationMatrix(double X, double Y, double Z);
    
    /*Composes two rotation matrices.*/
    std::vector<double> ComposeRotationMatrices(std::vector<double> A, std::vector<double> B);
    
    /*Converts a rotation matrix to a vector of roll, pitch, and yaw values*/
    std::vector<double> RotationMatrixToRPY(std::vector<double> R);
    
    /*Convert the pose data to a 6D camera pose of {x,y,z,roll,pitch,yaw}.*/
    std::vector<double> GetCameraPose(double x, double y, double theta, double camera_pan, double tilt);
public:
    /*The path to the base of the survey.*/
    std::string _base;
    
    /*The trajectory, in the format of 6D poses: {x,y,z,roll,pitch,yaw}.*/
    std::vector<std::vector<double> > boat;
    
    /*The time of each entry (seconds).*/
    std::vector<double> timings;
    
    /*The yaw rate.*/
    std::vector<double> omega;
    
    /*The pan angle of the pan-tilt-zoom camera. A value of |1.569978|
     represents a usable entry.*/
    std::vector<double> cam_pan;
    
    /*The image number that corresponds to the entry.*/
    std::vector<int> imageno;
    
    /*The constructor parses the aux file into the member variable vectors. 
     Access the data using those variables or the public member functions
     below. Each vector has the same number of entries, which correspond to 
     one another.
     */
    ParseSurvey(std::string base):
    _base(base), default_x(296866.7554855264), default_y(5442698.88922645){
        ReadDelimitedFile(_base + _auxfile, 0);

        if(timings.size() == 0) {
            std::cout << "Survey Error. The AUX file is empty." << std::endl;
        }
    }
    
    /*Returns the image path for a given image number and survey directory.*/
    static std::string GetImagePath(std::string base, int no);
    
    /*Finds the index in the aux file for a given image number.*/
    int GetIndexOfImage(int image) const;
    
    /* Returns the average yaw rate between two indices [sidx, eidx].*/
    double GetAvgAngularVelocity(int sidx, int eidx) const;
};




#endif /* defined(__ParseSurvey__) */
