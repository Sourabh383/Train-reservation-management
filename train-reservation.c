
// Usefull Libraries
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 1000     //size of the records
#define NAME_LEN 100  // name length 
#define CLASS_LEN 100 // class length of the Train
#define SEAT_LEN 6
typedef enum{
    FAILURE,SUCCESS
}status_code;

//Structure to represent Date
typedef struct D{
    int day;
    int month;
    int year;
}Date;
//Structure of Passenger
typedef struct PassengerNode
{
    char Passenger_Name[NAME_LEN];
    int Passenger_id;
    char Boarding_train[NAME_LEN];
    char Boarding_station[NAME_LEN];
    char class[CLASS_LEN];
    char Destination_station[NAME_LEN];
    int Train_id;
    int Seat_Number;
    Date d;
    int upgrade;
    struct PassengerNode *next;
}Node;

//This is to keep track of SeatMatrix of a particular Train
typedef struct TrainNode
{
    char Boarding_train[NAME_LEN];
    int Train_id;
    int Num_Passengers;
    char Train_Name[NAME_LEN];
    Date d;
    //Assumption->2 Boggies having 10 seats each
    //Seat Number is given as 1AC-1,1AC-2,1AC-3.....Sleeper-3,2AC-5..etc
    int Sleeper[2][10];
    int AC_1[2][10];
    int AC_2[2][10];
    int AC_3[2][10];
    //keep track of number of vacent seats which will help for promoting to next class
    int Sleeper_vacent;
    int AC_1_vacent;
    int AC_2_vacent;
    int AC_3_vacent;
    
    struct TrainNode *next;
}TrainNode;
/* --------------------------------------------------------------------------------------------------------------------------------- */
// DECLARATION OF THE FUNCTIONS USED IN ASSIGNMENT
Node *MakeNode(TrainNode* Trainhead,char Passenger_Name[],char Boarding_train[],char Boarding_station[],char class[],char Destination_station[],int Passenger_id,int Train_id,Date d1,int to_upgrade,int flag);
int insert(TrainNode* Trainhead,Node **records_head_address,char Passenger_Name[],char Boarding_train[],char Boarding_station[],char class[],char Destination_station[],int Passenger_id,int Train_id,int n,Date d1,int to_upgrade);
Node *sortOnTrain_id(Node *records_head);
status_code delete(TrainNode *TrainNode,Node **records_head_address,int Passenger_id);
Node *getListDestination(TrainNode* Trainhead,Node *records_head);
Node *SortByTravelDate(TrainNode* Trainhead,Node *records_head,int Passenger_id);
Node *makeList_ofparticularPassengerid(TrainNode* Trainhead,Node *records_head,int Passenger_id);
int compare_Date(Date t1,Date t2);
void displayDestination_asperdates(Node *records_head);
void SortTrains(Node *records_head);
void Sorted_Insert(TrainNode **head_ref,TrainNode *new_node);
void PromotePassengers(TrainNode *Trainhead,Node *records_head,int Train_id,Date d1);
int SeatAllocationafterpromoting(TrainNode *Trainhead,TrainNode *target,Node *records_head,int Train_id,char class[]);
TrainNode *Initialize_SeatMatrix();
TrainNode *MakeTrainNode(int id);
int allocate_Seats(TrainNode* Trainhead,int Train_id,char class[],int Passenger_id);
void dellocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id);
void displayTrainList(TrainNode *Trainhead);
TrainNode *findTrainNode(TrainNode *Trainhead,int Train_id);
int linear_search(Node *records_head,int Passenger_id);
int getCount(Node *records_head);
void displayRecord(Node *records_head);
Node *Divide(Node *lptr);
Node *Merge(Node *lptr,Node *nptr,int parameter);
Node *MergeSort(Node *lptr,int parameter);

