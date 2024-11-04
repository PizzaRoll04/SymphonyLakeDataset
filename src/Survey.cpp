

#include <symphony_lake_dataset/csv_functions.h>
#include <symphony_lake_dataset/Survey.h>

using namespace symphony_lake_dataset;


bool Survey::load(const std::string & mapdir, const std::string & imdir, const std::string & survey) {
    name = survey;
    image_folder = imdir;
    assert(loadClassesFromCSV(mapdir+"/"+survey+"/optposes.csv",pv,true));
    assert(pv.size() > 0);
    assert(loadClassesFromCSV(imdir+"/"+survey+"/image_auxilliary.csv",iv,true));
    assert(iv.size() > 0);
    for (size_t i=0;i<pv.size();i++) {
        pv_idx.set(pv[i].t,i);
    }

    for (size_t i=0;i<iv.size();i++) {
        iv_idx.set(iv[i].t,i);
    }
    pose_dis = std::uniform_int_distribution<>(0,pv.size()-1);
    image_dis = std::uniform_int_distribution<>(0,iv.size()-1);
    printf("Loaded %s: %d %d\n",name.c_str(),int(pv.size()),int(iv.size()));
    return true;
}

bool Survey::load(const std::string & imdir, const std::string & survey) {
    name = survey;
    image_folder = imdir;
    // TODO might have to create a flag that no mapdir has been passed and that pv is not populated
    assert(loadClassesFromCSV(imdir+"/"+survey+"/image_auxilliary.csv",iv,true));
    assert(iv.size() > 0);
    for (size_t i=0;i<iv.size();i++) {
        iv_idx.set(iv[i].t,i);
    }
    image_dis = std::uniform_int_distribution<>(0,iv.size()-1);
    printf("Loaded %s: %d\n",name.c_str(),int(iv.size()));
    return true;
}

cv::Mat_<float> Survey::getKByPose(size_t pose_index) const {
    size_t p = imageFromPose(pose_index);
    cv::Mat_<float> K(3,3,0.0);
    K(0,0) = iv[p].fx; K(0,2) = iv[p].cx;
    K(1,1) = iv[p].fy; K(1,2) = iv[p].cy;
    K(2,2) = 1.0;
    return K;
}

cv::Mat_<float> Survey::getKByTime(double time) const {
    size_t p = imageIndex(time);
    cv::Mat_<float> K(3,3,0.0);
    K(0,0) = iv[p].fx; K(0,2) = iv[p].cx;
    K(1,1) = iv[p].fy; K(1,2) = iv[p].cy;
    K(2,2) = 1.0;
    return K;
}

cv::Mat Survey::loadImageByImageIndex(size_t imageIndex) const {
    return iv[imageIndex].load(image_folder+"/"+name);
}

cv::Mat Survey::loadImageByPoseIndex(size_t pose_index) const {
    size_t p = imageFromPose(pose_index);
    return iv[p].load(image_folder+"/"+name);
}

cv::Mat Survey::loadImageByTime(double time) const {
    size_t p = imageIndex(time);
    printf("Load by time %.3f -> %ld %ld\n",time,seq(time),iv[p].seq);
    return iv[p].load(image_folder+"/"+name);
}

bool Survey::checkTime(double time) const {
    size_t i = imageIndex(time);
    size_t p = poseIndex(time);
    return (fabs(iv[i].t - time) < 0.2) && (fabs(pv[p].t - time) < 0.1);
}

void Survey::findNeighbours(size_t pose_index, const Survey & S2, std::multimap<double,Pose> & result) const {
    symphony_lake_dataset::Function X,Y,T;
    assert(pose_index < pv.size());
    const Pose & Pref = pv[pose_index];
    double ux = cos(Pref.thetag), uy = sin(Pref.thetag);
    double vx = -uy, vy = ux;
    for (size_t i=0;i<S2.pv.size();i++) {
        X.set(S2.pv[i].t, ux*(S2.pv[i].xg - Pref.xg) + uy*(S2.pv[i].yg - Pref.yg));
        Y.set(S2.pv[i].t, vx*(S2.pv[i].xg - Pref.xg) + vy*(S2.pv[i].yg - Pref.yg));
    }

    symphony_lake_dataset::Function::Support z = X.zeros();
    printf("Found %d zeros\n",int(z.size()));
    result.clear();
    for (symphony_lake_dataset::Function::Support::const_iterator it=z.begin();it!=z.end();it++) {
        if (fabs(Y(*it)) > 10.0) {
            // If Y is too far, there is no way the view point can be similar
            continue;
        }
        size_t i = std::max<int>(0,std::min<int>(S2.pv.size()-1,S2.pv_idx(*it)));
        const Pose & Q = S2.pv[i];
        double view_angle_P = Pref.thetag - pan(Pref.t);
        double view_angle_Q = Q.thetag - S2.pan(Q.t);
        double view_delta = fabs(remainder(view_angle_P - view_angle_Q, 2*M_PI));
        if (view_delta > M_PI/6) {
            // The viewing direction of Q and P must be approximately equal
            continue;
        }
        if (view_delta > M_PI/12) {
            printf("Warning: view_delta = %.1fdeg\n",view_delta*180./M_PI);
        }
        if (!S2.checkTime(Q.t)) {
            // There must be an image within a reasonnable time of Q.t
            continue;
        }
        result.insert(std::pair<double,Pose>(hypot(Y(Q.t),X(Q.t)),Q));
    }
    printf("Kept %d poses\n",int(result.size()));
}

