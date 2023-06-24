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
typedef struct Passenger
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
}Passenger;

typedef struct PassengerNode
{
    Passenger p;
    struct PassengerNode *left;
    struct PassengerNode *right;
    int height;
}PassengerNode;

//This is to keep track of SeatMatrix of a particular Train
typedef struct Train
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
}Train;

typedef struct TrainNode
{
    Train t;
    struct TrainNode *left;
    struct TrainNode *right;
    int height;
}TrainNode;

TrainNode *Train_Schedule;//To display the schedule
/* --------------------------------------------------------------------------------------------------------------------------------- */
// DECLARATION OF THE FUNCTIONS USED IN PROJECT


int allocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id,Date d1);//To allocate seat and returns seat Number which is allocated
void dellocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id);//deallocate seats when passenger cancells reservation or upgrades his class at that time his olderclass seat will be deallocated

int max(int a,int b);
int height_PassengerNode(PassengerNode *root);
int Balancefactor_Passenger(PassengerNode *root);
PassengerNode *leftRotate_Passenger(PassengerNode *root);
PassengerNode *rightRotate_Passenger(PassengerNode *root);
PassengerNode *AVL_Insert(PassengerNode *root,PassengerNode *newNode,int parameter);
int compare_Date(Date t1,Date t2);
PassengerNode *MakeTreeNode(char Passenger_Name[],char Boarding_train[],char Boarding_station[],char class[],char Destination_station[],int Passenger_id,int Train_id,Date d1,int to_upgrade,int flag);
void displayRecord(PassengerNode *nptr);
void Display_inorder(PassengerNode *root);
PassengerNode *MinvalueNode(PassengerNode *node);
void copyData(PassengerNode *d,PassengerNode *s);
PassengerNode *Delete(PassengerNode *root,int TrainTarget_id,int PassengerTarget_id,int flag);
void SortByTravelDate(PassengerNode *root,int Passenger_id);
void displayDestination_asperdates(PassengerNode *tempTree);
int height_TrainNode(TrainNode *root);
int Balancefactor_Train(TrainNode *root);
TrainNode *leftRotate_Train(TrainNode *root);
TrainNode *rightRotate_Train(TrainNode *root);
TrainNode *AVL_Insert_Train(TrainNode *root,TrainNode *newNode,int parameter);
void displayTrainList(TrainNode *Train_Schedule);//Displaying the Train Name and Train id which are available for reservation
TrainNode *MakeTrainNode(int id);//Making Train Node which will help to keep track of SeatMatrix particular Train
void Initialize_SeatMatrix();//Initialize seat Matrix
TrainNode *findTrainNode(TrainNode *root,int Train_id);//To find the train to make changes in its Seat Matrix
void SortTrains(PassengerNode *root,TrainNode *Train_Schedule);
void DisplayTrain_asperNumPassengers(TrainNode *root);
void makelist_SortedonName(PassengerNode *root);
void getListDestination(PassengerNode *root_Destination);
void RangeSearch(TrainNode *Train_Schedule,int from_Trainno,int to_Trainno);

/* --------------------------------------------------------------------------------------------------------------------------------- */
/*
Overview:
•	Used AVL Trees to implement.
•	A Train Database of 10 trains is maintained in a Tree to maintain the SEAT MATRIX and allocate proper seat to User.
•	The Trains are given id from 25671 to 25680(a try to make it real)
•	Seat Allocation
    o	When a user wants to allocate a seat in a certain class a SEAT is allocated to him in the proper coach and Train No and the seat number is CLASSNAME-SEATNUMBER (eg. 2AC-4).

•	Seat Deallocation
    o	When a User cancels his Reservation then his seat is deallocated from the SEAT-MATRIX from Train Database and the seat is made available for next reservations.

•	INSERTION:
    o	The First Comparision is made wrt  train_id as we want to keep Data sorted according to Train_id  But if the Train id is same The comparision is Done wrt 
        Passenger id as it would be beneficial for searching and Cancellation of Reservation.
    o	When a USER makes a RESERVATION in a certain Train a proper seat is allocated using the SEAT ALLOCATION method.

•	DELETION
    o	As done in Insertion first comparision is done wrt Train_id and then Passenger_id to find the Passenger and Cancel its Reservation
    o	If Reservation is cancelled successfully then seat is deallocated using the SEAT DEALLOCATION method.

•	GETLISTDESTINATION
    o	Comparision is firstly done wrt Trainid and then wrt Destination station due to which the passengers having same Trainid and Destination will come together and using 2 pointers a LIST with passengers having same destination and Train is made.

•	SORT BY TRAVEL DATE 
    o	A compare function is implemented to compare the dates and accordingly passenger Node is inserted in TREE

•	SORT TRAINS
    o	The SEAT MATRIX of Train also maintains Number of Reservations Done due to which it is easy to SORT TRAIN

•	RANGESEARCH
    o	The Trainid are within (25671-25680) so input must be given accordingly.
    o	 A simple scan of TrainDatabase is done and the Trains are displayed when it is in given Range.  
*/
/* --------------------------------------------------------------------------------------------------------------------------------- */
//AVL INSERTION PASSENGER NODE

int max(int a,int b)
{
    return (a > b)? a : b;
}
int height_PassengerNode(PassengerNode *root)
{
    int retval=0;
    if(root == NULL)
    {
        retval = 0;
    }
    else
    {
        retval = root->height;
    }
    return retval;
}

