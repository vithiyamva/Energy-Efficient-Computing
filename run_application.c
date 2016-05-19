#include<stdio.h>
#include<stdlib.h>
#include <time.h>
int main(int argc, char *argv[]) {
        FILE *fp;
        clock_t begin, end;
        double time_spent;
        int sleep_time;
        char * line = NULL;
        char command[1000];
        size_t len = 0;
        ssize_t read;
        int val;
        printf("argv[2]:%s\n",argv[2]);
        fp = fopen("dual100.txt", "r");
        if (fp == NULL)
                printf("return NULL\n");
        while ((read = getline(&line, &len, fp)) != -1) {
                time_spent = 0;
                sleep_time = 0;
                printf("Retrieved line of length %zu :\n", read);
                val = atoi(line);
                printf("running for val:%d\n", val);
                float x = (float)10/val;
                sprintf(command, "/home/ec2-user/httperf-0.9.0-varrarive/src/httperf --server=54.186.116.220 --uri=/%s  --num-conns=%d --period=%d:d%f", argv[1], val, val, x);
                //time_t seconds;
                //seconds = time(NULL);
                system(command);
        }
}
