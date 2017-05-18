//
//  ParseSurvey.cpp
//  VisualizationCode
//
//  Created by Shane Griffith on 6/9/15.
//  Copyright (c) 2015 shane. All rights reserved.
//

#include "ParseSurvey.h"

using namespace std;

#ifdef _WIN32
#define M_PI_2     1.57079632679489661923
const string ParseSurvey::_auxfile = "\\image_auxilliary.csv";
#else
const string ParseSurvey::_auxfile = "/image_auxilliary.csv";
#endif

FILE * ParseSurvey::OpenFile(string filename, const char * op) {
	FILE * fs = fopen(filename.c_str(), op);
	if (!fs) { cout << "FileParsing: Error. Couldn't open " << filename << "." << endl; exit(-1); }
	return fs;
}

vector<string> ParseSurvey::ParseLine(char * line) {
	return ParseLineAdv(line, ",");
}

vector<string> ParseSurvey::ParseLineAdv(char * line, string separator) {
	vector<string> parsedline;
	int idx = 0;
	const char* tok;
	for (tok = strtok(line, separator.c_str());
		tok && *tok;
		tok = strtok(NULL, string(separator + "\n").c_str()))
	{
		parsedline.push_back(tok);
		idx++;
	}

	return parsedline;
}

string ParseSurvey::GetImagePath(string base, int no) {
    char imgfile[100];
	string delim="/";
#ifdef _WIN32
	delim = "\\";
#endif
    sprintf(imgfile, "%s%s%04d%s%04d.jpg", base.c_str(), delim.c_str(), (((int) no)/1000), delim.c_str(), (((int) no)%1000));
    return string(imgfile);
}

std::vector<double> ParseSurvey::GetRotationMatrix(double X, double Y, double Z) {
    /*Converts the three orientation angles into a rotation matrix.
     see http://www.songho.ca/opengl/gl_anglestoaxes.html
     */

    std::vector<double> R = {cos(Y)*cos(Z), -cos(Y)*sin(Z), sin(Y),
              sin(X)*sin(Y)*cos(Z) + cos(X)*sin(Z), -sin(X)*sin(Y)*sin(Z)+cos(X)*cos(Z), -sin(X)*cos(Y),
              -cos(X)*sin(Y)*cos(Z)+sin(X)*sin(Z), cos(X)*sin(Y)*sin(Z)+sin(X)*cos(Z), cos(X)*cos(Y)};
    
    return R;
}

std::vector<double> ParseSurvey::ComposeRotationMatrices(std::vector<double> A, std::vector<double> B){
    std::vector<double> R = {
	A[0]*B[0]+A[1]*B[3]+A[2]*B[6],
        A[0]*B[1]+A[1]*B[4]+A[2]*B[7],
        A[0]*B[2]+A[1]*B[5]+A[2]*B[8],
        A[3]*B[0]+A[4]*B[3]+A[5]*B[6],
        A[3]*B[1]+A[4]*B[4]+A[5]*B[7],
        A[3]*B[2]+A[4]*B[5]+A[5]*B[8],
        A[6]*B[0]+A[7]*B[3]+A[8]*B[6],
        A[6]*B[1]+A[7]*B[4]+A[8]*B[7],
        A[6]*B[2]+A[7]*B[5]+A[8]*B[8]};
    return R;
}

std::vector<double> ParseSurvey::RotationMatrixToRPY(std::vector<double> R){
    std::vector<double> rpy = {atan2(R[7], R[8]), atan2(-1*R[6], sqrt(R[7]*R[7]+R[8]*R[8])), atan2(R[3],R[0])};
    return rpy;
}

vector<double> ParseSurvey::GetCameraPose(double x, double y, double theta, double camera_pan, double tilt) {
    std::vector<double> align_with_world = GetRotationMatrix(0, M_PI_2, -M_PI_2);
    std::vector<double> cam = GetRotationMatrix(tilt, 0, -camera_pan);
    std::vector<double> boat = GetRotationMatrix(0, 0, theta);

    std::vector<double> R = ComposeRotationMatrices(ComposeRotationMatrices(boat, cam), align_with_world);
    std::vector<double> RPY = RotationMatrixToRPY(R);
    std::vector<double> ret = {x-default_x, y-default_y, 0, RPY[0], RPY[1], RPY[2]};
    return ret;
}

void ParseSurvey::ProcessLineEntries(int type, vector<string> lp){
    if(lp[0].at(0)=='%')return;
    if(lp.size()<14)return;
    vector<double> p = GetCameraPose(stod(lp[2]), stod(lp[3]), stod(lp[4]), stod(lp[5]), stod(lp[6]));
    cam_pan.push_back(stod(lp[5]));
    boat.push_back(p);
    timings.push_back(stod(lp[0]));
    imageno.push_back((int)stod(lp[1]));
    omega.push_back(stod(lp[13]));
}

void ParseSurvey::ReadDelimitedFile(string file, int type) {
    FILE * fp = OpenFile(file,"r");
    char line[LINESIZE]="";
    
    while (fgets(line, LINESIZE-1, fp)) {
        char * tmp = line;
        vector<string> lp = ParseLine(tmp);
        ProcessLineEntries(type, lp);
    }
    fclose(fp);
}

int ParseSurvey::GetIndexOfImage(int image) const {
    /*Finds the index in the aux file for a given image. Direct mapping doesn't work if
     * the aux is cropped anywhere after the beginning or images are skipped. Iterative
     * direct mapping is used.
     */
    if(imageno.size()==0) return -1;
    
    int last_dist = image;
    int idx=0, last_idx=0;
    while(imageno[idx] != image){
        int dist = abs(imageno[idx]-image);
        if(last_dist < dist){
            cout << "ParseSurvey: Warning. Couldn't find the exact image in the aux file." << endl;
            return last_idx;
        }
        last_dist = dist;
        last_idx = idx;
        idx += image - imageno[idx];
    }
    return idx;
}

double ParseSurvey::GetAvgAngularVelocity(int sidx, int eidx) const {
    /* The average angular velocity between two indices [sidx, eidx].
     * */
    sidx = max(0,sidx);
    eidx = min((int)omega.size()-1, eidx);
    double sum = 0.0;
    for(int i=sidx; i<=eidx; i++) {
        sum += omega[i];
    }
    if(eidx-sidx==0) return omega[eidx];
    return sum/(eidx-sidx);
}