int Balancefactor_Passenger(PassengerNode *root)//Calculating the balance factor which is  (LH-RH)
{
    int retval=0;
    if(root!=NULL)
    {
        retval=height_PassengerNode(root->left)-height_PassengerNode(root->right);
    }
    return retval;
}

PassengerNode *leftRotate_Passenger(PassengerNode *root)
{
    // printf("lr\n");
    PassengerNode *x=root->right;
    PassengerNode *T2=x->left;
    // printf("rotations starts\n");
    //Doing Rotations
    x->left=root;
    root->right=T2;
    // printf("rotations end\n");
    //Updating Heights
    root->height=max(height_PassengerNode(root->left),height_PassengerNode(root->right)) + 1;
    x->height=max(height_PassengerNode(x->left),height_PassengerNode(x->right)) + 1;
    // printf("updations end\n");
    return x;//New Root
}

PassengerNode *rightRotate_Passenger(PassengerNode *root)
{
    PassengerNode *x=root->left;
    PassengerNode *T2=x->right;

    //Doing Rotations
    x->right=root;
    root->left=T2;

    //Updating Heights
    root->height=max(height_PassengerNode(root->left),height_PassengerNode(root->right)) + 1;
    x->height=max(height_PassengerNode(x->left),height_PassengerNode(x->right)) + 1;

    return x;//New Root
}

int compare_Date(Date t1,Date t2)//COMPARE DATE FUNCTION HELPS IN SORTING ACC TO DATES
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
//NODE MAKER FUNCTION
PassengerNode *MakeTreeNode(char Passenger_Name[],char Boarding_train[],char Boarding_station[],char class[],char Destination_station[],int Passenger_id,int Train_id,Date d1,int to_upgrade,int flag)
{
    //Making Node and setting values given by user
    PassengerNode *nptr;
    nptr = (PassengerNode *)malloc(sizeof(PassengerNode));
    if(nptr!=NULL)
    {
        strcpy(nptr->p.Passenger_Name,Passenger_Name);
        nptr->p.Passenger_id=Passenger_id;
        nptr->p.Train_id=Train_id;
        strcpy(nptr->p.Boarding_train,Boarding_train);
        strcpy(nptr->p.Boarding_station,Boarding_station);
        strcpy(nptr->p.class,class);
        strcpy(nptr->p.Destination_station,Destination_station);
        nptr->p.d.day=d1.day;
        nptr->p.d.month=d1.month;
        nptr->p.d.year=d1.year;
        
        if(flag)
            nptr->p.Seat_Number=allocate_Seats(Train_Schedule,Train_id,class,nptr->p.Passenger_id,d1);//allocating seat to user 
        nptr->p.upgrade=to_upgrade;

        nptr->left=NULL;
        nptr->right=NULL;
        nptr->height=1;
    }
    return nptr;
}


PassengerNode *AVL_Insert(PassengerNode *root,PassengerNode *newNode,int parameter)//taking parameter for different cases
{
    PassengerNode *retval;
    int balance;
    if(root==NULL)      
    {
        retval=newNode;
    }
    else
    {
        retval=root;
        if(parameter==1)//Normal insertion
        {
            if(newNode->p.Train_id < root->p.Train_id)//FIrst comparision is done by TRAIN ID as we have to keep sorted acc  to trainid
            {
                // printf("Inserting left\n");
                root->left=AVL_Insert(root->left,newNode,parameter);
            }
            else if(newNode->p.Train_id > root->p.Train_id)
            {
                // printf("Inserting right\n");
                root->right=AVL_Insert(root->right,newNode,parameter);
            }
            else//Equal Train_id (if Trainid is same) 
            {
                // printf("Equal trainid\n");
                if(newNode->p.Passenger_id==root->p.Passenger_id)
                {
                    if(compare_Date(newNode->p.d,root->p.d)==1)//For same passenger keeping sorted on BASIS of DATE 
                    {
                    // printf("Inserting right\n");
                        root->right=AVL_Insert(root->right,newNode,parameter);
                    }
                    else
                    {
                        // printf("Inserting left\n");
                        root->left=AVL_Insert(root->left,newNode,parameter);
                    }
                }
                else//Passenger id is not same
                {
                    if(root->p.Passenger_id < newNode->p.Passenger_id)//For different Passenger but Same Train id keeping sorted on Passenger id as it will be beneficial for  searching while DELETION
                    {
                        // printf("Inserting right\n");
                        root->right=AVL_Insert(root->right,newNode,parameter);
                    }
                    else
                    {
                        // printf("Inserting left\n");
                        root->left=AVL_Insert(root->left,newNode,parameter);
                    }
                }

            }
        }
        else if(parameter==2)//for Sorting acc to TravelDate of Particular Passenger
        {
            if(compare_Date(newNode->p.d,root->p.d)==1)//newNode->p.date > root->p.date
            {
                root->right=AVL_Insert(root->right,newNode,parameter);
            }
            else
            {
                root->left=AVL_Insert(root->left,newNode,parameter);
            }
        }

        else if(parameter==3)//For sorting acc to Trainid first and if TrainId is same then sorting acc to Destination station Names which helps like Passengers having same destination will be together
        {
            if(newNode->p.Train_id < root->p.Train_id)
            {
                // printf("Inserting left\n");
                root->left=AVL_Insert(root->left,newNode,parameter);
            }
            else if(newNode->p.Train_id > root->p.Train_id)
            {
                // printf("Inserting right\n");
                root->right=AVL_Insert(root->right,newNode,parameter);
            }
            else//Equal Train_id then need to compare wrt destination station as by doing this Passengers having same destination will come together
            {
                if(strcmp(newNode->p.Destination_station,root->p.Destination_station)>0)
                {
                    // printf("Inserting right\n");
                    root->right=AVL_Insert(root->right,newNode,parameter);
                }
                else
                {
                    // printf("Inserting left\n");
                    root->left=AVL_Insert(root->left,newNode,parameter);
                }
            }
        }



        root->height=1 + max(height_PassengerNode(root->left),height_PassengerNode(root->right));

        balance=Balancefactor_Passenger(root);

        if(balance >1 && Balancefactor_Passenger(root->left)>=0)//Here checking BALANCE FACTOR for a Node rather than checking its Value as we have also modified the equal condition while inserting
        {
            // printf("bal>1 && n<r\n");
            retval=rightRotate_Passenger(root);
        }

        else if(balance < -1 && Balancefactor_Passenger(root->right)<=0)
        {
            // printf("bal<-1 && n>r\n");
            retval=leftRotate_Passenger(root);
        }

        else if(balance > 1  && Balancefactor_Passenger(root->left)<0)
        {
            // printf("bal>1 && n>r\n");
            root->left=leftRotate_Passenger(root->left);
            retval=rightRotate_Passenger(root);
        }

        else if(balance < -1  && Balancefactor_Passenger(root->right)>0)
        {
            // printf("bal<-1 && n<r\n");
            root->right=rightRotate_Passenger(root->right);
            retval=leftRotate_Passenger(root);
        }
    }

    // printf("Returning\n");
    return retval;
}

