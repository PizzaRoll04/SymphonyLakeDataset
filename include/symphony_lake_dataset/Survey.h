#ifndef LSM_SURVEY_H
#define LSM_SURVEY_H

#include <assert.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <symphony_lake_dataset/Pose.h>
#include <symphony_lake_dataset/ImagePoint.h>
#include <symphony_lake_dataset/Function.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

namespace symphony_lake_dataset {
    class Survey {
        protected:
            std::string name;
            std::string image_folder;
            PoseVector pv;
            ImageAuxVector iv;
            // T: matches time to pv index
            // pidx: matches time to image index
            symphony_lake_dataset::Function iv_idx,pv_idx;

            mutable std::uniform_int_distribution<> pose_dis;
            mutable std::uniform_int_distribution<> image_dis;
        public:
            size_t getNumPoses() const {
                return pv.size();
            }

            size_t getNumImages() const {
                return iv.size();
            }


            const std::string & getName() const {
                return name;
            }

            const ImagePoint & getImagePoint(size_t i) const {
                assert(i < iv.size());
                return iv[i];
            }

            const Pose & getPose(size_t i) const {
                assert(i < pv.size());
                return pv[i];
            }

            size_t seq(double time) const { 
                int p = std::min<int>(iv.size()-1,std::max<int>(0,round(iv_idx(time))));
                return iv[p].seq;
            }

            float pan(double time) const {
                int p = std::min<int>(iv.size()-1,std::max<int>(0,round(iv_idx(time))));
                return iv[p].pan;
            }

            size_t poseIndex(double time) const {
                return std::min<int>(pv.size()-1,std::max<int>(0,round(pv_idx(time))));
            }

            size_t imageIndex(double time) const {
                return std::min<int>(iv.size()-1,std::max<int>(0,round(iv_idx(time))));
            }

            size_t imageFromPose(size_t pose_index) const {
                return imageIndex(pv[pose_index].t);
            }

            size_t poseFromImage(size_t imageIndex) const {
                return poseIndex(iv[imageIndex].t);
            }

            bool load(const std::string & mapdir, const std::string & imdir, const std::string & survey) ;
            bool load(const std::string & imdir, const std::string & survey) ;

            cv::Mat_<float> getKByPose(size_t pose_index) const ;

            cv::Mat_<float> getKByTime(double time) const ;

            cv::Mat loadImageByImageIndex(size_t image_index) const ;
            cv::Mat loadImageByPoseIndex(size_t pose_index) const ;

            cv::Mat loadImageByTime(double time) const ;

            bool checkTime(double time) const ;

            void findNeighbours(size_t pose_index, const Survey & S2, std::multimap<double,Pose> & result) const ;

            template <class Generator> 
                size_t randomPose(Generator & gen) const {
                    return pose_dis(gen);
                }

            template <class Generator> 
                size_t randomImage(Generator & gen) const {
                    return image_dis(gen);
                }
    };

};

#endif // LSM_SURVEY_H
