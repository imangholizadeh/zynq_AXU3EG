#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define page_size 1024

#define XGPIOPS_DATA_OFFSET  0xFF0A0040 // mio
#define DATA_1               0xFF0A0044 // mio
#define DATA_2               0xFF0A0000 // emio 
#define DATA_3               0xFF0A0000 // emio



int main() {
 
 int i=0;
 int c;
 int fd;

 unsigned long long page_addr, page_offset;
 void *ptr;


 system("devmem 0xFF0A0000 w 0x0000DF0D");  // unlock

 system("devmem 0xFF0A0204 w 0xFFFFFFFF");  // set as output (XGPIOPS_DIRM_OFFSET ) 
 system("devmem 0xFF0A0244 w 0x00004000");  // set as output (DIRM_1 ) 
 //system("devmem 0xFF0A0000 w 0xFFFFFFFF");  // set as output (DIRM_2 )   EMIO
 //system("devmem 0xFF0A0000 w 0xFFFFFFFF");  // set as output (DIRM_3 )   EMIO

 system("devmem 0xFF0A0208 w 0xFFFFFFFF");  // enable (XGPIOPS_OUTEN_OFFSET)    
 system("devmem 0xFF0A0248 w 0x00004000");  // enable (OEN_1 )
 //system("devmem 0xFF0A0000 w 0xFFFFFFFF");  // enable (OEN_2 )      EMIO   
 //system("devmem 0xFF0A0000 w 0xFFFFFFFF");  // enable (OEN_3 )      EMIO


 /* Open /dev/mem file */
 fd = open ("/dev/mem", O_RDWR);
    if (fd < 1) {
//		perror(argv[0]);
		return -1;
	}


        page_addr = (XGPIOPS_DATA_OFFSET & (~(page_size-1)));
	page_offset = XGPIOPS_DATA_OFFSET - page_addr;
	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr);
    if(ptr < 0){
      printf("mmap failed. see errno for reason.\n");
      close(fd);
      return -1;
    }
       
      for(;;){
          
          *((int*)(ptr + page_offset)) = 0xFFFFFFFF;
		  *((int*)(ptr + page_offset+4)) = 0x00004000; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000
		  
          for(long int i=0; i< 50000000; i++);
          *((int*)(ptr + page_offset)) = 0x00000000;
          *((int*)(ptr + page_offset+4)) = 0x00000000;
		  
          for(long int i=0; i< 50000000; i++);  

      }
	

	munmap(ptr, page_size);
	close(fd);

	return 0;
}



