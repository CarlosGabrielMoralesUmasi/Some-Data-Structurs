////////https://replit.com/@Carlos_Gabriel1/eda1#main.cpp 

#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <iomanip>

using std::cout;
using std::endl;

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




int main() {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
start = std::chrono::high_resolution_clock::now();
    int n;
    int a[20000];
    for (int i=0;i<20000;i++)
      {
        a[i]=random(n);
      // cout<<a[i]<<endl;
      }
  //cout<<"eucli:"<<endl;
  int const dimen=2;
  int cont=0,tam=0,i,j1,t,total=0;
  //el tam no importa ya que si el array desborda a numeros negativos entonces se sale del for con el BREAK
  //t y j1 me sirven para no estar chancando el valor de dimen (dimensión)
  //el seudocodigo sería "recursivo".
  t=dimen;j1=dimen;i=0;
  
  vector<int> distancias;
  int resultado=0;
    for(;tam<=30000;)
      {
        if (cont <dimen  )
        {
          if(resultado<0){break;}
          resultado=(total +(pow((a[dimen-t]-a[j1+i]),2)));
          resultado=sqrt(resultado);
          distancias.push_back(resultado);
          //cout<<a[dimen-t]<<"-"<<a[j1+i]<<"="<<resultado<<endl;
          t--;i++;
          cont++;
        }
          
         
        else
        {
          j1+=dimen;
          cont=0;
          t=dimen;
          i=0;
        }
       
        tam++;
          
      }
  sort(distancias.begin(), distancias.end());
  distancias.erase(distancias.begin()); 
  std::map<int, int> duplicateElements;
  //obtengo elementos repetidos del vector
  findDuplicates(distancias, duplicateElements);
  for (auto & elem : duplicateElements)
    {
      cout << elem.first << "   :: " << elem.second <<endl;
    }  
  end = std::chrono::high_resolution_clock::now();
int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); 
  cout<<duration<<endl;
}
