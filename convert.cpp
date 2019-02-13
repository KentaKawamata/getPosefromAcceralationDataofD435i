#include <iostream>
#include <fstream>
#include "convert.hpp"

ConvertPCL::ConvertPCL(std::string filename, float X, float Y, float Z):
    coodX(X),
    coodY(Y),
    coodZ(Z),
    roteFile(filename),
    signal(true)
{
    startSenser();
}

ConvertPCL::~ConvertPCL(){
    pipeline.stop();
}

void ConvertPCL::startSenser(){

    // デバイスを見つける
    rs2::context ctx;
    auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
    if (list.size() == 0)
        std::cout << "No device detected. Is it plugged in?" << std::endl;
    rs2::device dev = list.front();

    // Set Device Config
    rs2::config config;
    config.enable_stream(rs2_stream::RS2_STREAM_COLOR, color_width_, color_height_, rs2_format::RS2_FORMAT_BGR8, fps_);
    config.enable_stream(rs2_stream::RS2_STREAM_DEPTH, depth_width_, depth_height_, rs2_format::RS2_FORMAT_Z16, fps_);
    config.enable_stream(rs2_stream::RS2_STREAM_ACCEL);

    pipeline_profile = pipeline.start(config);
}

void ConvertPCL::drawPose() {

    std::cout << "==========================================" << "\n" 
        << "Pitch : " << std::setprecision(3) << Ptheta << "\n"
        << "Roll : " << std::setprecision(3) << Rtheta << std::endl;
}

void ConvertPCL::calcPose() {

    // ピッチ姿勢
    Pomega = std::atan(z/y);
    Ptheta = Pomega*180/M_PI;
    // ロール姿勢
    Romega = std::atan(x/y);
    Rtheta = Romega*180/M_PI;
}

void ConvertPCL::updateFrame() {
    // センサーからframeを得る
    frameset = pipeline.wait_for_frames();

    color_frame = frameset.get_color_frame();
    depth_frame = frameset.get_depth_frame().apply_filter(color_map);

    // Retrive Frame Size
    color_width_ = color_frame.as<rs2::video_frame>().get_width();
    color_height_ = color_frame.as<rs2::video_frame>().get_height();
    depth_width_ = depth_frame.as<rs2::video_frame>().get_width();
    depth_height_ = depth_frame.as<rs2::video_frame>().get_height();

}

void ConvertPCL::update_drawIMU() {

    frameset = pipeline.wait_for_frames();

    if(rs2::motion_frame accelFrame  
        = frameset.first_or_default(rs2_stream::RS2_STREAM_ACCEL)) {

        accelSample = accelFrame.get_motion_data();

        x = accelSample.x;
        y = accelSample.y;
        z = accelSample.z;

        calcPose();
        drawPose();
    } else {
        std::cout << "Could not get Pose" << std::endl;
        signal = false;
    }

}

void ConvertPCL::draw() {

    color_mat_ = cv::Mat(color_height_, color_width_, CV_8UC3, const_cast<void *>(color_frame.get_data()));
    depth_mat_ = cv::Mat(depth_height_, depth_width_, CV_8UC3, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);
}

void ConvertPCL::show() {

    cv::imshow("color_test", color_mat_);
    cv::imshow("depth_test", depth_mat_);
}

void ConvertPCL::savePose() {

    std::string file = roteFile + std::to_string(count_) + ".txt";
    std::ofstream fp(file);

    /*
     * 順番 : 
     *  roll 
     *  pitch 
     *  yaw
     */
    fp << std::setprecision(3) << Rtheta << "\n";
    fp << std::setprecision(3) << Ptheta << "\n";
    fp << "0.0" << "\n";
    fp << coodX << "\n";
    fp << coodY << "\n";
    fp << coodZ << "\n";

    fp.close();
    std::cout << "Save pose to " << file << std::endl;
}

void ConvertPCL::run() {

    count_=0;
    while(true){
        updateFrame();
        update_drawIMU();
        draw();
        show();

        const int key = cv::waitKey(20);
        if(key=='s' && signal==true){
            savePose();
            count_++;
        }
        else if(key=='s' && signal==false){
            std::cout << "Could not save pose" << std::endl;
        }
        else if(key==27){
            cv::destroyAllWindows();
            break;
        }
    }
}