//DISPLAY HELPER
void displayRecord(PassengerNode *nptr)
{
    printf("%s\t %d %s    %s    %s    %s    %d   ",nptr->p.Passenger_Name,nptr->p.Passenger_id,nptr->p.Boarding_train,nptr->p.Boarding_station,nptr->p.class,nptr->p.Destination_station,nptr->p.Train_id);
    printf("%d/%d/%d \t",nptr->p.d.day,nptr->p.d.month,nptr->p.d.year);
    if(nptr->p.Seat_Number>0)
    {
        if(strcmp(nptr->p.class,"Sleeper")==0)
            printf("Sleeper-");
        else if(strcmp(nptr->p.class,"3AC")==0)
            printf("3AC-");
        else if(strcmp(nptr->p.class,"2AC")==0)
            printf("2AC-");
        else
            printf("1AC-");
        printf("%d", nptr->p.Seat_Number);
    }
    else
        printf(" --");
    if(nptr->p.upgrade)
        printf("    YES");
    else
        printf("    NO");
    printf("\n");
}

void Display_inorder(PassengerNode *root)
{
    if(root!=NULL)
    {
        Display_inorder(root->left);
        displayRecord(root);
        Display_inorder(root->right);
    }
}
/* --------------------------------------------------------------------------------------------------------------------------------- */
//AVL Deletion

PassengerNode *MinvalueNode(PassengerNode *node)
{
    PassengerNode *curr=node;

    while(curr->left!=NULL)
    {
        curr=curr->left;
    }
    return curr;
}

void copyData(PassengerNode *d,PassengerNode *s)
{
    strcpy(d->p.Passenger_Name,s->p.Passenger_Name);
    d->p.Passenger_id=s->p.Passenger_id;
    d->p.Train_id=s->p.Train_id;
    strcpy(d->p.Boarding_train,s->p.Boarding_train);
    strcpy(d->p.Boarding_station,s->p.Boarding_station);
    strcpy(d->p.class,s->p.class);
    strcpy(d->p.Destination_station,s->p.Destination_station);
    d->p.d.day=s->p.d.day;
    d->p.d.month=s->p.d.month;
    d->p.d.year=s->p.d.year;
    //
    // if(flag)
        // nptr->Seat_Number=allocate_Seats(Trainhead,Train_id,class,nptr->Passenger_id);//allocating seat to user 
    d->p.upgrade=s->p.upgrade;
}

