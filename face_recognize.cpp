#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
/*
Input:
const string& faces_folder: 存放员工照片的文件夹(./database/faces/)
const string& unknow_faces: 要识别的图片所在的文件夹(./face_cam/)

Return:
vector<string>: 识别结果，可能有多个结果，所以存放在vector中。
*/
vector<string> face_recoginize(const string& faces_folder, const string& unknow_faces) {
    vector<string> res;
    string result_file("./result.txt");
    string cmd = "face_recognition " + faces_folder + " " + unknow_faces + " > " + result_file;
    system(cmd.c_str());
    
    ifstream in_file(result_file);
    if(in_file){
        string line;
        while(getline(in_file, line)){
            int dot_pos = line.find(",");
            res.push_back(line.substr(dot_pos+1));	    
        }
	    in_file.close();
    } else{
	    cerr << "couldn't open the result file" << endl;
    }

    string rm_resfile = "rm " + result_file;
    system(rm_resfile.c_str());

    return res;
}

int main(){
    string known_image("./database/faces/");
    string unknow_images("./face_cam/");
    vector<string> res = face_recoginize(known_image, unknow_images);
    for(auto i : res)
        cout << i << endl;
    
    return 0;
}
