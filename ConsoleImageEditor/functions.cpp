//
// Created by DUTCH on 2/14/2022.
//
#include "functions.h"

int applyLinearFilter(const Mat &src, Mat &dst)
{
    const float kernelData[] = {-0.1f, 0.2f, -0.1f,
                                0.2f, 3.0f, 0.2f,
                                -0.1f, 0.2f, -0.1f};

    const Mat kernel(3, 3, CV_32FC1,
                     (float *)kernelData);

    filter2D(src, dst, -1, kernel);
    return 0;
}

int applyBlur(const Mat &src, Mat &dst)
{
    int imgHeight, imgWidth;
    Size kernelSize(3, 3);
    imgHeight = src.size().height;
    imgWidth = src.size().width;
    do
    {
        printf("Set kernel size: \n");
        printf("(> 3, < max (img.width, img.height))");
        printf("\theight: ");
        scanf("%d", &kernelSize.height);
        printf("\twidth: ");
        scanf("%d", &kernelSize.width);
    }
    while ((kernelSize.width < 3 || kernelSize.width > imgWidth) ||
            (kernelSize.height < 3 || kernelSize.height > imgHeight));
    blur(src, dst, kernelSize);

    return 0;
}

int applyMedianBlur(const Mat &src, Mat &dst)
{
    int kSize = 3, minDim = -1;
    minDim = min(src.size().height, src.size().width);
    do
    {
        printf("Set kernel size:");
        printf("(odd, > 3, < -1");
        scanf("%d", &kSize);
    }
    while (kSize < 3 || kSize > minDim || kSize %2 == 0);
    medianBlur(src, dst, kSize);

    return 0;
}

int applyGaussianBlur(const Mat &src, Mat &dst)
{
    Size kernelSize(5, 5);
    double sigmaX = 0, sigmaY = 0;
    do
    {
        printf("Set kernel size: (0x0 for standard deviation)\n");
        printf(" value > 0 && value is odd \n");
        printf("(0x0 for standard deviation)\n");
        printf("\theight: ");
        scanf("%d", &kernelSize.height);
        printf("\twidth: ");
        scanf("%d", &kernelSize.width);
        if (kernelSize.width == 0 && kernelSize.height == 0){ sigmaY = 1; sigmaX = 1;}
    }
    while ((kernelSize.width < 0 || (((kernelSize.width % 2) == 0 )&& kernelSize.width !=0 ))&&
            (kernelSize.height < 0 || (((kernelSize.height % 2) == 0)&& kernelSize.width !=0 )));

    GaussianBlur(src,dst,kernelSize,sigmaX,sigmaY);

    return 0;
}

Mat applyErode(const Mat &src, Mat &dst)
{
    Mat erode_src,element;
    cvtColor(src,erode_src, COLOR_RGB2GRAY);
    element = Mat();
    erode(erode_src, dst, element);

    return erode_src;
}

Mat applyDilate(const Mat &src, Mat &dst)
{
    Mat dilate_src,element;
    cvtColor(src,dilate_src, COLOR_RGB2GRAY);
    element = Mat();
    dilate(dilate_src, dst, element);

    return dilate_src;
}

std::pair<Mat, Mat> applySobel(const Mat &src, Mat &dst)
{
    int ddepth = CV_16S;
    double alpha = 0.5, beta = 0.5;
    Mat grayImg, xGrad, yGrad, xGradAbs, yGradAbs, grad, Sobel_src;
    GaussianBlur(src, Sobel_src, Size(3,3),
                 0, 0, BORDER_DEFAULT);
    cvtColor(Sobel_src, grayImg, COLOR_RGB2GRAY);

    Sobel(grayImg, xGrad, ddepth, 1, 0); // по Ox
    Sobel(grayImg, yGrad, ddepth, 0, 1); // по Oy
    // преобразование градиентов в 8-битные
    convertScaleAbs(xGrad, xGradAbs);
    convertScaleAbs(yGrad, yGradAbs);
    // поэлементное вычисление взвешенной суммы
    addWeighted(xGradAbs,alpha,yGradAbs,beta,0,dst);

    return {xGradAbs, yGradAbs};
}

int applyLaplacian(const Mat &src, Mat &dst)
{
    Mat laplacian_src, grayImg, laplacianImg;
    int ddepth = CV_16S;
    GaussianBlur(src, laplacian_src, Size(3,3),0,0);
    cvtColor(laplacian_src, grayImg, COLOR_RGB2GRAY);

    Laplacian(grayImg, laplacianImg, ddepth);
    convertScaleAbs(laplacianImg, dst);

    return 0;
}

int applyCanny(const Mat &src, Mat &dst)
{
    Mat Canny_src, grayImg;
    double lowThreshold = 70, uppThreshold = 260;

    blur(src, Canny_src, Size(3,3));
    cvtColor(Canny_src, grayImg, COLOR_RGB2GRAY);

    Canny(grayImg, dst, lowThreshold, uppThreshold);

    return 0;
}

