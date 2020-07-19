#include "face_recognize.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

string full_cmd="face_recognition --tolerance 0.4";

void initFullPathCmd(){
    ifstream ifs("../cmd.config");
    if(ifs){
        getline(ifs,full_cmd);
        ifs.close();
    }
}

/*
Input:
const string& faces_folder: 存放员工照片的文件夹(./database/faces/)
const string& unknow_faces: 要识别的图片所在的文件夹(./face_cam/)

Return:
vector<string>: 识别结果，可能有多个结果，所以存放在vector中。
*/
vector<int> face_recognize(const string& faces_folder, const string& unknow_faces) {
    vector<int> res;
    string result_file("./result.txt");
    string cmd = full_cmd + " " + faces_folder + " " + unknow_faces + " > " + result_file;
//    string cmd = "face_recognition " + faces_folder + " " + unknow_faces + " > " + result_file;
    system(cmd.c_str());
    
    ifstream in_file(result_file);
    if(in_file){
        string line;
        while(getline(in_file, line)){
            int dot_pos = line.find(",");
            string tmp = line.substr(dot_pos+1);
            if(tmp == "unknown_person")
                res.push_back(-1);
            else
                res.push_back(stoi(tmp));	    
        }
	    in_file.close();
    } else{
	    cerr << "couldn't open the result file" << endl;
    }

    string rm_resfile = "rm " + result_file;
    system(rm_resfile.c_str());

    return res;
}
