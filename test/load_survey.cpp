#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <memory>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// #define INSPECT

bool loadFromString(const std::string & line, std::string & data) {
    data = line.substr(0,6);
    return true;
}
#include <symphony_lake_dataset/SurveyVector.h>


using namespace symphony_lake_dataset;

#define R2D(X) ((X)*180./M_PI)

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 4) {
        printf("Usage: %s <map root dir> <image root dir> <surveys...>",argv[0]);
        printf("Example:\n\t %s data/Maps data/VBags 140106\n",argv[0]);
        return -1;
    }

    std::string map_folders(argv[1]);
    std::string image_folders(argv[2]);
    std::vector<std::string> survey_list;
    for (int i=3;i<argc;i++) {
        survey_list.push_back(argv[i]);
    }

    SurveyVector surveys;
    surveys.load(map_folders,image_folders,survey_list);

    const Survey & S = surveys.get(0);
    printf("Loaded one survey: %d poses, %d images\n",int(S.getNumPoses()),int(S.getNumImages()));

    SurveyVector::PoseRef pref(0 /* survey index */ ,0 /* image index */);
    // Make sure that there is an image and a pose available at this time
    bool ok = surveys.checkPoseRef(pref); 
    cv::Mat I = surveys.loadImage(pref);
    const ImagePoint & ip = surveys.getImagePoint(pref);
    const Pose &  P = surveys.getPose(pref);
    double theta_cam = surveys.getCameraDir(pref);
    printf("Loaded image 0. Pose %s: (%.2f m, %.2f m) boat heading %.2f deg camera heading %.2f deg\n"
            "GPS %.2f %.2f Compass %.2f deg Camera Pan %.2f deg\n",
            (ok?"ok":"not ok"),P.xg, P.yg, R2D(P.thetag), R2D(theta_cam),
            ip.x, ip.y, R2D(ip.theta), R2D(ip.pan));

    cv::imshow("I",I);
    printf("Press 'q' in the graphic window to quit\n");
    while (1) {
        int k = cv::waitKey(0) & 0xFF;
        switch (k) {
            case 'q':
            case 'Q':
            case 0x27:
                return 0;
                break;
            default:
                break;
        }
    }
    return 0;
}
