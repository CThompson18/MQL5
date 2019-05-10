input long my_magic=555;
//+------------------------------------------------------------------+
//| Script program start function                                    |
//+------------------------------------------------------------------+
void GetTicketInfo()
	{
		//--- obtain the total number of orders
		int orders=OrdersTotal();
		//--- scan the list of orders
		for(int i=0;i<orders;i++)
			{
				ResetLastError();
				//--- copy into the cache, the order by its number in the list
				ulong ticket=OrderGetTicket(i);
				if(ticket!=0)// if the order was successfully copied into the cache, work with it
					{
						double price_open = OrderGetDouble(ORDER_PRICE_OPEN);
						datetime time_setup = OrderGetInteger(ORDER_TIME_SETUP);
						string symbol = OrderGetString(ORDER_SYMBOL);
						long magic_number = OrderGetInteger(ORDER_MAGIC);
						if(magic_number == my_magic)
							{
								//  process the order with the specified ORDER_MAGIC
							}
								PrintFormat("Order #%d for %s was set out %s, ORDER_MAGIC=%d",ticket,symbol,TimeToString(time_setup),magic_number);
					}
				else         // call OrderGetTicket() was completed unsuccessfully
					{
						PrintFormat("Error when obtaining an order from the list to the cache. Error code: %d",GetLastError());
					}
			}
	}