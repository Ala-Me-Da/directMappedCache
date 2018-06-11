# DirectMappedCache 
ECS 154a project to implement a 256-byte Direct mapped cache with 8 byte cache lines. Cache is byte-addressable and follows a write-back write policy and a write-allocate write miss policy. 


Input: A text file containing columns of 32-bit hexadecimal strings, e.g. 
```
36D2 FF C2
FC33 00 0B
... 

```
Output: This will output a text file named ```dm-out.txt```. 

# Compiling & Executing
Requires C++11 or greater to compile. 
Open command prompt and type the folloiwng to complie the project: 
```
g++ -std=c++11 DirectMappedCache.h DirectMappedCache.cpp dmcache2.cpp
```
Next, use run the resulting a.out executable with one of the test files. E.g., 
```
./a.out test-file.txt 
```

This will output ```dm-out.txt``` into the current directory. Each line of ```dm-out.txt``` corresponds to the results of each read operation peformed on the cache. The lines are formatted as follows: 

```
D2 0 1 
```
The information is, in order, the data returned from the read operation (in hexadecimal), a 'hit' output (0 or 1) that indicates whether or not the requested item was found in cache, and the dirty-bit (0 or 1) which indicates whether or not a memory block in cache was modified. 

# Implementation  

