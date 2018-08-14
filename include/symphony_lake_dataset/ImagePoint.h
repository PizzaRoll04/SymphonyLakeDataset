#ifndef LSM_IMAGE_POINT_H
#define LSM_IMAGE_POINT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace symphony_lake_dataset {

    struct ImagePoint {
        double t;
        size_t id; // for DB
        size_t fid;
        size_t idx;
        long seq;
        double x,y;
        float theta, omega;
        float pan,tilt;
        float fx,fy,cx,cy;
        size_t width,height;

        cv::Mat load(const std::string & dir) const {
            char folder[128];
            sprintf(folder,"/%04ld/%04ld.jpg",seq/1000,seq%1000);
            printf("Loading '%s'\n",(dir+folder).c_str());
            return cv::imread(dir+folder);
        }


        bool loadFromString(const std::string & s) { 
            double dseq,dw,dh;
            if (sscanf(s.c_str(),"%le,%le, %le,%le,%e, %e,%e, %e,%e,%e,%e, %le,%le,%e",
                        &t,&dseq,&x,&y,&theta,&pan,&tilt,
                        &fx,&fy,&cx,&cy,&dw,&dh,&omega)==14) {
                seq=(int)dseq;
                width=(int)dw;
                height=(int)dh;
                return true;
            }
            return false;
        }
    };


    typedef std::vector<ImagePoint> ImageAuxVector;



};




#endif // LSM_IMAGE_POINT_H
