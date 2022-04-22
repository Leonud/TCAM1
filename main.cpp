#include <string>
#include <iostream>
#include<vector>
#include <map>
#include <sstream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

map<string, int> adress;
vector <double> time_;

vector<string> split(string commad, char splitter) {
    string buf = "";
    vector<string> str_arr1;

    for (int i = 0; i < commad.length(); i++){
        if (commad[i] != splitter) {
            buf += commad[i];
        } else{
            str_arr1.push_back(buf);
            buf = "";
        }
    }
    return(str_arr1);
}

const char* HexCharToBin(char c) {
    switch(toupper(c)) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}

string HexStrToBinStr(const string& hex) {
    string bin;
    for(unsigned i = 0; i != hex.length(); ++i)
       bin += HexCharToBin(hex[i]);
    return bin;
}

vector<string> ReadDataFile(string name, int size_) {
    vector<string> result;
    vector<pair<string, string>> DataFile;
    vector<pair<string, string>> DataFile1;
    string line;
    int CountAdress = 0;

    ifstream in(name);
    if (in.is_open()) {
        string mas[2];
        int i = 0;
        while (getline(in, line)) {
            if (i == 0) {
                mas[i] = line;
                i++;
            } else if (i == 1) {
                mas[i] = line;
                i++;
            } else {
                i = 0;
                string buf = HexStrToBinStr(mas[1]);
                DataFile.push_back(make_pair(HexStrToBinStr(mas[0]), buf));
            }
        }
    }
    in.close();

    for(int i = 0; i < DataFile.size(); i++){
        string buf1 = DataFile[i].first;
        string buf2 = DataFile[i].second;
        //cout << buf1 << endl << buf2 << endl;
        for(int i = buf1.size() - 1; i >= 0; i--) {
            if (buf2[i] == '1') {
                buf1[i] = 'x';
            }
        }
        //cout << buf1 << endl << endl;
        if (buf1.size() >= size_) {
            adress[buf1.substr(0, size_)] = CountAdress;
            CountAdress += 1;
            result.push_back(buf1.substr(0, size_));
        } else {
            for (int i = 0; i < size_ - buf1.size(); i++) {
                buf1 = buf1 + '0';
            }
            adress[buf1.substr(0, size_)] = CountAdress;
            CountAdress += 1;
            result.push_back(buf1.substr(0, size_));
        }
    }
    return(result);
}

vector<string> ReadReqFile(string name){
    ifstream in(name);
    vector<string> ReqFile;
    string line;
    if (in.is_open()) {
        while (getline(in, line)) {
            ReqFile.push_back(HexStrToBinStr(line));
        }
    }
    in.close();
    return(ReqFile);
}

bool Find(string Search, string Searcher) {
    double start = clock();

    for (int i = 0; i < Searcher.size(); i++) {
        if ( (Search[0] == Searcher[i] || Search[0] == 'x') && i + Search.size() <= Searcher.size()) {
            bool flag = true;
            for (int j = 0; j < Search.size(); j++){
                if (!(Search[j] == Searcher[i + j] || Search[j] == 'x')) {
                    flag = false;
                }
            }
            if(flag){
                double end = clock();
                double t = (end - start) / 1000;
                time_.push_back(t);
                return true;
            }
        }
    }
    double end = clock();
    double t = (end - start) / 1000;
    time_.push_back(t);
    return false;
}

map <string, string> GetArguments (int argc, char *argv[]) {
    string commad = "";
    map <string, string> map_;
    int count_ = 0;
    for (int i = 1; i < argc; i++){
        commad += argv[i];
        commad += " ";
    }
    vector<string> str_arr1 = split(commad, ' ');
    for (int i = 0; i < str_arr1.size(); i++) {
        if (str_arr1[i] == "-s"){
            map_[str_arr1[i]] = str_arr1[i + 1];
        }
        else if (str_arr1[i] == "-i") {
            map_[str_arr1[i]] = str_arr1[i + 1];
        }
        else if (str_arr1[i] == "-a") {
            map_[str_arr1[i]] = str_arr1[i + 1];
        }
        else if (i > 0){
            if (str_arr1[i] != "-s" && str_arr1[i] != "-a" && str_arr1[i] != "-i" && str_arr1[i - 1] != "-s" && str_arr1[i - 1] != "-a" && str_arr1[i - 1] != "-i"){
                if (count_ == 0){
                    map_["data-file"] = str_arr1[i];
                    count_ += 1;
                } else {
                    map_["req-file"] = str_arr1[i];
                    count_ += 1;
                }
            }
        }
        else if (i == 0){
            if (str_arr1[i] != "-s" && str_arr1[i] != "-a" && str_arr1[i] != "-i"){
                if (count_ == 0){
                    map_["data-file"] = str_arr1[i];
                    count_ += 1;
                } else {
                    map_["req-file"] = str_arr1[i];
                    count_ += 1;
                }
            }
        }
    }
    return map_;
}

int main(int argc, char *argv[]) {
    cout << setprecision(5);
    map <string, string> Arguments= GetArguments(argc, argv);
    if(Arguments.count("-i") == 0) {
        Arguments["-i"] = "1";
    }

    vector<string> DataFile = ReadDataFile(Arguments["data-file"], stoi(Arguments["-s"]));
    vector<string> ReqFile = ReadReqFile(Arguments["req-file"]);
    bool flag = true;
    for (int c = 0; c < stoi(Arguments["-i"]) ; c++) {
        for(int i = 0; i < DataFile.size(); i++) {
            for(int j = 0; j < ReqFile.size(); j++) {
                if (Find(DataFile[i], ReqFile[j])) {
                    if (Arguments.count("-i") != 0 && flag) { cout << adress[DataFile[i]] << ": "<< DataFile[i] << " " << ReqFile[j] << endl;}
                }
            }
        }
        flag = false;
    }
    sort (time_.begin(), time_.end());
    double AverageTime = 0;
    for (int i = 0; i < time_.size(); i++) {
        AverageTime += time_[i];
    }
    cout << "Minimum time: " <<time_[0] << endl
    << "Average time: " << AverageTime/time_.size() << endl
    << "Maximum time: " << time_[time_.size() - 1] << endl;
    for(auto it = Arguments.begin(); it != Arguments.end(); ++it) {
        cout << it->first << " : " << it->second << endl;//вывод на экран
    }

    return 0;
}
