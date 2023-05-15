#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define page_size 1024


#define DATA_1_RO            0xFF0A0064 // mio26 key input
#define DATA_1               0xFF0A0044 // mio40 led output



int main() {
 
 int i=0;
 int c;
 int fd;

 unsigned long long page_addr, page_offset;
 unsigned long long page_addr_input, page_offset_input;
 void *ptr;

 system("devmem 0xFF0A0000 w 0x0000DF0D");  // unlock
	
 system("devmem 0xFF0A0244 w 0x00004000");  // set as output (DIRM_1 ) //led mio_40 &&&&&& set as input (DIRM_1 ) //key mio_26

 system("devmem 0xFF0A0248 w 0x00004000");  // enable (OEN_1 ) //led mio 40

 /* Open /dev/mem file */
 fd = open ("/dev/mem", O_RDWR);
    if (fd < 1) {
		return -1;
	}

//page_offset_input === key
        page_addr_input = (DATA_1_RO & (~(page_size-1)));
	page_offset_input = DATA_1_RO - page_addr_input;


//page_offset_output === LED
        page_addr = (DATA_1 & (~(page_size-1)));
	page_offset = DATA_1 - page_addr;


	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr);
    if(ptr < 0){
      printf("mmap failed. see errno for reason.\n");
      close(fd);
      return -1;
    }
  
  //led blink once
			  *((int*)(ptr + page_offset)) = 0x00004000; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000
			  for(long int i=0; i< 50000000; i++);
			  *((int*)(ptr + page_offset)) = 0x00000000;
			  for(long int i=0; i< 50000000; i++);  

      for(;;){
                if (!(*(int*)(ptr + page_offset_input) & 0x00000001)) // if (key ==0)
                  {   
                      *((int*)(ptr + page_offset)) = 0x00004000; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000____led blink
                      for(long int i=0; i< 50000000; i++);
                      *((int*)(ptr + page_offset)) = 0x00000000;
                      for(long int i=0; i< 50000000; i++);  
                  }
            }
	

	munmap(ptr, page_size);
	close(fd);

	return 0;
}




