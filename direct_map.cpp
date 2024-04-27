#include <bits/stdc++.h> 
#include <vector>
#include <string>
#include <cmath>

using namespace std;


//convertToBinary function is basically checking if element-2^x is greater than or equal to 0. x value starts with 
//being the maximum - maxbits-1, and then goes on till 0. if the difference is greater than 0 then you add 1 to the 
//string and subtract that number from element. else you add 0 to the string and continue.
//for example- if element is 20.
//first x value will be maxbits-1 = 7-1 = 6.
//so the loop checks if 20-2^6 >= 0. its not. so it adds 0 to string.
//then it checks if 20-2^5 >= 0. its not. so it adds 0 to string.
//then 20-2^4 >= 0.it is. so it adds 1 to string and element = 20-2^4 = 4.
//then 4-2^3 >= 0. its not, 1 is added to string.
//then 4-2^2 >= 0. it is. so 1 is added to string and element = 0.
//then 0-2^1 >= 0. 0 added to string.
//0-2^0 >= 0. 0 added to string.
//so string becomes 0010100

string convertToBinary(int ele, int maxbits){
    string binary;
    int i=maxbits-1;
    while(i>=0){
        if(ele-pow(2,i)>=0){  //pow is power function. it takes 2 parameters- base and exponent, 1st one is base, 2nd is exponent.
            binary.push_back('1'); //push_back is the parameter used for putting elements at the back in a string/vector.
            ele-=pow(2,i);
        }
        else    
            binary.push_back('0');
        i--;
    }
    return binary;
}


//convertToDec function is basically finding the decimal representation of the value in blockField.
//It takes each character from the string and converts it to int. theres a exp variable which in the beginning is equal to 
//blockField-1. so we find 2^exp and multiply that by each character.
//for example-
//blockField=2. and let temp= 01. exp = 2-1 = 1.
//so first, character at index 0 is converted to int. then we find 2^exp * 0. which is 0.
//exp--
//then, character at index 1 is converted to int. then we find 2^exp * 1. which is 1.
//so decimal representation of 01 is 1.

int convertToDec(string temp, int blockField){
    int exp=blockField-1;
    int decimal=0;
    for(char digit:temp){ //this loop is basically iterating for all characters in the string
        int numValue=digit-'0'; //converting charcter from string to int
        decimal+=numValue*pow(2, exp);
        exp--;
    }
    return decimal;
}

//in the end which displaying cache after each element is added, we are also displaying the block value of each- 00, 01, 10, 11 etc. 
//for printing that, I put all those values as a string in an array. and then each iteration I displayed one of the value.


vector<string> blocks(int blockField){
    vector<string> blocksList; //array of strings
    for(int i=0;i<pow(2, blockField);i++){ //this loop basically runs from 0 to 2^2 in this case. so 4 values are produced- 0,1,2,3.
                                            //they are then converted to binary and put in array.
        string temp=convertToBinary(i, blockField); //converting them to binary using the function
        blocksList.push_back(temp); //pushing the string in the temp array
    }
    return blocksList;
}


