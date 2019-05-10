//+------------------------------------------------------------------+
//|                                                   new_system.mq4 |
//|                                                                  |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
//Multi-function variables
//*********************************************************************
//---- input parameters
extern double glbRisk = 10.25;
extern int glbCountBars = 5000;
extern int glbMagic_Number = 1234;
extern int magic_number = 0; // set it if you'll use closing option 3 - closing by magic number
extern string comment_text = ""; // set it if you'll use closing option 4 - closing by comment
extern int before_day = 0; // set it if you'll use closing option 8 - closing by before day
extern int Slippage = 5; //Slippage

//Ticket
//*********************************************************************;
int glbTicket = 0;
int glbAskBid = 0;
datetime glbTicketDatetime  = "2017.01.01 00:00";
double glbOpenPrice = 0.000000;
double glbOrderLots = 0.000000;

//PlaceOrders
//*********************************************************************;
double glbLotSize = 0.75;
double glblTradeLots = 0.00;
extern double glbBid_SL = 150.00;
extern double glbBid_TP = 0.00;
extern double glbAsk_SL = 150.00;
extern double glbAsk_TP = 0.00;

//Prices
//*********************************************************************;
double glbAskPrice = 0.00000;
datetime glbAskDateTime = "1900.01.01 00:00:00";
double glbBidPrice = 0.00000;
datetime glbBidDateTime = "1900.01.01 00:00:00";
double glbCurrent = 0.00000;
int glbAskHour = 0;
int glbAskMinute = 0;
int glbBidHour = 0;
int glbBidMinute = 0;

//---- Trailing Stop
//*********************************************************************
extern double TrailingStart = 75; 
extern double TrailingStop = 35;
extern double TrailingStep = 5;


//DataDisplay function global variables
//*********************************************************************
extern int glb_right_edge_shift = 175;
extern int glb_upper_edge_shift = 10;
string glb_ea_name = "ChucknJill Financials";

//---- buffers
//*********************************************************************
double glbSellArray[];
double glbBuyArray[];

//CloseAll.
//*********************************************************************
extern int option = 0;
//+------------------------------------------------------------------+
// Set this prameter to the type of clsoing you want:
// 0- Close all (instant and pending orders) (Default)
// 1- Close all instant orders
// 2- Close all pending orders
// 3- Close by the magic number
// 4- Close by comment
// 5- Close orders in profit
// 6- Close orders in loss
// 7- Close not today orders
// 8- Close before day orders

//+------------------------------------------------------------------+


//+------------------------------------------------------------------+
//| ASCTrend1sig                                                     |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
int OnInit()
  {
//---
   
//---
   return(INIT_SUCCEEDED);
  }
void OnTick()
   {
      //---
      //if(TrailingStop > 0 && TrailingStart > 0) TrailOrder (TrailingStart, TrailingStop);
      Lots();
      glbCurrent = SymbolInfoDouble(Symbol(),SYMBOL_BID);
      //Comment(DoubleToString(glblTradeLots));
      glblTradeLots = .01;
      Prices();
      DataDisplay();
      Tickets();
      PlaceOrders();
      
      //Comment(StringFormat("Show ticket\nTicket = %d",glbTicket,0));
   }

