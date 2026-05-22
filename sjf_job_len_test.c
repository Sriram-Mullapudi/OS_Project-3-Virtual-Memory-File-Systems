#include "types.h"
#include "stat.h"
#include "user.h"


int main(void) {
	printf(1, "Predicted job length of process(PID: %d), is %d \n", getpid(), sjf_job_length(getpid()));
    

    exit();
}

