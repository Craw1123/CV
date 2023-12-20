#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <map>

using namespace std;

class Otel
{
public:
    string FIO;
    int nomer;
    long int UnixTimeDataPriesda;
    long int UnixTimeDataOtezda;
    int Summaoplati;
    }; 

/// \brief Перевод UnixTime в дату
/// \return Возвращает дату типа string 
string uttodt(long int a) //конвертация unixtime в дату 
{
    time_t t = a;    
    return asctime(localtime(&t));
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False    
bool operator >(Otel first,Otel second)
{
    if(first.FIO>second.FIO)
        return true;
    else if(first.FIO<second.FIO)            
        return false;
    else if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
        return true;
    else if(first.UnixTimeDataPriesda<second.UnixTimeDataPriesda)
        return false;
    else if(first.nomer>second.nomer)
        return true;
    else if(first.nomer<second.nomer)
        return false;
    return false;
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False
bool operator <(Otel first,Otel second)
{
    if(first.FIO>second.FIO)
        return false;
    else if(first.FIO<second.FIO)            
        return true;
    else if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
        return false;
    else if(first.UnixTimeDataPriesda<second.UnixTimeDataPriesda)
        return true;
    else if(first.nomer>second.nomer)
        return false;
    else if(first.nomer<second.nomer)
        return true;
    else if(first.FIO>second.FIO)
        return false;
    else if(first.FIO<second.FIO)            
        return true;
    return false;
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False 
bool operator >=(Otel first,Otel second)
{
    if(first.FIO>second.FIO)
        return true;
    else if(first.FIO<second.FIO)            
        return false;
    else if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
        return true;
    else if(first.UnixTimeDataPriesda<second.UnixTimeDataPriesda)
        return false;
    else if(first.nomer>second.nomer)
        return true;
    else if(first.nomer<second.nomer)
        return false;
    else if(first.FIO>second.FIO)
        return true;
    else if(first.FIO<second.FIO)            
        return false;
    return true;
    }
/// \brief Перегрузка операторов
/// \return Возвращает либо True, либо False
bool operator <=(Otel first,Otel second)
{
    if(first.FIO>second.FIO)
        return false;
    else if(first.FIO<second.FIO)            
        return true;
    else if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
        return false;
    else if(first.UnixTimeDataPriesda<second.UnixTimeDataPriesda)
        return true;
    else if(first.nomer>second.nomer)
        return false;
    else if(first.nomer<second.nomer)
        return true;
    return true;
    }
/// \brief Импорт базы данных
/// \details Импортирует базу данных в программу
/// \return Возвращает вектор классов Otel 
vector<Otel> import(string path )
{
    vector<Otel> ret; string s;
    ifstream file(path);
    vector<string> elems;
    while(getline(file,s))
    {
        elems.clear();
        const char *str = s.c_str();
        Otel a;
        char * pch = strtok((char*)str,"/"); //тут и 5 строчек ниже сплитим по символу / если бы я знал, что в c++ нет сплита, я бы пошел писать на qt
        while(pch != NULL)
        {
            elems.push_back(pch);
            pch = strtok(NULL,"/");
            }
    
        a.FIO = elems.at(0);
        a.nomer = stoi(elems.at(1));
        a.UnixTimeDataPriesda = stoi(elems.at(2)); 
        a.UnixTimeDataOtezda = stoi(elems.at(3));
        a.Summaoplati = stoi(elems.at(4));
    
        ret.push_back(a);
        }

    return ret;
    }
/// \brief Сортировка простыми вставками
/// \details Сортирует вектор класса Otel методом сортировки Простыми вставками
/// \return Возвращает отсортированный vector классов Otel
vector<Otel> SortProstVstavk(vector<Otel> ishod)
{
    vector<Otel> ready; // созд
    ready.push_back(ishod.at(0));
    ishod.erase(ishod.begin());
   // for(int xz = 0; xz<ishod.size();xz++)
     //   cout << ishod.at(xz).nomer << "\n"; 
           
    for(int i =0;i<ishod.size();i++) 
    {
        for(int z=0;z<ready.size();z++)
        {
            if(ishod.at(i)<ready.at(z))
            {   
                ready.insert(ready.begin()+z,ishod.at(i));
                //ishod.erase(ishod.begin()+i);

                break;
                }
            else if(z+1==ready.size())
            {   
                ready.push_back(ishod.at(i));
                break;
                }
            }
        
        }   


    return ready;    
    }       
/// \brief Рекурсивная Быстрая Сортировка
/// \details Использутеся для рекурсии в функции QuickSort
/// \return Возвращает 0
int QS(vector<Otel> *ishod, int f, int s)
{
    if(f<s)
    {    
        int l = f; int r = s; Otel mid = ishod->at((l+r)/2);
        while(l<r)
        {
            while(ishod->at(l)<mid)
                l++;
            while(ishod->at(r)>mid)
                r--;
            if(l<=r)
            {
                Otel k = ishod->at(l);
                ishod->at(l) = ishod->at(r);
                ishod->at(r) = k;
                l++;r--; 
                }        
        
            }
        QS(ishod,f,r);
        QS(ishod,l,s);  
    }
    return 0;            
    }
/// \brief Быстрая Сортировка
/// \details Сортирует вектор класса Otel методом Быстрой сортировки
/// \return Возвращает отсортированный vector классов Otel    
vector<Otel> QuickSort(vector<Otel> ishod)
{
    if(0<ishod.size()-1)
    {
        int l = 0; int r = ishod.size()-1; Otel mid = ishod.at(ishod.size()/2);
        while(l<r)
        {
            while(ishod.at(l)<mid)
                l++;
            while(ishod.at(r)>mid)
                r--;
            if(l<=r)
            {
                Otel k = ishod.at(l);
                ishod.at(l) = ishod.at(r);
                ishod.at(r) = k;
                l++;r--; 
                }        
        
            }
        QS(&ishod,0,r);
        QS(&ishod,l,ishod.size()-1);     
    }    
    
    
    return ishod;
    }
/// \brief Шейкер Сортировка
/// \details Сортирует вектор класса Otel методом Шейкер сортировки
/// \return Возвращает отсортированный vector классов Otel 
vector<Otel> myShakerSort(vector<Otel> ishod)
{
	int l = 1;
	int r = ishod.size() - 1;
	while (l <= r)
	{
		for (int i = r; i >= l; i--)
		if (ishod.at(i - 1) > ishod.at(i))
		{ 
		    Otel k;
	        k = ishod.at(i);
	        ishod.at(i) = ishod.at(i-1);
	        ishod.at(i-1) = k;
		    }
		l++;


		for (int i = l; i <= r; i++)
		if (ishod.at(i-1) > ishod.at(i))
		{ 
		    Otel k;
		    k = ishod.at(i);
	        ishod.at(i) = ishod.at(i-1);
	        ishod.at(i-1) = k;
		    }
		r--;
	}
    return ishod;	
}
/// \brief Прямой поиск
/// \details Ищет по вектору класса Otel методом Прямого Поиска
/// \return Возвращает найденый класс Otel
Otel pp(vector<Otel> ishod,string param)
{
    for(int i = 0;i<ishod.size();i++)
    {
        if(ishod.at(i).FIO == param)
        {
            return ishod.at(i);   
        
            }    
        }
    }

/// \brief Рекурсия Бинарного Поиска 
/// \details Рукурсивно ищет по вектору класса Otel методом Бинарного Поиска
/// \return Возвращает найденый индекса класса Otel

int bpreq(vector<Otel>& ishod, string& param,int start, int end)
{
    int mid = (start+end)/2;
    //cout << ishod.at(mid).FIO << "   " << param << endl;
    if(ishod.at(mid).FIO<param)
        bpreq(ishod,param,mid+1,end);
    else if(ishod.at(mid).FIO>param)
        bpreq(ishod,param,start,mid-1);
    else
        return mid;
    }  

/// \brief Бинарный Поиск 
/// \details Ищет по вектору класса Otel методом Бинарного Поиска
/// \return Возвращает найденый класса Otel
Otel bp(vector<Otel> ishod, string param)
{
    int mid = (ishod.size()/2);
    int start = 0;
    int end = ishod.size();
    //cout << ishod.at(mid).FIO << "   " << param << endl; 
    if(ishod.at(mid).FIO<param)
        return ishod.at(bpreq(ishod,param,mid+1,end));
    else if(ishod.at(mid).FIO>param)
        return ishod.at(bpreq(ishod,param,start,mid-1));
    return ishod.at(mid);
    }

class neints
{
public:
    int nomer;
    long int UnixTimeDataPriesda;
    long int UnixTimeDataOtezda;
    int Summaoplati;
    }; 
/// \brief Конвертация vector в multimap . 
/// \details Конвертирует vector<Otel> в multimap<string,neints>
/// \return Возвращает конвертируемый multimap    
multimap<string,neints> vectomap(vector<Otel> ishod)
{
    multimap<string,neints> ishodmap;
    for(int i = 0; i<ishod.size();i++)
    {
        neints a;
        a.nomer = ishod.at(i).nomer;
        a.UnixTimeDataPriesda = ishod.at(i).UnixTimeDataPriesda;
        a.UnixTimeDataOtezda = ishod.at(i).UnixTimeDataOtezda;
        a.Summaoplati = ishod.at(i).Summaoplati;
        string b = ishod.at(i).FIO;
        ishodmap.insert({b,a});
        }


    return ishodmap;
    }
    
int main()
{
    vector<Otel> a = import("20000.txt");
    vector<Otel> d = a;
    string nnnn = "Wanda Hansen";
    cout << "============================================================================" << endl << a.size()<<":" << endl;
    auto start = std::chrono::system_clock::now();
    pp(a,nnnn);
    auto stop = std::chrono::system_clock::now();
    cout << "Прямой Поиск:" << ((long double)(chrono::duration_cast<chrono::nanoseconds>(stop-start).count())/1000000000)<< endl << endl ;      

    a = QuickSort(a);
    start = std::chrono::system_clock::now();
    bp(a,nnnn);
    stop = std::chrono::system_clock::now();
    cout << "Бинарный Поиск:" << ((long double)(chrono::duration_cast<chrono::nanoseconds>(stop-start).count())/1000000000)<< endl << endl ;
    
    multimap<string,neints> c;
    c = vectomap(d);
    
    start = std::chrono::system_clock::now();
    c.find(nnnn);
    stop = std::chrono::system_clock::now();   
    cout << "Multimap Поиск:" << ((long double)(chrono::duration_cast<chrono::nanoseconds>(stop-start).count())/1000000000)<< endl << endl ;    
    cout << "============================================================================" << endl;
        
    return 0;
    } 
