#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "utility.h"

#include "customer.h"
#include "cashline.h"

#include <typeinfo> 

int customerId = 1;
int opendLines = 2;
int maxOpenedLines = 0;

//////REPORT VARIABLES

int hourlyLinesSum = 0;
int ShortestWaiting=10000;
int LongestWaiting=0;
int WaitingSum=0;

int ShortestService=10000;
int LongestService=0;
int ServiceSum=0;

int GlobalCheckoutedPeople=0;
int CheckoutedPeople=0;
int hourlyCustomers = 0;

int peopleMoreThan3 = 0;

int SmallestWQ=10000;
int LargestWQ=0;
int WaitingQSum=0;

int GlobalShortestWaiting=10000;
int GlobalLongestWaiting=0;
int GlobalWaitingSum=0;

int GlobalShortestService=10000;
int GlobalLongestService=0;
int GlobalServiceSum=0;

int GlobalMaxWaitingQ=0;
///////////////////////////////////

//TESTING

int closinglines = false; // stop receiving people after some time, to test line closing

//

List<customer *> WaitingLine; 

CashLine cashiers[10];

int
RandHandler(int min, int max){

// Function for generating the range between two integer numbers

    return min + (rand() % ( max - min + 1 ) );
}

int 
generateCustNumber(int minute){
// Generate random number according to given minute
    
    if (closinglines && minute>100)// to check that lines really close
    {
       return 0;
    }

    if ((minute>=0 && minute<120) || (minute>=240 && minute<300)) {
        return RandHandler(0, 5);
    }

    return RandHandler(5, 10);
}

int RoundUpToMinute(int t){
// Roundup seconds to minute

    if(t%60==0){
        return t/60;
    }else{
        return (t+60)/60;
    }
}

int 
calcServiceTime(int items){
// Calculate Service Time according to number of items

    int serviceTime = 10 + 5*items  + 90;
    return RoundUpToMinute(serviceTime);
}

void 
Initialize(){
// Initialize the program
// Generate Cashier lines
// Open first two lines by default

    for (int i = 0; i<10; i++){
        if (i<2)
        {
            cashiers[i] = CashLine(true); 
        }else{
            cashiers[i] = CashLine(false);
        }    
    }
}

int
openNextLine(){
// Opens the next line if Possible
// Returns opened line id or 
// Returns 0, if all lines are opened

    for (int i = 0; i<10; i++){
        
        if(!cashiers[i].isLineOpened()){
            cashiers[i].setLineOpened(true);
            opendLines++;

            // cout << "OPENED LINE "<<i+1<<" \n";
            return i;
        }

    }
    // cout << "ALL LINES ARE OPENED \n";
    return 0;
}

void checkInWaiting(int l){
// Gets people from waiting line and adds to Cashier Line till it gets filled
    
    while(cashiers[l].getCashLine()->NumInList()<5 && !WaitingLine.IsEmpty()){

        cashiers[l].getCashLine()->Append(WaitingLine.Front());

        WaitingLine.RemoveFront();
        
    } 

}

int chooseLine(){
// Chooses the line with least number of people
// if all lines have 5 people, chooses Waiting line for customer

    int lineId = 0;
    int peopleN = 5;

    for (int i = 0; i<10; i++){
        if (cashiers[i].isLineOpened()==true && cashiers[i].getCashLine()->NumInList() < peopleN)
        {

            peopleN = cashiers[i].getCashLine()->NumInList();
            lineId = i+1;
        }
    }
    if (lineId>0)
    {
        // cout <<"best line for customer is: " <<lineId << "\n";

        return lineId;
    }else{
    
        if (WaitingLine.NumInList()<10)
        {
            
            return 0;
        }else{
            int l =  openNextLine();
            if (l>0)
            {
                checkInWaiting(l-1);
                // cout<<"for opened line length "<< cashiers[l-1].getCashLine()->NumInList()<<"\n";
            }
            return l;
        }
        
    }
}


void assignToLine(int l, customer *c, int m){
// Assigns customer to Best line
// Assigns checkout time if possible 
// When customer comes and line is empty, checkout time can be set because serving will start on the same minute
// if Cashier line is not empty, checkou time is set as -1, as it will be updated later when customer's serving will start

    if (l>0)
    {

        if (cashiers[l-1].getCashLine()->IsEmpty())
        {
            c->setCheckoutTime(RoundUpToMinute(m*60+c->getServiceTime())); 
        }else{

            c->setCheckoutTime(-1); 

        }

        cashiers[l-1].getCashLine()->Append(c);

    }
    else{
        
        WaitingLine.Append(c);
    }   
}