/* -------------------------------------------------------------------------------------------------------------------------------------- */
int main()
{
    Node *records_head;
    records_head=NULL;

    TrainNode *Trainhead=Initialize_SeatMatrix();//Initializing seat Matrix for all 10 trains
    displayTrainList(Trainhead);//Displaying the Train Name and its id for user to choose a particular train

    char Passenger_Name[NAME_LEN],Boarding_train[NAME_LEN],Boarding_station[NAME_LEN],class[CLASS_LEN],Destination_station[NAME_LEN];
    int Passenger_id,Train_id,n;
    int i,input,flag,found,count,to_upgrade;
    flag=0;
    found=FAILURE;
    Node *temp,*list;
    printf("Perform operation\n");
    do
    {
            printf("--------------------------------------------------------------------------------\n");
            printf("\t\t\tTRAIN MANAGEMENT SYSTEM\n");
            printf("--------------------------------------------------------------------------------\n");
            printf(" 1  - To INSERT a new record\n");
            printf(" 2  - To Cancell Reservation\n");
            printf(" 3  - To get List of Passengers having Same Destination station and Same Train id\n");
            printf(" 4  - To sort by TravelDate for a particular Passenger\n");
            printf(" 5  - To Display Trainno and Date in sorted order of No of Passengers\n"); 
            printf(" 6  - To Promote the Passengers to upper class\n"); 
            printf(" 7  - To DISPLAY existing record(s)\n");
            printf(" 8  - To get count of Number of record(s)\n");
            printf(" .. - Any other key to EXIT\n");
            printf("--------------------------------------------------------------------------------\n");
            printf("Enter the CHOICE you want to Execute\n");



        scanf("%d", &input);
        switch(input)
        {
            case 1:
                found=FAILURE;
                int n=1;//TBD
                Date d1;
                printf("Enter the fields\nName of Passenger?\n->");
                scanf("%s",Passenger_Name);
                printf("Enter Passenger ID\n");
                do
                {
                    printf("(Remember that id should contain 3 digits only)\n->");
                    scanf("%d",&Passenger_id);

                } while (Passenger_id < 100 || Passenger_id > 999); // Specific id because system handle this way only. Again a try to make this real.
                printf("Enter the Boarding Train Name\n->");
                scanf("%s",Boarding_train);
                printf("Enter the Boarding Station\n->");
                scanf("%s",Boarding_station);
                printf("Enter the Travelling Class:\n->");
                do
                {
                    printf("(This can be \"Sleeper\" or \"3AC\" or \"2AC\" or \"1AC\") ");
                    scanf("%s", class);
                }  while (strcmp(class, "Sleeper") != 0 && strcmp(class, "3AC") != 0 && strcmp(class, "2AC") != 0 && strcmp(class, "1AC") != 0); //Taking correct class.
                printf("Enter the Destination Station\n->");
                scanf("%s",Destination_station);
                printf("Enter the Train id:");
                do
                {
                    printf("(Remember that id should be b/w 25671 to 25680)\n->");
                    scanf("%d", &Train_id);
                } while (Train_id < 25671 || Train_id > 25680); // Specific id because system handle this way only. Again a try to make this real.
                printf("Enter date\nFirst Enter Day(D)\n");
                scanf("%d",&d1.day);
                printf("Enter Month(MM)\n");
                scanf("%d",&d1.month);
                printf("Enter Year(YYYY)\n");
                scanf("%d",&d1.year);
                printf("DO YOU WANT TO UPGRADE YOUR CLASS IN FUTURE\nEnter 1 to upgrade else enter 0\n->");
                scanf("%d",&to_upgrade);                
                found=insert(Trainhead,&records_head,Passenger_Name,Boarding_train,Boarding_station,class,Destination_station,Passenger_id,Train_id,n,d1,to_upgrade);
                if (found)//if found means inserted successfully
                printf("RESERVATION DONE SUCCESSFULL\n");
                else
                printf("RESERVATION FAILED, check the record size\n");            
                break;

            case 2:
                found=FAILURE;
                printf("Enter the Passenger id for the record which is to be deleted\n");
                printf("Enter Passenger ID\n");
                do
                {
                    printf("(Remember that id should contain 3 digits only)\n->");
                    scanf("%d",&Passenger_id);

                } while (Passenger_id < 100 || Passenger_id > 999); // Specific id because system handle this way only. Again a try to make this real.
                found=delete(Trainhead,&records_head,Passenger_id);
                //if found means we have deleted it successfully
                if(found)
                    printf("RESERVATION CANCELLED SUCCESSFULLY!!!\n");
                else
                    printf("RESERVATION CANCELLATION FAILED!!!\n");
                break;
            
            case 3:
                if(records_head==NULL)
                    printf("The record Database is empty\n");
                else
                {
                    list=getListDestination(Trainhead,records_head);
                    if(list==NULL)
                        printf("There are no Passengers having same Destination and Same Train id!!\n");
                    else
                    {
                        printf("Displaying the list of Passengers having same Destination and Train\n");
                        displayRecord(list);
                    }

                }
                break;
            case 4:
                printf("Enter Passenger id\n");
                scanf("%d",&Passenger_id);
                if(!linear_search(records_head,Passenger_id))
                    printf("There is no Passenger with given id!!!\n");
                else
                {
                    Node *list=SortByTravelDate(Trainhead,records_head,Passenger_id);
                    printf("Displaying the List of Destination station as per Dates of travel(Sorted)\n");
                    displayDestination_asperdates(list);
                    // displayRecord(list);
                }
                break;

            case 5:
                SortTrains(records_head);
                break;
            case 6:
                if(records_head==NULL)
                printf("The record Database is empty\n");
                else
                {
                    printf("Enter Train id in which promotion is to be made\n");
                    do
                    {
                        printf("(Remember that id should be b/w 25671 to 25680)\n->");
                        scanf("%d", &Train_id);
                    } while (Train_id < 25671 || Train_id > 25680); // Specific id because system handle this way only. Again a try to make this real.
                    printf("Enter date\nFirst Enter Day(D)\n");
                    scanf("%d",&d1.day);
                    printf("Enter Month(MM)\n");
                    scanf("%d",&d1.month);
                    printf("Enter Year(YYYY)\n");
                    scanf("%d",&d1.year);
                    PromotePassengers(Trainhead,records_head,Train_id,d1);
                }
                break;
            case 7:
            printf("\t\tDISPLAYING RECORDS:\n");
                displayRecord(records_head);
                break;
            case 8:
                count=getCount(records_head);
                printf("No of Reservations : %d\n",count);
                break;
            default:
                 printf("You have successfully exited");
                flag = 1;    
        }
    }while(flag==0);

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//Q1.INSERT
Node *MakeNode(TrainNode *Trainhead,char Passenger_Name[],char Boarding_train[],char Boarding_station[],char class[],char Destination_station[],int Passenger_id,int Train_id,Date d1,int to_upgrade,int flag)
{
    //Making Node and setting values given by user
    Node *nptr;
    nptr = (Node *)malloc(sizeof(Node));
    strcpy(nptr->Passenger_Name,Passenger_Name);
    nptr->Passenger_id=Passenger_id;
    nptr->Train_id=Train_id;
    strcpy(nptr->Boarding_train,Boarding_train);
    strcpy(nptr->Boarding_station,Boarding_station);
    strcpy(nptr->class,class);
    strcpy(nptr->Destination_station,Destination_station);
    nptr->d.day=d1.day;
    nptr->d.month=d1.month;
    nptr->d.year=d1.year;
    //
    if(flag)
        nptr->Seat_Number=allocate_Seats(Trainhead,Train_id,class,nptr->Passenger_id);//allocating seat to user 
    nptr->upgrade=to_upgrade;
    nptr->next=NULL;
    return nptr;
}

int insert(TrainNode* Trainhead,Node **records_head_address,char Passenger_Name[],char Boarding_train[],char Boarding_station[],char class[],char Destination_station[],int Passenger_id,int Train_id,int n,Date d1,int to_upgrade)
{
    status_code sc=SUCCESS;
    Node *records_head = *records_head_address;
    Node *ptr = records_head;
    int allocate=1;
    if(!records_head)//first Node Case
    {
        Node *nptr;
        nptr=MakeNode(Trainhead,Passenger_Name,Boarding_train,Boarding_station,class,Destination_station,Passenger_id,Train_id,d1,to_upgrade,allocate);
        if(!nptr)
            sc=FAILURE;
        else
            *records_head_address=nptr;
    }
    else
    {
        Node *nptr;
        nptr=MakeNode(Trainhead,Passenger_Name,Boarding_train,Boarding_station,class,Destination_station,Passenger_id,Train_id,d1,to_upgrade,allocate);
        if(!nptr)
            sc=FAILURE;
        else
        {
            while(ptr->next!=NULL)
                ptr=ptr->next;
            ptr->next=nptr;
        }
        *records_head_address=sortOnTrain_id(records_head);// maintaing sorted order on basis of trainid
    }
    return sc;
}

Node *sortOnTrain_id(Node *records_head)//to sort on basis of Trainid
{
    Node *nptr;
    nptr = MergeSort(records_head,0);//passing parameter as  0 
    return nptr;
}
//-------------------------------------------------------------------------------------------------------------------------
//Q2. DELETE
status_code delete(TrainNode *Trainhead,Node **records_head_address,int Passenger_id)
{
    status_code sc=SUCCESS;
    if(!(*records_head_address))//Empty List case
        sc=FAILURE;
    else
    {
        Node *prev,*ptr;
        prev=NULL;
        ptr=*records_head_address;
        int found=0;

        while(ptr && !found)//finding the passenger
        {
            if(ptr->Passenger_id==Passenger_id)
                found=1;
            else
            {
                prev=ptr;
                ptr=ptr->next;
            }
        }
        if(found)//if found then we need to cancel the reservation
        {
            dellocate_Seats(Trainhead,ptr->Train_id,ptr->class ,ptr->Passenger_id);//if User cancell reservation then his seat must be deallocated
            if(!prev)//first node to be deleted
            {
                *records_head_address=ptr->next;
                free(ptr);
            }
            else
            {
                prev->next=ptr->next;
                free(ptr);
            }
        }
        else//user must have  provided incorrect record which cannot be deleted
            sc=FAILURE;
    }
    return sc;
}

//-------------------------------------------------------------------------------------------------------------------------
//Q3 .getListDestination
Node *getListDestination(TrainNode* Trainhead,Node *records_head)
{
    Node *ptr1,*ptr2=NULL,*temp1,*temp2,*nptr;
    Node *list=NULL;
    int allocate=0,firstmatchfound=0;//Here we are making temp Node so we dont want to allocate seat so allocate flag is set to 0
    if(records_head->next!=NULL)
        ptr2=records_head->next;
    ptr1=records_head;
    while(ptr2)//Using the concept of 2 pointers as we have kept record in sorted order of Train id ie consecutive Nodes will have same train id
    {
        if((strcmp(ptr1->Destination_station,ptr2->Destination_station)==0) && (ptr1->Train_id==ptr2->Train_id))
        {
            if(!firstmatchfound)
            {
                temp1=MakeNode(Trainhead,ptr1->Passenger_Name,ptr1->Boarding_train,ptr1->Boarding_station,ptr1->class,ptr1->Destination_station,ptr1->Passenger_id,ptr1->Train_id,ptr1->d,ptr1->upgrade,allocate);
                temp2=MakeNode(Trainhead,ptr2->Passenger_Name,ptr2->Boarding_train,ptr2->Boarding_station,ptr2->class,ptr2->Destination_station,ptr2->Passenger_id,ptr2->Train_id,ptr2->d,ptr2->upgrade,allocate);
                temp1->next=temp2;
            }
            else if(firstmatchfound)
            {
                temp1=MakeNode(Trainhead,ptr2->Passenger_Name,ptr2->Boarding_train,ptr2->Boarding_station,ptr2->class,ptr2->Destination_station,ptr2->Passenger_id,ptr2->Train_id,ptr2->d,ptr2->upgrade,allocate);
            }
            firstmatchfound=1;
            if(list==NULL)
            {
                list=temp1;
            }
            else
            {
                nptr=list;
                while(nptr->next!=NULL)
                    nptr=nptr->next;
                nptr->next=temp1;
            }
        
        }
        ptr1=ptr1->next;
        ptr2=ptr2->next;
    }
    return list;
}

//-------------------------------------------------------------------------------------------------------------------------
//Q4. SortByTravelDate
Node *SortByTravelDate(TrainNode* Trainhead,Node *records_head,int Passenger_id)
{
    Node *list=makeList_ofparticularPassengerid(Trainhead,records_head,Passenger_id);//Making list of the given passenger id to sort it by travel date
    list=MergeSort(list,1);//Passing parameter as 1 to sort according to date
    return list;
}

Node *makeList_ofparticularPassengerid(TrainNode* Trainhead,Node *records_head,int Passenger_id)
{
    Node *list_passengerid=NULL,*temp,*nptr;
    while(records_head)//traversing through the record and making list of Passengerid given by user
    {
        if(records_head->Passenger_id==Passenger_id)
        {
            temp=MakeNode(Trainhead,records_head->Passenger_Name,records_head->Boarding_train,records_head->Boarding_station,records_head->class,records_head->Destination_station,records_head->Passenger_id,records_head->Train_id,records_head->d,records_head->upgrade,0);//passing flag as 0 as this is temp Node and we dont want to agani allocate seat to it
            if(list_passengerid==NULL)
            {
                list_passengerid=temp;
            }
            else
            {
                nptr=list_passengerid;
                while(nptr->next!=NULL)
                    nptr=nptr->next;
                nptr->next=temp;
            }
        }
        records_head=records_head->next;
    }
    return list_passengerid;
}
//Function to compare the Dates
int compare_Date(Date t1,Date t2)
{
    int retval=0;//means date is same
    if (t1.year > t2.year) retval=1;
    else if (t1.year < t2.year) retval=-1;
    // year matches. so check month
    else if (t1.month > t2.month) retval=1;
    else if (t1.month < t2.month) retval=-1;
    // month also matches. so check day
    else if (t1.day > t2.day) retval=1;
    else if (t1.day < t2.day) retval=-1;
    // if date1==date2 then retval is 0
    return retval;
}

void displayDestination_asperdates(Node *records_head)
{
    if(records_head)
    {
        printf("_____________________________________________________________________________________\n");
        printf("Name\tDestination_Station\tDate \n\n");
        printf("_____________________________________________________________________________________\n");
        // printf("-----------------------------------------------------------------\n");
        
        while(records_head!=NULL)
        {
            printf("%s\t%s\t",records_head->Passenger_Name,records_head->Destination_station);
            printf("%d/%d/%d",records_head->d.day,records_head->d.month,records_head->d.year);
            printf("\n");
            records_head = records_head->next;
        }
        printf("_____________________________________________________________________________________\n");
        printf("\n\n");
    }
    else
        printf("\nNo records to DISPLAY\n\n");

}

//-------------------------------------------------------------------------------------------------------------------------
//Q5 .SortTrains
void SortTrains(Node *records_head)
{
    if(records_head==NULL)
        printf("The record Database is empty\n");
    else
    {
        int Passenger_count=1,index;
        Node *ptr1,*ptr2=NULL,*nptr;
        TrainNode *tnptr;
        TrainNode *Train_List=NULL;
        ptr1=records_head;
        if(records_head->next!=NULL)
            ptr2=records_head->next;
        else if(ptr2==NULL)//If there is only one record present
        {
            index=(ptr1->Train_id)%10;
            tnptr=MakeTrainNode(index);
            tnptr->Num_Passengers=(Passenger_count);
            tnptr->d.day=ptr1->d.day;
            tnptr->d.month=ptr1->d.month;
            tnptr->d.year=ptr1->d.year;
            Sorted_Insert(&Train_List,tnptr);
            Passenger_count=1;//Keep track of number of passengers on train
        }
        while(ptr2)//Using the concept of 2 pointers and matching adjacent Train id as we have kept sorted on train id
        {
            index=(ptr1->Train_id)%10;
            if((ptr1->Train_id==ptr2->Train_id))//If adjacent Train id matches
            {
                Passenger_count++;
                index=(ptr1->Train_id)%10;
            }
            else//if there is no match
            {
                tnptr=MakeTrainNode(index);
                tnptr->Num_Passengers=(Passenger_count);
                tnptr->d.day=ptr1->d.day;
                tnptr->d.month=ptr1->d.month;
                tnptr->d.year=ptr1->d.year;
                Sorted_Insert(&Train_List,tnptr);
                Passenger_count=1;
            }
            if(ptr2->next==NULL)//corner case 
            {
                index=ptr2->Train_id%10;
                tnptr=MakeTrainNode(index);
                tnptr->Num_Passengers=(Passenger_count);
                tnptr->d.day=ptr2->d.day;
                tnptr->d.month=ptr2->d.month;
                tnptr->d.year=ptr2->d.year;
                Sorted_Insert(&Train_List,tnptr);
                Passenger_count=1;
            }
            ptr1=ptr1->next;
            ptr2=ptr2->next;
        }

        //Displaying the Train Number and Travel Date
        printf("Displaying the Train Number and Travel Date in sorted order of Number of Passengers\n");
        if(Train_List)
        {
            printf("_____________________________________________________________________________________\n");
            printf("Train_No\tDate\tNumPassengers \n\n");
            printf("_____________________________________________________________________________________\n");
            // printf("-----------------------------------------------------------------\n");

            while(Train_List!=NULL)
            {
               printf("%d\t",Train_List->Train_id);
                printf("%d/%d/%d\t%d",Train_List->d.day,Train_List->d.month,Train_List->d.year,Train_List->Num_Passengers);
                printf("\n");
                Train_List= Train_List->next;
            }
            printf("_____________________________________________________________________________________\n");
            printf("\n\n");
        }   
        else
            printf("\nNo records to DISPLAY\n\n");

    }
    
}

void Sorted_Insert(TrainNode **head_ref,TrainNode *new_node)//To create a sorted list based on NUmber of Passengers
{
    TrainNode* current;
    /* Special case for the head end */
    if (*head_ref == NULL|| (*head_ref)->Num_Passengers>= new_node->Num_Passengers) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else {
        /* Locate the node before the
     point of insertion */
        current = *head_ref;
        while (current->next != NULL && current->next->Num_Passengers < new_node->Num_Passengers) 
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

//-------------------------------------------------------------------------------------------------------------------------
//Q6 .PromotePassengers
void PromotePassengers(TrainNode *Trainhead,Node *records_head,int Train_id,Date d1)
{
    int cnt=0;
    TrainNode *target=findTrainNode(Trainhead,Train_id);//Finding the train to make changes in its SeatMatrix
    char AC2[]="2AC";
    char AC3[]="3AC";
    char Sleeper[]="Sleeper";
    /*
    ORDER OF CALLING FUNCTION->
    The optimal way is to 
    First promote 2AC to 1AC as then MORE 2AC seats will be vacent for 3AC
    then promote 3AC to 2AC as MORE 2AC seats will be vacent for Sleeper
    and at last Sleeper is promoted to 3AC
    so calling the function by passing 2AC first then 3AC then Sleeper
    */
    cnt+=SeatAllocationafterpromoting(Trainhead,target,records_head,Train_id,AC2);
    cnt+=SeatAllocationafterpromoting(Trainhead,target,records_head,Train_id,AC3);
    cnt+=SeatAllocationafterpromoting(Trainhead,target,records_head,Train_id,Sleeper);    
    printf("Promoted %d Passengers!!\n",cnt);//Printing No of Passengers promoted

}


//To allocate seat at proper place after promotion of class
int SeatAllocationafterpromoting(TrainNode *Trainhead,TrainNode *target,Node *records_head,int Train_id,char class[])
{
    int cnt=0,done=0;
    while(records_head && !done)
    {
        if(records_head->Train_id==Train_id && records_head->upgrade)
        {
            if(strcmp(class,"2AC")==0)//If users class is 2AC then he must want promotion to 1AC
            {
                if(target->AC_1_vacent>0 && strcmp(records_head->class,"2AC")==0)//checking number of vacent seats in 1AC and promoting
                {
                    dellocate_Seats(Trainhead,Train_id,records_head->class,records_head->Passenger_id);//deallocating seat from 2AC
                    strcpy(records_head->class,"1AC");//changing class to 1AC
                    records_head->Seat_Number=allocate_Seats(Trainhead,Train_id,records_head->class,records_head->Passenger_id);//Allocating seat in 1AC
                    cnt++;
                }
            }          
            
            else if(strcmp(class,"3AC")==0)//if Users class is 3AC then he ,must want promotion to 2AC
            {
                if(target->AC_2_vacent>0 && strcmp(records_head->class,"3AC")==0)//checking number of vacent seats in 2AC and promoting
                {
                    dellocate_Seats(Trainhead,Train_id,records_head->class,records_head->Passenger_id);//deallocating seat from 3AC
                    strcpy(records_head->class,"2AC");//changing class to 2AC
                    records_head->Seat_Number=allocate_Seats(Trainhead,Train_id,records_head->class,records_head->Passenger_id);//Allocating seat in 2AC
                    cnt++;
                }
            }
            else 
            {
                if(target->AC_3_vacent>0 && strcmp(records_head->class,"Sleeper")==0)//if Users class is Sleeper then he must want promotion to 3AC
                {
                    dellocate_Seats(Trainhead,Train_id,records_head->class,records_head->Passenger_id);//deallocating seat from Sleeper
                    strcpy(records_head->class,"3AC");//changing class to 3AC
                    records_head->Seat_Number=allocate_Seats(Trainhead,Train_id,records_head->class,records_head->Passenger_id);//Allocating seat in 3AC
                    cnt++;
                }
            }
        }
        records_head=records_head->next;
    }
    return cnt;//keeps track of number of promotion made
}

//-------------------------------------------------------------------------------------------------------------------------------------
//Helper functions for Seat Allocation,Deallocation,Initialization of Seat Matrix
TrainNode *Initialize_SeatMatrix()//Initialize seat Matrix
{
    TrainNode *head=NULL;
    TrainNode *tptr;
    for(int k=1;k<=10;k++)//Assuming 10 trains available for reservation
    {
        TrainNode *temp=MakeTrainNode(k);//Making TrainNode and linking it at Proper place below
        if(head==NULL)
        {
            head=temp;
        }
        else
        {
            tptr=head;
            while(tptr->next!=NULL)
                tptr=tptr->next;
            tptr->next=temp;
        }
    } 
    return head;
}

TrainNode *MakeTrainNode(int id)//Making Train Node which will help to keep track of SeatMatrix particular Train
{
    char Name[10][30]={{"Rajdhani"},{"Shatabdi"},{"Sampark"},{"Humsafar"},{"Tejas"},{"Suvidha"},{"Superfast"},{"Suburban"},{"Ajanta"},{"Amarnath"}};    
    TrainNode *tnptr;
    tnptr=(TrainNode *)malloc(sizeof(TrainNode));
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<10;j++)
        {//setting the seat NUmbers
            tnptr->Sleeper[i][j]=(10*i+j+1);
            tnptr->AC_1[i][j]=(10*i+j+1);
            tnptr->AC_2[i][j]=(10*i+j+1);
            tnptr->AC_3[i][j]=(10*i+j+1);
        }
    }
    strcpy(tnptr->Train_Name,Name[id-1]);//
    tnptr->Sleeper_vacent=20;//assumed 2 bogies and each having seats of size 10
    tnptr->AC_1_vacent=20;
    tnptr->AC_2_vacent=20;
    tnptr->AC_3_vacent=20;
    tnptr->Num_Passengers=0;
    tnptr->Train_id=25670+id;
    return tnptr;
}

int allocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id)//To allocate seat and returns seat Number which is allocated
{
    TrainNode *temp=findTrainNode(Trainhead,Train_id);//finding Train in which Seat is to be allocated
    temp->Num_Passengers++;//incrementing NUmber of passengers in train
    // printf("INside allocateseats::%d\n",temp->Num_Passengers);
    int done=0,retval=0;
    if(strcmp(class,"Sleeper")==0)//if user want to do reservation in Sleeper class
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(temp->Sleeper[i][j]>0)//finding place where we can allocate seat to user
                {
                    retval=temp->Sleeper[i][j];//setting retval to seatNUmber
                    temp->Sleeper[i][j]=-(Passenger_id);//after allocating seat make seatvalue as  -(Passengerid) which will help in deallocating seat for particular passengerid
                    temp->Sleeper_vacent--;//decrementing Number of vacent seats
                    done=1;
                }
            }
        }
    }
    //Similar things done in 3AC ,2AC,1AC
    else if(strcmp(class,"3AC")==0)
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(temp->AC_3[i][j]>0)
                {
                    retval=temp->AC_3[i][j];
                    temp->AC_3[i][j]=-(Passenger_id);
                    temp->AC_3_vacent--;
                    done=1;
                }
            }
        }
    }
    else if(strcmp(class,"2AC")==0)
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(temp->AC_2[i][j]>0)
                {
                    retval=temp->AC_2[i][j];
                    temp->AC_2[i][j]=-(Passenger_id);
                    temp->AC_2_vacent--;
                    done=1;
                }
            }
        }

    }
    else
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(temp->AC_1[i][j]>0)
                {
                    retval=temp->AC_1[i][j];
                    temp->AC_1[i][j]=-(Passenger_id);
                    temp->AC_1_vacent--;
                    done=1;
                }
            }
        }

    }
    // printf("retval---%d\n",retval);
    // printf("Sleeper vacant%d\n",temp->Sleeper_vacent);
    // printf("AC3 vacant%d\n",temp->AC_3_vacent);
    // printf("AC2 vacant%d\n",temp->AC_2_vacent);
    // printf("AC1 vacant%d\n",temp->AC_1_vacent);

    return retval;
}

void dellocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id)//deallocate seats when passenger cancells reservation or upgrades his class at that time his olderclass seat will be deallocated
{
    TrainNode *temp=findTrainNode(Trainhead,Train_id);//finding Train to make changes in its Seat matrix
    temp->Num_Passengers--;//decrementing number of passengers
    int done=0;
    if(strcmp(class,"Sleeper")==0)
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(abs(temp->Sleeper[i][j])==Passenger_id)//while allocating we have made the seat value  -(Passengerid) so checking this
                {
                    temp->Sleeper[i][j]=10*i+j+1;//changing seat value to its Number
                    temp->Sleeper_vacent++;//increamenting vacent sleeper seats
                    done=1;
                }
            }
        }
    }
    //Similar things doing in 3AC 2AC 1AC classes
    else if(strcmp(class,"3AC")==0)
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(abs(temp->AC_3[i][j])==Passenger_id)
                {
                    temp->AC_3[i][j]=10*i+j+1;
                    temp->AC_3_vacent++;
                    done=1;
                }
            }
        }
    }
    else if(strcmp(class,"2AC")==0)
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(abs(temp->AC_2[i][j])==Passenger_id)
                {
                    temp->AC_2[i][j]=10*i+j+1;
                    temp->AC_2_vacent++;
                    done=1;
                }
            }
        }

    }
    else
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(abs(temp->AC_1[i][j])==Passenger_id)
                {
                    temp->AC_1[i][j]=10*i+j+1;
                    temp->AC_1_vacent++;
                    done=1;
                }
            }
        }

    }
    // printf("Sleeper vacane%d\n",temp->Sleeper_vacent);
    // printf("AC3 vacane%d\n",temp->AC_3_vacent);
    // printf("AC2 vacane%d\n",temp->AC_2_vacent);
    // printf("AC1 vacane%d\n",temp->AC_1_vacent);

}