//Prices from Base
//***************************************************************************************************************************************************            
void Prices()
    {
		int i,shift,counted_bars=IndicatorCounted(),Counter,i1,value10,value11,j1;
		double x1,x2,value2,value3,TrueCount,Range,AvgRange,MRO1,MRO2;
		double Table_value2[5000];		
		
		value10 = 3 + glbRisk * 2;
		x1 = 67 + glbRisk;
		x2 = 33 - glbRisk;
		value11 = value10;

		//----
		if(Bars <= 11 + 1) return(0);
		//---- initial zero

		if(counted_bars < 11 + 1){for(i = 1;i <= 0;i++) glbSellArray[glbCountBars - i] = 0.0;for(i = 1;i <= 0;i++) glbBuyArray[glbCountBars - i] = 0.0;}
		//----
		shift = glbCountBars - 11 - 1;
		while(shift >= 0)
			{
				Counter = shift;Range = 0.0;AvgRange = 0.0;
				for (Counter = shift; Counter <= shift + 9; Counter++) AvgRange = AvgRange + MathAbs(High[Counter] - Low[Counter]);Range = AvgRange / 10;Counter = shift;TrueCount = 0;
				while (Counter<shift+9 && TrueCount<1){if (MathAbs(Open[Counter] - Close[Counter + 1]) >= Range * 2.0) TrueCount = TrueCount + 1;Counter = Counter + 1;}
				if (TrueCount >= 1){MRO1 = Counter;} else {MRO1 = -1;} Counter = shift;TrueCount = 0;
				while (Counter<shift+6 && TrueCount<1){if (MathAbs(Close[Counter + 3] - Close[Counter]) >= Range * 4.6) TrueCount = TrueCount + 1;Counter = Counter + 1;}
				if (TrueCount >= 1) {MRO2 = Counter;} else {MRO2 = -1;}
				if (MRO1>-1) {value11 = 3;} else {value11 = value10;}
				if (MRO2>-1) {value11 = 4;} else {value11 = value10;}
				value2 = 100 - MathAbs(iWPR(NULL,0,value11,shift)); // PercentR(value11=9)
				Table_value2[shift] = value2;
				if (value2 < x2)
					{
						i1 = 1;
						glbBidPrice = 0.00000;
						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1){i1++;}
						if (Table_value2[shift + i1] > x1) 
						   j1=1;
							{
								glbAskPrice = High[shift] + Range * 0.5;
								//glbAskDateTime = TimeCurrent();
								if(j1 == 1 && glbAskPrice != 0)
								   {
								      glbAskHour = Hour();
								      glbAskMinute = Minute();
								      glbBidHour = 0;
                              glbBidMinute = 0;								      
								   }
                        Comment(StringFormat("Show count\nAskcount = %G\nAskHour = %G\nAskMinute = %G\nBidHour = %G\nBidMinute = %d",j1,glbAskHour,glbAskMinute,glbBidHour,glbBidMinute,0)); 
                        {j1++;}								   
							} 
					}

				if (value2 > x1)
					{
						i1=1;
						glbAskPrice = 0.00000;
						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1)
							{i1++;}
						if (Table_value2[shift + i1] < x2)
						   j1=1; 
							{
								glbBidPrice = Low[shift] + Range * 0.5;
								//glbBidDateTime = TimeCurrent();
								if (j1 == 1 && glbBidPrice != 0)
								   {
								      glbBidHour = Hour();
                              glbBidMinute = Minute();
								      glbAskHour = 0;
								      glbAskMinute = 0;                              
                           }
                        Comment(StringFormat("Show count\nBidcount = %G\nBidHour = %G\nBidMinute = %G\nAskHour = %G\nAskMinute = %d",j1,glbBidHour,glbBidMinute,glbAskHour,glbAskMinute,0)); 
                        {j1++;}
							}
					}      
			shift--;
	
			}
	
	return(0);
    }
 //Tickets
//***************************************************************************************************************************************************
void Tickets()
   {   
      int intOrders = OrdersTotal();
	   int cnt = 0;	   	   
      for (cnt = 0 ; cnt <=intOrders ; cnt++)
		   {
				OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
				glbTicket = OrderTicket();
				glbTicketDatetime = OrderOpenTime();
				glbOpenPrice = OrderOpenPrice();
				glbOrderLots = OrderLots();
            if (OrderType()==OP_BUY){glbAskBid = 1;}
            if (OrderType()==OP_SELL){glbAskBid = 2;}
			}
		}     
