#include "tData.h"

#include <fstream>
#include <iostream>
#include <sstream>

tData::tData()
{
    width=height=0;
}

double double_from_string(std::string s)
{
    double res;
    std::stringstream ss;
    ss << s;
    ss >> res;
    return res;
}


void tData::read_from_file(std::string path,char delim)
{
    std::ifstream fin(path);
    std::string s,dob="";
    std::getline(fin,s);
    std::vector<double> vec;
    for(unsigned int i=0;i<s.size();++i) {
        if(i==s.size()-1) {
            if(s[i]!=' '&&s[i]!='\n') {
                dob+=s[i];
            }
            double d = double_from_string(dob);
            vec.push_back(d);
        }
        else if(s[i]==delim) {
            double d = double_from_string(dob);
            vec.push_back(d);
            dob = "";
        }
        else {
            dob+=s[i];
        }
    }
    width = vec.size();
    inf.push_back(vec);
    vec.clear();
    while(!fin.eof()) {
        int count = 0;
        while(count<width) {
            char ch;
            fin>>ch;
            if (isdigit(ch)) {
                fin.putback(ch);
                double d;
                fin>>d;
                vec.push_back(d);
                ++count;
            }
        }
        inf.push_back(vec);
        vec.clear();
    }
    height = inf.size();
}


void tData::show() const
{
    for(auto v : inf) {
        for(auto d : v) {
            std::cout<<d<<' ';
        }
        std::cout<<std::endl;
    }
}


std::vector<double> tData::get_columm(int num) const
{
    std::vector<double> vec;
    for(auto v : inf) {
        vec.push_back(v[num]);
    }
    return vec;
}

void tData::push_str(std::vector<double> vec)
{
    inf.push_back(vec);
    if(width==0) {
        width = vec.size();
    }
    ++height;
}

void load_iris(tData &dat,std::vector<int> &res)
{
    std::ifstream fin("iris.txt");
    std:: string s = "setosa",ve = "versicolor",vi = "virginica";
    while(!fin.eof()) {
        std::vector<double> v;
        for(int i=0;i<4;++i) {
            double d;
            fin>>d;
            v.push_back(d);
        }
        dat.push_str(v);
        std::string name;
        fin>>name;
        if(name==s) {
            res.push_back(0);
        }
        else if(name==ve) {
            res.push_back(1);
        }
        else if(name==vi) {
            res.push_back(2);
        }
    }
}
