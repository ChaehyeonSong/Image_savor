#include "ros/ros.h"
#include <ros/package.h>
#include "CRawImage.h"
#include <image_transport/image_transport.h>

std::string pkg_path = ros::package::getPath("save_img");

void imageCallback(const sensor_msgs::ImageConstPtr& msg ){
    CRawImage* image = new CRawImage(msg->width,msg->height,msg->step/msg->width);
    memcpy(image->data,(void*)&msg->data[0],msg->step*msg->height);
    std::string path = pkg_path +"/data/image.bmp";
    image->saveBmp(path.c_str());
    std::cout<<"Image is saved"<<std::endl;
    
}


int main(int argc, char **argv){

    ros::init(argc,argv, "img_saver");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    // image_transport::Subscriber sub = it.subscribe("/thermal/image_raw", 1, imageCallback);
    image_transport::Subscriber sub = it.subscribe("/camera/color/image_raw", 1, imageCallback);
    ros::spin();


    return 0;
}