#include "tree.h"

std::vector<std::vector<std::string>> read_csv(char* name)
{
    std::vector<std::vector<std::string>> vec;
    std::string stroke;
    std::ifstream fin(name);
    if(!fin) {
        return vec;
    }
    while(!fin.eof()) {
        std::vector<std::string> v;
        std::getline(fin,stroke);
        std::string s="";
        for(unsigned int i=0;i<stroke.size();++i) {
            if(i==stroke.size()-1) {
                s+=stroke[i];
                v.push_back(s);
            }
            else if(stroke[i]==',') {
                if(s=="") {
                    v.push_back(" ");
                }
                else {
                    v.push_back(s);
                    s="";
                }
            }
            else {
                s+=stroke[i];
            }
        }
        vec.push_back(v);
    }
    return vec;
}


std::vector<std::vector<std::string>> del_colum(std::vector<std::vector<std::string>> vec,std::vector<int> colums)
{
    std::vector<std::vector<std::string>> copy_vec;
    for(auto v : vec) {
        std::vector<std::string> vect;
        for(unsigned int i=0;i<v.size();++i) {
            bool f = true;
            for(auto j : colums) {
                if((unsigned)j==i) {
                    f=false;
                    break;
                }
            }
            if(f) {
                vect.push_back(v[i]);
            }
        }
        copy_vec.push_back(vect);
    }
    return copy_vec;
}


std::vector<std::vector<std::string>> change(int col,std::map<std::string,std::string> m,std::vector<std::vector<std::string>> vec)
{
    for(unsigned int i=0;i<vec.size();++i) {
        for(auto x : m) {
            if(vec[i][col]==x.first) {
                vec[i][col] = x.second;
                break;
            }
        }
    }
    return vec;
}

std::vector<std::vector<std::string>> del_empty(std::vector<std::vector<std::string>> vec)
{
    std::vector<std::vector<std::string>> cop;
    for(auto v : vec) {
        if(!v.empty()) {
            bool f = true;
            for(auto s : v) {
                if(s==" ") {
                    f=false;
                    break;
                }
            }
            if(f) {
                cop.push_back(v);
            }
        }
    }
    return cop;
}



double DecisionTree::Gain(std::vector<double> dat,std::vector<int> vec)
{
    return Info(vec)-InfoX(dat,vec);
}

double DecisionTree::InfoX(std::vector<double> pr,std::vector<int> res)
{
    std::vector<double> col;
    for(auto d : pr) {
        bool find = true;
        for(auto x : col) {
            if(x==d) {
                find=false;
                break;
            }
        }
        if(find) {
            col.push_back(d);
        }
    }
    auto zpr = col;
    for(unsigned int i =0;i<col.size();++i) {
        int count = 0;
        for(auto x : pr) {
            if(x==col[i]) {
                ++count;
            }
        }
        col[i]= count;
    }
    double sum = 0;
    for(unsigned int i=0;i<col.size();++i) {
        std::vector<int> rs;
        for(unsigned int j=0;j<res.size();++j) {
            if(pr[j]==zpr[i]) {
                rs.push_back(res[j]);
            }
        }
        sum+=static_cast<double>(col[i])/static_cast<double>(pr.size())*Info(rs);
    }
    return sum;
}


double DecisionTree::Info(std::vector<int> res)
{
    double sum=0;
    std::vector<int> col;
    for(auto n : res) {
        bool find = true;
        for(auto x : col) {
            if(x==n) {
                find = false;
                break;
            }
        }
        if(find) {
            col.push_back(n);
        }
    }
    for(unsigned int i=0;i<col.size();++i) {
        int count = 0;
        for(auto x : res) {
            if(col[i]==x) {
                ++count;
            }
        }
        col[i]=count;
    }
    for(auto k : col) {
        double p = static_cast<double>(k)/static_cast<double>(res.size());
        sum+=p*log2(p);
    }
    return -sum;
}



