#include <bitset> 
#include <cstdlib>
#include <iostream> 
#include <sstream> 

#include "DirectMappedCache.h" 

int get_line_num(std::string address); 
int get_offset(std::string address); 

int main(int argc, char** argv) 
{
	/* Dynamically allocate a DirectMappedCache object; Constructor opens the final output file dm-out.txt */ 
	DirectMappedCache* dmc = new DirectMappedCache(); 
	
	/* Use ifstream object to open file containing input: 32-bit hex string whose bits given tag, address, op, and data */ 
	std::ifstream inFile;
	std::string str;
 	
	/* Input is retrived as command line input */ 
	inFile.open(argv[1]); 

	/* Loop to extract hex code one line at a time, store each letter in str, & parse into tag, address, and data.
	   Read or write operation is determine by the if-statement in the loop. 
	   Once address is parsed, further divide it into offset (memory block number) and cacheline number. 
	   line number and offset are crucial in storing data. */ 
	while(std::getline(inFile, str, '\n'))
	{
	   //Creates strings of size 2 to hold hex strings representing tag, address, and data. 
	   std::string address("  "); 
	   std::string data("  "); 
	   std::string tag("  "); 
	   int lineNum; 
	   int offset;
	   int integerTag; 
	   int integerData; 
 	
	   /* str contains the entire 32-bit hexadecimal string in a string object. The string is a 10 element char array. 
	    *  Different parts of the string represent our tag, data, and address for the cache to use. 
	    *  index 0 - 1: tag (8 bits) 
	    *  index 2 - 3: address (8 bits) 8 bits are parsed depending on # of cachelines and # of bytes in each cacheline
	    *  index 4    : white space 
	    *  index 5 - 6: read/write operation ('00' = read, 'FF' = write)
            *  index     7: white space 
	    *  index 8 - 9: data (8 bits) to be stored in cache */ 
	   tag[0] = str[0]; 
	   tag[1] = str[1]; 
	   data[0] = str[8]; 
	   data[1] = str[9]; 
	   address[0] = str[2]; 
	   address[1] = str[3]; 
	
	   /* Convert string to integer */ 
	   lineNum = get_line_num(address); 
	   offset = get_offset(address); 
	   integerTag = dmc->hex_to_decimal(tag.c_str());
	   integerData = dmc->hex_to_decimal(data.c_str()); 
 	   
	   if(str[5] == '0') { 
 	 	   /*  Read operation */   
		  dmc->read(lineNum, offset, integerTag); 
	    } else { 
		   /* Write operation */ 
	       	  dmc->write(lineNum, offset, integerTag, integerData); 		
	    } 
	}
 	
	/* Garbage collection */ 
	delete dmc; 
	inFile.close();  
	
	return 0;  
} 

/* Helper functions 
 *  These functions parse the individual bits of the address 
 *  to extract the offset and cache line number
 *  address bits 0 - 2: offset 
 *  address bits 3 - 7: line number 
 */ 

int get_line_num(std::string address) 
{
	/* store address in stringstream with format flag hex
	 *  this allows storeage of the address an a number. 
	 *  bitset container to work with individual bits, and to 
	 *  convert those bits into integers using bitset::to_ulong 
	 */ 
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
	
	return lineBits.to_ulong();
} 


int get_offset(std::string address) 
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

	return blockBits.to_ulong(); 
} 
