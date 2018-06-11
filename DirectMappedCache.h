#ifndef DIRECT_MAPPED_CACHE_H
#define DIRECT_MAPPED_CACHE_H

#define BLOCK_SIZE 8
#define CACHE_SIZE 32 
#define RAM_SIZE 65536 

#include <cstdlib>
#include <cstring> 
#include <fstream> 
#include <iostream>  
#include <string> 

class DirectMappedCache {
	struct cacheLine {
		int memory_block[BLOCK_SIZE];  
		int dirty_bit; 
		int tag;	 
	}; 

	struct Block { 
		cacheLine ram_entry[CACHE_SIZE]; 
	}; 
 
	cacheLine cache[CACHE_SIZE]; 
	Block RAM[RAM_SIZE];  
	std::ofstream outputFile;  
	
	public:
	DirectMappedCache() { outputFile.open("dm-out.txt", std::ios::app); }  
 	~DirectMappedCache() { outputFile.close(); } 
	void read(int line_num, int offset, int tag); 
	void write(int line_num, int offset, int tag, int data); 
	void evict_cache_line(int line_num, int new_tag, int old_tag); 
	int hex_to_decimal(std::string hexstr) { return (int)strtol(hexstr.c_str(), 0, 16); } 	
}; 

#endif //DIRECT_MAPPED_CACHE_H 
