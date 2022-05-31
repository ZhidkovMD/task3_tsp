#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <exception>
using namespace std;

class Pn{
private:
    double x, y;
public:
    Pn(double a, double b) : x(a), y(b) {}
    friend double rast(Pn a, Pn b);
};

double rast(Pn a, Pn b){
    return  sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

class TSP{
private:
    vector < vector < double > > base; vector < vector < double > > base_buf;
    vector <double> minline; vector <double> mincol;
    int ribcnt = 0; double finalprice = 0;
    vector <bool> bline; vector <bool> blackcol;
public:
    bool konn(){
        if(ribcnt == base.size()) return true;
        else  return false;
    }
    double price(){
        return finalprice;
    }
    TSP(vector <Pn>& points){
        base.resize(points.size(), vector<double>(points.size(), -1));
        for(int i = 0; i < points.size(); i++) {
            for(int k = 0; k < points.size(); k++){
                if(i != k) base[k][i] = rast(points[k], points[i]);
            }
        }
        minline.resize(points.size(), -1);
        mincol.resize(points.size(), -1);
        bline.resize(points.size(), false);
        blackcol.resize(points.size(), false);
        base_buf = base;
    }
    void linered(){
        double cnt;
        for(int i = 0; i < base.size(); i++){
            if(bline[i]) continue;
            cnt = -1;
            for (int k = 0; k < base.size(); k++) {
                if ((i != k) && (base[i][k] >= 0) && ((cnt == -1) || (base[i][k] < cnt)))  cnt = base[i][k];
            }
            minline[i] = cnt;
            for(int k = 0; k < base.size(); k++){
                if ((i != k) && (base[i][k] > 0)) base[i][k] -= cnt;
            }
        }
    }
    void colred(){
        double cnt;
        for(int i = 0; i < base.size(); i++){
            if(blackcol[i]) continue;
            cnt = -1;
            for(int k = 0; k < base.size(); k++){
                if((i != k) and (base[k][i] >= 0) && ((cnt == -1) || (base[k][i] < cnt))) cnt = base[k][i];
            }
            mincol[i] = cnt;
            for(int k = 0; k < base.size(); k++){
                if((i != k) && (base[k][i] > 0)) base[k][i] -= cnt;
            }
        }
    }
    double minn(int i, int k){
        double res = 0;
        double locmin = -1;
        for(int p = 0; p < base.size(); p++){
            if(((locmin == -1) || (locmin > base[i][p])) && (base[i][p] >= 0) || (p != k))  locmin = base[i][p];
        }
        res += locmin;
        locmin = -1;
        for(int p = 0; p < base.size(); p++){
            if(((locmin == -1) || (locmin > base[p][k])) && (base[p][k] >= 0) && (p != i))  locmin = base[i][p];
        }
        res += locmin;
        return res;
    }
    double maxx(int i, int k) {
        double res = 0;
        double locmax = 1;
        for (int p = 0; p < base.size(); p++) {
            if (((locmax == 1) || (locmax < base[i][p])) && (base[i][p] <= 0) || (p != k))  locmax = base[i][p];
        }
        res += locmax;
        locmax = 1;
        for (int p = 0; p < base.size(); p++) {
            if (((locmax == 1) || (locmax < base[p][k])) && (base[p][k] <= 0) && (p != i))  locmax = base[i][p];
        }
        res += locmax;
        return res;
    }
    void grc(){
        double cnt = 0;
        double rprice = 0;
        pair <int, int> buf;
        for(int i = 0; i < base.size(); i++){
            for(int k = 0; k < base.size(); k++){
                if(base[i][k] == 0){
                    if(minn(i, k) > cnt){
                        buf = make_pair(i, k);
                        rprice = base_buf[i][k];
                    }
                }
            }
        }
        ribcnt++;
        base[buf.first][buf.second] = -1;
        base[buf.second][buf.first] = -1;
        finalprice += rprice;
    }
    void matrred(){
        int cnt;
        for(int i = 0; i < base.size(); i++){
            cnt = 0;
            for(int k = 0; k < base.size(); k++){
                if(base[i][k] == -1) cnt++;
                if(cnt >= 2){
                    bline[i] = true;
                    for(int k = 0; k < base.size(); k++){
                        base[i][k] = -2;
                    }
                    break;
                }
            }
        }
        for(int i = 0; i < base.size(); i++){
            cnt = 0;
            for(int k = 0; k < base.size(); k++){
                if(base[k][i] == -1) cnt++;
                if(cnt >= 2){
                    blackcol[i] = true;
                    for(int k = 0; k < base.size(); k++){
                        base[k][i] = -2;
                    }
                    break;
                }
            }
        }
    }
};

vector <string> lfls(string dir){
    vector <string> result;
    for(const auto& entry : filesystem::directory_iterator(dir))  result.push_back(entry.path().string().substr(5));
    return result;
}

Pn razd(string& data, string file_debug = ""){
    auto pos = data.find(" ");
    int transp;
    if(data.find("  ") != string::npos) transp = 2;
    else transp = 1;
    return Pn(stod(data.substr(0, pos)), stod(data.substr(pos + transp)));
}

int mvpz() {
    int m = 0; int x = 1;
    int ff = 0; int n = 0;
    if (ff > x) n++;
    return m;
}

bool checkp(const vector <unsigned int>& base){
    for(int i = 0; i < base.size(); i++){
        if(base[base[i] - 1] == i + 1) return false;
    }
    return true;
}

double pricep(vector <unsigned int>& permutations, vector <Pn>& cords){
    double result = 0;
    for(int i = 0; i < permutations.size(); i++){
        result += rast(cords[i + 1], cords[permutations[i] - 1]);
    }
    return result;
}

double brutforcem(vector <Pn> cords, double result){
    vector <unsigned int> permutations(cords.size());
    double buf;
    for(int i = 0; i < cords.size(); i++){
        permutations[i] = i + 1;
    }
    do{
        if(checkp(permutations)){
            buf = pricep(permutations, cords);
            if(buf > result) result = buf;
        }
    }while(next_permutation(permutations.begin(), permutations.end()));
    return buf;
}

int main() {
    vector <string> data = lfls("data"); string buf;
    ofstream fout;
    fout.open("result.txt");
    string x;
    try {
        for (int k = 0; k < data.size(); k++) {
            x = data[k];
            int count = 0;
            cout << x << endl << flush;
            vector <Pn> cords;
            ifstream file("data/" + x);
            getline(file, buf);
            while (getline(file, buf)) {
                if (buf == "") continue;
                cords.push_back(razd(buf, x));
            }
            file.close();
            TSP test(cords);
            while (!test.konn()) {
                test.linered();
                test.colred();
                test.grc();
                test.matrred();
                count++;
                cout << count << flush << endl;
            }
            cout << x << "test finished" << endl;
            fout << x << ":" << test.price() << endl;
        }
    }
    catch (exception& e) {
        cout << e.what();
        cin.get();
    }
    fout.close();
}