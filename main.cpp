#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>

using namespace std;
using namespace cv;

void cancel_keys_except(const string& key);
void press_key(const string& key);
void mouse_movement(const string& direction);

int main()
{
    VideoCapture cap(0);

    cap.set(CAP_PROP_FRAME_WIDTH, 700);
    cap.set(CAP_PROP_FRAME_HEIGHT, 600);

    Mat frame;

    int cont = 0; // get all four fingers


    namedWindow("FrameLeft", WINDOW_NORMAL);
    resizeWindow("FrameLeft", Size(160, 160));
    namedWindow("FrameRight", WINDOW_NORMAL);
    resizeWindow("FrameRight", Size(160, 160));
    namedWindow("FrameUp", WINDOW_NORMAL);
    resizeWindow("FrameUp", Size(160, 160));
    namedWindow("FrameDown", WINDOW_NORMAL);
    resizeWindow("FrameDown", Size(160, 160));
    namedWindow("FrameEnter", WINDOW_NORMAL);
    resizeWindow("FrameEnter", Size(160, 160));

    while(true) {
        cap.read(frame);
        flip(frame, frame, 1);

        // define left side
        rectangle(frame, Point(320, 300), Point(400, 370), Scalar(255,255,255), 2);

        // define down side
        rectangle(frame, Point(420, 390), Point(500, 460), Scalar(255,255,255), 2);

        // define right side
        rectangle(frame, Point(520, 300), Point(600, 370), Scalar(255,255,255), 2);

        // define up side
        rectangle(frame, Point(420, 210), Point(500, 280), Scalar(255,255,255), 2);

        // define enter key
        rectangle(frame, Point(40, 390), Point(120, 460), Scalar(255,255,255), 2);

        // getting rectangle area to analysis correctly each button
        Mat roiImageLeft = frame(Rect(Point(320, 300), Point(400, 370)));
        Mat roiImageRight = frame(Rect(Point(520, 300), Point(600, 370)));
        Mat roiImageUp = frame(Rect(Point(420, 210), Point(500, 280)));
        Mat roiImageDown = frame(Rect(Point(420, 390), Point(500, 460)));
        Mat roiImageEnter = frame(Rect(Point(40, 390), Point(120, 460)));

        // convert to HSV to facilitate the detection of the color
        Mat hsvroiImageLeft, hsvroiImageRight, hsvroiImageUp, hsvroiImageDown, hsvroiImageEnter;

        cvtColor(roiImageLeft, hsvroiImageLeft, COLOR_BGR2HSV);
        cvtColor(roiImageRight, hsvroiImageRight, COLOR_BGR2HSV);
        cvtColor(roiImageUp, hsvroiImageUp, COLOR_BGR2HSV);
        cvtColor(roiImageDown, hsvroiImageDown, COLOR_BGR2HSV);
        cvtColor(roiImageEnter, hsvroiImageEnter, COLOR_BGR2HSV);

        // checking color for each rectangle
        Mat maskLeft, maskRight, maskUp, MaskDown, maskEnter;
        inRange(hsvroiImageLeft, Scalar(100, 150, 0), Scalar(140, 255, 255), maskLeft);
        inRange(hsvroiImageRight, Scalar(100, 150, 0), Scalar(140, 255, 255), maskRight);
        inRange(hsvroiImageUp, Scalar(100, 150, 0), Scalar(140, 255, 255), maskUp);
        inRange(hsvroiImageDown, Scalar(100, 150, 0), Scalar(140, 255, 255), MaskDown);
        inRange(hsvroiImageEnter, Scalar(100, 150, 0), Scalar(140, 255, 255), maskEnter);


        Mat kernel = Mat::ones(Size(7,7), CV_8U);
        dilate(maskUp, maskUp, kernel);
        dilate(maskLeft, maskLeft, kernel);
        dilate(maskRight, maskRight, kernel);
        dilate(MaskDown, MaskDown, kernel);
        dilate(maskEnter, maskEnter, kernel);

        // obtaining amount of color for each square.
        int contLeft = countNonZero(maskLeft);
        int contRight = countNonZero(maskRight);
        int contUp = countNonZero(maskUp);
        int contDown = countNonZero(MaskDown);
        int contEnter = countNonZero(maskEnter);

        cout << "Cont left: " << contLeft << endl;
        cout << "Cont right: " << contRight << endl;
        cout << "Cont up: " << contUp << endl;
        cout << "Cont down: " << contDown << endl;
        cout << "Cont Enter: " << contEnter << endl;

        if (contLeft > contRight && contLeft > contUp && contLeft > contDown && contLeft > contEnter && contLeft > 400) {
            cancel_keys_except("left");
            press_key("left");
        } else if (contRight > contLeft && contRight > contUp && contRight > contDown && contRight > contEnter && contRight > 400) {
            cancel_keys_except("right");
            press_key("right");
        } else if (contUp > contRight && contUp > contLeft && contUp > contDown && contUp > contEnter && contUp > 400) {
            cancel_keys_except("up");
            press_key("up");
        } else if (contDown > contRight && contDown > contUp && contDown > contLeft && contDown > contEnter && contDown > 400) {
            cancel_keys_except("down");
            press_key("down");
        } else if (contEnter > contRight && contEnter > contUp && contEnter > contLeft && contEnter > contDown && contEnter > 400) {
            cancel_keys_except("enter");
            press_key("enter");
        } else {
            cancel_keys_except("");
        }

        imshow("Frame", frame);
        imshow("FrameLeft", maskLeft);
        imshow("FrameRight", maskRight);
        imshow("FrameUp", maskUp);
        imshow("FrameDown", MaskDown);
        imshow("FrameEnter", maskEnter);

        waitKey(1);
    }
    return 0;
}


// this method is used to move the mouse
void mouse_movement(const string& direction) {
    POINT pos;
    GetCursorPos(&pos);
    if (direction == "up") {
        pos.y-=20;
    } else if (direction == "left") {
        pos.x-=20;
    } else if (direction == "right") {
        pos.x+=20;
    } else if (direction == "down") {
        pos.y+=20;
    }

    SetCursorPos(pos.x, pos.y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, pos.x, pos.y, 0, 0);
}

// method used to pass the arrow key to be pressed
void press_key(const string& key) {
    if (key == "up") {
        keybd_event(VK_UP, 0, 0, 0);
    } else if (key == "left") {
        keybd_event(VK_LEFT, 0, 0, 0);
    } else if (key == "right") {
        keybd_event(VK_RIGHT, 0, 0, 0);
    } else if (key == "down") {
        keybd_event(VK_DOWN, 0, 0, 0);
    }  else if (key == "enter") {
        keybd_event(VK_RETURN, 0, 0, 0);
    }
}

// method used to unpressed other keys, if the user wants to press some keys, others must be unpressed
void cancel_keys_except(const string& key) {
    if (key == "up") {
        keybd_event(VK_DOWN, 0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
    } else if (key == "left") {
        keybd_event(VK_DOWN, 0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
    } else if (key == "right") {
        keybd_event(VK_DOWN, 0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
    } else if (key == "down") {
        keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
    } else if (key == "enter") {
        keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_DOWN,0,KEYEVENTF_KEYUP,0);
    } else if (key == "") {
        keybd_event(VK_DOWN, 0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
    }
}

