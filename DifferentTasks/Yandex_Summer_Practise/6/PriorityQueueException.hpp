/*
 * =====================================================================================
 *
 *       Filename:  PriorityQueueException.hpp
 *
 *    Description:  Exception, throwed from PriorityQueue.
 *
 *        Version:  1.0
 *        Created:  26.04.2014 14:48:06
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

class PriorityQueueException: public std::exception {
	std::string message;
public:
	PriorityQueueException(const std::string& _message) : message(_message) {}
	PriorityQueueException(const PriorityQueueException& another) {
        message = another.message;
    }
    const char* what() const noexcept {
		return message.c_str();
	}
};
