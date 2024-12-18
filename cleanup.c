#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <sys/ipc.h>

#include <sys/shm.h>
#include <pthread.h>
#include <sys/msg.h>

#define MAXLEN 512

struct shut{
    long mtype;
    char h;

};
struct planeInfo
{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
};


int main()
{
    printf("Do you want the Air Traffic Control System to terminate?(Y for Yes and N for No)");
    char m = 'n';
    while (m != 'y' && m != 'Y')
    {
        scanf("%c", &m);
    }

    key_t key;
    int msgid;
    struct planeInfo x;
    x.mtype=61;
    x.typeOfPlane=3;
    

    key = ftok("plane.c", 'A');
    if (key == -1){
        printf("error in creating unique key\n");
        exit(1);
    }

    msgid = msgget(key, 0644|IPC_CREAT);    
    if (msgid == -1){
        printf("error in creating message queue\n");
        exit(1);
    }
   // printf("expect\n");

    if(msgsnd(msgid, (void *)&x, sizeof(struct shut), 0) == -1){
    	   //printf("error in sending message %s",buf);
        exit(1);
    }


}