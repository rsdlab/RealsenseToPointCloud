// -*- C++ -*-
/*!
 * @file  RealsenseToPointCloud.h
 * @brief Creating Point Cloud and RGBD CameraImage from Realsense Camera
 * @date  $Date$
 *
 * $Id$
 */

#ifndef REALSENSETOPOINTCLOUD_H
#define REALSENSETOPOINTCLOUD_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">
#include "DepthCameraStub.h"
#include "pointcloudStub.h"

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="port_stub_h">
using namespace RGBDCamera;
using namespace PointCloudTypes;
// </rtc-template>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

using namespace RTC;

//Include file for Librealsense functions
#include <librealsense2/rs.hpp>

//Include file for OpenCV functions
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

#include <iostream>
#include <algorithm> 
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/thread/thread.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>

/*!
 * @class RealsenseToPointCloud
 * @brief Creating Point Cloud and RGBD CameraImage from Realsense Camera
 *
 */
class RealsenseToPointCloud
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  RealsenseToPointCloud(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~RealsenseToPointCloud();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  Image_width
   * - DefaultValue: 1280
   */
  int m_Image_width;
  /*!
   * 
   * - Name:  Image_height
   * - DefaultValue: 720
   */
  int m_Image_height;
  /*!
   * 
   * - Name:  depth_width
   * - DefaultValue: 1280
   */
  int m_depth_width;
  /*!
   * 
   * - Name:  depth_height
   * - DefaultValue: 720
   */
  int m_depth_height;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RGBDCamera::TimedRGBDCameraImage m_RGBDCameraImage;
  /*!
   */
  OutPort<RGBDCamera::TimedRGBDCameraImage> m_RGBDCameraImageOut;
  PointCloudTypes::PointCloud m_OutPointCloud;
  /*!
   */
  OutPort<PointCloudTypes::PointCloud> m_OutPointCloudOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

  // <librealsense>
  rs2::pipeline pipe;
  rs2::config cfg;
  rs2::pointcloud pointcloud;
  rs2::points points;
  // <opencv>
  //cv::Mat CameraMatrix1,DistCoeffs1;
  //cv::Mat CameraMatrix(3,3,CV_8UC1);
  //cv::Mat DistCoeffs(1,5,CV_8UC1);

};


extern "C"
{
  DLL_EXPORT void RealsenseToPointCloudInit(RTC::Manager* manager);
};

#endif // REALSENSETOPOINTCLOUD_H
