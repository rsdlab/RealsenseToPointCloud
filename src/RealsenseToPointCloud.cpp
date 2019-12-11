// -*- C++ -*-
/*!
 * @file  RealsenseToPointCloud.cpp
 * @brief Creating Point Cloud and RGBD CameraImage from Realsense Camera
 * @date $Date$
 *
 * $Id$
 */

#include "RealsenseToPointCloud.h"

// Module specification
// <rtc-template block="module_spec">
static const char* realsensetopointcloud_spec[] =
  {
    "implementation_id", "RealsenseToPointCloud",
    "type_name",         "RealsenseToPointCloud",
    "description",       "Creating Point Cloud and RGBD CameraImage from Realsense Camera",
    "version",           "1.0.0",
    "vendor",            "Masayuki Fukao, Robot System Design Laboratory, Meijo University",
    "category",          "Pointcloud",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.Image_width", "1280",
    "conf.default.Image_height", "720",
    "conf.default.depth_width", "1280",
    "conf.default.depth_height", "720",

    // Widget
    "conf.__widget__.Image_width", "text",
    "conf.__widget__.Image_height", "text",
    "conf.__widget__.depth_width", "text",
    "conf.__widget__.depth_height", "text",
    // Constraints

    "conf.__type__.Image_width", "int",
    "conf.__type__.Image_height", "int",
    "conf.__type__.depth_width", "int",
    "conf.__type__.depth_height", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RealsenseToPointCloud::RealsenseToPointCloud(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_RGBDCameraImageOut("RGBDCameraImage", m_RGBDCameraImage),
    m_OutPointCloudOut("OutPointCloud", m_OutPointCloud)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RealsenseToPointCloud::~RealsenseToPointCloud()
{
}



RTC::ReturnCode_t RealsenseToPointCloud::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("RGBDCameraImage", m_RGBDCameraImageOut);
  addOutPort("OutPointCloud", m_OutPointCloudOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("Image_width", m_Image_width, "1280");
  bindParameter("Image_height", m_Image_height, "720");
  bindParameter("depth_width", m_depth_width, "1280");
  bindParameter("depth_height", m_depth_height, "720");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

RTC::ReturnCode_t RealsenseToPointCloud::onActivated(RTC::UniqueId ec_id)
{
  m_RGBDCameraImage.data.cameraImage.image.format = Img::CF_RGB;
  m_RGBDCameraImage.data.cameraImage.intrinsic.distortion_coefficient.length(5);
  
  //デプスとカラーデータのゲット
  cfg.enable_stream(RS2_STREAM_DEPTH);
  cfg.enable_stream(RS2_STREAM_COLOR);
  rs2::pipeline_profile selection = pipe.start(cfg);

  //カメラ内部情報の取得
  auto color_stream = selection.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
  auto depth_stream = selection.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>(); 
  rs2_intrinsics ci = color_stream.get_intrinsics();
  //rs2_extrinsics ei = color_stream.get_extrinsics();
  
  rs2_distortion model = ci.model;
  //カメラパラメータの格納
  m_RGBDCameraImage.data.cameraImage.intrinsic.matrix_element[0] = ci.fx;
  m_RGBDCameraImage.data.cameraImage.intrinsic.matrix_element[1] = ci.ppx;
  m_RGBDCameraImage.data.cameraImage.intrinsic.matrix_element[2] = ci.fy;
  m_RGBDCameraImage.data.cameraImage.intrinsic.matrix_element[3] = ci.ppy;
  
  for(int i = 0;i<5;i++){
    
    m_RGBDCameraImage.data.cameraImage.intrinsic.distortion_coefficient[i] = ci.coeffs[i];

  }
 
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealsenseToPointCloud::onDeactivated(RTC::UniqueId ec_id)
{
  pipe.stop();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealsenseToPointCloud::onExecute(RTC::UniqueId ec_id)
{
  //カメラデータの取得
  rs2::frameset frameset = pipe.wait_for_frames();
  //アライン
  //rs2::align align_to_depth(RS2_STREAM_DEPTH);
  rs2::align align_to_color(RS2_STREAM_COLOR);
  //アライン（カラーデータにデプス情報を合わせる）
  frameset = align_to_color.process(frameset);
  
  //カラーデータとデプスデータをそれぞれで取得
  rs2::frame Imagecolor = frameset.get_color_frame();
  rs2::frame depth = frameset.get_depth_frame();

  //幅と高さの格納
  m_RGBDCameraImage.data.cameraImage.image.width = Imagecolor.as<rs2::video_frame>().get_width();
  m_RGBDCameraImage.data.cameraImage.image.height = Imagecolor.as<rs2::video_frame>().get_height();
  m_OutPointCloud.width  = Imagecolor.as<rs2::video_frame>().get_width();
  m_OutPointCloud.height = Imagecolor.as<rs2::video_frame>().get_height();

  m_RGBDCameraImage.data.depthImage.width = depth.as<rs2::video_frame>().get_width();
  m_RGBDCameraImage.data.depthImage.height = depth.as<rs2::video_frame>().get_height();

  //メモリの確保
  m_RGBDCameraImage.data.cameraImage.image.raw_data.length(m_RGBDCameraImage.data.cameraImage.image.width * m_RGBDCameraImage.data.cameraImage.image.height * 3);

  m_RGBDCameraImage.data.depthImage.raw_data.length(m_RGBDCameraImage.data.depthImage.width * m_RGBDCameraImage.data.depthImage.height );

  m_OutPointCloud.raw_RGBdata.length(m_OutPointCloud.width * m_OutPointCloud.height * 3);
  m_OutPointCloud.raw_XYZdata.length(m_OutPointCloud.width * m_OutPointCloud.height * 3);

  //カラーデータの格納
  //r : m_RGBDCameraImage.data.cameraImage.image.raw_data[Imagecounter]
  //g : m_RGBDCameraImage.data.cameraImage.image.raw_data[Imagecounter+1]
  //b : m_RGBDCameraImage.data.cameraImage.image.raw_data[Imagecounter+2]
  
  uint8_t* Imagecolordata = (uint8_t*)Imagecolor.get_data();
  
  for(int i = 0 ; i < m_RGBDCameraImage.data.cameraImage.image.height ; i++){
    for(int j = 0 ; j < m_RGBDCameraImage.data.cameraImage.image.width ; j ++){
      int Imagecounter = i * m_RGBDCameraImage.data.cameraImage.image.width * 3 + j * 3;
      
      m_RGBDCameraImage.data.cameraImage.image.raw_data[Imagecounter + 2] = Imagecolordata[Imagecounter + 2];
      m_RGBDCameraImage.data.cameraImage.image.raw_data[Imagecounter + 1] = Imagecolordata[Imagecounter + 1];
      m_RGBDCameraImage.data.cameraImage.image.raw_data[Imagecounter] = Imagecolordata[Imagecounter];
      //
      m_OutPointCloud.raw_RGBdata[Imagecounter + 2] = Imagecolordata[Imagecounter + 2];
      m_OutPointCloud.raw_RGBdata[Imagecounter + 1] = Imagecolordata[Imagecounter + 1];
      m_OutPointCloud.raw_RGBdata[Imagecounter ] = Imagecolordata[Imagecounter ];

    }
  }
  //デプスデータの格納
  uint8_t* depthdata = (uint8_t*)depth.get_data();
  for(int i = 0 ; i < m_RGBDCameraImage.data.depthImage.height ; i++){
    for(int j = 0 ; j < m_RGBDCameraImage.data.depthImage.width ; j ++){
      //
      int depthcounter = i * m_RGBDCameraImage.data.depthImage.width * 2  + j * 2 + 1 ;
      int depthcounter1 = i * m_RGBDCameraImage.data.depthImage.width  + j  ;
      m_RGBDCameraImage.data.depthImage.raw_data[depthcounter1] =  depthdata[depthcounter];
      
    }
  }

  m_RGBDCameraImageOut.write();

  //カラーデータ格納
  pointcloud.map_to(Imagecolor);

  //格納されたカラーデータとデプスデータを用いて点群の作成
  points = pointcloud.calculate(depth);
  
  //点群データのみを取り出す
  auto Vertex = points.get_vertices();
  
  //点群データの格納
  for (int i = 0; i < m_OutPointCloud.height ; i++){
    for(int j = 0 ; j < m_OutPointCloud.width ; j++){
      //メモリ番地を決める
      int XYZcounter = i * m_OutPointCloud.width * 3 + j * 3;
      int vertexcounter = i * m_OutPointCloud.width + j;

      m_OutPointCloud.raw_XYZdata[XYZcounter] = Vertex[vertexcounter].x;
      m_OutPointCloud.raw_XYZdata[XYZcounter + 1] = Vertex[vertexcounter].y;
      m_OutPointCloud.raw_XYZdata[XYZcounter + 2] = Vertex[vertexcounter].z;

    }
  }

  m_OutPointCloudOut.write();
  
  return RTC::RTC_OK;
}

extern "C"
{
 
  void RealsenseToPointCloudInit(RTC::Manager* manager)
  {
    coil::Properties profile(realsensetopointcloud_spec);
    manager->registerFactory(profile,
                             RTC::Create<RealsenseToPointCloud>,
                             RTC::Delete<RealsenseToPointCloud>);
  }
  
};


