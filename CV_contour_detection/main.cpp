#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    const char *srcWinName = "original", *contourWinName = "contour", *grayscaleWinName = "grayscale",
    *threshWinName = "threshold";
    namedWindow(srcWinName, 1);
    namedWindow(contourWinName,1);
    namedWindow(grayscaleWinName,1);
    namedWindow(threshWinName,1);

    //load original image
    Mat src = imread(R"(C:\Users\DUTCH\CLionProjects\CV_LR1_Apple\apple.bmp)", 1);
    if (src.data == nullptr)
    {
        printf("Incorrect image name or format.\n");
        return 1;
    }
    //create a single channel 1 byte image (i.e. gray-level image)
    //make a copy of the original image to draw the detected contour
    Mat copy = src.clone();
    Mat gray, grayThresh;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //show grayscale image
    imshow(grayscaleWinName, gray);

    threshold(gray, grayThresh, 60, 255, THRESH_BINARY);
    //show grayscale image after threshold
    imshow(threshWinName, grayThresh);

    //find the contour
    std::vector<std::vector<Point> > contours;
    findContours( grayThresh, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    Scalar color( 0, 255, 0 );
    drawContours(copy, contours, -1, color, 2);
    imshow(contourWinName, copy);
    imwrite("result.jpg", copy);
    imshow(srcWinName, src);
    waitKey(0);
    gray.release();
    grayThresh.release();
    copy.release();
    src.release();
    return 0;
}