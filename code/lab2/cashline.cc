#include "cashline.h" 


CashLine:: CashLine(){	 

};

CashLine:: CashLine(bool i){	 
    cashLine = new List<customer *>();
    opened = i;
};

void 
CashLine:: setLineOpened(bool i){
    opened = i;
};         

bool 
CashLine:: isLineOpened(){
    return opened;
}; 

List<customer *> *CashLine:: getCashLine(){
	return cashLine;
}