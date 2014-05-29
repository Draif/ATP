/*
 * =====================================================================================
 *
 *       Filename:  ImageVector.hpp
 *
 *    Description:  Class for Image. Contain path to original Image in system
 *                  also rgb coordinates. Also it has mode to store all image.
 *
 *        Version:  1.0
 *        Created:  06.05.2014 23:31:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <opencv2/opencv.hpp>

struct RGB {
    typedef unsigned char uchar;
    RGB(uchar _r, uchar _g, uchar _b) : r(_r), g(_g), b(_b) {}

    uchar GetR() { 
        return r;
    }

    uchar GetG() {
        return g;
    }

    uchar GetB() {
        return b;
    }

    unsigned char operator[](size_t indx) const {
        if (indx == 0)
            return r;
        else if (indx == 1)
            return g;
        else if (indx == 2)
            return b;
        else 
            throw std::runtime_error("Operator[] in RGB - indx >3");
    }
private:
    uchar r;
    uchar g;
    uchar b;
};

struct Pixel {
    Pixel(int _x, int _y, RGB _rgb) : x(_x), y(_y), rgb(_rgb) {}
    
    unsigned char operator[](size_t indx) const {
        return rgb[indx];
    }
    
    double operator*(const Pixel& another) const {
        double temp = 0.0;
        for (int i = 0; i < 3; ++i) 
            temp += rgb[i] * rgb[i];
        return temp;
    }

private:
    RGB rgb;
    int x;
    int y;
};

class ImageVector {
public:
    ImageVector() {}
    
    ImageVector(const ImageVector& another) {
        vec = another.vec;
    }

    ImageVector& operator=(const ImageVector& another) {
        if (this != &another)
            vec = another.vec;
    }
    
    void AddPixel(const Pixel& pixel) {
        vec.push_back(pixel);
    }
    
    Pixel operator[](size_t indx) const {
        return vec[indx];
    }
    
    size_t size() const {
        return vec.size();
    }

private:
   std::vector<Pixel> vec; 
};

class ImageTuple {
public:
    ImageTuple() {}
    
    ImageTuple(const ImageVector& _vec, const std::string& path, size_t _size) : vec(_vec), pathToImage(path), size(_size) {
        double temp = 0.0;
        for (int i =0; i < vec.size(); ++i)
            temp += vec[i] * vec[i];
        length = sqrt(temp);
    }
    
    ImageTuple(const ImageTuple& another) {
        vec = another.vec;
        pathToImage = another.pathToImage;
        size = another.size;
        image = another.image;
        length = another.length;
    }
    
    ImageTuple& operator=(const ImageTuple& another) {
        if (this != &another) {
            vec = another.vec;
            pathToImage = another.pathToImage;
            size = another.size;
            image = another.image;
            length = another.length;
        }
        return *this;
    }

    void AddImage(const cv::Mat& mat) {
        image = mat;
    }
    
    
    unsigned char operator[](size_t indx) const {
        size_t temp = indx / 3;
        indx = indx % 3;
        Pixel pixel = vec[temp];
        return pixel[indx];
    }
    
    size_t Dimension() const {
        return 1200;
    }
    
    void PrintInformation() {
        std::cout << "Path: " << pathToImage << std::endl;
    }

    void DisplayImage() {
        cv::namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
        cv::imshow( "Display Image", image );
        cv::waitKey(0);
    }

    double Length() const {
        return length;
    }

private:
    double length;
    ImageVector vec;
    std::string pathToImage;
    size_t size;
    cv::Mat image;
};

class VectorDistance {
public:
    double operator()(const ImageTuple& a, const ImageTuple& b) {
        double temp = 0.0;
        for (int i = 0; i < a.Dimension(); ++i) 
            temp += a[i] * b[i];
        return 1 - temp / (a.Length() * b.Length());
    }
};

