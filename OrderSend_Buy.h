//+------------------------------------------------------------------+ 
//| Script program start function                                    | 
//+------------------------------------------------------------------+ 
void OnStart() 
	{ 
		//--- get minimum stop level 
		double minstoplevel=MarketInfo(Symbol(),MODE_STOPLEVEL); 
		Print("Minimum Stop Level=",minstoplevel," points"); 
		double price=Ask; 
		//--- calculated SL and TP prices must be normalized 
		double stoploss=NormalizeDouble(Bid-minstoplevel*Point,Digits); 
		double takeprofit=NormalizeDouble(Bid+minstoplevel*Point,Digits); 
		//--- place market order to buy 1 lot 
		int ticket=OrderSend(Symbol(),OP_BUY,1,price,3,stoploss,takeprofit,"My order",16384,0,clrGreen); 
		if(ticket<0) 
			{ 
				Print("OrderSend failed with error #",GetLastError()); 
			} 
			else 
				Print("OrderSend placed successfully"); 
//--- 
  }