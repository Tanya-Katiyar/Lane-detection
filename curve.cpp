q#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void detectLanes(Mat& image);

// Function to detect lanes in the video

void detectLanes(Mat& image) { 

    Mat gray, binary, img, edges;

    cvtColor(image, gray, COLOR_BGR2GRAY);            //converting to gray image

    GaussianBlur(gray, gray, Size(5,5), 2,2);          // noise removal

    
    Canny(gray, edges, 40, 150);

    int width = edges.cols;
    int height = edges.rows; 

    Mat mask(edges.size(), edges.type(), Scalar::all(0));        //mask
    Mat masked_img(edges.size(), edges.type(), Scalar::all(0));

    std::vector<Point> pts(4);

    //points for defining the ROI

    pts[0] = Point(2*image.cols/4+130, 3*image.rows/5+40);
  
    pts[1]= Point(1.7*image.cols/4, 3*image.rows/5+40);

    pts[2]= Point(image.cols/4-100, image.rows-60);
    
    pts[3]= Point(7*image.cols/8, image.rows-60);


    fillPoly(mask, pts, Scalar(255,255,255));              // creating a mask polygon using the points.

    bitwise_and(edges, mask, masked_img);                 // and operation to get the Region of Interest.

    imshow("mask", masked_img);

    std::vector<Vec4i> lines;  //accumulator 
    HoughLinesP(masked_img, lines, 1, CV_PI/180, 50, 50, 10);

    for(size_t i=0; i<lines.size() ;i++ ) {

        Vec4i l = lines[i];
        line(image,Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,255), 2, LINE_AA,0);
        
    }

    imshow("image", image);
}


int main () {

    Mat frame;
    
    VideoCapture cap("/home/kpit/dev/Opencv/Mini-Projects/Road-detection/build/project_video.mp4"); 

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