void DecisionTree::fit(tData dat,std::vector<int> res)
{
    root = new tree;
    double g = Gain(dat.get_columm(0),res);
    root->res = res;
    int index=0;
    for(int i=1;i<dat.weight();++i) {
        double e = Gain(dat.get_columm(i),res);
        if(e>g) {
            g=e;
            index = i;
        }
    }
    std::vector<int> lres,rres;
    tData ldata,rdata;
    for(int i=0;i<dat.size();++i) {
        if(dat.get_value(index,i)==0) {
            lres.push_back(res[i]);
            ldata.push_str(dat.get_stroke(i));
        }
        else {
            rres.push_back(res[i]);
            rdata.push_str(dat.get_stroke(i));
        }
    }
    tree *r=new tree,*l = new tree;
    root->vec = dat;
    root->right =r;
    root->left = l;
    r->parent=l->parent = root;
    root->gini = g;
    root->ind = index;
    MakeTree(rdata,rres,r);
    MakeTree(ldata,lres,l);
}


void DecisionTree::MakeTree(tData dat,std::vector<int> res,tree* rt)
{
    rt->vec = dat;
    rt->res = res;
    if(dat.size()<=1) {
        return;
    }
    double g = Gain(dat.get_columm(0),res);
    int index = 0;
    for(int i=1;i<dat.weight();++i) {
        double e = Gain(dat.get_columm(i),res);
        if(e>g) {
            g=e;
            index = i;
        }
    }
    if(g==0) {
        return;
    }
    std::vector<int> lres,rres;
    tData ldata,rdata;
    for(int i=0;i<dat.size();++i) {
        if(dat.get_value(index,i)==0) {
            lres.push_back(res[i]);
            ldata.push_str(dat.get_stroke(i));
        }
        else {
            rres.push_back(res[i]);
            rdata.push_str(dat.get_stroke(i));
        }
    }
    tree *r=new tree,*l = new tree;
    rt->right =r;
    rt->left = l;
    r->parent=l->parent = rt;
    rt->gini = g;
    rt->ind = index;
    MakeTree(rdata,rres,r);
    MakeTree(ldata,lres,l);
}

void DecisionTree::showTree()
{
    Show(root);
}


void DecisionTree::Show(tree *r)
{
    if(r!=nullptr) {
        Show(r->left);
        for(int i=0;i<r->vec.size();++i) {
            for(int j=0;j<r->vec.weight();++j) {
                std::cout<<r->vec.get_value(j,i)<<" ";
            }
            std::cout<<"------ "<<r->res[i]<<std::endl;
        }
        std::cout <<"Gini:"<<r->gini<<std::endl;
        std::cout<<"Index:"<<r->ind<<"\nSamples:"<<r->res.size()<<"\n--------\n";
        Show(r->right);
    }
}


void DecisionTree::fit_iris(tData dat,std::vector<int> res)
{
    root = new tree;
    root->vec=dat;
    root->res=res;
    double a = AttrGain(dat.get_columm(0),res);
    int ind = 0;
    double g = SrGain(dat.get_columm(0),res,a);
    for(int i=1;i<dat.weight();++i) {
        double _a = AttrGain(dat.get_columm(i),res);
        double _g = SrGain(dat.get_columm(i),res,_a);
        if(_g>g) {
            g=_g;
            a=_a;
            ind =i;
        }
    }
    tree *r=new tree;
    tree *l = new tree;
    std::vector<int> rres,lres;
    tData ldata,rdata;
    for(int i=0;i<dat.size();++i) {
        if(dat.get_value(ind,i)<=a) {
            lres.push_back(res[i]);
            ldata.push_str(dat.get_stroke(i));
        }
        else {
            rres.push_back(res[i]);
            rdata.push_str(dat.get_stroke(i));
        }
    }
    root->right=r;
    root->left = l;
    l->parent=r->parent = root;
    root->gini = g;
    root->ind = ind;
    MakeIrisTree(r,rdata,rres);
    MakeIrisTree(l,ldata,lres);
}

