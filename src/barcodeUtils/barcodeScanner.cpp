#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <string>
#include <iostream>
#include "menuUtils.h"


// Max frames to scan in headless mode 
const int MAX_HEADLESS_FRAMES = 300; // (~10 seconds at 30 FPS)

std::string scanRobotBarcode(bool camera = false, bool headless = false) {
    printStartTextScan();

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Cannot open webcam.\n";
        return "";
    }

    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    cv::Mat frame, gray;
    std::string barcodeData = "";
    int frameCount = 0;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        frameCount++;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        zbar::Image image(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
        scanner.scan(image);

        for (auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            barcodeData = symbol->get_data();
        }

        if (camera) {  // GUI mode
            cv::imshow("Scan Barcode", frame);
            int key = cv::waitKey(1);
            if (!barcodeData.empty() || key == 27) break;
        } else if (headless) {  // headless mode
            if (!barcodeData.empty() || frameCount >= MAX_HEADLESS_FRAMES) {
                if (barcodeData.empty())
                    std::cout << "\n                        Barcode reader timed out. Please manually enter your ID." << std::endl;
                break;
            }
        }
    }

    cap.release();
    if (camera) {  // only destroy windows if GUI was used
        cv::destroyAllWindows();
        cv::waitKey(1);
    }

    if (barcodeData.size() >= 12) barcodeData = barcodeData.substr(5, 7);

    return barcodeData;
}