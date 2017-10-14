#include "capture.h"
extern bool threadend;
extern bool saveData;
Capture::Capture(QObject *parent) : QObject(parent),
                                    ipaddress("192.168.3.10"),
                                    port("2368"),
                                    pcap("/home/ym/20170301.pcap"),
                                    correct("/home/ym/32db.xml"),
                                    cap_cloud_(new Cloud()),
                                    frameid(0),
                                    vcap(1),
                                    entersec(false)
{


}
void Capture::cloud_callback (const CloudConstPtr& cloud)
{
  boost::mutex::scoped_lock lock (cloud_mutex_);
  cloud_ = cloud;
}

void Capture::section_callback(const CloudConstPtr& cloud,float start,float end)
{
    boost::mutex::scoped_lock lock(sec_mutex_);
    sec_cloud_ = cloud;
    if(start <= ENDANGLE && end >= STARTANGLE && !entersec)
    {
        cap_cloud_->clear();
        entersec = true;
    }

    if(entersec)
    {
       cap_cloud_->insert(cap_cloud_->end(),cloud->begin(),cloud->end());
    }

    if(end >= ENDANGLE && entersec)
    {
        vcap >> image;
        if(saveData)
        {
            char pcdfilename[255] = {0};
            char imgfilename[255] = {0};
            sprintf(pcdfilename,"pcd/data%d.pcd",frameid);
            sprintf(imgfilename,"img/img%d.jpg",frameid++);
            pcl::io::savePCDFileASCII(pcdfilename,*cap_cloud_);
            imwrite(imgfilename,image);
            saveData = false;
        }
        entersec = false;
    }
}

void Capture::loop()
{
    boost::shared_ptr<PCLVisualizer> cloud_viewer_(new PCLVisualizer("PCL HDL Cloud"));
    pcl::visualization::PointCloudColorHandlerGenericField<PointType> color_handler ("intensity");//PCL可视化
    /*Set viewer*/
    cloud_viewer_->addCoordinateSystem (3.0);
    cloud_viewer_->setBackgroundColor (0, 0, 0);
    cloud_viewer_->initCameraParameters ();
    cloud_viewer_->setCameraPosition (0.0, 0.0, 30.0, 0.0, 1.0, 0.0, 0);
    cloud_viewer_->setCameraClipDistances (0.0, 50.0);

    /*Callback Function*/

    boost::function<void (const CloudConstPtr&)> cloud_cb = boost::bind (
            &Capture::cloud_callback, this, _1);

    boost::function<void (const CloudConstPtr&,float,float)> sec_cb = boost::bind(&Capture::section_callback,this,_1,_2,_3);

    /*Set Grabber Option*/
    pcl::HDLGrabber grabber_(boost::asio::ip::address::from_string(ipaddress),boost::lexical_cast<unsigned short>(port));

    /*Register callback Function*/
    boost::signals2::connection cloud_connection = grabber_.registerCallback (
        cloud_cb);
    boost::signals2::connection sec_connection = grabber_.registerCallback(sec_cb);

    pcl::visualization::PointCloudColorHandler<PointType> &handler_ = color_handler;
    /*Show Viewer*/
    grabber_.start();

    while (!cloud_viewer_->wasStopped () && !threadend)
    {
      CloudConstPtr cloud;

      // See if we can get a cloud
      if (cloud_mutex_.try_lock ())
      {
        cloud_.swap (cloud);
        cloud_mutex_.unlock ();
      }

      if (cloud)
      {
        handler_.setInputCloud (cloud);
        if (!cloud_viewer_->updatePointCloud (cloud, handler_, "HDL"))
          cloud_viewer_->addPointCloud (cloud, handler_, "HDL");

        if(!image.empty())
        {
            if (image.channels()==3) {
                cvtColor(image, image, CV_BGR2RGB);
                qimage = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                                image.cols*image.channels(), QImage::Format_RGB888);
                emit setImage(qimage);
        }
        }
        cloud_viewer_->spinOnce ();
      }

      if (!grabber_.isRunning ())
        cloud_viewer_->spin ();

      boost::this_thread::sleep (boost::posix_time::microseconds (100));
    }


    grabber_.stop();

    /* Disconnect Callback Function*/
    if(cloud_connection.connected())
    {
        cloud_connection.disconnect();
    }

    if(sec_connection.connected())
    {
        sec_connection.disconnect();
    }

    threadend = false;
    return;

}
