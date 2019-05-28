#include <numeric>
#include <vector>
#include "math.h"

using namespace std;

// retourne la moyenne d'un vector de flottants
double mean(vector<double> v)
{
    double sum = accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

double variance(vector<double> v)
{
    double m = mean(v);
    double var = 0;
    for (size_t i = 0; i < v.size(); i++)    
    {
        var += pow((v[i]-m), 2);
    }
    return var/v.size();
}

double type_ecart(vector<double> v)
{
    double var = variance(v);

    return sqrt(var);
}

double coef_variation(vector<double> v)
{
    double moyenne = mean(v);
    double var = variance(v);
    double ecart_type = sqrt(var);

    return ecart_type/moyenne;
}