/*
 * =====================================================================================
 *
 *       Filename:  ImageProcessing.hpp
 *
 *    Description:  Process Images into RGB coordinates
 *
 *        Version:  1.0
 *        Created:  06.05.2014 23:48:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "ImageTuple.hpp"

ImageTuple ConvertToTuple(const std::string path, bool show = false, const size_t size = 20); 
