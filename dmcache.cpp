#include <iostream> 
#include <iomanip> //This for uppercase hex decimal values 
#include <fstream> 
#include <string.h> 
#include <sstream> 
#include <math.h> 
#include <stdlib.h> 
#include <algorithm> 
#include <bitset> 

void readCache(std::string tagptr, int lineNum, int offset); 
void writeCache(std::string tag, int lineNum , int offset, std::string data); 
void evictCacheLine(std::string newTag, int lnum,  std::string oldTag,  int blockNum); 
int hexToDecimal(std::string hexstr); 

#define BLOCK_SIZE 8 
#define CACHE_SIZE 32 
#define RAM_SIZE 65536 

class cacheLine { 
	
	public: 	
	std::string tag;    
	int memoryBlock[BLOCK_SIZE]; 
	int dirty; 
	
	cacheLine() 
	    { memset(memoryBlock, 0, sizeof(memoryBlock)); } 
}; 

class Block {

	public: 
	//int array[BLOCK_SIZE]; 
	cacheLine array[CACHE_SIZE]; 
	
	Block() 
	    { memset(array, 0, sizeof(array)); } 

	
}; 
// Fix this to avoid memory leaks 
bool isCacheEmpty = true;
bool isRAMEmpty = true; 
Block RAM[RAM_SIZE]; 
cacheLine cache[CACHE_SIZE]; 
std::ofstream outputFile("dm-out.txt", std::ios::app); 

void readCache(std::string tag, int lineNum, int offset) 
{
	int lnum = lineNum;
	int ofs  = offset;   
	int hit = 1; 
 	
 	
	/* Read Miss if true, Read Hit otherwise */ 
 	if(tag != cache[lnum].tag) { 
		hit = 0;  
		evictCacheLine(tag, lnum, cache[lnum].tag, offset);
		outputFile << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << cache[lnum].memoryBlock[ofs] << " " << hit << " " << cache[lnum].dirty << "\n";
		cache[lnum].dirty = 0; 
	} else {
		outputFile << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << cache[lnum].memoryBlock[ofs] << " " << hit << " " << cache[lnum].dirty << "\n";
		//cache[lnum].dirty = 0; 
	} 
} 

void writeCache(std::string tag, int lineNum, int offset, std::string data) 
{ 
	int lnum = lineNum;  
	int ofs = offset; 
	int d = hexToDecimal(data);  
	
	if(isCacheEmpty) { 
		cache[lnum].tag = tag; 
		cache[lnum].memoryBlock[ofs] = d; 
		cache[lnum].dirty = 1; 
		isCacheEmpty = false;  
		return; 
	} 

	if(tag != cache[lnum].tag ) { 
		/* Write Miss. Evict current line then write to new cache line, set dirty bit */
		evictCacheLine(tag, lnum, cache[lnum].tag, offset);
		//std::cout << "Write w/ evict: " <<  tag << " " << lineNum << " " << offset << " " << d << std::endl;   
		cache[lnum].memoryBlock[ofs] = d; 
		cache[lnum].dirty = 1;   
	} else { 
		/* Store the data in cache, set dirty bit */ 
		cache[lnum].memoryBlock[ofs] = d; 
		cache[lnum].dirty = 1; 
	}  
}   

int hexToDecimal(std::string hexstr) 
{
	return (int)strtol(hexstr.c_str(), NULL, 16); 	
}

int getLineNumber(std::string address) 
{
	std::stringstream ss; 
	ss << std::hex << address; 
	unsigned num; 
	ss >> num; 
	std::bitset<8> addressBits(num);  
	std::bitset<5> lineBits; 

	for(int i = 3; i < addressBits.size(); i++) 
	{
		lineBits.set(i - 3, addressBits[i]); 
	} 
	
	return (int)lineBits.to_ulong(); 
} 

int getBlockNumber(std::string address) 
{
	std::stringstream ss; 
	ss << std::hex << address; 
	unsigned num; 
	ss >> num; 
	std::bitset<8> addressBits(num); 
	std::bitset<3> blockBits; 

	for(int i = 0; i < 3; i++) 
	{
		blockBits.set(i, addressBits[i]); 
	} 

	return (int)blockBits.to_ulong(); 
}


void evictCacheLine(std::string newTag, int lnum,  std::string oldTag, int blockNum) 
{ 	
	int nTag = hexToDecimal(newTag); 
	int oTag = hexToDecimal(oldTag); 

	memmove(RAM[oTag].array[lnum].memoryBlock, cache[lnum].memoryBlock, sizeof(int)*BLOCK_SIZE); 
	memmove(cache[lnum].memoryBlock, RAM[nTag].array[lnum].memoryBlock, sizeof(int)*BLOCK_SIZE); 
	cache[lnum].tag = newTag; 

	/* std::swap(RAM[oTag].array, cache[lnum].memoryBlock); 
	std::swap(cache[lnum].memoryBlock, RAM[nTag].array); 
	cache[lnum].tag = newTag; 
	*/ 
	
 
} 

int main(int argc, char **argv) 
{  
	std::ifstream inFile;
	std::string str; 
	inFile.open(argv[1]); 

	while(std::getline(inFile, str, '\n'))
	{
	   std::string address("  "); 
	   std::string data("  "); 
	   std::string tag("  "); 
	   int lineNum; 
	   int blockNum;

	   tag[0] = str[0]; 
	   tag[1] = str[1]; 
	   data[0] = str[8]; 
	   data[1] = str[9]; 
	   address[0] = str[2]; 
	   address[1] = str[3]; 

	   lineNum = getLineNumber(address); 
	   blockNum = getBlockNumber(address); 

	   if(str[5] == '0') { 
 	 	   /*  Read operation */   
		  readCache(tag, lineNum, blockNum); 
	    } else { 
		   /* Write operation */ 
	       	  writeCache(tag, lineNum, blockNum, data); 		
	    } 
	} 
	inFile.close(); 
	outputFile.close(); 
	return 0; 
} 
