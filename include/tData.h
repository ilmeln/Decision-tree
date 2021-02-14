#ifndef TDATA_H
#define TDATA_H


#include <vector>
#include <string>


class tData
{
    public:
        tData();
        void read_from_file(std::string path,char delim=',');
        void show() const;
        int weight() const {return width;}
        int size() const {return height;}
        std::vector<double> get_columm(int num) const;
        double get_value(int x,int y) const {return inf[y][x];}
        void push_str(std::vector<double> vec);
        std::vector<double> get_stroke(int num) const {return inf[num];}
    private:
        int width,height;
        std::vector<std::vector<double>> inf;
};


double double_from_string(std::string s);
void load_iris(tData &dat,std::vector<int> &res);


#endif // TDATA_H
