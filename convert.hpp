//
// Created by kawa on 10/6/18.
//
#ifndef CONVERTPCL_CONVERT_H
#define CONVERTPCL_CONVERT_H

#include <librealsense2/rs.hpp>
#include <librealsense2/rs_advanced_mode.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

class ConvertPCL{

private:

    //Buffer関係
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;
    rs2::frame color_frame;
    rs2::frame depth_frame;
    rs2::colorizer color_map;

    // ポイントクラウドとテクスチャマッピングを計算するためのポイントクラウドオブジェクトの宣言
    rs2::pointcloud pc;
    // ポイントオブジェクトを永続化して、フレームがドロップしたときに最後のクラウドを表示できるようにします
    rs2::points points;

    rs2_vector accelSample;

    cv::Mat color_mat_;
    cv::Mat depth_mat_;
    int color_width_ = 640;
    int color_height_ = 480;
    int depth_width_ = 640;
    int depth_height_ = 480;
    int width_ = 640;
    int height_ = 480;
    int fps_ = 30;
    int count_=0;
   
    double Pomega = 0.0;
    double Romega = 0.0;
    double Ptheta = 0.0;
    double Rtheta = 0.0;
    double x;
    double y;
    double z;
    bool signal;

    std::string roteFile;

public:

    ConvertPCL(std::string filename);
    ~ConvertPCL();
    void run();

private:

    inline void startSenser();
    void updateFrame();
    void draw();
    void show();
    void update_drawIMU();
    void calcPose();
    void drawPose();
    void savePose();
};

#endif //CONVERTPCL_CONVERT_H
