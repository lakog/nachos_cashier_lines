#ifndef CUSTOMER_H
#define CUSTOMER_H

class customer
{
	public:
		void setId(int id);
		void setArrivingTime(int minute);
		void setNumberOfItems(int items);
		void setServiceTime(int minute);
		void setLineId(int id);
		void setCheckoutTime(int id);

		int getArrivingTime();
		int getServiceTime();
		int getCheckoutTime();
		int getLineId();

	
	private:
        int id;
        int arrivingTime;
        int numberOfItems;
        int serviceTime;
        int lineId;
        int checkOutTime;
	
};

#endif