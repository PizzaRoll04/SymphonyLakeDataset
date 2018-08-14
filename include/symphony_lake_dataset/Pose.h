#ifndef LSM_POSE_H
#define LSM_POSE_H

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdio.h>
#include <vector>

namespace symphony_lake_dataset {
    struct Pose {
        int kf;
        double t;
        double xm,ym,thetam; // SE2 pose w.r.t. map
        double xg,yg,thetag; // SE2 pose w.r.t. gps

        bool loadFromString(const std::string & s) {
            if (sscanf(s.c_str(),"%d, %le, %le, %le, %le, %le, %le, %le",&kf,&t, &xm,&ym,&thetam, &xg, &yg, &thetag) != 8) {
                return false;
            }

            return true;
        }
    };

    typedef std::vector<Pose> PoseVector;
};

#endif // LSM_POSE_H
