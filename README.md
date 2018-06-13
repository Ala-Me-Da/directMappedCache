# DirectMappedCache Overview & Introduction 


The DirectMappedCache repo contains files for a computer architecture project to implement a 256-byte Direct mapped cache with 8 byte long cache lines. The cache is byte-addressable, and follows a write-back write policy and a write-allocate write miss policy. 

Input: A text file containing a column of 32-bit hexadecimal numbers stored as strings, e.g. 

```
36D2 FF C2
FC33 00 0B
... 
```

The individual bits of the hexadecimal string contains ```data``` to be stored in cache, and instructions on how to store the data in cache, and in main memory during a 
write-miss / read-miss eviction. These instructions are the ```tag```, ```address``` (```offset```, and ```line number```), and the read/write indicator: 0x00 = read, 0xFF = write. 

For example, extracting these features for one line ```36D2 FF C2``` is done as follows: 

```tag```: Is the first leftmost 8-bits in the hexadecimal number: 36.

```data``` : Is the last rightmost 8-bits in the hexadecimal number: C2.

read/write operation: 8-bits in the middle of the hexadecimal number: FF => write to cache. 

```address``` : The next 8-bits after the tag, D2. 
The address is paritioned into an ```offset``` and ```line number```. The address assigns values to ```offset``` and ```line number``` depending on the memory size of the cacheline and number of cachelines, respectively.
 
In this case, the offset requires 3-bits since the cacheline is 8 bytes, and would need only log_2(8) = 3 bits for addressing.
Simiarily, the line number requires the remaining  5-bits. There are 32 cachelines, so 5-bits are necessary for addressing. 
Example: 
D2 = 11010010, which means offset = 010 = 2, and line number = 11010 = 26. So given a write operation, the program stores  ```data```  in line number 26, inside line number 26's memory block at index 2. 


Note: When a read operation is performed, the  ```data```  contained in the hexadecimal string is ignored. 

Output: A text file named ```dm-out.txt```. Each line of ```dm-out.txt``` corresponds to the results of each read operation peformed on the cache. The lines are formatted as follows: 


```

D2 0 1 

```

The information is, in order, the data returned from the read operation (in hexadecimal), a 'hit' output (1 or 0) that indicates whether or not the requested item was found in cache, and the dirty-bit (1 or 0) which indicates whether or not a memory block in cache was modified. 

# Compiling & Executing

Requires C++11 or greater to compile. 
Open command prompt, enter the directory containing this repo, and type the following to complie the project: 

```
g++ -std=c++11 DirectMappedCache.h DirectMappedCache.cpp dmcache2.cpp
```


Next, use run the resulting a.out executable with one of the test files. E.g.,
```
./a.out test-file.txt 
```
This will output ```dm-out.txt``` into the current directory. 

# Implementation  

## DirectMappedCache.h 

Header file containing the definition of the ```DirectMappedCache``` class and its core member functions and variables. 

## DirectMappedCache.cpp 

Implements ```read()```, ```write()``` operations and ```evict_cache_line()``` function for the write-allocate write-miss policy.  

## dmcache2.cpp

```dmcache2.cpp``` is the main .cpp file to utilizing the ```DirectMappedCache``` class to produce run the direct mapped cache simulation and produce the desired ```dm-out.txt``` showcasing correct direct mapped cachcing behavior. 

Direct mapped cache is implemented utilizing an object-oriented approach. A class ```DirectMappedCache``` is defined to use by the main program ```dmcache2.cpp``` for reading/writing to cache. 

Cache is modelled as an array of 256/8 = 32 cache lines objects. The cache line object is defined as a  struct ``` cacheLine```  with member variables integers ```tag``` , ``` dirty_bit``` , and then an integer array ``` memory_block``` . ```memory_block``` forms the core of the cacheline, as it stores the data partitioned from the input text file. 

Main memory, or RAM, is treated as an array of caches. Each array element is a ``` Block```  object which in itself contains an array of 32 ``` cacheLine```  objects. This model makes for easy eviction between ``` cache```  and ``` RAM```  whenever a cache miss calls the ``` evict_cache_line''' member function. 
 
