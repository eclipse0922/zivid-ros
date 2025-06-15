#include <pybind11/pybind11.h>
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <zivid_interfaces/srv/is_connected.hpp>
#include <zivid_interfaces/srv/capture_and_save.hpp>
#include <zivid_interfaces/srv/capture_assistant_suggest_settings.hpp>
#include <zivid_interfaces/srv/camera_info_model_name.hpp>
#include <zivid_interfaces/srv/camera_info_serial_number.hpp>

#include "zivid_camera/zivid_camera.hpp"

namespace py = pybind11;
using namespace zivid_camera;

class ZividCameraPrivateAccess
{
public:
  static void capture(ZividCamera & cam)
  {
    auto req = std::make_shared<std_srvs::srv::Trigger::Request>();
    auto resp = std::make_shared<std_srvs::srv::Trigger::Response>();
    cam.captureServiceHandler(nullptr, req, resp);
    if(!resp->success)
    {
      throw std::runtime_error(resp->message);
    }
  }

  static void capture2D(ZividCamera & cam)
  {
    auto req = std::make_shared<std_srvs::srv::Trigger::Request>();
    auto resp = std::make_shared<std_srvs::srv::Trigger::Response>();
    cam.capture2DServiceHandler(nullptr, req, resp);
    if(!resp->success)
    {
      throw std::runtime_error(resp->message);
    }
  }

  static bool isConnected(ZividCamera & cam)
  {
    auto req = std::make_shared<zivid_interfaces::srv::IsConnected::Request>();
    auto resp = std::make_shared<zivid_interfaces::srv::IsConnected::Response>();
    cam.isConnectedServiceHandler(nullptr, req, resp);
    return resp->is_connected;
  }

  static void captureAndSave(ZividCamera & cam, const std::string & file_path)
  {
    auto req = std::make_shared<zivid_interfaces::srv::CaptureAndSave::Request>();
    req->file_path = file_path;
    auto resp = std::make_shared<zivid_interfaces::srv::CaptureAndSave::Response>();
    cam.captureAndSaveServiceHandler(nullptr, req, resp);
    if(!resp->success)
    {
      throw std::runtime_error(resp->message);
    }
  }

  static std::string captureAssistantSuggestSettings(
    ZividCamera & cam,
    double max_capture_time = 1.0,
    uint8_t ambient_light_frequency =
      zivid_interfaces::srv::CaptureAssistantSuggestSettings::Request::AMBIENT_LIGHT_FREQUENCY_NONE)
  {
    auto req = std::make_shared<zivid_interfaces::srv::CaptureAssistantSuggestSettings::Request>();
    req->max_capture_time.sec = static_cast<int32_t>(max_capture_time);
    req->max_capture_time.nanosec = static_cast<uint32_t>((max_capture_time - req->max_capture_time.sec) * 1e9);
    req->ambient_light_frequency = ambient_light_frequency;
    auto resp = std::make_shared<zivid_interfaces::srv::CaptureAssistantSuggestSettings::Response>();
    cam.captureAssistantSuggestSettingsServiceHandler(nullptr, req, resp);
    if(!resp->success)
    {
      throw std::runtime_error(resp->message);
    }
    return resp->suggested_settings;
  }

  static std::string cameraInfoModelName(ZividCamera & cam)
  {
    auto req = std::make_shared<zivid_interfaces::srv::CameraInfoModelName::Request>();
    auto resp = std::make_shared<zivid_interfaces::srv::CameraInfoModelName::Response>();
    cam.cameraInfoModelNameServiceHandler(nullptr, req, resp);
    return resp->model_name;
  }

  static std::string cameraInfoSerialNumber(ZividCamera & cam)
  {
    auto req = std::make_shared<zivid_interfaces::srv::CameraInfoSerialNumber::Request>();
    auto resp = std::make_shared<zivid_interfaces::srv::CameraInfoSerialNumber::Response>();
    cam.cameraInfoSerialNumberServiceHandler(nullptr, req, resp);
    return resp->serial_number;
  }
};

PYBIND11_MODULE(zivid_camera_pybind, m)
{
  py::class_<ZividCamera, std::shared_ptr<ZividCamera>>(m, "ZividCamera")
      .def(py::init([]() {
        rclcpp::NodeOptions options;
        return std::make_shared<ZividCamera>(options);
      }))
      .def("capture", &ZividCameraPrivateAccess::capture)
      .def("capture_2d", &ZividCameraPrivateAccess::capture2D)
      .def("is_connected", &ZividCameraPrivateAccess::isConnected)
      .def(
        "capture_and_save",
        &ZividCameraPrivateAccess::captureAndSave,
        py::arg("file_path"))
      .def(
        "capture_assistant_suggest_settings",
        &ZividCameraPrivateAccess::captureAssistantSuggestSettings,
        py::arg("max_capture_time") = 1.0,
        py::arg("ambient_light_frequency") =
          zivid_interfaces::srv::CaptureAssistantSuggestSettings::Request::AMBIENT_LIGHT_FREQUENCY_NONE)
      .def("camera_info_model_name", &ZividCameraPrivateAccess::cameraInfoModelName)
      .def(
        "camera_info_serial_number",
        &ZividCameraPrivateAccess::cameraInfoSerialNumber);
}

