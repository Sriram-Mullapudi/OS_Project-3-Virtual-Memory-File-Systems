#include "types.h"
#include "stat.h"
#include "user.h"

int main (int argc , char *argv[]){
	int ticks;

	if(argc!=2){
	   printf(2,"Usage : Sleep <ticks>\n");
           exit();
        }
        ticks=atoi(argv[1]);
        if(ticks<=0){
             printf(2,"Invalid number of ticks : %d\n",ticks);
             exit();
        }
        sleep(ticks);
        exit();
}
