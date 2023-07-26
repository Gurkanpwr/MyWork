#include "threadfunctions.h"


#define thread_num 6
pthread_t threads[thread_num];

extern Buffer buffer;



long double usage;
volatile int Terminate=0;
FILE* fp;

time_t time_of_deadlock; //it will be stored here, if any deadlock occurs
pthread_cond_t Log_deadlock_time= PTHREAD_COND_INITIALIZER;
volatile int watch_counter[4]={0,0,0,0};

//pthread_mutex_t mutex;

void* Reader(__attribute__((unused)) void* p) {
    long double CPU[4]; //create temporal array.
    while(!Terminate){

        fp=fopen("/proc/stat","r");
        if(fp == NULL){
            perror("Reader couldn't open proc/stat");
            exit(EXIT_FAILURE);
        }

        fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&CPU[0],&CPU[1],&CPU[2],&CPU[3]);
        fclose(fp);
        add_to_buffer(CPU); // write to buffer

        sleep(1);
        watch_counter[0]++;

    }
    pthread_exit(NULL);
}

void* Analyzer(__attribute__((unused)) void* p){
    long double prev_tmp[4]={},tmp[4];
    while(!Terminate) {
        remove_from_buffer(tmp); // read from

        if(array_comp(tmp,prev_tmp,4) != 0){

            usage = ((tmp[0] + tmp[1] + tmp[2]) - (prev_tmp[0] + prev_tmp[1] + prev_tmp[2])) /
                    ((tmp[0] + tmp[1] + tmp[2] + tmp[3]) - (prev_tmp[0] + prev_tmp[1] + prev_tmp[2] + prev_tmp[3]))*100;

        }

        for(int i=0;i<4;i++){
            prev_tmp[i]=tmp[i];
        }
        sleep(1);
        watch_counter[1]++;
    }
    pthread_exit(NULL);
}

void* Printer(__attribute__((unused)) void* p){
    float data;
    while(!Terminate){

        data=(float)usage;
        progress_bar(data);


        sleep(1);
        watch_counter[2]++;

    }
    pthread_exit(NULL);
}

void* Watchdog(__attribute__((unused)) void* p){
    int last_progress[4] = {0,0, 0, 0};
    int deadlock_occurred;

    while(!Terminate){
        sleep(2);
        pthread_mutex_lock(&buffer.mutex); // counter has to be protected.
        deadlock_occurred = 0;
        for (int i = 0; i < 4; i++) {
            if (last_progress[i] == watch_counter[i]) {
                deadlock_occurred = 1;
                break;
            } else {
                last_progress[i] = watch_counter[i];
            }
        }
        pthread_mutex_unlock(&buffer.mutex);

        if (deadlock_occurred && !Terminate) {
            printf("Deadlock occurred. Attempting to fix.\n");
            time(&time_of_deadlock);
            pthread_cond_signal(&Log_deadlock_time); // Send signal to logger thread.
            // Attempt to resolve the deadlock by unlocking the mutex
            pthread_mutex_unlock(&buffer.mutex);
        }
    }

    pthread_exit(NULL);
}
void* Logger(__attribute__((unused)) void* p){
    FILE* Logfp;
    time_t last_deadlock_time = 0;

    while (!Terminate) {
        // Acquire the mutex lock
        pthread_mutex_lock(&buffer.mutex);
        watch_counter[3]++;
        // Wait for the deadlock_time_updated condition variable with a timeout
        struct timespec timeout;
        struct timeval now;
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec + 1;
        timeout.tv_nsec = now.tv_usec;
        // Program will wait 1 second here
        if (pthread_cond_timedwait(&Log_deadlock_time, &buffer.mutex, &timeout) == 0) {

            if (last_deadlock_time != time_of_deadlock) {
                last_deadlock_time = time_of_deadlock;
                char time_buffer[26];
                ctime_r(&time_of_deadlock, time_buffer);
                time_buffer[strlen(time_buffer) - 1] = '\0';
                Logfp = fopen("CPUlog.txt", "a");
                fprintf(Logfp, "Deadlock occurred at %s\n", time_buffer);
                fclose(Logfp);
            }
        }

        // Get the current CPU usage
        long double CUT = usage ;

        // Release the mutex lock
        pthread_mutex_unlock(&buffer.mutex);

        // Log the CPU usage
        time_t current_time;
        time(&current_time);
        char time_buffer[26];
        ctime_r(&current_time, time_buffer);
        time_buffer[strlen(time_buffer) - 1] = '\0'; // Remove newline character

        int size = snprintf(NULL, 0, "%s.CPU usage: %Lf%%.\n", time_buffer, CUT);
        char *string = malloc(size + 1);
        if (string == NULL) {
            perror("Malloc error in logger thread");
            exit(EXIT_FAILURE);
        }
        sprintf(string, "%s.CPU usage: %.4Lf%%.\n", time_buffer, CUT);
        Logfp = fopen("CPUlog.txt", "a");

        if (Logfp == NULL) {
            perror("Log error");
        }

        fprintf(Logfp, "%s", string);
        fclose(Logfp);
        free(string);
    }

    pthread_exit(NULL);
}

void sigterm_handler(int sig){
    if (sig == SIGINT){
    printf("SIGINT signal detected...\n");
    }
    else if (sig == SIGTERM){
        printf("SIGTERM signal detected \n");
    }
    else if(sig ==SIGHUP){
        printf("SIGHUP signal detected \n");
    }
    Terminate=1;
}

int array_comp(const long double* a,const long double* b,int size){

    for(int i=0;i<size;i++){
        if(a[i] !=b[i]){
            return 1;

        }
    }
    return 0;
}

void unit_test(void){
    /*
     * Main purpose of this test is:
     * Testing array_comp function. If it returns correct value or not. */
    long double a[4]=   {1,2,3,4};

    long double b[4]=   {1, 2, 3, 4};

    long double c[4]=   {1,2,3,5};

    assert(array_comp(a,b,4) ==0); // checking the same array with different address
    assert(array_comp(a,a,4)==0);// checking the same array
    assert(array_comp(a,c,4)==1); // checking different array

    // function passed the test.
}

void progress_bar(float data){
    const int BAR_LENGTH = 100;
    const int MAX_CPU_USAGE = 100;

    int fill = (int) (data * BAR_LENGTH / (float) MAX_CPU_USAGE);
    int remain=BAR_LENGTH-fill;
    printf("[");
    if(data<=30){
        for(int i=0;i<fill;i++){
            printf("\x1b[1;32m*\x1b[0m");
        }
    }
    else if(data>30 && data <60){
        for(int i=0;i<fill;i++){
            printf("\x1b[1;33m*\x1b[0m");
        }
    }
    else{
        for(int i=0;i<fill;i++){
            printf("\x1b[1;31m*\x1b[0m");
        }
    }
    for(int j=0;j<remain;j++){
        printf(" ");
    }

    printf("] %3.2f%% \r",data);
    fflush(stdout);

}

