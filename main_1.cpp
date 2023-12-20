#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
/*! \file */
using namespace std;

class Account
{
public:
    string FIO;
    string dolgnost;
    string podrazdelenie;
    int zarplata;
    }; 


/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False   
bool operator >(Account first, Account second)
{
    if(first.FIO > second.FIO)
        return true;
    else if(first.FIO < second.FIO)
        return false;
    else if(first.podrazdelenie > second.podrazdelenie)
        return true;
    else if(first.podrazdelenie < second.podrazdelenie)
        return false;
    else if(first.zarplata > second.zarplata)
        return true;
    else if(first.zarplata < second.zarplata)            
        return false;
    return false;
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False 
bool operator <(Account first, Account second)
{
    if(first.zarplata > second.zarplata)
        return false;
    else if(first.zarplata < second.zarplata)
        return true;
    else if(first.podrazdelenie > second.podrazdelenie)
        return false;
    else if(first.podrazdelenie < second.podrazdelenie)
        return true;
    else if(first.FIO > second.FIO)
        return false;
    else if(first.FIO < second.FIO)            
        return true;
    return false;
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False 
bool operator >=(Account first, Account second)
{
    if(first.zarplata > second.zarplata)
        return true;
    else if(first.zarplata < second.zarplata)
        return false;
    else if(first.podrazdelenie > second.podrazdelenie)
        return true;
    else if(first.podrazdelenie < second.podrazdelenie)
        return false;
    else if(first.FIO > second.FIO)
        return true;
    else if(first.FIO < second.FIO)            
        return false;
    return true;
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False 
bool operator <=(Account first, Account second)
{
    if(first.zarplata > second.zarplata)
        return false;
    else if(first.zarplata < second.zarplata)
        return true;
    else if(first.podrazdelenie > second.podrazdelenie)
        return false;
    else if(first.podrazdelenie < second.podrazdelenie)
        return true;
    else if(first.FIO > second.FIO)
        return false;
    else if(first.FIO < second.FIO)            
        return true;
    return true;
    }
/// \brief Импорт базы данных
/// \return Возвращает вектор классов Account 
vector<Account> import(string path ){
    vector<Account> ret; 
    string s;
    ifstream file(path);
    vector<string> element;
    while(getline(file,s)){
        element.clear();
        const char *str = s.c_str();
        Account a;
        char * pch = strtok((char*)str,"/"); 
        while(pch != NULL){
            element.push_back(pch);
            pch = strtok(NULL,"/");
        }
        a.FIO = element.at(0);
        a.dolgnost = element.at(1);
        a.podrazdelenie = element.at(2); 
        a.zarplata = stoi(element.at(3));
    
        ret.push_back(a);
    }
    return ret;
}
    
/// \brief Сортировка простыми вставками
/// \return Возвращает отсортированный vector классов Account   
vector<Account> ProstVstav(vector<Account > basic){
    vector<Account> fin; 
    fin.push_back(basic.at(0));
    basic.erase(basic.begin());       
    for(int i = 0; i < basic.size(); i++) {
        for(int j = 0; j < fin.size(); j++){
            if(basic.at(i) < fin.at(j)){   
                fin.insert(fin.begin() + j, basic.at(i));
                break;
            }
            else if(j + 1 == fin.size()){   
                fin.push_back(basic.at(i));
                break;
            }
        }
    }   
    return fin;    
}       
/// \brief Шейкер Сортировка
/// \return Возвращает отсортированный vector классов Account 
vector<Account> ShakerSort(vector<Account> basic){
	int left = 1;
	int right = basic.size() - 1;
	while (left <= right){
		for (int i = right; i >= left; i--)
		if (basic.at(i - 1) > basic.at(i)){ 
		    Account k;
	        k = basic.at(i);
	        basic.at(i) = basic.at(i - 1);
	        basic.at(i - 1) = k;
		}
		left++;

		for (int i = left; i <= right; i++)
		if (basic.at(i - 1) > basic.at(i)){ 
		    Account k;
		    k = basic.at(i);
	        basic.at(i) = basic.at(i - 1);
	        basic.at(i - 1) = k;
		}
		right--;
	}
    return basic;	
}   
/// \brief  Сортировка слиянием(метод)
/// \return Возвращает отсортированный vector классов Account 
void Merge(vector<Account> basic, int low, int high){
    vector<Account> fin;
    int mid = (low + high) / 2;
    int h = low;
    int i = 0;
    int j = mid +1;
    int k;
    while((h <= mid) && (j <=high)){
        if(basic.at(h) <= basic.at(j)){
            fin.at(i) = basic.at(h);
            h++;
        }
        else{
            fin.at(i) = basic.at(j);
            j++;
        }
        i++;
    }
    if(h > mid){
        for(k = j; k <= high; k++){
            fin.at(i) = basic.at(k);
            i++;
        }
    }
    else{
        for(k = h; k <= mid; k++){
            fin.at(i) = basic.at(k);
            i++;
        }
    }
    for(k = 0; k <= high - low; k++){
        basic.at(k + low) = fin.at(k);
    }
}
/// \brief  Сортировка слиянием
/// \return Возвращает отсортированный vector классов Account
vector<Account> MergeSort(vector<Account> basic, int low, int high){
    int mid;
    if(low < high){
        mid = (low + high) / 2;
        MergeSort(basic, low, (low + high) / 2);
        MergeSort(basic, (low + high) / 2 + 1, high);
        Merge(basic, low, high);
    }
    return basic;
}

int main()
{
    typedef std::chrono::duration<float> float_seconds;
    vector<Account> a = import("100.txt");
    vector<Account> b = a;

    auto start = std::chrono::system_clock::now();
    a = ProstVstav(a);
    auto stop = std::chrono::system_clock::now();
    auto time = stop-start;
    cout << a.size()<<":" << endl;
    cout << "Сортировка простыми вставками:" << (float(chrono::duration_cast<chrono::nanoseconds>(time).count())/1000000000)<< endl << endl ;    
    a = b;
    start = std::chrono::system_clock::now();
    a = ShakerSort(a);
    stop = std::chrono::system_clock::now();
    time = stop-start;
    cout << "Шейкер–сортировка:" << (float(chrono::duration_cast<chrono::nanoseconds>(time).count())/1000000000) << endl << endl; 
    a = b;
    start = std::chrono::system_clock::now();
    a = MergeSort(a, 0, a.size());
    stop = std::chrono::system_clock::now();
    time = stop-start;
    cout << "Сортировка слиянием:" << (float(chrono::duration_cast<chrono::nanoseconds>(time).count())/1000000000) << endl;          
     
    return 0;
}