#ifndef CAPTURE_H
#define CAPTURE_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/hdl_grabber.h>
#include <pcl/visualization/point_cloud_color_handlers.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/console/parse.h>
#include <pcl/io/pcd_io.h>
#include <string>

using namespace std;
using namespace cv;
using namespace pcl::console;
using namespace pcl::visualization;

typedef pcl::PointXYZI PointType;

#define STARTANGLE  135.0
#define ENDANGLE    225.0


class Capture : public QObject
{
    Q_OBJECT
    typedef pcl::PointCloud<PointType> Cloud;
    typedef typename Cloud::ConstPtr CloudConstPtr;
    typedef typename Cloud::Ptr CloudPtr;
private:
    string                                                              ipaddress;                                                         //设置IP 地址
    string                                                              port;                                                              //设置端口
    string                                                              pcap;                                                              //设置pcap文件
    string                                                              correct;                                                           //grabber



    boost::mutex                                                        cloud_mutex_;
    boost::mutex                                                        sec_mutex_;
    CloudConstPtr                                                       cloud_;
    CloudConstPtr                                                       sec_cloud_;
    CloudPtr                                                            cap_cloud_;




    uint32_t                                                            frameid;

    //Image
    Mat                                                                 image;
    VideoCapture                                                        vcap;
    QImage                                                              qimage;


    bool                                                                entersec;







public:
    explicit Capture(QObject *parent = 0);

    void     cloud_callback(const pcl::PointCloud<PointType>::ConstPtr& cloud);
    void     section_callback(const pcl::PointCloud<PointType>::ConstPtr& cloud,float start,float end);

signals:
    void     setImage(QImage img);

public slots:
    void     loop();
};

#endif // CAPTURE_H