void displayTrainList(TrainNode *Trainhead)//Displaying the Train Name and Train id which are available for reservation
{
    printf("\t\tFollowing Express Trains are Available!!\n");
    if(Trainhead)
    {
        printf("\t__________________________________\n");
        printf("\tName                  Train_id\n\n");
        printf("\t__________________________________\n");
        // printf("-----------------------------------------------------------------\n");
        
        while(Trainhead!=NULL)
        {
            printf("\t->%s                %d",Trainhead->Train_Name,Trainhead->Train_id);
            printf("\n");
            Trainhead= Trainhead->next;
        }
        printf("\t____________________________________\n");
        printf("\n\n");
    }
    else
        printf("\nNo records to DISPLAY\n\n");
} 

TrainNode *findTrainNode(TrainNode *Trainhead,int Train_id)//To find the train to make changes in its Seat Matrix
{
    int Target_id=25670+Train_id%10;
    TrainNode *temp=Trainhead,*retval;
    int found=0;
    while(temp && !found)
    {
        if(temp->Train_id==Target_id)
        {
            found=1;
            retval=temp;
        }
        temp=temp->next;
    }
    return retval;
}

//-------------------------------------------------------------------------------------------------------------------------
//Helper functions
int linear_search(Node *records_head,int Passenger_id)//searching passengerid
{
    int node_num=0;
    int found=0;
    int i=0;
    while(records_head && !found)
    {
        i++;
        if(records_head->Passenger_id==Passenger_id)
        {
            node_num=i;
            found=1;
        }
        else
            records_head=records_head->next;
    }
    return node_num;
}