void directMapping(vector<int> mainmem, int wordField, int blockField, int tagField, int load, int writeback){
    vector<string> cache(pow(2, blockField), "-1"); //this is basically initialising a vector having string elements. and its size is initialised 
                                                    //as 2^blockField. which in this case is 2^2 = 4. so at a time only 4 elements will be in our cache
                                                    //array. And "-1" is used to initialise all 4 values as a string having "-1" by default.     
    vector<int> dirtybits(pow(2, blockField), 0);                                                                          
    int hits=0;
    int misses=0;
    for(int i=0;i<mainmem.size();i++){ //iterating through all elements given by user
        int element=mainmem[i];
        cout<<"Element to be mapped: "<<element<<endl;
        string binary;
        binary=convertToBinary(element, wordField+blockField+tagField); //this string represents the binary value of the current element
        cout<<"Binary representation: ";
        for(int i=0;i<binary.length();i++){
            cout<<binary[i];
        }
        cout<<"\n";

        //substr function takes 2 parameters :- first one is the starting index and 2nd is how many elements after that should be in the 
        //substring. so if its substr(2,7), then starting from 2nd index till 2+7 = 9th index, are considered. note 9th index is not counted.
        //2nd index-8th index values are taken.
        string tagBin=binary.substr(0, tagField); //from the string representating the binary value of the element, we take the substring with index 0 to 
                                                  //index 0+tagField. and this part of the binary value is representing the tag value in binary.
        string blockBin=binary.substr(tagField, blockField); // similarly from the binary we take the substring form tagField index to tagField+blockField
                                                            // index.
        string wordBin=binary.substr(tagField+blockField, wordField); //same for this

        cout<<"Tag\tBlock\tWord"<<endl;
        cout<<tagBin<<"\t"<<blockBin<<"\t"<<wordBin<<"\n"<<endl;

   
        int blockInDec=convertToDec(blockBin, blockField); //finding the decimal value of block field.

        if(cache[blockInDec]=="-1"){ //checking if there is any element in that particular block in cache. in the beginning all values are -1. so
                                    // if its -1 then nothing has been put there yet.
            cout<<"Its a cache MISS\nThis cache line is empty."<<endl;
            misses++;
            cache[blockInDec]=tagBin; //put the value of tag field in that specific line in cache.
        }
        else{
            if(cache[blockInDec]==tagBin){ //if there is some value already in cache and that value is same as out current tag value. then its a hit.
                hits++;
                cout<<"Its a cache HIT"<<endl;
                if(!load && writeback){
                    dirtybits[blockInDec]=1;
                    cout<<"\nRequested tag is found in cache table. Dirty bit is updated to 1 using write-back policy"<<endl;
                }
                if(!load && !writeback){
                    cout<<"\nRequested tag is found in cache table. using write through policy, memory and cache are updated simultaneously"<<endl;
                }
            }
            else{ // if theres some value in that line, but its not same as our current tag field value. then exchange it with our current value
                 // and it'll end up being a miss.
                misses++; 
                cout<<"Its a cache MISS.\nCache does not contain requested tag."<<endl;
                cache[blockInDec]=tagBin;
            }
        }

        cout<<"\nNew cache:\n "<<endl;
        vector<string> blocksList;
        blocksList=blocks(blockField); //for displaying the blocks each time while printing the cache
        cout<<"Block\tTag\tDirty bits"<<endl;
        for(int i=0;i<cache.size();i++){
            cout<<blocksList[i]<<"\t";
            if(cache[i]=="-1") //if that line in cache is empty just display --
                cout<<"--\t"<<dirtybits[i]<<endl;
            else
                cout<<cache[i]<<"\t"<<dirtybits[i]<<endl;
        }
        cout<<"\n";
        cout<<"\n";
    }

    float hitRatio=(float)hits/mainmem.size();
    float missRatio=(float)misses/mainmem.size();
    cout<<"Hit Ratio: "<<hitRatio<<endl; //printing hit ratio
    cout<<"Miss Ratio: "<<missRatio<<endl; //printing miss ratio
}


int main(){
    int mmSize, chSize, word;
    cout<<"Enter size of main memory(in bytes): 2^"; //cout is used to display something in terminal in c++.  
    cin>>mmSize; //cin is used for taking an input and storing it in that vaiable.
    cout<<"Enter size of cache memory(in bytes): 2^";
    cin>>chSize;
    cout<<"Enter block size(in bytes): 2^";
    cin>>word;
    int tag, block;
    block=chSize-word;
    tag=mmSize-block-word;
    cout<<"Tag, Block and Word fields for cache: "<<endl; //'endl' can be thought as next line.
    cout<<"Tag\tBlock\tWord"<<endl;
    cout<<tag<<"\t"<<block<<"\t"<<word<<endl;

    int size;
    cout<<"How many input addresses are you giving: ";
    cin>>size; 

    vector<int> mainmem; //vector is basically a dynamic array. we can specify the size initially or later. and its a vector of integers.

    cout<<"Input a sequence of main memory addresses: ";
    for(int i=0;i<size;i++){
        int address;
        cin>>address; //we are taking each address from the user 
        mainmem.push_back(address); //and then putting them in the vector. push_back is used for putting elements at the back of a vector.
    }

    int load;
    cout<<"Is it instruction load?('1' for yes, '0' for no): ";
    cin>>load;

    int writeback;
    if(!load){
        cout<<"Write-back?('1' for yes, '0' for no): ";
        cin>>writeback;
    }

    cout<<"\nMapping the following elements based on direct mapping:\n\n"<<endl;

    directMapping(mainmem, word, block, tag, load, writeback);
}