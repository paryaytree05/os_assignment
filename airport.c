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

#include <stdbool.h>

#define MAXLEN 512

pthread_mutex_t runways[11] = {PTHREAD_MUTEX_DEFAULT};
key_t key;
int msgid;

struct planeInfoDeparted
{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
};
struct planeInfoDepart
{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
    
};
struct thread{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
    int airPortNumber;
    int numberOfRunways;
    int arr[4];
    

};

void *depart(void *bal)
{
    struct thread *myTTD = (struct thread*)bal;
    int n = sizeof(myTTD->arr) / 4;
    int small = myTTD->arr[0];
    int index = 0;

    struct planeInfoDeparted deep;
    for (int i = 0; i < myTTD->numberOfRunways; i++)
    {

        if (myTTD->arr[i] > myTTD->totalPlaneWeight)
        {
            if(small<myTTD->totalPlaneWeight){
                small=myTTD->arr[i];
                index = i;
            }
            else if(small>myTTD->arr[i]){
                small=myTTD->arr[i];
                index = i;
            }
            
            
        }
    }

    deep.airPortArrival=myTTD->airPortArrival;
    deep.airportDearture=myTTD->airportDearture;
    deep.numberOfPassangers=myTTD->numberOfPassangers;
    deep.planeId=myTTD->planeId;
    deep.totalPlaneWeight=myTTD->totalPlaneWeight;
    deep.typeOfPlane=myTTD->typeOfPlane;
    

    if (small >= myTTD->totalPlaneWeight)
    {
        
            deep.mtype=myTTD->airportDearture+20;
            

            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has completed boarding/loading and taken off from Runway No. %d of Airport No. %d\n", deep.planeId, index + 1, myTTD->airPortNumber);
           

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                 printf("error in sending message \n");
                exit(1);
            }
            // printf("plane arrival %d\n",deep.airPortArrival);
            pthread_mutex_unlock(&runways[index]);
           // printf("afterlock\n");
            pthread_exit(0);
            //printf("after end\n");
        }
        else
        {
           //printf("plane arrival2 %d\n",deep.airPortArrival); 
            deep.mtype=myTTD->airportDearture+20;
            index=myTTD->numberOfRunways+1;
            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has completed boarding/loading. %d of Airport No. %d \n", deep.planeId, index + 1, myTTD->airPortNumber);

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                 printf("error in sending message \n");
                exit(1);
            }

            pthread_mutex_unlock(&runways[index]);

            pthread_exit(0);
        }
    }
    

void* Arrive(void*param)
{
    //printf("threadarrive\n");
    struct thread *myTTD;
    myTTD = (struct thread*)param;
    struct thread *ths;
    //int *val_p = (int *)param;
    int n = sizeof(myTTD->arr) / 4;
    int small = myTTD->arr[0];
    int index = 0;

    struct planeInfoDeparted deep;
   for (int i = 0; i < myTTD->numberOfRunways; i++)
    {

        if (myTTD->arr[i] > myTTD->totalPlaneWeight)
        {
            if(small<myTTD->totalPlaneWeight){
                small=myTTD->arr[i];
                index = i;
            }
            else if(small>myTTD->arr[i]){
                small=myTTD->arr[i];
                index = i;
            }
            
            
        }
    }

    deep.airPortArrival=myTTD->airPortArrival;
    deep.airportDearture=myTTD->airportDearture;
    deep.numberOfPassangers=myTTD->numberOfPassangers;
    deep.planeId=myTTD->planeId;
    deep.totalPlaneWeight=myTTD->totalPlaneWeight;
    deep.typeOfPlane=myTTD->typeOfPlane;

    if (small >= myTTD->totalPlaneWeight)
    {
        
            deep.mtype=myTTD->airPortArrival+40;

            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has completed deboarding/deloading and landed off from Runway No. %d of Airport No. %d\n", deep.planeId, index + 1, myTTD->airPortNumber);

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }
            pthread_mutex_unlock(&runways[index]);
            pthread_exit(0);
        }
        else
        {
            deep.mtype=myTTD->airportDearture+40;
            index=myTTD->numberOfRunways+1;
            pthread_mutex_lock(&runways[index]);
            sleep(3);
            printf("Plane %d has landed on Runway. %d of Airport No. %d and has completed deboarding/unloading.\n", deep.planeId, index + 1, myTTD->airPortNumber);

            if (msgsnd(msgid, (void *)&deep, sizeof(struct planeInfoDeparted), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }

            pthread_mutex_unlock(&runways[index]);

            pthread_exit(0);
        }
    }


int main()
{
    int airPortNumber;//1-10
    int numberOfRunways;

    printf("Enter Airport Number : \n");
    scanf("%d", &airPortNumber);

    printf("Enter number of Runways : \n");
    scanf("%d", &numberOfRunways);//even number


    int size=0;

    int a[numberOfRunways];
    printf("Enter loadCapacity of Runways (give as a space separated list as a single line): \n");
    for (int i = 0; i < numberOfRunways; i++)
    {
        scanf("%d", &a[i]);//1000-12000
    }

    key_t key;
    int msgid;
    struct planeInfoDepart planeRecDepart;

    key = ftok("plane.c", 'A');
    if (key == -1)
    {
        printf("error in creating unique key\n");
        exit(1);
    }

    msgid = msgget(key, 0644 | IPC_CREAT);
    if (msgid == -1)
    {
        printf("error in creating message queue\n");
        exit(1);
    }

    pthread_t pla[10];
    pthread_t t;
    
    pthread_attr_t attr;
    struct thread *bal=malloc(sizeof(struct thread));
    pthread_attr_init(&attr);

    int x = 0;
    int l = 0;

    for (int s = 0; s < numberOfRunways; s++)
        {
            bal->arr[s] = a[s];
        }

    while (1)
    {
        l++;

        if (msgrcv(msgid, (void *)&planeRecDepart, sizeof(struct planeInfoDepart), airPortNumber+10, 0) == -1)
        {
            printf("error in receiving message\n");
            exit(1);
        }
        if(planeRecDepart.typeOfPlane==3){break;}
        bal->airPortArrival=planeRecDepart.airPortArrival;
        bal->airportDearture=planeRecDepart.airportDearture;
        bal->mtype=planeRecDepart.mtype;
        bal->numberOfPassangers=planeRecDepart.numberOfPassangers;
        bal->totalPlaneWeight=planeRecDepart.totalPlaneWeight;
        bal->planeId=planeRecDepart.planeId;
        bal->typeOfPlane=planeRecDepart.typeOfPlane;
        bal->airPortNumber=airPortNumber;
        bal->numberOfRunways=numberOfRunways;
        //printf("gautam : %d\n",planeRecDepart.airportDearture);
        if(planeRecDepart.airportDearture==airPortNumber){
            pthread_create(&pla[x], NULL, depart, (void *)bal);
        x++;

        }
        else{
             pthread_create(&pla[x], NULL, Arrive, (void *)bal);
              x++;
        }
        
    }

    for (int m = 0; m < x; m++)
    {
        pthread_join(pla[m], NULL);
    }
}