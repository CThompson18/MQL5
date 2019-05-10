//+------------------------------------------------------------------+
//|                                                      Dan_EA1.mq4 |
//|                                                      4X Edge LLC |
//|                                                       4xedge.com |
//+------------------------------------------------------------------+
//Global Multi-function variables
//*********************************************************************
extern int glbMagic_Number = 1234;
extern int glbTimeFrame=0;
extern int glbCount_Bars=1000;
extern double glbLots=0.1;
extern int glbSlippage=10;
extern int glbTrade_Bar=0;
extern string glbStartDate="2016.01.01 00:00";
extern int glbSignal_Risk=15;
extern int glbATR_Period = 250;
extern double glbATR_Multiplier = 5.2;
extern double glbStop_ATR_Multiplier = 3.0;
extern int glbPullback_Pips = 20;
extern int glbPullback_Timeout = 25;
extern int glbUsed_Bar=1;
int glbATR_Timeframe = 0;

//Global variables
//*********************************************************************
string Indicator_Name = "New_System2";
datetime last_t = 0;
string com = "Dan_EA_dantrend";
bool up = false;
bool dn = false;
double last_ask = 0;
double last_bid = 0;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
	{
		return(0);
	}
//+------------------------------------------------------------------+
//| Custom indicator De-initialization function                      |
//+------------------------------------------------------------------+
int deinit()
	{
		return(0);
	}
//+------------------------------------------------------------------+
//| Start function                                                   |
//+------------------------------------------------------------------+	
int start()
	{
		int i = 0;
		double ask, bid;
		double SL = 0,TP = 0;
		double dantrend0 = iCustom(Symbol(),0,"dans_trend_history5",glbTimeFrame,glbStartDate,glbSignal_Risk,glbATR_Period,glbATR_Multiplier,glbStop_ATR_Multiplier,glbATR_Timeframe,glbPullback_Pips,glbPullback_Timeout,glbUsed_Bar,0,glbTrade_Bar);
		double dantrend1 = iCustom(Symbol(),0,"dans_trend_history5",glbTimeFrame,glbStartDate,glbSignal_Risk,glbATR_Period,glbATR_Multiplier,glbStop_ATR_Multiplier,glbATR_Timeframe,glbPullback_Pips,glbPullback_Timeout,glbUsed_Bar,1,glbTrade_Bar);
		double dantrend4 = iCustom(Symbol(),0,"dans_trend_history5",glbTimeFrame,glbStartDate,glbSignal_Risk,glbATR_Period,glbATR_Multiplier,glbStop_ATR_Multiplier,glbATR_Timeframe,glbPullback_Pips,glbPullback_Timeout,glbUsed_Bar,4,glbTrade_Bar);
		double dantrend5 = iCustom(Symbol(),0,"dans_trend_history5",glbTimeFrame,glbStartDate,glbSignal_Risk,glbATR_Period,glbATR_Multiplier,glbStop_ATR_Multiplier,glbATR_Timeframe,glbPullback_Pips,glbPullback_Timeout,glbUsed_Bar,5,glbTrade_Bar);
		double patr=iATR(Symbol(),ATR_Timeframe,glbATR_Period,glbTrade_Bar);
		
		if (dantrend0!=EMPTY_VALUE)
			{
				Print ("BUY CONDITIONS");
				up = true;
				dn = false;
			}
		if (dantrend1 != EMPTY_VALUE)
			{
				Print ("SELL CONDITIONS");
				up = false;
				dn = true;
			}
		Print(dantrend0 + " " + dantrend1 + " " +dantrend4 + " " +dantrend5);

		int mult = 1;
		if (((StringFind(Symbol(),"JPY",0) != - 1 || StringFind(Symbol(),"jpy",0) != - 1) && Point < 0.01) ||
			((StringFind(Symbol(),"JPY",0) == - 1 || StringFind(Symbol(),"jpy",0) == - 1) && Point < 0.0001))
		mult = 10;

		if (TotalOrders(glbMagic_Number) == 0 && Time[0] > last_t)
			{
				//if (up && ask<=dantrend4 && last_ask > dantrend4 && last_ask != 0 && dantrend4 != EMPTY_VALUE){
				if (up && Low[glbTrade_Bar] <= dantrend4 && High[glbTrade_Bar] >= dantrend4 && last_ask != 0 && dantrend4 != EMPTY_VALUE)
					{
						RefreshRates();
						ask = MarketInfo(Symbol(),MODE_ASK);
						SL = NormalizeDouble(ask - (patr * Stop_glbATR_Multiplier),Digits);
						TP = NormalizeDouble(ask + (patr * glbATR_Multiplier),Digits);
						if (OrderSend(Symbol(),OP_BUY,glbLots,NormalizeDouble(ask,Digits),glbSlippage,SL,TP,com+Period(),glbMagic_Number,0,Blue)==-1) 
							{
								i=GetLastError();
								if (i!=1 && i!=0) Print("Buy Error :"+error(i)+" at "+Symbol()+" "+Period());                 
							}
							else
							{
								last_t=Time[0];
								return(0);
							}
					}
				//if (dn && bid>=dantrend5 && last_bid < dantrend5 && last_bid != 0 && dantrend5 != EMPTY_VALUE){
				if (dn && Low[glbTrade_Bar] <= dantrend5 && High[glbTrade_Bar] <= dantrend5 && last_bid != 0 && dantrend5 != EMPTY_VALUE)
					{
						RefreshRates();
						bid = MarketInfo(Symbol(),MODE_BID);
						SL = NormalizeDouble(bid + (patr * Stop_glbATR_Multiplier),Digits);
						TP = NormalizeDouble(bid - (patr * glbATR_Multiplier),Digits);
						if (OrderSend(Symbol(),OP_SELL,glbLots,NormalizeDouble(bid,Digits),glbSlippage,SL,TP,com+Period(),glbMagic_Number,0,Red)==-1) 
							{
								i=GetLastError();
								if (i!=1 && i!=0) Print("Sell Error :"+error(i)+" at "+Symbol()+" "+Period());                 
							}
							else
							{
								last_t=Time[0];
								return(0);
							}
					}         
			}
   
			last_ask = ask;
			last_bid = bid;
   
			if (TotalOrders(glbMagic_Number)>0)
				{
					for (int t = 0;t < OrdersTotal();t++)
						{
							OrderSelect(t,SELECT_BY_POS);
							if (OrderType() == glbMagic_Number)
								{
									break;
								}
						}
					if (OrderType() == OP_BUY && dn)
						{
							OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Digits),glbSlippage,Pink);
						}
					if (OrderType() == OP_SELL && up)
						{
							OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Digits),glbSlippage,Pink);
						}
				}
   
		return(0);   
	}

	int TotalOrders(int magic)
		{
			int rez = 0;
			for (int i = 0;i < OrdersTotal();i++)
				{
					OrderSelect(i,SELECT_BY_POS);
					if (OrderMagicNumber() == magic)
						{
							rez++;
						}
				}   
			return(rez);
		} 

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