void DecisionTree::AttrSort(std::vector<double> &rs)
{
    for(unsigned int i=0;i<rs.size();++i) {
        for(unsigned int j=0;j<rs.size()-1;++j) {
            if(rs[j]>rs[j+1]) {
                double d = rs[j];
                rs[j]= rs[j+1];
                rs[j+1]=d;
            }
        }
    }
}


double DecisionTree::AttrGain(std::vector<double> pr,std::vector<int> res)
{
    AttrSort(pr);
    double g = SrGain(pr,res,(pr[0]+pr[1])/2.0);
    double rs = (pr[0]+pr[1])/2.0;
    for(unsigned int i=1;i<pr.size()-1;++i) {
        double pos = (pr[i]+pr[i+1])/2.0;
        double e = SrGain(pr,res,pos);
        if(e>g) {
            g=e;
            rs = pos;
        }
    }
    return rs;
}



double DecisionTree::SrGain(std::vector<double> pr,std::vector<int> res,double por)
{
    return AttrInfo(res)-AttrInfoX(pr,res,por);
}


double DecisionTree::AttrInfoX(std::vector<double> pr,std::vector<int> res,double por)
{
    int count = 0;
    std::vector<int> rs;
    double sum = 0;
    for(unsigned int i=0;i<pr.size();++i) {
        if(pr[i]<=por) {
            ++count;
            rs.push_back(res[i]);
        }
    }
    sum+=static_cast<double>(count)/static_cast<double>(pr.size())*AttrInfo(rs);
    count = 0;
    rs.clear();
    for(unsigned int i=0;i<pr.size();++i) {
        if(pr[i]>por) {
            ++count;
            rs.push_back(res[i]);
        }
    }
    sum+=static_cast<double>(count)/static_cast<double>(pr.size())*AttrInfo(rs);
    return sum;
}


double DecisionTree::AttrInfo(std::vector<int> rs)
{
    std::vector<int> pr;
    for(auto x : rs) {
        bool find = true;
        for(auto y : pr)  {
            if(y==x) {
                find=false;
                break;
            }
        }
        if(find) {
            pr.push_back(x);
        }
    }
    double sum=0;
    for(auto x : pr) {
        int count=0;
        for(auto y : rs) {
            if(x==y) {
                ++count;
            }
        }
        double p = static_cast<double>(count)/static_cast<double>(rs.size());
        sum+=p*log2(p);
    }
    return -sum;
}


void DecisionTree::MakeIrisTree(tree *r,tData dat,std::vector<int> res)
{
    r->vec = dat;
    r->res = res;
    r->ind = r->parent->ind;
    if(dat.size()<=1) {
        return;
    }
    double pr = AttrGain(dat.get_columm(0),res);
    double g = SrGain(dat.get_columm(0),res,pr);
    int ind = 0;
    for(int i=1;i<dat.weight();++i) {
        double e = AttrGain(dat.get_columm(i),res);
        double e1 = SrGain(dat.get_columm(i),res,e);
        if(e1>g) {
            g=e1;
            pr=e;
            ind = i;
        }
    }
    if(g==0) {
        return;
    }
    std::vector<int> rres,lres;
    tData rdata,ldata;
    for(int i=0;i<dat.size();++i) {
        if(dat.get_value(ind,i)<=pr) {
            lres.push_back(res[i]);
            ldata.push_str(dat.get_stroke(i));
        }
        else {
            rres.push_back(res[i]);
            rdata.push_str(dat.get_stroke(i));
        }
    }
    tree *rig= new tree,*l=new tree;
    r->gini = g;
    r->ind = ind;
    r->right = rig;
    r->left = l;
    l->parent=rig->parent = r;
    MakeIrisTree(rig,rdata,rres);
    MakeIrisTree(l,ldata,lres);
}
