#include <bits/stdc++.h> 
#include <vector>
#include <string>
#include <cmath>

using namespace std;

string convertToBinary(int ele, int maxbits){ 
    //funciton to convert element to be mapped to its binary representation
    string binary;
    int i=maxbits-1;
    while(i>=0){
        if(ele-pow(2,i)>=0){ 
            binary.push_back('1'); 
            ele-=pow(2,i);
        }
        else    
            binary.push_back('0');
        i--;
    }
    return binary;
}


void fifo_replace(vector<string>& cache, string tagBin, string prev, int load, int writeback, vector<int>& dirtybits, int &hits, int &misses, int& elementHit){ 
    // Function for FIFO (First In First Out) replacement policy in cache
    for(int i=0;i<cache.size();i++){ //iterating through the cache and checking if the tag bits of the element already exists in the cache
        if(cache[i]==tagBin){
            hits++;
            elementHit=1;
            cout<<"Its a cache HIT. ";
            if(!load && writeback){
                cout<<"Dirty bit becomes 1 through writeback policy. ";
                dirtybits[i]=1;
            }
            else
                cout<<"We are using write-through policy.";
            cout<<"\n"<<endl;
            return;
        }
    }
    misses++;
    elementHit=0;
    cout<<"Its a cache MISS"<<endl;
    for(int i=0;i<cache.size();i++){ //finding an empty slot in the cache and storing the new tag in that
        if(cache[i]=="-1"){
            cache[i]=tagBin;
            return;
        }
    }
    for(int i=0;i<cache.size();i++){ //since there's no empty slot so replacing the oldest value with the new tag
        if(cache[i]==prev){
            cache[(i+1)%cache.size()]=tagBin;
        }
    }
}

void lru_replace(vector<string>& cache, map<int, string>& mp, string tagBin, int currnum, int load, int writeback, vector<int>& dirtybits, int& hits, int& misses){
    // Function for LRU (Least Recently Used) replacement policy in cache
    if(mp.empty()){ //condition when cache is empty- no tags in cache
        misses++;
        cout<<"Its a cache MISS"<<endl;
        mp[currnum]=tagBin;
        cache[0]=tagBin;
        return;
    }
    for(int i=0;i<cache.size();i++){ //iterating through the cache and checking if the tag bits of the new element already exists in the cache
        if(cache[i]==tagBin){
            hits++;
            cout<<"Its a cache HIT. ";
            if(!load && writeback){
                dirtybits[i]=1;
                cout<<"Dirty bit becomes 1 using writeback policy.";
            }
            cout<<"\n";
            mp[currnum]=tagBin;
            break;
        }
        if(cache[i]=="-1"){
            misses++;
            cout<<"Its a cache MISS"<<endl;
            cache[i]=tagBin;
            mp[currnum]=tagBin;
            return;
        }
    }
    int minn=INT_MAX;
    int x=0;
    for(auto it: mp){ //iterating through the map and finding the tag with the numerically least key
        if(it.second==tagBin && it.first!=currnum){ //finding the key-value pair with the same tag in case of a hit and removing it from map
            mp.erase(it.first);
            return;
        }
        minn=min(minn, it.first);
        x++;
    }
    misses++;
    cout<<"Its a cache MISS"<<endl;
    string toreplace=mp[minn];
    for(int i=0;i<cache.size();i++){ //iterating through the cache and finding the tag thats the least recently used and replacing it
        if(cache[i]==toreplace)
            cache[i]=tagBin;
    }
    mp.erase(minn);
    mp[currnum]=tagBin;
}


void fullyAssociative(vector<int> mainmem, int wordField, int tagField, int load, int writeback, int lru, int chSize){
    int blockSize=chSize-wordField;
    vector<string> cache(pow(2,blockSize), "-1"); 
    map<int, string> mp;
    vector<int> dirtybits(pow(2, blockSize), 0);      

    int hits=0;
    int misses=0;
    string prev="-1";
    int elementHit=0;

    for(int i=0;i<mainmem.size();i++){ //iterating through each element in the main memory
        int element=mainmem[i];
        cout<<"Element to be mapped: "<<element<<endl;
        string binary;
        binary=convertToBinary(element, wordField+tagField); //converting element to its binary representation and displaying it
        cout<<"Binary representation: ";
        for(int i=0;i<binary.length();i++){
            cout<<binary[i];
        }
        cout<<"\n";

        string tagBin=binary.substr(0, tagField); 

        string wordBin=binary.substr(tagField, wordField); 

        cout<<"Block\t\t\tOffset"<<endl;
        cout<<tagBin<<"\t\t"<<wordBin<<"\n"<<endl; // Extract tag and word fields from the binary representation

        // // Replace tag in cache based on the specified replacement policy (FIFO or LRU)
        if (lru && load)
            lru_replace(cache, mp, tagBin, i+1, load, writeback, dirtybits, hits, misses);

        else
            fifo_replace(cache, tagBin, prev, load, writeback, dirtybits, hits, misses, elementHit);


        // // Print the updated cache
        cout<<"\nNew cache:\n "<<endl;
        cout<<"      Tag\t\t     Dirty bits"<<endl;
        for(int i=0;i<cache.size();i++){
            if(cache[i]=="-1")
                cout<<"--\t\t\t\t"<<dirtybits[i]<<endl;
            else
                cout<<cache[i]<<"\t\t\t"<<dirtybits[i]<<endl;
        }
        cout<<"\n";
        cout<<"\n";
        if(!elementHit)
            prev=tagBin;
    }

    // Calculate hit and miss ratios
    float hitRatio=(float)hits/mainmem.size();
    float missRatio=(float)misses/mainmem.size();
    cout<<"Hit Ratio: "<<hitRatio<<endl; 
    cout<<"Miss Ratio: "<<missRatio<<endl;
}


int main(){
    int mmSize, chSize, word;
    cout<<"Enter size of main memory(in bytes): 2^";  
    cin>>mmSize; 
    cout<<"Enter size of cache memory(in bytes): 2^";
    cin>>chSize;
    cout<<"Offset bits: ";
    cin>>word;
    int tag;
    tag=mmSize-word;
    cout<<"\nTag and Offset fields for cache: "<<endl; 
    cout<<"Block\tOffset"<<endl;
    cout<<tag<<"\t"<<word<<endl;

    int size;
    cout<<"\n\nHow many input addresses are you giving: ";
    cin>>size; 

    vector<int> mainmem; 

    cout<<"Input a sequence of main memory addresses: ";
    for(int i=0;i<size;i++){
        int address;
        cin>>address; 
        mainmem.push_back(address); 
    }

    int lru;
    cout<<"LRU or FIFO replacement policy?('0' for FIFO, '1' for LRU): ";
    cin>>lru;

    int load;
    cout<<"Is it instruction load or store?('0' for store, '1' for load): ";
    cin>>load;

    int writeback;
    if(!load){
        cout<<"Is it instruction Write-back or Write-through?('0' for write-through, '1' for write-back): ";
        cin>>writeback;
    }

    cout << "\nMapping the following elements based on fully associative mapping using " << (lru ? "Least Recently Used replacement technique" : "First In First Out replacement technique") << ":\n\n" << endl;

    // Perform fully associative mapping based on the specified parameters
    fullyAssociative(mainmem, word, tag, load, writeback, lru, chSize);
}