int applyCalcHist(const Mat &src, Mat &dst)
{
    Mat bgrChannels[3], bHist, gHist, rHist;
    int kBins = 256;
    float range[] = {0.0f, 256.0f};
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    int histWidth = 512, histHeight = 400;
    int binWidth = cvRound((double)histWidth / kBins);
    int i;
    Scalar colors[] = {Scalar(255, 0, 0),
                       Scalar(0, 255, 0),
                       Scalar(0, 0, 255)};
    split(src, bgrChannels);

    calcHist(&bgrChannels[0], 1, 0, Mat(), bHist, 1,
             &kBins, &histRange, uniform, accumulate);
    calcHist(&bgrChannels[1], 1, 0, Mat(), gHist, 1,
             &kBins, &histRange, uniform, accumulate);
    calcHist(&bgrChannels[2], 1, 0, Mat(), rHist, 1,
             &kBins, &histRange, uniform, accumulate);

    dst = Mat(histHeight, histWidth, CV_8UC3,Scalar(0, 0, 0));

    normalize(bHist, bHist, 0, dst.rows,NORM_MINMAX, -1, Mat());
    normalize(gHist, gHist, 0, dst.rows,NORM_MINMAX, -1, Mat());
    normalize(rHist, rHist, 0, dst.rows,NORM_MINMAX, -1, Mat());

    for (i = 1; i < kBins; i++)
    {
        line(dst, Point(binWidth * (i-1),
                        histHeight-cvRound(bHist.at<float>(i-1))) ,
             Point(binWidth * i,
                   histHeight-cvRound(bHist.at<float>(i)) ),
             colors[0], 2, 8, 0);

        line(dst, Point(binWidth * (i-1),
                        histHeight-cvRound(gHist.at<float>(i-1))) ,
             Point(binWidth * i,
                   histHeight-cvRound(gHist.at<float>(i)) ),
             colors[1], 2, 8, 0);

        line(dst, Point(binWidth * (i-1),
                        histHeight-cvRound(rHist.at<float>(i-1))) ,
             Point(binWidth * i,
                   histHeight-cvRound(rHist.at<float>(i)) ),
             colors[2], 2, 8, 0);
    }
    return 0;
}

Mat applyEqualizeHist(const Mat &src, Mat &dst)
{
    Mat grayImg;
    cvtColor(src, grayImg, COLOR_BGR2GRAY);
    equalizeHist(grayImg, dst);

    return grayImg;
}

int applyOperation(const Mat &src, const int operationIdx)
{
    char key = -1;
    Mat dst;
    switch (operationIdx)
    {
        case 1:
        {
            // "1 - Apply linear filter DONE"
            applyLinearFilter(src, dst);
            break;
        }
        case 2:
        {
            // "2 - Apply blur(...)"
            applyBlur(src, dst);
            imwrite("res_Blur.jpg",dst);
            break;
        }
        case 3:
        {
            // "3 - Apply medianBlur(...)"
            applyMedianBlur(src, dst);
            imwrite("res_medianBlur.jpg",dst);
            break;
        }
        case 4:
        {
            // "4 - Apply GaussianBlur(...)" DONE
            applyGaussianBlur(src, dst);
            imwrite("res_GaussianBlur.jpg",dst);
            break;
        }
        case 5:
        {
            // "5 - Apply erode(...) DONE "
            Mat grayscale = applyErode(src,dst);
            namedWindow(winNames[12], 1);
            imshow(winNames[12], grayscale);
            imwrite("res_erode.jpg",dst);
            break;
        }
        case 6:
        {
            // "6 - Apply dilate(...)"  DONE
            Mat grayscale = applyDilate(src,dst);
            namedWindow(winNames[12], 1);
            imshow(winNames[12], grayscale);
            imwrite("res_dilate.jpg",dst);
            break;
        }
        case 7:
        {
            // "7 - Apply Sobel(...)" DONE
            Mat SobelOX = applySobel(src,dst).first;
            Mat SobelOY = applySobel(src,dst).second;
            namedWindow(winNames[13], 1);
            imshow(winNames[13], SobelOX);
            namedWindow(winNames[14], 1);
            imshow(winNames[14], SobelOY);
            imwrite("res_Sobel_OX.jpg",SobelOX);
            imwrite("res_Sobel_OY.jpg",SobelOY);
            imwrite("res_Sobel.jpg",dst);
            break;
        }
        case 8:
        {
            // "8 - Apply Laplacian(...)"    DONE
            applyLaplacian(src,dst);
            imwrite("res_Laplacian.jpg",dst);
            break;
        }
        case 9:
        {
            // "9 - Apply Canny(...)" DONE
            applyCanny(src, dst);
            imwrite("res_Canny.jpg",dst);
            break;
        }
        case 10:
        {
            // "10 - Apply calcHist(...)" DONE
            applyCalcHist(src, dst);
            imwrite("res_CalcHist.jpg",dst);
            break;
        }
        case 11:
        {
            // "11 - Apply equalizeHist(...)"   DONE
            Mat grayscale = applyEqualizeHist(src,dst);
            namedWindow(winNames[12], 1);
            imshow(winNames[12], grayscale);
            imwrite("res_Equalize.jpg",dst);
            break;
        }
        case 12:
        {
            std::exit(0);
        }
    }
    // show initial image
    namedWindow(winNames[0], 1);
    imshow(winNames[0], src);

    // show processed image
    namedWindow(winNames[operationIdx]);
    imshow(winNames[operationIdx], dst);

    return 0;
}