void checkIn(int minute){
// Main Checkin Function, run every minute and takes care of new comer people


    //FOR REPORT 4,5,6
    hourlyLinesSum += opendLines;

    if (maxOpenedLines<opendLines)
    {
        maxOpenedLines=opendLines;
    }


    //FOR REPORT 7 
    int WQLength = WaitingLine.NumInList();
    SmallestWQ = min(SmallestWQ, WQLength);
    LargestWQ = max(LargestWQ, WQLength);  

    WaitingQSum += WQLength;
    ////


    int numberofCustomers = generateCustNumber(minute);

    
    for(int i =0; i < numberofCustomers; i++){
        
        customer *c = new customer();

        c->setId(customerId);
        c->setArrivingTime(minute);
        
        int numberOfItems = RandHandler(5, 40);
        
        c->setNumberOfItems(numberOfItems);
        c->setServiceTime(calcServiceTime(numberOfItems));
        
        int bestLine = chooseLine();
        c->setLineId(bestLine);
        

        assignToLine(bestLine, c, minute);

        // TESTING
        // cout << " arrival: "<< c->getArrivingTime() << "  line id: "<< c->getLineId()<<" \n";
        // cout << " Waiting line : "<< WaitingLine.NumInList()<<" \n";

        // if (bestLine>0)
        // {
        //    cout << " Cashier line "<< bestLine <<" : "<< cashiers[bestLine-1].getCashLine()->NumInList()<<" \n";
        // }
        
        
        customerId++;


    }

}

void checkOut(int minute){  
// Main Checkout Function, run every minute and takes care of people who are checking out on this minute
      
    for (int i = 0; i<10; i++){
        
        //FOR REPORT 6

        if (cashiers[i].isLineOpened()==true && cashiers[i].getCashLine()->NumInList() > 3)
        {
            peopleMoreThan3++;
        }
        ////
         
        if(cashiers[i].getCashLine()->IsEmpty() == false && cashiers[i].getCashLine()->Front()->getCheckoutTime() == minute){  
            // cout << "\n------CHECKOUT CUSTOMER--from line----"<<i+1<<"  \n";      
   

            int serviceTime = cashiers[i].getCashLine()->Front()->getServiceTime();

            // FOR REPORT 3     
            ShortestService = min(ShortestService, serviceTime);
            LongestService = max(LongestService, serviceTime);            
            ServiceSum += serviceTime;
            ////

            cashiers[i].getCashLine()->RemoveFront(); // Checkout customer
            CheckoutedPeople++;   

            checkInWaiting(i);
                
            // cout << "------WAITING LINE CHECKED------\n";         

            if (!cashiers[i].getCashLine()->IsEmpty())
            {
                customer *c = cashiers[i].getCashLine()->Front();

                c->setCheckoutTime(minute + c->getServiceTime());

                //FOR REPORT 2 
                            
                int waitingTime = minute - c->getArrivingTime();

                ShortestWaiting = min(ShortestWaiting, waitingTime);
                LongestWaiting = max(LongestWaiting, waitingTime);   
                WaitingSum += waitingTime;
                
                ////

                // cout << "------NEXT CHECKOUT TIME FOR THIS LINE IS ----"<<c->getCheckoutTime()<<"----\n"; 

            }else{
                if (opendLines>2)
                {
                    cashiers[i].setLineOpened(false);
                    opendLines-=1;
                    // cout<<"!!!!!!!!!!!!! LINE "<<i+1<<" CLOSED !!!!!\n";
                }
                
                
            }
       
        }    
  
    }
    //TESTING
    // for (int i = 0; i<10; i++){
    //     if(cashiers[i].getCashLine()->IsEmpty()==false){
    //         cout<<"CHECKOUTS "<<cashiers[i].getCashLine()->Front()->getCheckoutTime()<<"\n";  
    //     } 
    // }
}


void Report1(){
// o    average number of customers arriving for checkout

    int avgCustomers = (customerId-hourlyCustomers)/60;

    cout << " o "<< avgCustomers << " People came to ckeckout on average per minute \n";
    
    hourlyCustomers = customerId;
    
}

void Report2(){
// o    average/shortest/longest waiting time

    cout<<" o Shortest Waiting Time was "<< ShortestWaiting <<" minutes\n";
    cout<<" o Longest Waiting Time was "<< LongestWaiting <<" minutes\n";    
    cout<<" o Average Waiting Time was "<< float(WaitingSum)/CheckoutedPeople <<" \n";

}

