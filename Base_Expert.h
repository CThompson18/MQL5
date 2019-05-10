//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
//Multi-function variables
//*********************************************************************
//---- input parameters
extern int glbMagic_Number = 1234;
extern double glbRisk = 3.025;
extern int glbATR_Period = 250;
extern string glbStartDate = "2017.03.31 00:00";
double glbCurrent = 0;
double glbBid = 0.00;
double glbAsk = 0.00;
string glbSpread = 0.00;
input long my_magic = 555;

//BuySell orders.
//*********************************************************************
extern double glbBid_SL = 150.00;
extern double glbBid_TP = 300.00;
extern double glbAsk_SL = 150.00;
extern double glbAsk_TP = 300.00;
extern double glbPercentageOfFreeMargin = 0.25;
string glb_ea_name = "ChucknJill Financials";

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
extern int magic_number = 0; // set it if you'll use closing option 3 - closing by magic number
extern string comment_text = ""; // set it if you'll use closing option 4 - closing by comment
extern int before_day = 0; // set it if you'll use closing option 8 - closing by before day
extern int Slippage = 5; //Slippage


//start
//***************************************************************************************************************************************************            
int start()
	{
		//Prices();
		return(0);
   }	
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
   {
      //---
      Prices();
      
   }
//+------------------------------------------------------------------+	
//deinit
//***************************************************************************************************************************************************            
int deinit()
  {
   return(0);
  }
//Prices from Base
//***************************************************************************************************************************************************            
void Prices()
   {
		int CountBars = iBarShift(Symbol(),0,StrToTime(glbStartDate),false);				
		for (int kk = CountBars - 1;kk >= 0; kk--)
         {
            double atr = iATR(Symbol(),0,glbATR_Period,kk);
            double dblBid = iCustom(Symbol(),0,"Base",glbRisk,CountBars,1,kk+1);
            double dblAsk = iCustom(Symbol(),0,"Base",glbRisk,CountBars,0,kk+1);
            //double dblAsk = iCustom(Symbol(),0,"Base",glbRisk,CountBars,1,kk+1);
            //double dblBid = iCustom(Symbol(),0,"Base",glbRisk,CountBars,0,kk+1);            
            if(dblBid > 0){glbBid = dblBid;}
            if(dblAsk > 0){glbAsk = dblAsk;}
            glbCurrent = SymbolInfoDouble(Symbol(),SYMBOL_BID);
            double OrderPrice = OrderOpenPrice();
	         int intOrderType = OrderType();
            static double spread;
            spread = MarketInfo(Symbol(), MODE_SPREAD);
            glbSpread = "Spread: "+DoubleToStr(spread, 0)+" points";
            long Orders = OrdersTotal();
            Comment(StringFormat("Show prices\nOrders = %G\nCurrent = %G\nBid = %G\nAsk = %G\nATR = %G\nCountBars = %G\nkk = %G\nB = %d",Orders,glbCurrent,glbBid,glbAsk,atr,CountBars,kk,0));
            if(dblAsk > glbCurrent)
               {
                  Comment(StringFormat("Show prices\nAsk = %G\nCurrent = %G\nOrderPrice = %G\nDifference = %G\nSpread = %G\nOrders = %G\nB = %d",glbAsk,glbCurrent,OrderPrice,glbAsk - glbCurrent,glbSpread,Orders,0));
                  if(Orders == 1 && intOrderType == 0)
                     {
                        Comment(StringFormat("Show prices\nOrders = %G\nOrderType = %G\nAsk = %d",Orders,intOrderType,0));
	                     //CloseAll();
	                     //Order_Ask();
                     }
                  if(Orders == 0)
                     {
                        Comment("Ask");
                        //Order_Ask();
                     }                     
               }
            if(glbCurrent > glbBid)
               {
                  Comment(StringFormat("Show prices\nBid = %G\nCurrent = %G\nOrderPrice = %G\nDifference = %G\nSpread = %G\nOrderType = %G\nOrders = %G\nB = %d",glbBid,glbCurrent,OrderPrice,(glbCurrent - glbBid) * 10000,glbSpread,intOrderType,Orders,0));
                  if(Orders == 1 && intOrderType == 1)
	                  { 
                        Comment(StringFormat("Show prices\nOrders = %G\nOrderType = %G\nBid = %d",Orders,intOrderType,0));
	                     CloseAll();
	                     //Order_Bid();
                     }
                  if(Orders == 0)
                     {
                        Comment(StringFormat("Show prices\nOrders = %G\nOrderType = %G\nBid = %G\nCurrent = %G\nB = %d",Orders,intOrderType,glbBid,glbCurrent,0));
                        Order_Bid();
                     }                                             
               }
               
         }
   }
   
//Buy
//*************************************************************************************************************************************************** 
           
void Order_Bid()
	{ 
		double One_Lot = MarketInfo(Symbol(),MODE_MARGINREQUIRED);//Cost per 1 lot
		double Buy_Lot = (AccountBalance()/One_Lot) * glbPercentageOfFreeMargin;
		double Buy_price = MarketInfo(Symbol(),MODE_BID); // Request for the value of Ask 
		double Ask_Price = MarketInfo(Symbol(),MODE_ASK); // Request for the value of Ask 
		//--- calculated SL and TP prices must be normalized  
		double SL = NormalizeDouble(Bid - glbBid_SL * Point,Digits); 
		double TP = NormalizeDouble(Bid + glbBid_TP * Point,Digits); 
		//--- place market order to buy 1 lot 
		Comment(StringFormat("Show Order values\nOneLot = %G\nBuyLot = %G\nBidPrice = %G\nAskPrice = %G\nStopLoss = %G\nTakeProfit = %G\nAdd = %d",One_Lot,Buy_Lot,Buy_price,Ask_Price,SL,TP,0));
		int ticket = OrderSend(Symbol(),OP_BUY,1,Buy_price,3,SL,TP,glb_ea_name,glbMagic_Number,0,clrViolet); 
		if(ticket < 0) 
			{ 
				Print("OrderSend failed with error #",GetLastError()); 
			} 
			else 
			{
				Print("OrderSend placed successfully");
			}
	}

//Ask
//***************************************************************************************************************************************************             

 void Order_Ask()
	{ 
		double One_Lot = MarketInfo(Symbol(),MODE_MARGINREQUIRED);//Cost per 1 lot
		double Ask_Lot = (AccountBalance()/One_Lot) * glbPercentageOfFreeMargin;
		double Ask_Price = MarketInfo(Symbol(),MODE_ASK); // Request for the value of Ask 
		double Buy_price = MarketInfo(Symbol(),MODE_BID); // Request for the value of Ask 
		//--- calculated SL and TP prices must be normalized 
		double SL = NormalizeDouble(Ask + glbAsk_SL * Point,Digits); 
		double TP = NormalizeDouble(Ask - glbAsk_SL * Point,Digits); 
		//--- place market order to buy 1 lot
		Comment(StringFormat("Show Order values\nOneLot = %G\nAskLot = %G\nAskPrice = %G\nBidPrice = %G\nStopLoss = %G\nTakeProfit = %G\nAdd = %d",One_Lot,Ask_Lot,Ask_Price,Buy_price,SL,TP,0)); 
		int ticket = OrderSend(Symbol(),OP_SELL,1,Ask_Price,3,SL,TP,glb_ea_name,glbMagic_Number,0,clrViolet); 
		if(ticket < 0) 
			{ 
				Print("OrderSend failed with error #",GetLastError()); 
			} 
			else 
			{
				Print("OrderSend placed successfully");
			}
	}
	
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
