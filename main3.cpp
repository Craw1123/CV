#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <map>
#include <cmath>

using namespace std;


uint64_t bad_hash(string param)
{
uint64_t hash = 0;

    for(int i=0;i<4;i++)
    {
        for(int z = 0;z<param.size();z++)
        {
            hash += uint64_t((z*uint64_t(param[z]))^hash*11) & 0xFFFFFFFF;
            //cout << hash << endl;
            
            }
        
        
        }


    return hash;
    }

uint64_t good_hash(string param)
{
uint64_t hash = 0;

        for(int z = 0;z<param.size();z++)
        {
            hash += uint64_t((z*uint64_t(param[z]))-hash*11) & 0xFFFFFFFF;
            //cout << hash << endl;
            
            }
    return hash;
    }




class Otel
{
public:
    string FIO;
    int nomer;
    long int UnixTimeDataPriesda;
    long int UnixTimeDataOtezda;
    int Summaoplati;
    uint64_t bad_hash;
    uint64_t good_hash;
    }; 

string uttodt(long int a) //конвертация unixtime в дату 
{
    time_t t = a;    
    return asctime(localtime(&t));
    }
    
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
        a.bad_hash = bad_hash(a.FIO);
        a.good_hash = good_hash(a.FIO);
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
    
    
map<uint64_t,vector<int>> bad_hash_table(vector<Otel> a)
{
    map<uint64_t,vector<int>> returned;
    vector<int> index; index.push_back(0);
    returned[a[0].bad_hash] = index;
    for(int i=1;i<a.size();i++)
    {
        vector<int> index;
        if(returned.contains(a[i].bad_hash))
        {
            index = returned[a[i].bad_hash];
            index.push_back(i);
            returned[a[i].bad_hash] = index;
            //cout << "Ebat', ya nashel koliziu" << endl;
            }
        else
        {
            index.push_back(i);
            returned[a[i].bad_hash] = index;
        
            }
    
        }    

    return returned;
    }
    
map<uint64_t,vector<int>> good_hash_table(vector<Otel> a)
{
    map<uint64_t,vector<int>> returned;
    vector<int> index; index.push_back(0);
    returned[a[0].good_hash] = index;
    for(int i=1;i<a.size();i++)
    {
        vector<int> index;
        if(returned.contains(a[i].good_hash))
        {
            index = returned[a[i].good_hash];
            index.push_back(i);
            returned[a[i].good_hash] = index;
            //cout << "Ebat', ya nashel koliziu" << endl;
            }
        else
        {
            index.push_back(i);
            returned[a[i].good_hash] = index;
        
            }
    
        }    

    return returned;
    }    
    
    

Otel bad_hash_find(map<uint64_t,vector<int>> a,string needtofind,vector<Otel> b)
{
    if(a[bad_hash(needtofind)].size()>0)
    {
        for(int i=0;i<a[bad_hash(needtofind)].size();i++)
        {
            if(b[a[bad_hash(needtofind)][i]].FIO==needtofind)
                return b[a[bad_hash(needtofind)][i]];
            
            }
    
        }
    else
        return b[a[bad_hash(needtofind)][0]];



    }
    
    
    
Otel good_hash_find(map<uint64_t,vector<int>> a,string needtofind,vector<Otel> b)
{
    if(a[good_hash(needtofind)].size()>0)
    {
        for(int i=0;i<a[good_hash(needtofind)].size();i++)
        {
            if(b[a[good_hash(needtofind)][i]].FIO==needtofind)
                return b[a[good_hash(needtofind)][i]];
            
            }
    
        }
    else
        return b[a[good_hash(needtofind)][0]];



    }    

int main()
{


    vector<Otel> a = import("100000.txt");
    
    //Поиск по плохому хэшу
    auto bht = bad_hash_table(a);
    Otel bhfd = bad_hash_find(bht,"Dr. Lisa Craig",a);
    cout << bht.size() << " " << bhfd.FIO << " " << bhfd.nomer << endl;

    //vector<Otel> a = import("100000.txt");    
    //Поиск по хорошему хэшу
    auto ght = good_hash_table(a);
    Otel ghfd = good_hash_find(ght,"Dr. Lisa Craig",a);
    cout << ght.size()<<" "<<ghfd.FIO << " "<< ghfd.nomer;
    return 0;
    } 
