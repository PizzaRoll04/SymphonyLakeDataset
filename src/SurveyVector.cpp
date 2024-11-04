
#include "symphony_lake_dataset/SurveyVector.h"

using namespace symphony_lake_dataset;

bool SurveyVector::load(const std::string & map_folders, const std::string & image_folders, 
                    const std::vector<std::string> &file_list)
{
    surveys.resize(file_list.size());
    for (size_t i=0;i<file_list.size();i++) {
        surveys[i].load(map_folders,image_folders,file_list[i]);
    }
    dis = std::uniform_int_distribution<>(0, surveys.size()-1);
    return true;

}

bool SurveyVector::load(const std::string & image_folders, 
                    const std::vector<std::string> &file_list)
{
    surveys.resize(file_list.size());
    for (size_t i=0;i<file_list.size();i++) {
        surveys[i].load(image_folders,file_list[i]);
    }
    dis = std::uniform_int_distribution<>(0, surveys.size()-1);
    return true;

}

void SurveyVector::getAllNeighbours(const PoseRef & pref,std::vector<PoseRef> & all_neighbours) const {
    const Survey & S1 = surveys[pref.first];
    for (size_t i=0;i<surveys.size();i++) {
        const Survey & S2 = surveys[i];
        std::multimap<double,Pose> neighbours;
        S1.findNeighbours(S1.poseFromImage(pref.second),S2,neighbours);
        if (neighbours.size() == 0) {
            continue;
        }
        PoseRef p2(i,S2.imageIndex(neighbours.begin()->second.t));
        all_neighbours.push_back(p2);
    }

}




