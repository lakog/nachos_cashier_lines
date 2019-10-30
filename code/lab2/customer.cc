#include "customer.h" 


void
customer:: setId(int i){
    id = i;
}

void
customer:: setArrivingTime(int i){
    arrivingTime = i;
}

void
customer:: setNumberOfItems(int i){
    numberOfItems = i;
}

void
customer:: setServiceTime(int i){
    serviceTime = i;
}

void
customer:: setLineId(int i){
    lineId = i;
}
	 
void
customer:: setCheckoutTime(int i){
    checkOutTime = i;
}

int 
customer:: getLineId(){
	return lineId;
}
int 
customer:: getArrivingTime(){
    return arrivingTime;
}

int 
customer:: getServiceTime(){
	return serviceTime;
}
int 
customer:: getCheckoutTime(){
	return checkOutTime;
}

	    