int deleted;
PassengerNode *Delete(PassengerNode *root,int TrainTarget_id,int PassengerTarget_id,int flag)
{
    PassengerNode *retval=root;
    if(root==NULL)
    {
        retval=root;
    }
    else
    {
        if(root->p.Train_id < TrainTarget_id)
        {
            root->right=Delete(root->right,TrainTarget_id,PassengerTarget_id,flag);
        }
        
        else if(root->p.Train_id > TrainTarget_id)
        {
            root->left=Delete(root->left,TrainTarget_id,PassengerTarget_id,flag);
        }

        else//TargetTrain_id==root->p.Train_id
        {
            if(root->p.Passenger_id<PassengerTarget_id)//Here comparing Passengerid as while inserting the data is sorted according to Passengerid if Trainid was same
            {
                root->right=Delete(root->right,TrainTarget_id,PassengerTarget_id,flag);
            }
            else if(root->p.Passenger_id > PassengerTarget_id)
            {
                root->left=Delete(root->left,TrainTarget_id,PassengerTarget_id,flag);
            }
            else//root->p.Passenger_id == PassengerTarget_id)
            {
                if((root->left==NULL) || (root->right==NULL))//one child or no child
                {
                    PassengerNode *temp=root->left?root->left:root->right;
                
                    if(temp==NULL)//No child
                    {
                        if(flag)//if flag is one then only we have to deallocate seats
                            dellocate_Seats(Train_Schedule,root->p.Train_id,root->p.class ,root->p.Passenger_id);//if User cancell reservation then his seat must be deallocated
                        temp=root;
                        root=NULL;
                    }
                    else
                    {//One child
                        if(flag)
                            dellocate_Seats(Train_Schedule,root->p.Train_id,root->p.class ,root->p.Passenger_id);//if User cancell reservation then his seat must be deallocated
                        *root=*temp;
                    }
                    deleted=1;
                    free(temp);
                }
                else
                {
                    //Node with 2 child
                    PassengerNode *temp=MinvalueNode(root->right);
                    if(flag)
                        dellocate_Seats(Train_Schedule,root->p.Train_id,root->p.class ,root->p.Passenger_id);//if User cancell reservation then his seat must be deallocated
                    copyData(root,temp);
                    root->right=Delete(root->right,temp->p.Train_id,temp->p.Passenger_id,0);//Setting flag as 0 as we have deallocated the seat and Now we are calling the recursive func so we dont need to deallocate the eat now

                }

            }
        }

        if(root==NULL)
            retval=root;
        else
        {//Rotating if neccessary to maintain the Properties of AVL Tree
            root->height = 1 + max(height_PassengerNode(root->left),height_PassengerNode(root->right));
            int balance=Balancefactor_Passenger(root);

            if(balance>1 && Balancefactor_Passenger(root->left)>=0)
            {
                retval=rightRotate_Passenger(root);
            }
            else if(balance >1 && Balancefactor_Passenger(root->left)<0)
            {
                root->left=leftRotate_Passenger(root->left);
                retval=rightRotate_Passenger(root);
            }
            else if(balance<-1 && Balancefactor_Passenger(root->right)<=0)
            {
                retval=leftRotate_Passenger(root);
            }
            else if(balance<-1 && Balancefactor_Passenger(root->right)>0)
            {
                root->right=rightRotate_Passenger(root->right);
                retval=leftRotate_Passenger(root);
            }


        }

    }
    return retval;
}

/* --------------------------------------------------------------------------------------------------------------------------------- */
//SortByTravelDate

PassengerNode *tempTree;
void SortByTravelDate(PassengerNode *root,int Passenger_id)
{
    if(root!=NULL)
    {
        SortByTravelDate(root->left,Passenger_id);
        
        if(root->p.Passenger_id==Passenger_id)
        {
            PassengerNode *Node=MakeTreeNode(root->p.Passenger_Name,root->p.Boarding_train,root->p.Boarding_station,root->p.class,root->p.Destination_station,root->p.Passenger_id,root->p.Train_id,root->p.d,root->p.upgrade,0);
            tempTree=AVL_Insert(tempTree,Node,2);//Passing the SORTINGPARAMETER as 2 aswe have to sort on basis of DATES
        }
        
        SortByTravelDate(root->right,Passenger_id);
    }

}

//DISPLAY HELPER
void displayDestination_asperdates(PassengerNode *tempTree)
{
    if(tempTree!=NULL)
    {
        displayDestination_asperdates(tempTree->left);
        
        printf("%s\t%s\t",tempTree->p.Passenger_Name,tempTree->p.Destination_station);
        printf("%d/%d/%d",tempTree->p.d.day,tempTree->p.d.month,tempTree->p.d.year);
        printf("\n");
        
        displayDestination_asperdates(tempTree->right);
    }
}

/* --------------------------------------------------------------------------------------------------------------------------------- */
//TrainTree for keeping Track of Seat Matrix of a every Train
int height_TrainNode(TrainNode *root)
{
    int retval=0;
    if(root!=NULL)
        retval=root->height;
    return retval;
}

int Balancefactor_Train(TrainNode *root)
{
    int retval=0;
    if(root!=NULL)
    {
        retval=height_TrainNode(root->left)-height_TrainNode(root->right);
    }
    return retval;
}

TrainNode *leftRotate_Train(TrainNode *root)
{
    TrainNode *x=root->right;
    TrainNode *T2=x->left;;
    //Doing Rotations
    x->left=root;
    root->right=T2;
    //Updating Heights
    root->height=max(height_TrainNode(root->left),height_TrainNode(root->right)) + 1;
    x->height=max(height_TrainNode(x->left),height_TrainNode(x->right)) + 1;

    return x;//New Root
}

TrainNode *rightRotate_Train(TrainNode *root)
{
    TrainNode *x=root->left;
    TrainNode *T2=x->right;

    //Doing Rotations
    x->right=root;
    root->left=T2;

    //Updating Heights
    root->height=max(height_TrainNode(root->left),height_TrainNode(root->right)) + 1;
    x->height=max(height_TrainNode(x->left),height_TrainNode(x->right)) + 1;

    return x;//New Root
}

