//
// Created by quantum on 8/11/21.
//

#include <cstdio>
#include "videocapture.hpp"


#include <iostream>
#include <iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class ZedOpenCapture{

};


int main(int argc, char *argv[])
{
   // ----> Create Video Capture
   sl_oc::video::VideoParams params;
   params.res = sl_oc::video::RESOLUTION::HD720;
   params.fps = sl_oc::video::FPS::FPS_60;

   sl_oc::video::VideoCapture cap(params);
   if( !cap.initializeVideo() )
   {
      std::cerr << "Cannot open camera video capture" << std::endl;
      std::cerr << "See verbosity level for more details." << std::endl;

      return EXIT_FAILURE;
   }
   std::cout << "Connected to camera sn: " << cap.getSerialNumber() << std::endl;
   // <---- Create Video Capture



#ifdef TEST_FPS
// Timestamp to check FPS
double lastTime = static_cast<double>(getSteadyTimestamp())/1e9;
// Frame timestamp to check FPS
uint64_t lastFrameTs = 0;
#endif

// Infinite video grabbing loop
while (1)
{
   // Get last available frame
   const sl_oc::video::Frame frame = cap.getLastFrame();

   // ----> If the frame is valid we can display it
   if(frame.data!=nullptr)
   {
#ifdef TEST_FPS
      if(lastFrameTs!=0)
      {
         // ----> System time
         double now = static_cast<double>(getSteadyTimestamp())/1e9;
         double elapsed_sec = now - lastTime;
         lastTime = now;
         std::cout << "[System] Frame period: " << elapsed_sec << "sec - Freq: " << 1./elapsed_sec << " Hz" << std::endl;
         // <---- System time

         // ----> Frame time
         double frame_dT = static_cast<double>(frame.timestamp-lastFrameTs)/1e9;
         std::cout << "[Camera] Frame period: " << frame_dT << "sec - Freq: " << 1./frame_dT << " Hz" << std::endl;
         // <---- Frame time
      }
      lastFrameTs = frame.timestamp;
#endif

      // ----> Conversion from YUV 4:2:2 to BGR for visualization
      cv::Mat frameYUV = cv::Mat( frame.height, frame.width, CV_8UC2, frame.data );
      cv::Mat frameBGR;
      cv::cvtColor(frameYUV,frameBGR,cv::COLOR_YUV2BGR_YUYV);
      // <---- Conversion from YUV 4:2:2 to BGR for visualization

      // Show frame
      cv::imshow( "Stream RGB", frameBGR );
   }
   // <---- If the frame is valid we can display it

   // ----> Keyboard handling
   int key = cv::waitKey( 5 );
   if(key=='q' || key=='Q') // Quit
      break;
   // <---- Keyboard handling
}

return EXIT_SUCCESS;
}

