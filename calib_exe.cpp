// #include <opencv2/core/core.hpp>
// #include <opencv2/calib3d/calib3d.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <stdio.h>
// #include <iostream>

#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d.hpp"
using namespace std;
using namespace cv;

vector< vector< Point3f > > object_points;
vector< vector< Point2f > > imagePoints1, imagePoints2;
vector< Point2f > corners1, corners2;
vector< vector< Point2f > > left_img_points, right_img_points;

Mat img1, img2, gray1, gray2;



void load_image_points(int board_width, int board_height, int num_imgs, float square_size,
                         string leftimg_dir,  string rightimg_dir,  string leftimg_filename, string rightimg_filename) {

   Size board_size = Size(board_width, board_height);
   int board_n = board_width * board_height;

   for (int i = 1; i <= num_imgs; i++) {
    string left_img, right_img ;
     left_img = leftimg_dir + leftimg_filename + to_string(i)+".jpg";
     right_img = rightimg_dir + rightimg_filename + to_string(i)+".jpg";
     img1 = imread( left_img.c_str(), IMREAD_COLOR);
     img2 = imread(right_img.c_str(), IMREAD_COLOR);
    
     cvtColor(img1, gray1, COLOR_RGB2GRAY);
     cvtColor(img2, gray2, COLOR_RGB2GRAY);

     bool found1 = false, found2 = false;

     found1 = findChessboardCorners(img1, board_size, corners1,
  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_FILTER_QUADS );
     found2 = findChessboardCorners(img2, board_size, corners2,
   CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_FILTER_QUADS );

     if (found1)
     {
       cornerSubPix(gray1, corners1, cv::Size(5, 5), cv::Size(-1, -1),
   cv::TermCriteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.1));
       drawChessboardCorners(gray1, board_size, corners1, found1);
     }
     if (found2)
     {
       cornerSubPix(gray2, corners2, cv::Size(5, 5), cv::Size(-1, -1),
   cv::TermCriteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.1));
       drawChessboardCorners(gray2, board_size, corners2, found2);
     }

     vector< Point3f > obj;
     for (int i = 0; i < board_height; i++)
       for (int j = 0; j < board_width; j++)
         obj.push_back(Point3f((float)j * square_size, (float)i * square_size, 0));

     if (found1 && found2) {
       cout << i << ". Found corners!" << endl;
       imagePoints1.push_back(corners1);
       imagePoints2.push_back(corners2);
       object_points.push_back(obj);
     }
   }
   for (int i = 0; i < imagePoints1.size(); i++) {
     vector< Point2f > v1, v2;
     for (int j = 0; j < imagePoints1[i].size(); j++) {
       v1.push_back(Point2f((double)imagePoints1[i][j].x, (double)imagePoints1[i][j].y));
       v2.push_back(Point2f((double)imagePoints2[i][j].x, (double)imagePoints2[i][j].y));
     }
     left_img_points.push_back(v1);
     right_img_points.push_back(v2);
   }
}

int main(int argc, char const *argv[])
{
 	
  ifstream leftImgFile,rightImgFile;
  string leftImgDir = "/home/edna/Documents/opencv-edna/image_capture/";
  string rigthtImgDir = "/home/edna/Documents/opencv-edna/image_capture/";
  string leftImgName = "left";
  string rightImgName ="right";
  string outFileName = "cam_stereo.yml";

  int num_imgs = 10;



   FileStorage fsl("/home/edna/Documents/opencv-edna/left_img.yml", FileStorage::READ);
   FileStorage fsr("/home/edna/Documents/opencv-edna/right_img.yml", FileStorage::READ);

   load_image_points(fsl["board_width"], fsl["board_height"], 10, fsl["square_size"],
                    leftImgDir, rigthtImgDir, leftImgName, rightImgName);

   /*printf("Starting Calibration\n");
   Mat K1, K2, R, F, E;
   Vec3d T;
   Mat D1, D2;
   fsl["K"] >> K1;
   fsr["K"] >> K2;
   fsl["D"] >> D1;
   fsr["D"] >> D2;
   int flag = 0;
   flag |= CALIB_FIX_INTRINSIC;
  
   cout << "Read intrinsics" << endl;
  
   stereoCalibrate(object_points, left_img_points, right_img_points, K1, D1, K2, D2, img1.size(), R, T, E, F);

   cv::FileStorage fs1(out_file, cv::FileStorage::WRITE);
   fs1 << "K1" << K1;
   fs1 << "K2" << K2;
   fs1 << "D1" << D1;
   fs1 << "D2" << D2;
   fs1 << "R" << R;
   fs1 << "T" << T;
   fs1 << "E" << E;
   fs1 << "F" << F;
  
   printf("Done Calibration\n");

   printf("Starting Rectification\n");

   cv::Mat R1, R2, P1, P2, Q;
   stereoRectify(K1, D1, K2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);

   fs1 << "R1" << R1;
   fs1 << "R2" << R2;
   fs1 << "P1" << P1;
   fs1 << "P2" << P2;
   fs1 << "Q" << Q;

   printf("Done Rectification\n");
*/
  return 0;
}