//AVL INSERT FOR TRAIN TO KEEP TRACK OF THE SEAT MATRIX FOR EVERY TRAIN
TrainNode *AVL_Insert_Train(TrainNode *root,TrainNode *newNode,int parameter)
{
    TrainNode *retval;
    int balance;
    if(root==NULL)
        retval=newNode;
    else
    {
        retval=root;
        if(parameter==1)//normal insertion and sorting wrt Trainid
        {
            if(newNode->t.Train_id < root->t.Train_id)
            {
                root->left=AVL_Insert_Train(root->left,newNode,parameter);
            }
            else
            {
                root->right=AVL_Insert_Train(root->right,newNode,parameter);
            }
        }
        else if(parameter==2)//ie we need to sort wrt Number of Passengers
        {
            if(newNode->t.Num_Passengers < root->t.Num_Passengers)
            {
                // printf("Inserting left\n");
                root->left=AVL_Insert_Train(root->left,newNode,parameter);
            }
            else if(newNode->t.Num_Passengers > root->t.Num_Passengers)
            {
                // printf("Inserting right\n");
                root->right=AVL_Insert_Train(root->right,newNode,parameter);
            }
            else//Equal Number of PAssengers
            {
                if(newNode->t.Train_id < root->t.Train_id)//if equal Passengers then sort according to Trainid
                {
                    root->left=AVL_Insert_Train(root->left,newNode,parameter);
                }
                else
                {
                    root->right=AVL_Insert_Train(root->right,newNode,parameter);
                }
            }
        }
        
        
        root->height=1 + max(height_TrainNode(root->left),height_TrainNode(root->right));

        balance=Balancefactor_Train(root);

        if(balance >1 && Balancefactor_Train(root->left)>=0)
        {
            // printf("bal>1 && n<r\n");
            retval=rightRotate_Train(root);
        }

        else if(balance < -1 && Balancefactor_Train(root->right)<=0)
        {
            // printf("bal<-1 && n>r\n");
            retval=leftRotate_Train(root);
        }

        else if(balance > 1  && Balancefactor_Train(root->left)<0)
        {
            // printf("bal>1 && n>r\n");
            root->left=leftRotate_Train(root->left);
            retval=rightRotate_Train(root);
        }

        else if(balance < -1  && Balancefactor_Train(root->right)>0)
        {
            // printf("bal<-1 && n<r\n");
            root->right=rightRotate_Train(root->right);
            retval=leftRotate_Train(root);
        }
    }

    // printf("Returning\n");
    return retval;
}
/* --------------------------------------------------------------------------------------------------------------------------------- */
//Helper functions for Seat Allocation,Deallocation,Initialization of Seat Matrix
void displayTrainList(TrainNode *Train_Schedule)//Displaying the Train Name and Train id which are available for reservation
{
    if(Train_Schedule!=NULL)
    {
        displayTrainList(Train_Schedule->left);
            
            printf("\t->%s                %d",Train_Schedule->t.Train_Name,Train_Schedule->t.Train_id);
            // printf("     %d",Train_Schedule->t.Num_Passengers);
            printf("\n");

        displayTrainList(Train_Schedule->right);
    }

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
            tnptr->t.Sleeper[i][j]=(10*i+j+1);
            tnptr->t.AC_1[i][j]=(10*i+j+1);
            tnptr->t.AC_2[i][j]=(10*i+j+1);
            tnptr->t.AC_3[i][j]=(10*i+j+1);
        }
    }

    strcpy(tnptr->t.Train_Name,Name[id-1]);//
    tnptr->t.Sleeper_vacent=20;//assumed 2 bogies and each having seats of size 10
    tnptr->t.AC_1_vacent=20;
    tnptr->t.AC_2_vacent=20;
    tnptr->t.AC_3_vacent=20;
    tnptr->t.Num_Passengers=0;
    if(id==0)
        tnptr->t.Train_id=25680;
    else
        tnptr->t.Train_id=25670+id;

    tnptr->height=1;
    tnptr->left=NULL;
    tnptr->right=NULL;

    return tnptr;
}


void Initialize_SeatMatrix()//Initialize seat Matrix
{
    for(int k=1;k<=10;k++)//Assuming 10 trains available for reservation
    {
        TrainNode *temp=MakeTrainNode(k);//Making TrainNode and linking it at Proper place below
        Train_Schedule=AVL_Insert_Train(Train_Schedule,temp,1);
    } 
}


TrainNode *findTrainNode(TrainNode *root,int Train_id)//To find the train to make changes in its Seat Matrix
{
    TrainNode *retval;
    if(root!=NULL)
    {
        if(root->t.Train_id < Train_id)
        {
            retval=findTrainNode(root->right,Train_id);
        }
        else if(root->t.Train_id > Train_id)
        {
            retval=findTrainNode(root->left,Train_id);
        }
        else
        {
            retval=root;
        }
    }
    return retval;
}

int allocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id,Date d1)//To allocate seat and returns seat Number which is allocated
{
    TrainNode *temp=findTrainNode(Trainhead,Train_id);//finding Train in which Seat is to be allocated
    temp->t.Num_Passengers++;//incrementing NUmber of passengers in train
    // printf("INside allocateseats::%d\n",temp->t.Num_Passengers);
    temp->t.d.day=d1.day;
    temp->t.d.month=d1.month;
    temp->t.d.year=d1.year;
    int done=0,retval=0;
    if(strcmp(class,"Sleeper")==0)//if user want to do reservation in Sleeper class
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(temp->t.Sleeper[i][j]>0)//finding place where we can allocate seat to user
                {
                    retval=temp->t.Sleeper[i][j];//setting retval to seatNUmber
                    temp->t.Sleeper[i][j]=-(Passenger_id);//after allocating seat make seatvalue as  -(Passengerid) which will help in deallocating seat for particular passengerid
                    temp->t.Sleeper_vacent--;//decrementing Number of vacent seats
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
                if(temp->t.AC_3[i][j]>0)
                {
                    retval=temp->t.AC_3[i][j];
                    temp->t.AC_3[i][j]=-(Passenger_id);
                    temp->t.AC_3_vacent--;
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
                if(temp->t.AC_2[i][j]>0)
                {
                    retval=temp->t.AC_2[i][j];
                    temp->t.AC_2[i][j]=-(Passenger_id);
                    temp->t.AC_2_vacent--;
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
                if(temp->t.AC_1[i][j]>0)
                {
                    retval=temp->t.AC_1[i][j];
                    temp->t.AC_1[i][j]=-(Passenger_id);
                    temp->t.AC_1_vacent--;
                    done=1;
                }
            }
        }

    }
    // printf("retval---%d\n",retval);
    // printf("Sleeper vacant%d\n",temp->t.Sleeper_vacent);
    // printf("AC3 vacant%d\n",temp->t.AC_3_vacent);
    // printf("AC2 vacant%d\n",temp->t.AC_2_vacent);
    // printf("AC1 vacant%d\n",temp->t.AC_1_vacent);

    return retval;
}

