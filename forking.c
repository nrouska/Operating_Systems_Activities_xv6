#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#define SLEEP_TIME 1
int main(int argc , char *argv[])
{
int n = 0;
printf("PID: %ld\n", (long)getpid ());
/* Sleep for SLEEP_TIME seconds. */
sleep(SLEEP_TIME);
while (1) {
sleep(SLEEP_TIME);
/* break after a few iterations (too many processes) */

if (n++ > 3) {   //Όταν έχουμε 16 διεργασίες (με τον γονέα) το πρόγραμμα τερματίζει
   sleep(20);// καθυστέρηση για να προλάβω να φορτώσω το module
   exit(0);
}
// Η fork() επιστρέφει 0 όταν καλείται από διεργασία-παιδί, 
// επιστρέφει το pid του παιδιού όταν καλείται απο διεργασία-γονέα
printf("Forked! PID: %d\n", fork()); 
}


return 0;
}