#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>

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

string uttodt(long int a) //конвертация unixtime в дату 
{
    time_t t = a;    
    return asctime(localtime(&t));
    }
    
bool operator >(Otel first,Otel second)
{
    if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
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
    return false;
    }

bool operator <(Otel first,Otel second)
{
    if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
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
    
bool operator >=(Otel first,Otel second)
{
    if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
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

bool operator <=(Otel first,Otel second)
{
    if(first.UnixTimeDataPriesda>second.UnixTimeDataPriesda)
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
    return true;
    }

vector<Otel> import(string path )
{
    vector<Otel> ret; string s;
    ifstream file(path);
    vector<string> elems;
    while(getline(file,s))
    {
        elems.clear();
        char *str = (char*)s.c_str();
        Otel a;
        char * pch = strtok(str,"/"); //тут и 5 строчек ниже сплитим по символу / если бы я знал, что в c++ нет сплита, я бы пошел писать на qt
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
int main()
{
    typedef std::chrono::duration<float> float_seconds;
    vector<Otel> a = import("import.txt");
    for(int i = 0; i<a.size();i++)
        cout << a.at(i).nomer << "\n";  
    cout << "Nachalniy" << a.size()<< "\n\n";    
  //  cout << a.at(0).nomer << endl << a.at(2).nomer << endl <<(a.at(0)<a.at(2))<< endl;          
  
  
    auto start = std::chrono::system_clock::now();
    a = myShakerSort(a);
    auto stop = std::chrono::system_clock::now();
    
    
    cout << "\n";
    for(int i = 0; i<a.size();i++)
        cout << a.at(i).nomer << "\n";
    cout << "Konechniy"<< a.size();  
    
    auto time = stop-start;
    cout << endl << endl <<std::chrono::duration_cast<float_seconds>(time).count() ;
        
    return 0;
    } 
