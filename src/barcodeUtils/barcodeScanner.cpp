#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <string>
#include <iostream>
#include "menuUtils.h"

std::string scanRobotBarcode(bool headless = false) {
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

    int frames = 0;
    const int maxFrames = headless ? 300 : -1; // ~10 seconds in headless mode

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        zbar::Image image(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
        scanner.scan(image);

        for (auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
            barcodeData = symbol->get_data();

        if (!headless) {
            cv::imshow("Scan Barcode", frame);
            int key = cv::waitKey(1);
            if (!barcodeData.empty() || key == 27) break;
        } else if (!barcodeData.empty()) {
            break;
        }

        if (headless && frames++ >= maxFrames) break;
    }

    cap.release();
    if (!headless) {
        cv::destroyAllWindows();
        cv::waitKey(1);
    }

    if (barcodeData.size() >= 12) {
        barcodeData = barcodeData.substr(5, 7); // positions 6–12
    }

    // If headless and barcode not found, prompt user automatically
    if (headless && barcodeData.empty()) {
        std::cout << "Barcode not detected. Enter student ID manually: s";
        std::cin >> barcodeData;
    }

    return barcodeData;
}