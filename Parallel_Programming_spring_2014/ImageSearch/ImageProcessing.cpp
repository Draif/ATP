/*
 * =====================================================================================
 *
 *       Filename:  ImageProcessing.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.05.2014 00:14:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#include "ImageProcessing.hpp"

ImageTuple ConvertToTuple(const std::string path, bool storeImage, const size_t size)  { 
    if (path.empty())
        throw std::runtime_error("Path is empty!");
    
    cv::Mat sourceImage;
    sourceImage = cv::imread(path.c_str(), 1);

    if (!sourceImage.data) {
        throw std::runtime_error("No image data!");
    }

    cv::Size newSize(size, size);
    
    cv::Mat tuple;
    resize(sourceImage, tuple, newSize);

    ImageVector vec;
    for (size_t i = 0; i < size; ++i)
        for (size_t j = 0; j < size; ++j) {
            cv::Vec3b pixel = tuple.at<cv::Vec3b>(i, j);
            RGB rgb (pixel[2], pixel[1], pixel[0]);
            Pixel p (i, j, rgb);
            vec.AddPixel(p);
        }
    ImageTuple returnTuple(vec, path, size);
    
    if (storeImage)
        returnTuple.AddImage(sourceImage);
    
    return returnTuple;
}


