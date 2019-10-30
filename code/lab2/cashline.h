#ifndef CASH_H
#define CASH_H

#include "customer.h"
#include "../lib/list.h"

class CashLine {
    
    public:
        
        CashLine();  
        CashLine(bool i);

        void setLineOpened(bool i);
        bool isLineOpened();

        List<customer *> *getCashLine();


    private:
        bool opened;
        List<customer *> *cashLine;
};

#endif