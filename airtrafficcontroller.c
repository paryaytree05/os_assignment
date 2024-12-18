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
key_t key;
    int msgid;
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

struct planeInfoDepart
{
    long mtype;
    int airPortArrival;
    int airportDearture;
    int planeId;
    int totalPlaneWeight;
    int typeOfPlane;
    int numberOfPassangers;
    // int arriveOrDepart;
};

void depart(struct planeInfo planeInformation)
{

    struct planeInfoDepart planeDepart;
    planeDepart.airPortArrival = planeInformation.airPortArrival;
    planeDepart.airportDearture = planeInformation.airportDearture;
    planeDepart.planeId = planeInformation.planeId;

    planeDepart.mtype = (long)(planeInformation.airportDearture + 10);

    planeDepart.totalPlaneWeight = planeInformation.totalPlaneWeight;
    planeDepart.typeOfPlane = planeInformation.typeOfPlane;
    planeDepart.numberOfPassangers = planeInformation.numberOfPassangers;
    // planeDepart.arriveOrDepart=0;

    if (msgsnd(msgid, (void *)&planeDepart, sizeof(struct planeInfoDepart), 0) == -1)
    {
        // printf("error in sending message %s",buf);
        exit(1);
    }
}

key_t key;
int msgid;
char buf[MAXLEN];

int main(){
    int numberOfAirports;
    struct planeInfo planeRec;
    
    printf("Enter the number of airports to be handled/managed : \n");
    //printf("king\n");
    

    scanf("%d", &numberOfAirports);//2-10

    // struct msg_buffer message;
    //printf("king\n");
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
     struct planeInfoDepart planeDepart;

     int rz=0;
     int dep=0;
     int arv=0;

     struct shut x;

    int l=0;
    while(1){
        l++;

        if(dep==arv&&rz==1){
            break;
        }
         if (msgrcv(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0, 0) == -1)
    {
        
         printf("error in receiving message\n");
         exit(1);
        
    }
    
    if(planeRec.mtype>10&&planeRec.mtype<20){
        if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                
                exit(1);
            }
            sleep(5);

    }
    else if(planeRec.mtype>30&&planeRec.mtype<40){
        if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }
            sleep(5);

    }
    else if(planeRec.mtype>50&&planeRec.mtype<60){
        if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }
            sleep(5);

    }
    
     if(planeRec.mtype<=10&&rz!=1){
                //printf("%d\n",planeRec.airPortArrival);
                planeRec.mtype=planeRec.airportDearture+10;
                
                //depart(planeRec);
                //printf("%ld\n",planeRec.mtype);
                if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }

                dep++;
             }
     else if(planeRec.mtype<=30&&planeRec.mtype>=21){
        //printf("arrival mtype %d\n",planeRec.airPortArrival);
        planeRec.mtype=planeRec.airPortArrival+10;
        //printf("arrival mtype %d\n",planeRec.airPortArrival);
        if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }


     }
     else if(planeRec.mtype<=50&&planeRec.mtype>=40){
        planeRec.mtype=planeRec.planeId+50;
        if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }

            char *filename = "AirTrafficController.txt";

            if(arv==0){

                // open the file for writing
                FILE *fp = fopen(filename, "w");
                if (fp == NULL)
                {
                    printf("Error opening the file %s", filename);
                    return -1;
                }
                // write to the text file
                
                    fprintf(fp, "Plane %d has departed from Airport %d and will land at Airport %d\n", planeRec.planeId,planeRec.airportDearture,planeRec.airPortArrival);

                // close the file
                fclose(fp);

    

            }
            else{
                // open the file for writing
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }
    // write to the text file
    
        fprintf(fp, "Plane %d has departed from Airport %d and will land at Airport %d\n", planeRec.planeId,planeRec.airportDearture,planeRec.airPortArrival);

    // close the file
    fclose(fp);

            }
        arv++;
     }
     else if(planeRec.mtype==61){
        rz=1;

        if(dep==arv){
            for(int s=1;s<=numberOfAirports;s++){
            planeRec.typeOfPlane=3;
            planeRec.mtype=s+10;
            if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }
            
        }
        }
        else{
            sleep(3);
            if (msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1)
            {
                // printf("error in sending message %s",buf);
                exit(1);
            }

        }

    }

    msgctl(msgid,IPC_RMID,NULL);
   

    }
    
