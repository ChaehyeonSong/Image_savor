#include "ros/ros.h"
#include <ros/package.h>
#include "CRawImage.h"
#include <image_transport/image_transport.h>

std::string pkg_path = ros::package::getPath("save_img");

void imageCallback(const sensor_msgs::ImageConstPtr& msg ){
    std::cout<<"start callback\n";
    CRawImage* image = new CRawImage(msg->width,msg->height,3);
    if(msg->step/msg->width ==3){
        memcpy(image->data,(void*)&msg->data[0],msg->step*msg->height);
    }
    else {
        CRawImage* image2 = new CRawImage(msg->width,msg->height,msg->step/msg->width);
        memcpy(image2->data,(void*)&msg->data[0],msg->step*msg->height);
        for (int j=0; j<msg->height;j++){
            for(int i=0; i<msg->width; i++){
                int ip= j*msg->width+i;
                int dp= (j*msg->width+i)*3;
                image->data[dp] = image2->data[ip];
                image->data[dp+1] = image2->data[ip];
                image->data[dp+2] = image2->data[ip];
            }
        }
        delete image2;
    }


    std::string path = pkg_path +"/data/image.bmp";
    image->saveBmp(path.c_str());
    std::cout<<"Image is saved"<<std::endl;
    delete image;
    
}


int main(int argc, char **argv){

    ros::init(argc,argv, "img_saver");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    // image_transport::Subscriber sub = it.subscribe("/camera/color/image_raw", 1, imageCallback);
    image_transport::Subscriber sub = it.subscribe("/thermal/image_raw", 1, imageCallback);
    ros::spin();

    return 0;
}
