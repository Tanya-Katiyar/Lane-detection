#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void detectLanes(Mat& image);

// Function to detect lanes in the video

void detectLanes(Mat& image) { 

    Mat gray, binary, img, edges;

    cvtColor(image, gray, COLOR_BGR2GRAY);            //converting to gray image

    // threshold(gray, binary, 100, 255, cv::THRESH_BINARY);  //converting to binary image

    GaussianBlur(gray, gray, Size(5,5), 2,2);          // noise removal

    Mat kernel = getStructuringElement(MORPH_CROSS, Size(5,5));   //morphologocal operations
    dilate(gray, gray, kernel);
    erode(gray, gray, kernel);

    Canny(gray, edges, 50, 100);

    int width = edges.cols;
    int height = edges.rows; 

    Mat mask(edges.size(), edges.type(), Scalar::all(0));        //mask
    Mat masked_img(edges.size(), edges.type(), Scalar::all(0));

    std::vector<Point> pts(4);

    pts[0] = cv::Point{3 * width / 8, height / 2};
    pts[1] = cv::Point{width / 6, height};
    pts[2] = cv::Point{5 * width / 6, height};
    pts[3] = cv::Point{13 * width / 24, height / 2};


    fillPoly(mask, pts, Scalar(255,255,255));              // creating a mask polygon using the points.

    bitwise_and(edges, mask, masked_img);                 // and operation to get the Region of Interest.
    imshow("mask", masked_img);

    std::vector<Vec4i> lines;
    HoughLinesP(masked_img, lines, 1, CV_PI/180, 50, 50, 10);

    for(size_t i=0; i<lines.size() ;i++ ) {

        Vec4i l = lines[i];
        line(image,Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,255), 2, LINE_AA,0);
        
    }

    imshow("lanes", image);


}

int main() {

    Mat frame; 

    // Input video
    VideoCapture cap("/home/kpit/dev/Opencv/Mini-Projects/Road-detection/build/test2.mp4");   

    while(true) {

        cap >> frame;   // capturing frames

        if(frame.empty()){
            break;
        }

        detectLanes(frame);

        int j = waitKey(10);

        if(j=='q'){
            break;
        }
    }
    cap.release();
    return 0;
}