//PlaceOrders
//***************************************************************************************************************************************************            
void PlaceOrders()
   {  
		
      //if(glbTicket == 0){if (glbAskPrice > glbCurrent){Order_Ask();}}
      //if(glbTicket == 0){if (glbBidPrice < glbCurrent){Order_Bid();}}
      
      if(glbTicket == 0){if (glbAskPrice != 0){Order_Ask();}}
      if(glbTicket == 0){if (glbBidPrice != 0){Order_Bid();}}
      
      if(glbBidPrice != 0 && glbTicket != 0 && OrderType()==OP_SELL){CloseAll();Order_Bid();}
      if(glbAskPrice != 0 && glbTicket != 0 && OrderType()==OP_BUY){CloseAll();Order_Ask();} 
      
      //if(glbTicket != 0){if (OrderType()==OP_SELL){if(glbBidPrice != 0){CloseAll();Order_Bid();}}}
      //if(glbTicket != 0){if (glbAskBid == 1){if(glbAskPrice != 0){CloseAll();Order_Ask();}}}
       

   }
   
//Lots
//***************************************************************************************************************************************************  
 void Lots()
   { 
      double dblLotsSize = MarketInfo(Symbol(),MODE_MARGINREQUIRED);
      double dblAccountSize = AccountBalance();
      glblTradeLots = NormalizeDouble(dblAccountSize / dblLotsSize,2); 
   }
//+------------------------------------------------------------------+
int deinit()
	{
		return(0);
	}
	
	
//DataDisplay
//***************************************************************************************************************************************************
//void DataDisplay(const double dvalue2,const string BuySell)
void DataDisplay()
   {
		string text = "";
		int xAxis = (int)(ChartGetInteger(ChartID(),CHART_WIDTH_IN_PIXELS,0) - glb_right_edge_shift);
		int yAxis = glb_upper_edge_shift;

		text = "***************************************************";
		createObject(1,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);
		
		text= "Bid = "+DoubleToString(glbBidPrice ,5);
		createObject(2,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 2,text);
		
		text = "***************************************************";
		createObject(3,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 3,text);
		
		text= "Ask = "+DoubleToString(glbAskPrice ,5);
		createObject(4,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 4,text);
		
		text = "***************************************************";
		createObject(5,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 5,text);				

		text= "Ticket  = "+IntegerToString(glbTicket,9);
		createObject(6,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 6,text);

		text = "***************************************************";
		createObject(7,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 7,text);

		text= "Current = "+DoubleToString(glbCurrent ,5);
		createObject(8,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 8,text);

		text = "***************************************************";
		createObject(9,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 9,text);
				
		text= "Lot = "+DoubleToString(MarketInfo(Symbol(),MODE_MARGINREQUIRED) ,5);
		createObject(10,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 10,text);	
		
		text = "***************************************************";
		createObject(11,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 11,text);
		
		text= "TradLots = "+DoubleToString(glblTradeLots ,2);
		createObject(12,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 12,text);
						
									
   }
   
//createObject
//***************************************************************************************************************************************************
void createObject(int st_ID,ENUM_OBJECT obj,int window,int xA,int yA,string txt="")
	{
		color text_color = Red;
		int text_size = 10;
		ObjectCreate(glb_ea_name + "_" + IntegerToString(st_ID),obj,window,xA,yA);
		ObjectSet(glb_ea_name + "_" + IntegerToString(st_ID),OBJPROP_XDISTANCE,xA);
		ObjectSet(glb_ea_name + "_" + IntegerToString(st_ID),OBJPROP_YDISTANCE,yA);
		ObjectSetText(glb_ea_name + "_" + IntegerToString(st_ID),txt,8,"TimesNewRoman",clrAqua);
	} 
         
  //Buy
//***************************************************************************************************************************************************            
void Order_Bid()
	{   
		double SL = NormalizeDouble(Bid - glbBid_SL * Point,Digits); 
		double TP = 0; 
		//--- place market order to buy 1 lot 
		int Ticket = OrderSend(Symbol(),0,glblTradeLots,glbCurrent,2,SL,TP,glb_ea_name,glbMagic_Number,0,clrViolet); 
		if(Ticket < 0) 
			{ 
				Print("OrderSend failed with error #",GetLastError()); 
			} 
			else 
			{
				Print("OrderSend placed successfully");
			}
		int Orders = OrdersTotal();
	   int cnt = 0;   
      for (cnt = 0 ; cnt <= Orders ; cnt++)
		   {
				OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
				glbTicket = OrderTicket();
			}
	}

