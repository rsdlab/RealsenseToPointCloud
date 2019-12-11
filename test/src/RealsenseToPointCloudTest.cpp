// -*- C++ -*-
/*!
 * @file  RealsenseToPointCloudTest.cpp
 * @brief Creating Point Cloud and RGBD CameraImage from Realsense Camera
 * @date $Date$
 *
 * $Id$
 */

#include "RealsenseToPointCloudTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* realsensetopointcloud_spec[] =
  {
    "implementation_id", "RealsenseToPointCloudTest",
    "type_name",         "RealsenseToPointCloudTest",
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
RealsenseToPointCloudTest::RealsenseToPointCloudTest(RTC::Manager* manager)
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
RealsenseToPointCloudTest::~RealsenseToPointCloudTest()
{
}



RTC::ReturnCode_t RealsenseToPointCloudTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("RGBDCameraImage", m_RGBDCameraImageIn);
  addInPort("OutPointCloud", m_OutPointCloudIn);
  
  // Set OutPort buffer
  
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

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RealsenseToPointCloudTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealsenseToPointCloudTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealsenseToPointCloudTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealsenseToPointCloudTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RealsenseToPointCloudTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(realsensetopointcloud_spec);
    manager->registerFactory(profile,
                             RTC::Create<RealsenseToPointCloudTest>,
                             RTC::Delete<RealsenseToPointCloudTest>);
  }
  
};


