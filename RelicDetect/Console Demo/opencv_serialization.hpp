#pragma once
// file: cvmat_serilization.h

#include <opencv2/opencv.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
using namespace cv;

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive &ar, cv::Mat& mat, const unsigned int)
		{
			int cols, rows, type;
			bool continuous;

			if (Archive::is_saving::value) {
				cols = mat.cols; rows = mat.rows; type = mat.type();
				continuous = mat.isContinuous();
			}

			ar & cols & rows & type & continuous;

			if (Archive::is_loading::value)
				mat.create(rows, cols, type);

			if (continuous) {
				const unsigned int data_size = rows * cols * mat.elemSize();
				ar & boost::serialization::make_array(mat.ptr(), data_size);
			}
			else {
				const unsigned int row_size = cols*mat.elemSize();
				for (int i = 0; i < rows; i++) {
					ar & boost::serialization::make_array(mat.ptr(i), row_size);
				}
			}
		}

		template<class Archive>
		void serialize(Archive &ar, cv::KeyPoint& kp, const unsigned int)
		{
			ar & kp.pt;
			ar & kp.size;
			ar & kp.angle;
			ar & kp.response;
			ar & kp.octave;
			ar & kp.class_id;
		}

		template<class Archive>
		void serialize(Archive &ar, cv::Point2f& p2f, const unsigned int)
		{
			ar & p2f.x;
			ar & p2f.y;
		}
		
	}//namespace serialization
}//namespace boost