void dellocate_Seats(TrainNode *Trainhead,int Train_id,char class[],int Passenger_id)//deallocate seats when passenger cancells reservation or upgrades his class at that time his olderclass seat will be deallocated
{
    TrainNode *temp=findTrainNode(Trainhead,Train_id);//finding Train to make changes in its Seat matrix

    // printf("deallocation::temp->trainid%d\n",temp->t.Train_id);
    // printf("deallocation::Trainid%d\n",Train_id);
    // printf("deallocation::SEATS B4%d\n",temp->t.Num_Passengers);
    temp->t.Num_Passengers--;//decrementing number of passengers
    // printf("deallocation::SEATS B4%d\n",temp->t.Num_Passengers);

    int done=0;
    if(strcmp(class,"Sleeper")==0)
    {
        for(int i=0;i<2 && !done;i++)
        {
            for(int j=0;j<10 && !done;j++)
            {
                if(abs(temp->t.Sleeper[i][j])==Passenger_id)//while allocating we have made the seat value  -(Passengerid) so checking this
                {
                    temp->t.Sleeper[i][j]=10*i+j+1;//changing seat value to its Number
                    temp->t.Sleeper_vacent++;//increamenting vacent sleeper seats
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
                if(abs(temp->t.AC_3[i][j])==Passenger_id)
                {
                    temp->t.AC_3[i][j]=10*i+j+1;
                    temp->t.AC_3_vacent++;
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
                if(abs(temp->t.AC_2[i][j])==Passenger_id)
                {
                    temp->t.AC_2[i][j]=10*i+j+1;
                    temp->t.AC_2_vacent++;
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
                if(abs(temp->t.AC_1[i][j])==Passenger_id)
                {
                    temp->t.AC_1[i][j]=10*i+j+1;
                    temp->t.AC_1_vacent++;
                    done=1;
                }
            }
        }

    }
    // printf("Sleeper vacane%d\n",temp->t.Sleeper_vacent);
    // printf("AC3 vacane%d\n",temp->t.AC_3_vacent);
    // printf("AC2 vacane%d\n",temp->t.AC_2_vacent);
    // printf("AC1 vacane%d\n",temp->t.AC_1_vacent);

}

/* --------------------------------------------------------------------------------------------------------------------------------- */
TrainNode *tempTrainTree,*rightMostNode;

void SortTrains(PassengerNode *root,TrainNode *Train_Schedule)
{
    if(Train_Schedule!=NULL)
    {
        SortTrains(root,Train_Schedule->left);

        if(Train_Schedule->t.Num_Passengers!=0)
        {
            TrainNode *temp=MakeTrainNode((Train_Schedule->t.Train_id)%10);
            temp->t.Num_Passengers=Train_Schedule->t.Num_Passengers;
            temp->t.d.day=Train_Schedule->t.d.day;
            temp->t.d.month=Train_Schedule->t.d.month;
            temp->t.d.year=Train_Schedule->t.d.year;

            tempTrainTree=AVL_Insert_Train(tempTrainTree,temp,2);
        }

        SortTrains(root,Train_Schedule->right);

    }

}

void DisplayTrain_asperNumPassengers(TrainNode *root)
{
    if(root!=NULL)
    {
        DisplayTrain_asperNumPassengers(root->left);
        
        printf("%d\t",root->t.Train_id);
        printf("%d/%d/%d\t%d",root->t.d.day,root->t.d.month,root->t.d.year,root->t.Num_Passengers);
        printf("\n");
        
        DisplayTrain_asperNumPassengers(root->right);

    }
}

/* --------------------------------------------------------------------------------------------------------------------------------- */
PassengerNode *temp_Destination,*root_Destination=NULL,*prev_cmp=NULL;
int firstmatchfound=0;
void makelist_SortedonName(PassengerNode *root)
{
    if(root!=NULL)
    {
        makelist_SortedonName(root->left);

        PassengerNode *newNode1=MakeTreeNode(root->p.Passenger_Name,root->p.Boarding_train,root->p.Boarding_station,root->p.class,root->p.Destination_station,root->p.Passenger_id,root->p.Train_id,root->p.d,root->p.upgrade,0);
        root_Destination=AVL_Insert(root_Destination,newNode1,3);//Keep passengers who have same destination together
        
        makelist_SortedonName(root->right);
    }
}
/*
root_destination is List which is SORTED firstly on basis of TRAINID and if Trainid is same then sorted on basis of DESTINATION STATION 
ADVANTAGE of doing this->
The Passengers having same trainid and same Destination Station will be together and they can be collected using TWO POINTER TECHNIQUE
which is implemented below
*/

//this gives the List of ALL  having sameTrainid and DESTINATION
void getListDestination(PassengerNode *root_Destination)
{
    if(root_Destination!=NULL)
    {
        getListDestination(root_Destination->left);

        if(prev_cmp!=NULL)
        {
            if(root_Destination->p.Train_id == prev_cmp->p.Train_id)
            {
                
                if(strcmp(root_Destination->p.Destination_station,prev_cmp->p.Destination_station)==0)
                {
                    if(!firstmatchfound)
                    {
                        PassengerNode *tempp1=MakeTreeNode(root_Destination->p.Passenger_Name,root_Destination->p.Boarding_train,root_Destination->p.Boarding_station,root_Destination->p.class,root_Destination->p.Destination_station,root_Destination->p.Passenger_id,root_Destination->p.Train_id,root_Destination->p.d,root_Destination->p.upgrade,0);
                        temp_Destination=AVL_Insert(temp_Destination,tempp1,3);
                        PassengerNode *tempp2=MakeTreeNode(prev_cmp->p.Passenger_Name,prev_cmp->p.Boarding_train,prev_cmp->p.Boarding_station,prev_cmp->p.class,prev_cmp->p.Destination_station,prev_cmp->p.Passenger_id,prev_cmp->p.Train_id,prev_cmp->p.d,prev_cmp->p.upgrade,0);
                        temp_Destination=AVL_Insert(temp_Destination,tempp2,3);

                    }
                    else
                    {
                        PassengerNode *tempp=MakeTreeNode(root_Destination->p.Passenger_Name,root_Destination->p.Boarding_train,root_Destination->p.Boarding_station,root_Destination->p.class,root_Destination->p.Destination_station,root_Destination->p.Passenger_id,root_Destination->p.Train_id,root_Destination->p.d,root_Destination->p.upgrade,0);
                        temp_Destination=AVL_Insert(temp_Destination,tempp,3);
                    }
                    firstmatchfound=1;
                }

            }
            else
            {
                firstmatchfound=0;
            }
        }

        prev_cmp=root_Destination;

        getListDestination(root_Destination->right);
    }
}

/* --------------------------------------------------------------------------------------------------------------------------------- */

int rangefound;
void RangeSearch(TrainNode *Train_Schedule,int from_Trainno,int to_Trainno)//Scans the Train Schedule list and perform RANGE SEARCHING
{
    if(Train_Schedule!=NULL)
    {
        if(from_Trainno < Train_Schedule->t.Train_id)
        {
            RangeSearch(Train_Schedule->left,from_Trainno,to_Trainno);
        }
        if((from_Trainno <= Train_Schedule->t.Train_id)  && to_Trainno >= Train_Schedule->t.Train_id)
        {
            rangefound=1;
            printf("\t->%s\t%d",Train_Schedule->t.Train_Name,Train_Schedule->t.Train_id);
            printf("\n");
        }

        RangeSearch(Train_Schedule->right,from_Trainno,to_Trainno);
    }
}

/* --------------------------------------------------------------------------------------------------------------------------------- */

int main()
{

    Initialize_SeatMatrix();//Initializing seat Matrix for all 10 trains

    printf("\t\tFollowing Express Trains are Available!!\n");
    printf("\t__________________________________\n");
    printf("\tName                  Train_id\n\n");
    printf("\t__________________________________\n");
    displayTrainList(Train_Schedule);//Displaying the Train Name and its id for user to choose a particular train
    printf("\t____________________________________\n");
    printf("\n\n");


    PassengerNode *root=NULL,*temp;
    

    PassengerNode *newNode,*newNode1;
    char Passenger_Name[NAME_LEN],Boarding_train[NAME_LEN],Boarding_station[NAME_LEN],class[CLASS_LEN],Destination_station[NAME_LEN];
    int Passenger_id,Train_id,n;
    int i,input,flag,found,count,to_upgrade;
    flag=0;
    // Node *temp,*list;
    
    printf("Perform operation\n");
    do
    {
            printf("--------------------------------------------------------------------------------\n");
            printf("\t\t\tTRAIN RESERVATION SYSTEM\n");
            printf("--------------------------------------------------------------------------------\n");
            printf(" 1  - To INSERT a new record\n");
            printf(" 2  - To Cancell Reservation\n");
            printf(" 3  - To get List of Passengers having Same Destination station and Same Train id\n");
            printf(" 4  - To sort by TravelDate for a particular Passenger\n");
            printf(" 5  - To Display Trainno and Date in sorted order of No of Passengers\n"); 
            printf(" 6  - To do RANGESEARCH\n"); 
            printf(" 7  - To DISPLAY existing record(s)\n");
            printf(" 8  - To Display Train Schedule\n");
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
                newNode=MakeTreeNode(Passenger_Name,Boarding_train,Boarding_station,class,Destination_station,Passenger_id,Train_id,d1,to_upgrade,1);             
                // newNode1=MakeTreeNode(Passenger_Name,Boarding_train,Boarding_station,class,Destination_station,Passenger_id,Train_id,d1,to_upgrade,0);             
                
                root=AVL_Insert(root,newNode,1);
                // root_Destination=AVL_Insert(root_Destination,newNode1,3);//Keep passengers who have same destination together
                if (root)//if found means inserted successfully
                    printf("RESERVATION DONE SUCCESSFULL\n");
                else
                    printf("RESERVATION FAILED, check the record size\n");            
                break;

            case 2:
                deleted=0;
                printf("Enter the Passenger id for the record which is to be deleted\n");
                printf("Enter Passenger ID\n");
                do
                {
                    printf("(Remember that id should contain 3 digits only)\n->");
                    scanf("%d",&Passenger_id);

                } while (Passenger_id < 100 || Passenger_id > 999); // Specific id because system handle this way only. Again a try to make this real.
                printf("Enter the Train id(Kindly Enter the correct Train id):");
                do
                {
                    printf("(Remember that id should be b/w 25671 to 25680)\n->");
                    scanf("%d", &Train_id);
                } while (Train_id < 25671 || Train_id > 25680); // Specific id because system handle this way only. Again a try to make this real.
                root=Delete(root,Train_id,Passenger_id,1);
                //if found means we have deleted it successfully
                if(deleted)
                    printf("RESERVATION CANCELLED SUCCESSFULLY!!!\n");
                else
                    printf("RESERVATION CANCELLATION FAILED!!!\n");
                break;
            
            case 3:

                temp_Destination=NULL,root_Destination=NULL;
                if(root==NULL)
                {
                    printf("No Reservations Done!!!\n");
                }

                else
                {
                    firstmatchfound=0;
                    makelist_SortedonName(root);
                    getListDestination(root_Destination);
                
                    if(temp_Destination==NULL)
                    {
                        printf("There are no Passengers having same Destination station and Train_id\n");
                    }
                    else
                    {
                        printf("Displaying the list of Passengers having same destination station and same train id\n");
                        printf("\n\nLIST OF RESERVATIONS : \n\n");
                        printf("___________________________________________________________________________________________\n");
                        printf("Name    ID   B_train B_station   Class   D_Station  TrainID Date SeatNo  Upgrade_Request\n\n");
                        printf("___________________________________________________________________________________________\n");
                        Display_inorder(temp_Destination);
                    }
                }
                

                break;
            case 4:
                tempTree=NULL;
                printf("Enter Passenger id\n");
                scanf("%d",&Passenger_id);
                SortByTravelDate(root,Passenger_id);
                if(tempTree==NULL)
                    printf("There is no Passenger with given id!!!\n");
                else
                {
                    printf("Displaying the List of Destination station as per Dates of travel(Sorted)\n");
                    printf("_____________________________________________________________________________________\n");
                    printf("Name\tDestination_Station\tDate \n\n");
                    printf("_____________________________________________________________________________________\n");
                    displayDestination_asperdates(tempTree);
                }
                break;

            case 5:
                tempTrainTree=NULL;
                if(root==NULL)
                    printf("Empty Record\n");
                else
                {
                    SortTrains(root,Train_Schedule);
                    printf("_____________________________________________________________________________________\n");
                    printf("Train_No\tDate\tNumPassengers \n\n");
                    printf("_____________________________________________________________________________________\n");
                    DisplayTrain_asperNumPassengers(tempTrainTree);
                }
                break;
            case 6:
                rangefound=0;
                int from_Trainno,to_Trainno;
                printf("!!!Remember that the Trains available have Trainid within range 25671-25680\nSo Enter Range Accordingly\n");
                printf("Enter From-train-Number\n");
                scanf("%d",&from_Trainno);
                printf("Enter To-train-Number\n");
                scanf("%d",&to_Trainno);
                
                printf("Displaying the Trains which lie in the given Range\n");
                printf("\t__________________________________\n");
                printf("\tTrainName\tTrainid \n\n");
                printf("\t__________________________________\n");
                RangeSearch(Train_Schedule,from_Trainno,to_Trainno);
                printf("\t__________________________________\n");
                

                if(rangefound==0)
                {
                    printf("\nThere are No Train whose Train_id lies in the Given Range\n");
                }

                break;
            case 7:
                if(root==NULL)
                    printf("Empty Record\n");
                else
                {
                    printf("\n\nLIST OF RESERVATIONS : \n\n");
                    printf("___________________________________________________________________________________________\n");
                    printf("Name    ID   B_train B_station   Class   D_Station  TrainID Date SeatNo  Upgrade_Request\n\n");
                    printf("___________________________________________________________________________________________\n");
                    Display_inorder(root);
                }
                break;
            case 8:
                displayTrainList(Train_Schedule);//Displaying the Train Name and its id for user to choose a particular train
                
                break;
            default:
                 printf("You have successfully exited");
                flag = 1;    
        }
    }while(flag==0);

    return 0;
}