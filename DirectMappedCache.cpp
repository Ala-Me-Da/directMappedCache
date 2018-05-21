#include "DirectMappedCache.h" 

#include <cstring> 
#include <iomanip> //for I/O manipulators 

/* 
 * Reads a value from the cache member variable. 
 * The read() function writes the data read, the hit bit value, and dirty bit value 
 * our output file dm-out.txt 
 */ 
void DirectMappedCache::read(int line_num, int offset, int tag) 
{
	int hit = 1; 
	
	if(tag != cache[line_num].tag) { 
		hit = 0; 
		evict_cache_line(line_num, tag, cache[line_num].tag); 
		outputFile << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << cache[line_num].memory_block[offset] << " " << hit << " " << cache[line_num].dirty_bit << "\n";
		cache[line_num].dirty_bit = 0; 
	}  else { 
		outputFile << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << cache[line_num].memory_block[offset] << " " << hit << " " << cache[line_num].dirty_bit << "\n";
	} 
} //DirectMappedCache::read() 

/* 
 * Writes data to the cache member variable. 
 */ 
void DirectMappedCache::write(int line_num, int offset, int tag, int data) 
{
	if(isCacheEmpty) { 
		cache[line_num].tag = tag; 
		cache[line_num].memory_block[offset] = data; 
		cache[line_num].dirty_bit = 1; 
		isCacheEmpty = false;  
		return; 
	} 

	if(tag != cache[line_num].tag ) { 
		/* Write Miss. Evict current line then write to new cache line, set dirty bit */
		evict_cache_line(line_num, tag, cache[line_num].tag);    
		cache[line_num].memory_block[offset] = data; 
		cache[line_num].dirty_bit = 1;   
	} else { 
		/* Store the data in cache, set dirty bit */ 
		cache[line_num].memory_block[offset] = data; 
		cache[line_num].dirty_bit = 1; 
	} 

} //DirectMappedCache::write() 

/* 
 * Evicts a cacheline to RAM from the cache member variable to RAM upon a read miss or write miss. 
 * After an evict, a cacheline stored in RAM with the proper tag takes place of the old cache line. 
 */ 
void DirectMappedCache::evict_cache_line(int line_num, int new_tag, int old_tag) 
{
	std::swap(RAM[old_tag].ram_entry[line_num].memory_block, cache[line_num].memory_block); 
	std::swap(cache[line_num].memory_block, RAM[new_tag].ram_entry[line_num].memory_block); 
	cache[line_num].tag = new_tag; 
	
} //DirectMappedCache::evict_cache_line 