//Ask
//***************************************************************************************************************************************************             

 void Order_Ask() 
	{ 
		//--- calculated SL and TP prices must be normalized 
		double SL = NormalizeDouble(Ask + glbAsk_SL * Point,Digits); 
		double TP = 0; 
		//--- place market order to buy 1 lot 
		int Ticket = OrderSend(Symbol(),1,glblTradeLots,glbCurrent,3,SL,TP,glb_ea_name,glbMagic_Number,0,clrViolet); 
		if(Ticket < 0) 
			{ 
				Print("OrderSend failed with error #",GetLastError()); 
			} 
			else 
			{
				Print("OrderSend placed successfully");
			}
		int Orders = OrdersTotal();
	   int cnt = 0;   
      for (cnt = 0 ; cnt <= Orders ; cnt++)
		   {
				OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
				glbTicket = OrderTicket();
			}
	}
//TrailOrder
//***************************************************************************************************************************************************
void TrailOrder(double Trailingstart,double Trailingstop)
	{
		int ticket = 0;
		double tStopLoss = NormalizeDouble(OrderStopLoss(), Digits); // Stop Loss
		int cnt,vPoint,vSlippage;
		double sl     = OrderStopLoss(); // Stop Loss

		if (Digits == 3 || Digits == 5)
		   {
		      vPoint = Point * 10; 
		      vSlippage = Slippage * 10;
		   } 
		else 
		   {
		      vPoint = Point; 
		      vSlippage = Slippage;
		   }
		 //Comment(StringFormat("Show prices\nTrailingstart = %G\nTrailingstop = %G\ntStopLoss = %G\nvPoint = %G\nvSlippage = %G\nsl = %d",Trailingstart,Trailingstop,tStopLoss,vPoint,vSlippage,sl,0));  
	
		RefreshRates();
		if(OrdersTotal()>0)
			{
				for(cnt=OrdersTotal();cnt>=0;cnt--)
					{
						OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES);
						if(OrderType()<=OP_SELL && OrderSymbol()==Symbol() && OrderMagicNumber()==glbMagic_Number)
							{
								if(OrderType()==OP_BUY)
									{
										if(Ask > NormalizeDouble(OrderOpenPrice() + TrailingStart * vPoint,Digits) && tStopLoss < NormalizeDouble(Bid - (TrailingStop + TrailingStep ) * vPoint ,Digits))
											{
												tStopLoss = NormalizeDouble(Bid-TrailingStop*vPoint,Digits);
												//Comment(StringFormat("Show StopLoss\nStopLoss = %G\nOP_BUY = %d",tStopLoss,0,0));
												//ticket = OrderModify(OrderTicket(),OrderOpenPrice(),tStopLoss,OrderTakeProfit(),0,Blue);
												if (ticket > 0)
													{
														Print ("TrailingStop #2 Activated: ", OrderSymbol(), ": SL", tStopLoss, ": Bid", Bid);
														return(0);
													}
											}
									}

								if (OrderType()==OP_SELL) 
									{
										if (Bid < NormalizeDouble(OrderOpenPrice()-TrailingStart*vPoint,Digits) && (sl >(NormalizeDouble(Ask+(TrailingStop+TrailingStep)*vPoint,Digits))) || (OrderStopLoss()==0))
											{
												tStopLoss = NormalizeDouble(Ask+TrailingStop*vPoint,Digits);
												//Comment(StringFormat("Show StopLoss\nStopLoss = %G\nOP_SELL = %d",tStopLoss,0,0));
												//ticket = OrderModify(OrderTicket(),OrderOpenPrice(),tStopLoss,OrderTakeProfit(),0,Red);
												if (ticket > 0)
													{
														Print ("Trailing #2 Activated: ", OrderSymbol(), ": SL ",tStopLoss, ": Ask ", Ask);
														return(0);
													}
											}
									}
							}
					}
			}
			
	}
