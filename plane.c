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

#define READ_END 0
#define WRITE_END 1
#define MAXLEN 512



int main()
{
    
    int planeId;
    int typeOfPlane;
    int numberOfOccupiedSeats;
    int passangerWeight = 0;
    int planeWeight;

    printf("Enter Plane ID : \n");
    scanf("%d", &planeId);

    printf("Enter Type of Plane : \n");
    scanf("%d", &typeOfPlane);

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

    if (typeOfPlane == 1)
    {

        printf("Enter Number of Occupied Seats: \n");
        scanf("%d", &numberOfOccupiedSeats);

        // int write_msg[BUFFER_SIZE];
        // int read_msg[BUFFER_SIZE];

        struct info
        {
            int luggageWeight;
            int bodyWeight;
            // int sum ;
        };

        

        int fd[2];
        pid_t pid;

        

        for (int i = 0; i < numberOfOccupiedSeats; i++)
        {
            if (pipe(fd) == -1)
            {
                fprintf(stderr, "pipe failed");
                return 1;
            }

            pid = fork();

            if (pid < 0)
            {
                fprintf(stderr, "Fork Failed");
                return 1;
            }
            if (pid > 0)
            {

                sleep(2);

                close(fd[WRITE_END]);

                struct info data;

                read(fd[READ_END], &data, sizeof(struct info));
                // printf("luggae weight is %d\n",data.luggageWeight);

                passangerWeight = passangerWeight + data.luggageWeight + data.bodyWeight;

                close(fd[READ_END]);
            }
            else
            {
                close(fd[READ_END]);

                int luggageWeight;
                int bodyWeight;

                struct info data;

                printf("Enter Weight of Your Luggage : \n");
                scanf("%d", &luggageWeight);//0-25

                printf("Enter Your Body Weight : \n");
                scanf("%d", &bodyWeight);//10-100

                data.luggageWeight = luggageWeight;
                data.bodyWeight = bodyWeight;
                // data.sum=data.sum+luggageWeight+bodyWeight;

                write(fd[WRITE_END], &data, sizeof(struct info));

                close(fd[WRITE_END]);
                exit(0);
            }
        }
        planeWeight = passangerWeight + 75 * 7;//75 is avg weight of crew members*7
        // printf("plane weight is %d\n",planeWeight);
    }
    else
    {
        int cargoItems;
        int totalItemWeight=0;
        //int planeWeight;

        printf("Enter Number of Cargo Items : \n");
        scanf("%d", &cargoItems);//1-100

        for (int i = 0; i < cargoItems; i++)
        {
            int itemWeight;
            printf("Enter Average Weight of Cargo Items : \n");
            scanf("%d", &itemWeight);
            totalItemWeight=totalItemWeight+itemWeight;
            
        }
       // printf("Total Item Weight is : %d",totalItemWeight);
        planeWeight=totalItemWeight+75*2;
    }

    //printf("plane weight %d \n",planeWeight);

    int airportNumberArrival;
    int airPortNumberDeparture;

    printf("Enter Airport Number for Departure : \n");
    scanf("%d",&airPortNumberDeparture);//1-10

    printf("Enter Airport Number for Arrival : \n");
    scanf("%d",&airportNumberArrival);//1-10
    

    
    //printf("guess\n");
    
    


    struct planeInfo planeRec;
    
    

    planeRec.airPortArrival=airportNumberArrival;
    planeRec.airportDearture=airPortNumberDeparture;
    planeRec.planeId=planeId;

    planeRec.mtype=(long)planeId;

    planeRec.totalPlaneWeight=planeWeight;
    planeRec.typeOfPlane=typeOfPlane;
    planeRec.numberOfPassangers=numberOfOccupiedSeats;
    //printf("guess\n");

    //printf("plane id is : %ld\n",planeRec.mtype);

    key_t key;
    int msgid;

    key = ftok("plane.c", 'A');
    if (key == -1){
        printf("error in creating unique key\n");
        exit(1);
    }

    msgid = msgget(key, 0644);    
    if (msgid == -1){
        printf("error in creating message queue\n");
        exit(1);
    }

    if(msgsnd(msgid, (void *)&planeRec, sizeof(struct planeInfo), 0) == -1){
    	   //printf("error in sending message %s",buf);
        exit(1);
    }
   // sleep(5)
   //printf("guess\n");
   
   //sleep(5);

     if (msgrcv(msgid, (void *)&planeRec, sizeof(struct planeInfo), planeId+50, 0) == -1)
    {
        
        printf("error in receiving message\n");
        exit(1);
    }

    printf("Done with the full tour\n");
    //msgctl(msgid,IPC_RMID,NULL);
    
}