int getCount(Node *records_head)//To get count of number of Records
{
    int cnt=0;
    while(records_head!=NULL)
    {
        cnt++;
        records_head = records_head->next;
    }
    return cnt;
}

void displayRecord(Node *records_head)//Displaying list
{
    if(records_head)
    {
        printf("___________________________________________________________________________________________\n");
        printf("Name    ID   B_train B_station   Class   D_Station  TrainID Date SeatNo  Upgrade_Request\n\n");
        printf("___________________________________________________________________________________________\n");
        // printf("-----------------------------------------------------------------\n");
        
        while(records_head!=NULL)
        {
            printf("%s\t %d %s    %s    %s    %s    %d   ",records_head->Passenger_Name,records_head->Passenger_id,records_head->Boarding_train,records_head->Boarding_station,records_head->class,records_head->Destination_station,records_head->Train_id);
            printf("%d/%d/%d \t",records_head->d.day,records_head->d.month,records_head->d.year);
            if(records_head->Seat_Number>0)
            {
                if(strcmp(records_head->class,"Sleeper")==0)
                    printf("Sleeper-");
                else if(strcmp(records_head->class,"3AC")==0)
                    printf("3AC-");
                else if(strcmp(records_head->class,"2AC")==0)
                    printf("2AC-");
                else
                    printf("1AC-");
                printf("%d", records_head->Seat_Number);
            }
            else
                printf(" --");
            if(records_head->upgrade)
                printf("    YES");
            else
                printf("    NO");
            printf("\n");
            records_head = records_head->next;
        }
        printf("_____________________________________________________________________________________\n");
        printf("\n\n");
    }
    else
        printf("\nNo records to DISPLAY\n\n");

}
//-------------------------------------------------------------------------------------------------------------------------
//MergeSort
//Divide Function used in Sorting
Node *Divide(Node *lptr)
{
    Node *slow, *fast, *prev;
    slow = fast = lptr;
    while (fast != NULL && fast->next != NULL)
    {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    prev->next = NULL;
    return slow;
}
//Merge Function used in Sorting
Node *Merge(Node *lptr,Node *nptr,int parameter)
{
    Node *head,*ptr;
    if(parameter==0)//sort on train id
    {
        if(lptr->Train_id > nptr->Train_id)
        {
            head=nptr;
            nptr=nptr->next;
        }
        else
        {
            head=lptr;
            lptr=lptr->next;
        }
        ptr=head;
        while(nptr!=NULL && lptr!=NULL)
        {
            if (lptr->Train_id > nptr->Train_id)
            {
                ptr->next = nptr;
                nptr = nptr->next;
            }
            else
            {
                ptr->next = lptr;
                lptr = lptr->next;
            }
            ptr = ptr->next;
        } 
        if (lptr != NULL)
            ptr->next = lptr;
        else
            ptr->next = nptr;
    }

    if(parameter==1)//Sort on Train Date
    {
        if(compare_Date(lptr->d,nptr->d)==1)//lptr->Date> nptr->Date
        {
            head=nptr;
            nptr=nptr->next;
        }
        else
        {
            head=lptr;
            lptr=lptr->next;
        }
        ptr=head;
        while(nptr!=NULL && lptr!=NULL)
        {
            if (compare_Date(lptr->d,nptr->d)==1)
            {
                ptr->next = nptr;
                nptr = nptr->next;
            }
            else
            {
                ptr->next = lptr;
                lptr = lptr->next;
            }
            ptr = ptr->next;
        } 
        if (lptr != NULL)
            ptr->next = lptr;
        else
            ptr->next = nptr;
    }

    return head;
}
//Sorting using MERGESORT
Node *MergeSort(Node *lptr,int parameter)
{
    Node *nptr;
    if(lptr!=NULL && lptr->next!=NULL)
    {
        nptr = Divide(lptr);
        lptr = MergeSort(lptr, parameter);
        nptr = MergeSort(nptr, parameter);
        lptr = Merge(lptr, nptr, parameter);
    }
    return lptr;
}
//-------------------------------------------------------------------------------------------------------------------------
