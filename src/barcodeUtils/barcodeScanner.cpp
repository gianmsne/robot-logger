#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <string>
#include <iostream>
#include "menuUtils.h"

std::string scanRobotBarcode() {
    printStartText();
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Cannot open webcam.\n";
        return "";
    }

    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    cv::Mat frame, gray;
    std::string barcodeData = "";

    
    //std::cout << "Scanning barcode... Press ESC to cancel.\n";

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        zbar::Image image(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
        scanner.scan(image);

        for (auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            barcodeData = symbol->get_data();
        }

        cv::imshow("Scan Barcode", frame);
        int key = cv::waitKey(1);  // process GUI events
        if (!barcodeData.empty() || key == 27) {  // ESC or barcode found
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    cv::waitKey(1);  // ensure windows close on some platforms

    // If ESC was pressed, return empty string for manual entry
    if (barcodeData.empty()) return "";

    // Extract substring positions 6–12 (1-based)
    if (barcodeData.size() >= 12) {
        barcodeData = barcodeData.substr(5, 7);
    }

    return barcodeData;
}