//https://www.cashbackforex.com/en-us/school/tabid/426/ID/435771/trailingStop-with-profit-threshold
//CloseAll
//***************************************************************************************************************************************************
int CloseAll()
	{
	   int total = OrdersTotal();
	   int cnt = 0;
	   switch (option)
		{
			case 0:
				{
					for (cnt = 0 ; cnt <=total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
							if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
						}
					break;
				}
			case 1:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
						}
					break;
				}
			case 2:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
						}
					break;
				}
			case 3:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if (OrderMagicNumber() == magic_number)
								{
									if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
									if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
								}
						}         
					break;
				}
			case 4:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if (StringFind(OrderComment(),comment_text)>-1)
								{
									if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
									if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
								}
						}         
				break;
				}      
			case 5:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(OrderProfit() > 0)
								{
									if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
									if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
								}
						}         
					break;
				}            
			case 6:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(OrderProfit() < 0)
								{
									if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
									if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
								}
						}         
					break;
				}            
			case 7:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(TimeDay(OrderOpenTime())!=TimeDay(TimeCurrent()))
								{
									if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
									if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
								}
						}         
					break;
				}   
			case 8:
				{
					for (cnt = 0 ; cnt <total ; cnt++)
						{
							OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
							if(TimeDay(OrderOpenTime())<before_day)
								{
									if(OrderType()<=OP_SELL) CloseOrder(OrderTicket(),0,Slippage,5,500);
									if(OrderType()>OP_SELL) OrderDelete(OrderTicket());
								}
						}         
					break;
				}                           
		}

	}

//CloseOrder
//***************************************************************************************************************************************************
bool CloseOrder(int ticket, double lots, int slippage, int tries, int pause)
	{
	   bool result=false;
	   double ask , bid;

	   if(OrderSelect(ticket,SELECT_BY_TICKET,MODE_TRADES))
		{
			RefreshRates();
			ask = NormalizeDouble(MarketInfo(OrderSymbol(),MODE_ASK),MarketInfo(OrderSymbol(),MODE_DIGITS));
			bid = NormalizeDouble(MarketInfo(OrderSymbol(),MODE_BID),MarketInfo(OrderSymbol(),MODE_DIGITS));
			if(OrderType()==OP_BUY)
				{
					for(int c = 0 ; c < tries ; c++)
						{
							if(lots==0) result = OrderClose(OrderTicket(),OrderLots(),bid,slippage,Violet);
							else result = OrderClose(OrderTicket(),lots,bid,slippage,Violet);
							if(result==true) break; 
							else
							{
							   Sleep(pause);
							   RefreshRates();
							   ask = NormalizeDouble(MarketInfo(OrderSymbol(),MODE_ASK),MarketInfo(OrderSymbol(),MODE_DIGITS));
							   bid = NormalizeDouble(MarketInfo(OrderSymbol(),MODE_BID),MarketInfo(OrderSymbol(),MODE_DIGITS));      
							   continue;
							}
						}
				}
			if(OrderType()==OP_SELL)
				{
					for(c = 0 ; c < tries ; c++)
						{
							if(lots==0) result = OrderClose(OrderTicket(),OrderLots(),ask,slippage,Violet);
							else result = OrderClose(OrderTicket(),lots,ask,slippage,Violet);
							if(result==true) break;						
							else
							{
								Sleep(pause);
								RefreshRates();
								ask = NormalizeDouble(MarketInfo(OrderSymbol(),MODE_ASK),MarketInfo(OrderSymbol(),MODE_DIGITS));
								bid = NormalizeDouble(MarketInfo(OrderSymbol(),MODE_BID),MarketInfo(OrderSymbol(),MODE_DIGITS));  
								continue;
							}
						}		
				}
		}
   return(result);
}
          
 //Error Handling