void Report3(){
// o    average/shortest/longest service time

    cout<<" o Shortest Service Time was "<< ShortestService <<" minutes\n";
    cout<<" o Longest Service Time was "<< LongestService <<" minutes\n";    
    cout<<" o Average Service Time was "<< float(ServiceSum)/CheckoutedPeople <<" \n";

}

void Report4(){
// o    average number of open lines

    cout<<" o On Average there ware "<< hourlyLinesSum/60 <<" lines opend\n";
    
}

void Report5(){
// o    maximum number of open lines    

    cout<<" o Max Number of Opened Lines was "<< maxOpenedLines <<" \n";

}

void Report6(){
// o   average time each casher will have more than 3 customers standing in line   
// NOTE: As the question is a little confusing, I assumed that it was asking for number of times per hour, 
// when any cashier had more than 3 customers in line 

    cout<<" o Average time each casher will have more than 3 customers standing in line was "<< peopleMoreThan3/maxOpenedLines<<" \n";
 
}

void Report7(){
// o average/smallest/largest number of customers in the waiting queue  

    cout<<" o Smallest Number of customers in the waiting queue was "<< SmallestWQ <<" \n";
    cout<<" o Largest Number of customers in the waiting queue was "<< LargestWQ <<" \n";
    cout<<" o Average Number of customers in the waiting queue was "<< WaitingQSum/60 <<" \n";

}

void updateGlobalReport(){
// Handles report variable updating for global report summary

    GlobalShortestWaiting = min(GlobalShortestWaiting, ShortestWaiting);
    GlobalLongestWaiting = max(GlobalLongestWaiting, LongestWaiting);    
    GlobalWaitingSum += WaitingSum;

    GlobalShortestService = min(GlobalShortestService, ShortestService);
    GlobalLongestService = max(GlobalLongestService, LongestService);

    GlobalMaxWaitingQ = max(GlobalMaxWaitingQ,LargestWQ);    
    GlobalServiceSum += ServiceSum;

    GlobalCheckoutedPeople += CheckoutedPeople;

}

void clearReportParameters(){
// Clear Hourly report variables after each hour

    ShortestWaiting=10000;
    LongestWaiting=0;
    WaitingSum=0;

    ShortestService=10000;
    LongestService=0;
    ServiceSum=0;

    CheckoutedPeople = 0;
    hourlyLinesSum = 0;
    maxOpenedLines = 0;

    peopleMoreThan3 = 0;

    SmallestWQ=10000;
    LargestWQ=0;
    WaitingQSum=0;

}

void hourlyReport(int minute){
// Hourly Report wrapper function that generates and handles all reports

    cout<<"\n_______________HOUR: " << (minute+1)/60<<" _______________\n\n";
    
    Report1();

    Report2();

    Report3();

    Report4();

    Report5();

    Report6();

    Report7();

    updateGlobalReport();

    clearReportParameters();

}

void generalReport(){
// Print general Report Summary 
// o   average/shortest/longest waiting time
// o   average/shortest/longest service time
// o   maximum number of customers in the waiting queue at any time

    cout<< "\n_______________GENERAL REPORT FOR SIMULATION_______________\n\n";

    cout<<" o Shortest Waiting Time was "<< GlobalShortestWaiting <<" minutes\n";
    cout<<" o Longest Waiting Time was "<< GlobalLongestWaiting <<" minutes\n";    
    cout<<" o Average Waiting Time was "<< float(GlobalWaitingSum)/GlobalCheckoutedPeople <<" \n\n";

    cout<<" o Shortest Service Time was "<< GlobalShortestService <<" minutes\n";
    cout<<" o Longest Service Time was "<< GlobalLongestService <<" minutes\n";    
    cout<<" o Average Service Time was "<< float(GlobalServiceSum)/GlobalCheckoutedPeople <<" \n\n";
    
    cout<<" o Largest Number of customers in the waiting queue was "<< GlobalMaxWaitingQ <<" \n\n";

}

void
Simulation(){
// Main Simulation Function

    Initialize();
    for (int minute = 0; minute < 300; minute++) {

        checkIn(minute);
        checkOut(minute);
      
        if ((minute+1)%60==0)
        {        
            hourlyReport(minute);

        }       
    } 
    generalReport();
}

void
ThreadTest()
{
    Simulation();
}
