#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define page_size 1024


#define DATA_0               0xFF0A0040 //  mio_B0 led output
#define DATA_1               0xFF0A0044 //  mio_B1 led output
#define DATA_2               0xFF0A0048 //  mio_B2 led output
#define DATA_3               0xFF0A004C //  Emio_B3 led output
#define DATA_4               0xFF0A0050 //  Emio_B4 led output
#define DATA_5               0xFF0A0054 //  Emio_B5 led output


#define DATA_1_RO            0xFF0A0064 // mio_1 key input
#define DATA_3_RO            0xFF0A006c // Emio_1 key input


int main() {
 
 int i=0;
 int c;
 int fd;

 unsigned long long page_addr_0, page_offset_0;
 unsigned long long page_addr_1, page_offset_1;
 unsigned long long page_addr_2, page_offset_2;
 unsigned long long page_addr_3, page_offset_3;
 unsigned long long page_addr_4, page_offset_4;
 unsigned long long page_addr_5, page_offset_5;


 unsigned long long page_addr_input_1, page_offset_input_1;
 unsigned long long page_addr_input_3, page_offset_input_3;
 void *ptr;

 system("devmem 0xFF0A0000 w 0x0000DF0D");  // unlock
	

 system("devmem 0xFF0A0204 w 0xFFFFFFFF");  // set as output (DIRM_B0 ) //MIO
 system("devmem 0xFF0A0208 w 0xFFFFFFFF");  // enable (OEN_0 )

 system("devmem 0xFF0A0244 w 0x00004000");  // set as output (DIRM_B1 ) //MIO
 system("devmem 0xFF0A0248 w 0x00004000");  // enable (OEN_1 )

 system("devmem 0xFF0A02c4 w 0xFFFFFFFD");  // set as output (DIRM_B3 ) // EMIO
 system("devmem 0xFF0A02c8 w 0xFFFFFFFD");  // enable (OEN_3 ) //

 system("devmem 0xFF0A0304 w 0xFFFFFFFF");  // set as output (DIRM_B4 ) // EMIO 
 system("devmem 0xFF0A0308 w 0xFFFFFFFF");  // enable (OEN_4 ) //

 system("devmem 0xFF0A0344 w 0xFFFFFFFF");  // set as output (DIRM_B5 ) // EMIO
 system("devmem 0xFF0A0348 w 0xFFFFFFFF");  // enable (OEN_5 ) 



 /* Open /dev/mem file */
 fd = open ("/dev/mem", O_RDWR);
    if (fd < 1) {
		return -1;
	}

//page_offset_input === mio key
        page_addr_input_1 = (DATA_1_RO & (~(page_size-1)));
	page_offset_input_1 = DATA_1_RO - page_addr_input_1;

//page_offset_input === EMIO key
        page_addr_input_3 = (DATA_3_RO & (~(page_size-1)));
	page_offset_input_3 = DATA_3_RO - page_addr_input_3;


//page_offset_output === mio LED
        page_addr_0 = (DATA_0 & (~(page_size-1)));
	page_offset_0= DATA_0 - page_addr_0;

//page_offset_output === mio LED
        page_addr_1 = (DATA_1 & (~(page_size-1)));
	page_offset_1 = DATA_1 - page_addr_1;

//page_offset_output === emio LED
        page_addr_3 = (DATA_3 & (~(page_size-1)));
	page_offset_3 = DATA_3 - page_addr_3;

//page_offset_output === EMIO LED
        page_addr_4 = (DATA_4 & (~(page_size-1)));
	page_offset_4 = DATA_4 - page_addr_4;

//page_offset_output === EMIO LED
        page_addr_5 = (DATA_5 & (~(page_size-1)));
	page_offset_5 = DATA_5 - page_addr_5;



	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr_0);
    if(ptr < 0){
      printf("mmap failed. see errno for reason.\n");
      close(fd);
      return -1;
    }
  




  //led blink once
			  *((int*)(ptr + page_offset_3)) = 0x00000005; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000
			  for(long int i=0; i< 50000000; i++);
			  *((int*)(ptr + page_offset_3)) = 0x00000000;
			  for(long int i=0; i< 50000000; i++);  

			  *((int*)(ptr + page_offset_4)) = 0xFFFFFFFF; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000
			  for(long int i=0; i< 50000000; i++);
			  *((int*)(ptr + page_offset_4)) = 0x00000000;
			  for(long int i=0; i< 50000000; i++);  

			  *((int*)(ptr + page_offset_5)) = 0xFFFFFFFF; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000
			  for(long int i=0; i< 50000000; i++);
			  *((int*)(ptr + page_offset_5)) = 0x00000000;
			  for(long int i=0; i< 50000000; i++);  

  //led blink once
			  *((int*)(ptr + page_offset_1)) = 0x00004000; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000
			  for(long int i=0; i< 50000000; i++);
			  *((int*)(ptr + page_offset_1)) = 0x00000000;
			  for(long int i=0; i< 50000000; i++);  

  
      for(;;){
				
			printf("DATA_3_RO: %d\n", *(int*)(ptr + page_offset_input_3)); 
                if (!(*(int*)(ptr + page_offset_input_3) & 0x00000002)) // if (key ==0)	
                  {   
                      *((int*)(ptr + page_offset_3)) = 0x00000005; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000____led blink
                      for(long int i=0; i< 50000000; i++);
                      *((int*)(ptr + page_offset_3)) = 0x00000000;
                      for(long int i=0; i< 50000000; i++);  
                  }


		if (!(*(int*)(ptr + page_offset_input_1) & 0x00000001)) // if (key ==0)
                  {   
                      *((int*)(ptr + page_offset_1)) = 0x00004000; //MIO40 *((int*)(ptr + page_offset+4)) = 0x00004000____led blink
                      for(long int i=0; i< 50000000; i++);
                      *((int*)(ptr + page_offset_1)) = 0x00000000;
                      for(long int i=0; i< 50000000; i++);  
                  }
            
            }
	

	munmap(ptr, page_size);
	close(fd);

	return 0;
}




