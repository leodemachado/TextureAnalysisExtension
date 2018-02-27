#ifndef PERCENTILECALC_H
#define PERCENTILECALC_H

#include "Global.h"

using namespace std;

class PercentileCalc
{
    
public:
    PercentileCalc(int *freq, string path);
    ~PercentileCalc();
    void Run();
    double GetPercentiles(int i);
private:
    string path;
    int *freq;
    double percentiles[101];
};

#endif // PERCENTILECALC_H