//***************************************************************************************************************************************************
string error(int err)
    {
        switch (err)
            {
				case 0 :return("No error returned.");
				case 1 :return("No error returned, but the result is unknown. ");
				case 2 :return("Common error. ");
				case 3 :return("Invalid trade parameters. ");
				case 4 :return("Trade server is busy. ");
				case 5 :return("Old version of the client terminal. ");
				case 6 :return("No connection with trade server. ");
				case 7 :return("Not enough rights. ");
				case 8 :return("Too frequent requests."); 
				case 9 :return("Malfunctional trade operation. ");
				case 64 :return("Account disabled. ");
				case 65 :return("Invalid account. ");
				case 128 :return("Trade timeout. ");
				case 129 :return("Invalid price. ");
				case 130 :return("Invalid stops. ");
				case 131 :return("Invalid trade volume. ");
				case 132 :return("Market is closed. ");
				case 133 :return("Trade is disabled. ");
				case 134 :return("Not enough money. ");
				case 135 :return("Price changed. ");
				case 136 :return("Off quotes. ");
				case 137 :return("Broker is busy."); 
				case 138 :return("Requote. ");
				case 139 :return("Order is locked. ");
				case 140 :return("Long positions only allowed. ");
				case 141 :return("Too many requests. ");
				case 145 :return("Modification denied because order too close to market. ");
				case 146 :return("Trade context is busy. ");
				case 147 :return("Expirations are denied by broker. ");
				case 148 :return("The amount of open and pending orders has reached the limit set by the broker. ");
				case 4000 :return("No error. ");
				case 4001 :return("Wrong function pointer. ");
				case 4002 :return("Array index is out of range. ");
				case 4003 :return("No memory for function call stack. ");
				case 4004 :return("Recursive stack overflow. ");
				case 4005 :return("Not enough stack for parameter. ");
				case 4006 :return("No memory for parameter string. ");
				case 4007 :return("No memory for temp string. ");
				case 4008 :return("Not initialized string. ");
				case 4009 :return("Not initialized string in array. ");
				case 4010 :return("No memory for array string. ");
				case 4011 :return("Too long string. ");
				case 4012 :return("Remainder from zero divide. ");
				case 4013 :return("Zero divide. ");
				case 4014 :return("Unknown command. ");
				case 4015 :return("Wrong jump (never generated error). ");
				case 4016 :return("Not initialized array. ");
				case 4017 :return("DLL calls are not allowed. ");
				case 4018 :return("Cannot load library. ");
				case 4019 :return("Cannot call function. ");
				case 4020 :return("Expert function calls are not allowed. ");
				case 4021 :return("Not enough memory for temp string returned from function. ");
				case 4022 :return("System is busy (never generated error). ");
				case 4050 :return("Invalid function parameters count. ");
				case 4051 :return("Invalid function parameter value. ");
				case 4052 :return("String function internal error. ");
				case 4053 :return("Some array error. ");
				case 4054 :return("Incorrect series array using. ");
				case 4055 :return("Custom indicator error. ");
				case 4056 :return("Arrays are incompatible. ");
				case 4057 :return("Global variables processing error. ");
				case 4058 :return("Global variable not found. ");
				case 4059 :return("Function is not allowed in testing mode. ");
				case 4060 :return("Function is not confirmed. ");
				case 4061 :return("Send mail error. ");
				case 4062 :return("String parameter expected. ");
				case 4063 :return("Integer parameter expected. ");
				case 4064 :return("Double parameter expected. ");
				case 4065 :return("Array as parameter expected. ");
				case 4066 :return("Requested history data in updating state. ");
				case 4067 :return("Some error in trading function. ");
				case 4099 :return("End of file. ");
				case 4100 :return("Some file error."); 
				case 4101 :return("Wrong file name. ");
				case 4102 :return("Too many opened files. ");
				case 4103 :return("Cannot open file. ");
				case 4104 :return("Incompatible access to a file. ");
				case 4105 :return("No order selected. ");
				case 4106 :return("Unknown symbol. ");
				case 4107 :return("Invalid price. ");
				case 4108 :return("Invalid ticket. ");
				case 4109 :return("Trade is not allowed. ");
				case 4110 :return("Longs are not allowed. ");
				case 4111 :return("Shorts are not allowed. ");
				case 4200 :return("Object exists already. ");
				case 4201 :return("Unknown object property. ");
				case 4202 :return("Object does not exist. ");
				case 4203 :return("Unknown object type. ");
				case 4204 :return("No object name. ");
				case 4205 :return("Object coordinates error. ");
				case 4206 :return("No specified subwindow. ");
				case 4207 :return("Some error in object function. ");
				default: return("Unknown error:"+err);

			}
	}

 
 