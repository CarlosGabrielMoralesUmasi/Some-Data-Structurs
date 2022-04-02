#include <iostream>
#include <random>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <iomanip>
constexpr int MIN = 1;
constexpr int MAX = 100;

constexpr int RAND_NUMS_TO_GENERATE = 1;
using namespace std;
int random(int a)
{   
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(MIN, MAX);

    for (int n = 0; n < RAND_NUMS_TO_GENERATE; ++n) 
    {
        return distr(eng);
    }
}
template <typename T>
void findDuplicates(std::vector<T> & vecOfElements, std::map<T, int> & countMap)
{
    for (auto & elem : vecOfElements)
    {
        auto result = countMap.insert(std::pair<int, int>(elem, 1));
        if (result.second == false)
            result.first->second++;
    }
    for (auto it = countMap.begin() ; it != countMap.end() ;)
    {
        if (it->second == 1)
            it = countMap.erase(it);
        else
            it++;
    }
}
int main()
{ 
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now(); 
    const int datos = 20000;
    vector<int> distancias;
    int el_random;
    int dimension = 1;//poner la dimensión que se quiere
    int matriz[20000][dimension];//hasta 20000*100

    for (int i = 0; i < datos; ++i) 
    {
        for(int j=0;j<dimension;j++)
        {
          el_random = random(0);
          matriz[i][j]=el_random;
        }
    }
    int distancia;//variables para euclidiano
    int resta;
    int sumatoria;

    int valor_matr = matriz[1][dimension];
    for (int i = 0; i < 20000; ++i)
    {
        for(int j=0;j<dimension;j++){
        if (matriz[1][j]!=valor_matr)//recursivo
          {
            resta = matriz[i][j] - matriz[1][j];
            distancia = resta * resta;
            sumatoria = sumatoria + distancia;
          }
        }
        sumatoria=sqrt(sumatoria);
        distancias.push_back(sumatoria);
    }
    sort(distancias.begin(), distancias.end());
    int cont = 0;
    for (int j = 0; j < 19998; j++) 
    {
      if(distancias[j] == distancias[j+1]){ cont++; }//contador de repetidos
      else 
      {
        cout  << distancias[j] << " :: " << cont <<endl;
        cont = 0;
      }
     }
    
  end = std::chrono::high_resolution_clock::now();
int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); 
  cout<<duration<<endl;
  return 0;
}
