#ifndef _FACE_RECOGNIZE_H
#define _FACE_RECOGNIZE_H
#include <vector>
#include <string>

void initFullPathCmd();
std::vector<int> face_recognize(const std::string&, const std::string&);
#endif
