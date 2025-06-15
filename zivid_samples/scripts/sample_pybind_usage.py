#!/usr/bin/env python

import zivid_camera_pybind


def main():
    cam = zivid_camera_pybind.ZividCamera()
    if not cam.is_connected():
        print("Camera not connected")
        return

    print("Model:", cam.camera_info_model_name())
    print("Serial:", cam.camera_info_serial_number())
    cam.capture()
    cam.capture_2d()
    cam.capture_and_save("/tmp/sample_pybind.zdf")
    yaml = cam.capture_assistant_suggest_settings()
    print("Suggested settings length:", len(yaml))


if __name__ == "__main__":
    main()
