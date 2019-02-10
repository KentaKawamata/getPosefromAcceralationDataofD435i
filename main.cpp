#include <iostream>
#include <cmath>
#include <iomanip>
#include <librealsense2/rs.hpp>

int main(int argc, char *argv[]){

  rs2::context ctx;
  auto list = ctx.query_devices();
  if(list.size() == 0){
      std::cout << "Could not found device" << std::endl;
      return -1;
  }

  rs2::pipeline pipe;
 
  rs2::config cfg;
  //cfg.enable_stream(rs2_stream::RS2_STREAM_GYRO, rs2_format::RS2_FORMAT_MOTION_XYZ32F, 5);
  //cfg.enable_stream(rs2_stream::RS2_STREAM_GYRO);
  cfg.enable_stream(rs2_stream::RS2_STREAM_ACCEL);
  
  rs2::pipeline_profile pipe_profile;
  pipe_profile = pipe.start(cfg);
   
  double Pomega = 0.0;
  double Romega = 0.0;
  double Ptheta = 0.0;
  double Rtheta = 0.0;
  double x;
  double y;
  double z;

  while(true) {

      rs2::frameset frameset = pipe.wait_for_frames();
          
      // Find and retrieve IMU and/or tracking data
      if (rs2::motion_frame accel_frame = frameset.first_or_default(rs2_stream::RS2_STREAM_ACCEL)){

        rs2_vector accel_sample = accel_frame.get_motion_data();

        x = accel_sample.x;
        y = accel_sample.y;        
        z = accel_sample.z;        
        
        Pomega = std::atan(z/y);
        Ptheta = Pomega*180/3.1415926;
        Romega = std::atan(x/y);
        Rtheta = Romega*180/3.1415926;

        std::cout << "==========================================" << "\n" 
                  << "Pitch : " << std::setprecision(3) << Ptheta << "\n"
                  << "Roll : " << std::setprecision(3) << Rtheta << std::endl;
      }

  }

  pipe.stop();

  return 0;
}
