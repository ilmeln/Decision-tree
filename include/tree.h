#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <cmath>
#include <tData.h>


std::vector<std::vector<std::string>> read_csv(char* name);
std::vector<std::vector<std::string>> del_colum(std::vector<std::vector<std::string>> vec,std::vector<int> colums);
std::vector<std::vector<std::string>> change(int col,std::map<std::string,std::string> m,std::vector<std::vector<std::string>> vec);
std::vector<std::vector<std::string>> del_empty(std::vector<std::vector<std::string>> vec);



struct tree {
    tData vec;
    double gini;
    int ind;
    std::vector<int> res;
    tree *right,*left,*parent;
    tree() {right=left=parent=nullptr;gini=0;}
};


void show(tree *root);



class DecisionTree {
private:
    tree *root;
    double Info(std::vector<int> res);
    double InfoX(std::vector<double> pr,std::vector<int> res);
    double Gain(std::vector<double> dat,std::vector<int> vec);
    void MakeTree(tData dat,std::vector<int> res,tree *r);
    double AttrGain(std::vector<double> pr,std::vector<int> res);
    double SrGain(std::vector<double> pr,std::vector<int> res,double por);
    double AttrInfoX(std::vector<double> pr,std::vector<int> res,double por);
    double AttrInfo(std::vector<int> rs);
    void AttrSort(std::vector<double> &rs);
    void MakeIrisTree(tree *r,tData dat,std::vector<int> res);
    void Show(tree *r);
public:
    DecisionTree() {}
    void fit(tData dat,std::vector<int> res);
    void fit_iris(tData dat,std::vector<int> res);
    void showTree();
};

#endif // TREE_H
