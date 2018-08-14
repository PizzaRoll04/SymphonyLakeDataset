#ifndef SURVEY_VECTOR_H
#define SURVEY_VECTOR_H

#include <vector>
#include "symphony_lake_dataset/Survey.h"

namespace symphony_lake_dataset {

    class SurveyVector {
        protected:
            std::vector<Survey> surveys;

            mutable std::uniform_int_distribution<> dis;
        public:
            typedef std::pair<size_t,size_t> PoseRef;
            typedef std::pair<PoseRef,PoseRef> PosePair;

        public:

            SurveyVector() {}

            bool load(const std::string & map_folders, const std::string & image_folders, 
                    const std::vector<std::string> &file_list);

            template<class Generator> 
                PoseRef getRandomImage(Generator &gen) const {
                    size_t s1 = dis(gen);
                    const Survey & S1 = surveys[s1];
                    int p1 = S1.randomPose(gen);
                    return PoseRef(s1,S1.imageFromPose(p1));
                }

            template<class Generator> 
                PosePair findRandomPair(Generator &gen) const {
                    size_t counter = 0;
                    assert(surveys.size()>1);
                    while (counter < 1000) {
                        PoseRef p1 = getRandomImage(gen);
                        size_t s2 = dis(gen);
                        while (s2 == p1.first) s2=dis(gen);
                        const Survey & S2 = surveys[s2];
                        std::multimap<double,Pose> neighbours;
                        surveys[p1.first].findNeighbours(surveys[p1.first].poseFromImage(p1.second),S2,neighbours);
                        if (neighbours.size() == 0) {
                            continue;
                        }
                        PoseRef p2(s2,S2.imageIndex(neighbours.begin()->second.t));
                        return PosePair(p1,p2);
                    }
                    assert(counter < 1000);
                    return PosePair(); // for the compiler
                }

            const ImagePoint & getImagePoint(const PoseRef & pref) const {
                const Survey & S = surveys[pref.first];
                return S.getImagePoint(pref.second);
            }

            cv::Mat loadImage(const PoseRef & pref) const {
                const Survey & S = surveys[pref.first];
                return S.loadImageByImageIndex(pref.second);
            }

            void poseRefToImageRef(const PoseRef & pref,std::string & surveyname, size_t & seq) const {
                const Survey & S = surveys[pref.first];
                surveyname = S.getName();
                seq = S.getImagePoint(pref.second).seq;
            }

            void getAllNeighbours(const PoseRef & pref,std::vector<PoseRef> & all_neighbours) const ;

            cv::Mat_<float> getK(const PoseRef & pref) const {
                const Survey & S = surveys[pref.first];
                return S.getKByPose(S.poseFromImage(pref.second));
            }

            Pose getPose(const PoseRef & pref) const {
                const Survey & S = surveys[pref.first];
                return S.getPose(S.poseFromImage(pref.second));
            }

            float getCameraDir(const PoseRef & pref) const {
                const Survey & S = surveys[pref.first];
                const Pose & P = S.getPose(S.poseFromImage(pref.second));
                return P.thetag - S.pan(P.t);
            }

            size_t getNumSurveys() const {
                return surveys.size();
            }

            const Survey & get(size_t i) const {
                return surveys[i];
            }

            bool checkPoseRef(const PoseRef & pref) const {
                if (pref.first >= surveys.size()) return false;
                const Survey & S = surveys[pref.first];
                if (pref.second >= S.getNumImages()) return false;
                const ImagePoint & ip = S.getImagePoint(pref.second);
                return S.checkTime(ip.t);
            }
                
    };


};



#endif // SURVEY_VECTOR_H
