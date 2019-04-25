/** Load PRU program and run it
*/

#include <stdio.h>
#include <stdlib.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU	0

int main (void)
{
   if(getuid()!=0){
      printf("Please run program as root.\n");
      exit(EXIT_FAILURE);
   }
   // Initialize structure needed for PRU use
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Initialize PRU
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);

   // Map PRU's interrupts
   prussdrv_pruintc_init(&pruss_intc_initdata);
   
   // Load and execute the PRU program on the PRU
   prussdrv_exec_program (PRU, "./assign5_pru.bin");

   // Wait PRU program to finish (it won't because it is infinite loop)
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("PRU exited with code %d.\n", n);

   // Stop PRU
   prussdrv_pru_disable(PRU);
   prussdrv_exit ();
   return EXIT_SUCCESS;
}

