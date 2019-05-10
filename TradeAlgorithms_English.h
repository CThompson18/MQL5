//+------------------------------------------------------------------+
//|                                              TradeAlgorithms.mqh |
//|                               Copyright © 2013, Nikolay Kositsin |
//|                              Khabarovsk,   farria@mail.redcom.ru | 
//+------------------------------------------------------------------+ 
//| Trading algorithms for brokers offering non-zero spread!   |
//+------------------------------------------------------------------+ 
#property copyright "2013, Nikolay Kositsin"
#property link      "farria@mail.redcom.ru"
#property version   "1.23"
//+------------------------------------------------------------------+
//|  Enumeration for options for calculating the lot                        |
//+------------------------------------------------------------------+
enum MarginMode //Constant type for Margin_Mode variable of trading functions
  {
   FREEMARGIN=0,     //MM of free funds on the account
   BALANCE,          //MM from the balance of funds on the account
   LOSSFREEMARGIN,   //MM on losses from free funds on the account
   LOSSBALANCE,      //MM on losses from the balance of funds on the account
   LOT               //Lot unchanged
  };
//+------------------------------------------------------------------+
//|  The algorithm for determining the moment when a new bar appears             |
//+------------------------------------------------------------------+  
class CIsNewBar
  {
   //----
public:
   //---- The function of determining the moment when a new bar appears
   bool IsNewBar(string symbol,ENUM_TIMEFRAMES timeframe)
     {
      //---- we will get the time of appearance of the current bar
      datetime TNew=datetime(SeriesInfoInteger(symbol,timeframe,SERIES_LASTBAR_DATE));

      if(TNew!=m_TOld && TNew) // check for a new bar
        {
         m_TOld=TNew;
         return(true); // there was a new bar!
        }
      //----
      return(false); //No new bars yet!
     };

   //---- class constructor    
                     CIsNewBar(){m_TOld=-1;};

protected: datetime m_TOld;
   //---- 
  };
//+==================================================================+
//| Algorithms for trading operations                                  |
//+==================================================================+

//+------------------------------------------------------------------+
//| Open a long position                                       |
//+------------------------------------------------------------------+
bool BuyPositionOpen
(
bool &BUY_Signal,           // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
int Margin_Mode,            // method of calculating the value of a lot
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit              // take-profit in points
)
//BuyPositionOpen(BUY_Signal,symbol,TimeLevel,Money_Management,deviation,Margin_Mode,StopLoss,Takeprofit);
  {
//----
   if(!BUY_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(PositionSelect(symbol)) return(true);

//----
   double volume=BuyLotCount(symbol,Money_Management,Margin_Mode,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(): Invalid volume for trade request structure");
      return(false);
     }

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Ask;
//----   
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price-dStopLoss,int(digit));
     }
   else request.sl=0.0;

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price+dTakeprofit,int(digit));
     }
   else request.tp=0.0;

//----
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Open Buy position on",symbol," ============ >>>");
   Print(comment);

//---- Open the BUY position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      BUY_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under",symbol," open ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Open a short position                                      |
//+------------------------------------------------------------------+
bool SellPositionOpen
(
bool &SELL_Signal,          // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
int Margin_Mode,            // method of calculating the value of a lot
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit              // take-profit in points
)
//SellPositionOpen(SELL_Signal,symbol,TimeLevel,Money_Management,deviation,Margin_Mode,StopLoss,Takeprofit);
  {
//----
   if(!SELL_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(PositionSelect(symbol)) return(true);

//----
   double volume=SellLotCount(symbol,Money_Management,Margin_Mode,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(): Invalid volume for trade request structure");
      return(false);
     }

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Bid;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(true);

//---- Initializing the MqlTradeRequest Trade Request Structure to Open the SELL Position
   request.type   = ORDER_TYPE_SELL;
   request.price  = Bid;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss!=0)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price+dStopLoss,int(digit));
     }
   else request.sl=0.0;

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit!=0)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price-dTakeprofit,int(digit));
     }
   else request.tp=0.0;
//----
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Open the Sell position by",symbol," ============ >>>");
   Print(comment);

//---- Open the SELL position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      SELL_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position on ",symbol," открыта ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Open a long position                                        |
//+------------------------------------------------------------------+
bool BuyPositionOpen
(
bool &BUY_Signal,           // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
int Margin_Mode,            // method of calculating the value of a lot
uint deviation,             // slipping
double dStopLoss,           // stoploss in units of price chart
double dTakeprofit          // take-profit in units of price schedule
)
//BuyPositionOpen(BUY_Signal,symbol,TimeLevel,Money_Management,deviation,Margin_Mode,StopLoss,Takeprofit);
  {
//----
   if(!BUY_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(PositionSelect(symbol)) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Ask;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(true);

//---- Correction of distances to stop-loss and take-profit in units of the price schedule
   if(!dStopCorrect(symbol,dStopLoss,dTakeprofit,PosType)) return(false);
   int StopLoss=int((Ask-dStopLoss)/point);
//----
   double volume=BuyLotCount(symbol,Money_Management,Margin_Mode,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(): Invalid volume for trade request structure");
      return(false);
     }

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;
   request.sl=dStopLoss;
   request.tp=dTakeprofit;
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Open Buy position on",symbol," ============ >>>");
   Print(comment);

//---- Open the BUY position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      BUY_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under",symbol," open ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Open a short position                                      |
//+------------------------------------------------------------------+
bool SellPositionOpen
(
bool &SELL_Signal,          // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
int Margin_Mode,            // method of calculating the value of a lot
uint deviation,             // slipping
double dStopLoss,           // stoploss in units of price chart
double dTakeprofit          // take-profit in units of price schedule
)
//SellPositionOpen(SELL_Signal,symbol,TimeLevel,Money_Management,deviation,Margin_Mode,StopLoss,Takeprofit);
  {
//----
   if(!SELL_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(PositionSelect(symbol)) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Bid;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(true);

//---- Correction of distances to stop-loss and take-profit in units of the price schedule
   if(!dStopCorrect(symbol,dStopLoss,dTakeprofit,PosType)) return(false);
   int StopLoss=int((dStopLoss-Bid)/point);
//----
   double volume=SellLotCount(symbol,Money_Management,Margin_Mode,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(): Invalid volume for trade request structure");
      return(false);
     }

//---- Initializing the MqlTradeRequest Trade Request Structure to Open the SELL Position
   request.type   = ORDER_TYPE_SELL;
   request.price  = Bid;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): OrderCheck(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Open the Sell position by",symbol," ============ >>>");
   Print(comment);

//---- Open the SELL position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): OrderSend(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      SELL_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position on ",symbol," open ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): OrderSend(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Close the long position                                     |
//+------------------------------------------------------------------+
bool BuyPositionClose
(
bool &Signal,         // permission flag for a deal
const string symbol,  // trading pair of a deal
uint deviation        // slipping
)
  {
//----
   if(!Signal) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

//---- Check for an open BUY position
   if(PositionSelect(symbol))
     {
      if(PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_BUY) return(false);
     }
   else return(false);

   double MaxLot,volume,Bid;
//---- obtaining data for calculation   
   if(!PositionGetDouble(POSITION_VOLUME,volume)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MAX,MaxLot)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(true);

//---- check the lot for the maximum allowable value      
   if(volume>MaxLot) volume=MaxLot;

//---- Initializing the MqlTradeRequest trade request structure to close the BUY position
   request.type   = ORDER_TYPE_SELL;
   request.price  = Bid;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;
   request.sl = 0.0;
   request.tp = 0.0;
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }
//----     
   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Close Buy position on ",symbol," ============ >>>");
   Print(comment);

//---- Sending an order to close a position on the trading server
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): You can not close the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under",symbol," closed ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): You can not close the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Close the short position                                   |
//+------------------------------------------------------------------+
bool SellPositionClose
(
bool &Signal,         // permission flag for a deal
const string symbol,  // trading pair of a deal
uint deviation        // slipping
)
  {
//----
   if(!Signal) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

//---- Check for open SELL position
   if(PositionSelect(symbol))
     {
      if(PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_SELL)return(false);
     }
   else return(false);

   double MaxLot,volume,Ask;
//---- obtaining data for calculation   
   if(!PositionGetDouble(POSITION_VOLUME,volume)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MAX,MaxLot)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(true);

//---- check the lot for the maximum allowable value      
   if(volume>MaxLot) volume=MaxLot;

//---- Initializing the MqlTradeRequest trade request structure to close the SELL item
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;
   request.sl = 0.0;
   request.tp = 0.0;
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }
//----    
   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Close Sell position by ",symbol," ============ >>>");
   Print(comment);

//---- Sending an order to close a position on the trading server
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): You can not close the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Signal=false;
     }
   else
     {
      Print(__FUNCTION__,"(): You can not close the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position on ",symbol," closed ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| We add a long position                                       |
//+------------------------------------------------------------------+
bool ReBuyPositionOpen
(
bool &BUY_Signal,           // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
double MaxPosVolume,        // maximum open position volume
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit              // take-profit in points
)
//ReBuyPositionOpen(BUY_Signal,symbol,TimeLevel,Money_Management,Margin_Mode,MaxPosVolume,deviation,Margin_Mode,StopLoss);
  {
//----
   if(!BUY_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(!PositionSelect(symbol) || PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_BUY) return(true);
//----
   double volume=BuyLotCount(symbol,Money_Management,LOT,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(): Invalid volume for trade request structure");
      Print(__FUNCTION__,"(): Received volume = ",volume);
      return(false);
     }
   double maxvolume=BuyLotCount(symbol,MaxPosVolume,LOT,StopLoss,deviation);
   if(PositionGetDouble(POSITION_VOLUME)+volume>MaxPosVolume)
     {
      return(true);
     }
//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Ask;
//----   
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price-dStopLoss,int(digit));
     }
   else request.sl=0.0;

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price+dTakeprofit,int(digit));
     }
   else request.tp=0.0;
//----
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): We add Buy position on ",symbol," ============ >>>");
   Print(comment);

//----Add the BUY position and check the result of the trade request
   if(!OrderSend(request,result) || (result.retcode!=TRADE_RETCODE_DONE && result.retcode!=TRADE_RETCODE_PLACED))
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE || result.retcode==TRADE_RETCODE_PLACED)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      BUY_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under",symbol," the volume of ",DoubleToString(request.volume,4)," ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
      Sleep(5000);
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| We add a short position                                |
//+------------------------------------------------------------------+
bool ReSellPositionOpen
(
bool &SELL_Signal,          // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
double MaxPosVolume,        // maximum open position volume
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit              // take-profit in points
)
//ReSellPositionOpen(SELL_Signal,symbol,TimeLevel,Money_Management,Margin_Mode,MaxPosVolume,deviation,Margin_Mode,StopLoss);
  {
//----
   if(!SELL_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(!PositionSelect(symbol) || PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_SELL) return(true);

//----
   double volume=SellLotCount(symbol,Money_Management,LOT,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(): Invalid volume for trade request structure");
      Print(__FUNCTION__,"(): Received volume = ",volume);
      return(false);
     }
//---- Check for volume constraint cumulative position  
   double maxvolume=SellLotCount(symbol,MaxPosVolume,LOT,StopLoss,deviation);
   if(PositionGetDouble(POSITION_VOLUME)+volume>MaxPosVolume)
     {
      return(true);
     }

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Bid;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(true);

//---- Initializing the MqlTradeRequest Trade Request Structure to Open the SELL Position
   request.type   = ORDER_TYPE_SELL;
   request.price  = Bid;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss!=0)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price+dStopLoss,int(digit));
     }
   else request.sl=0.0;

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit!=0)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price-dTakeprofit,int(digit));
     }
   else request.tp=0.0;
//----
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"():We increase the Sell position by ",symbol," ============ >>>");
   Print(comment);

//---- Open the SELL position and check the result of the trade request
   if(!OrderSend(request,result) || (result.retcode!=TRADE_RETCODE_DONE && result.retcode!=TRADE_RETCODE_PLACED))
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE || result.retcode==TRADE_RETCODE_PLACED)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      SELL_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position on ",symbol," the volume of ",DoubleToString(request.volume,4)," ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
      Sleep(5000);
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| We add a long position                                        |
//+------------------------------------------------------------------+
bool ReBuyPositionOpen_X
(
bool &BUY_Signal,           // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
int Margin_Mode,            // method of calculating the value of a lot
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit,             // take-profit in points
string PosComment           // comment on position
)
//ReBuyPositionOpen_X(BUY_Signal,symbol,TimeLevel,Money_Management,Margin_Mode,deviation,StopLoss,Takeprofit,PosComment);
  {
//----
   if(!BUY_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(!PositionSelect(symbol) || PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_BUY) return(true);
     
//---- Calculate the amount of position for refilling
   double volume=BuyLotCount(symbol,Money_Management,LOT,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(1): Invalid volume for trade request structure");
      Print(__FUNCTION__,"(1): Received volume = ",volume);
      Print(__FUNCTION__,"(1): The initial value of the variable Money_Management = ",Money_Management);
      return(false);
     }
   double LastVolume=PositionGetDouble(POSITION_VOLUME);
   double LastOpenPrice=PositionGetDouble(POSITION_PRICE_OPEN);
   double ResaltVolume=BuyLotCount(symbol,LastVolume+volume,4,0,deviation); 
   if(ResaltVolume<=LastVolume)
     {
      Print(__FUNCTION__,"(2): Invalid volume for trade request structure");
      Print(__FUNCTION__,"(2): Received volume = ",ResaltVolume);
      return(false);
     }
   volume=ResaltVolume-LastVolume;
//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Ask;
//----   
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;
   request.comment = PosComment+"/"+DoubleToString(volume,3);

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price-dStopLoss,int(digit));
     }
   else request.sl=0.0;

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price+dTakeprofit,int(digit));
     }
   else request.tp=0.0;
//----
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): We add Buy position on ",symbol," ============ >>>");
   Print(comment);

//---- Add the BUY position and check the result of the trade request
   if(!OrderSend(request,result) || (result.retcode!=TRADE_RETCODE_DONE && result.retcode!=TRADE_RETCODE_PLACED))
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE || result.retcode==TRADE_RETCODE_PLACED)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      BUY_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under",symbol," the volume of ",DoubleToString(request.volume,4)," ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
      Sleep(5000);
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| We add a short position                                |
//+------------------------------------------------------------------+
bool ReSellPositionOpen_X
(
bool &SELL_Signal,          // permission flag for a deal
const string symbol,        // trading pair of a deal
const datetime &TimeLevel,  // the time after which the next transaction will be made after the current
double Money_Management,    // MM
int Margin_Mode,            // method of calculating the value of a lot
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit,             // take-profit in points
string PosComment           // comment on position
)
//ReSellPositionOpen_X(SELL_Signal,symbol,TimeLevel,Money_Management,Margin_Mode,deviation,StopLoss,Takeprofit,PosComment);
  {
//----
   if(!SELL_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;
//---- Check for the expiration of the time limit for the previous transaction and the completeness of the volume
   if(!TradeTimeLevelCheck(symbol,PosType,TimeLevel)) return(true);

//---- Checking for an open position
   if(!PositionSelect(symbol) || PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_SELL) return(true);
//---- Calculate the amount of position for refilling
   double volume=BuyLotCount(symbol,Money_Management,LOT,StopLoss,deviation);
   if(volume<=0)
     {
      Print(__FUNCTION__,"(1): Invalid volume for trade request structure");
      Print(__FUNCTION__,"(1): Received volume = ",volume);
      Print(__FUNCTION__,"(1): The initial value of the variable Money_Management = ",Money_Management);
      return(false);
     }
   double LastVolume=PositionGetDouble(POSITION_VOLUME);
   double LastOpenPrice=PositionGetDouble(POSITION_PRICE_OPEN);
   double ResaltVolume=BuyLotCount(symbol,LastVolume+volume,4,0,deviation); 
   if(ResaltVolume<=LastVolume)
     {
      Print(__FUNCTION__,"(2): Invalid volume for trade request structure");
      Print(__FUNCTION__,"(2): Received volume = ",ResaltVolume);
      return(false);
     }
   volume=ResaltVolume-LastVolume;

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;
//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Bid;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(true);

//---- Initializing the MqlTradeRequest Trade Request Structure to Open the SELL Position
   request.type   = ORDER_TYPE_SELL;
   request.price  = Bid;
   request.action = TRADE_ACTION_DEAL;
   request.symbol = symbol;
   request.volume = volume;
   request.comment = PosComment+"/"+DoubleToString(volume,3);

//----Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss!=0)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price+dStopLoss,int(digit));
     }
   else request.sl=0.0;

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit!=0)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price-dTakeprofit,int(digit));
     }
   else request.tp=0.0;
//----
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"():We increase the Sell position by ",symbol," ============ >>>");
   Print(comment);

//---- Open the SELL position and check the result of the trade request
   if(!OrderSend(request,result) || (result.retcode!=TRADE_RETCODE_DONE && result.retcode!=TRADE_RETCODE_PLACED))
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE || result.retcode==TRADE_RETCODE_PLACED)
     {
      TradeTimeLevelSet(symbol,PosType,TimeLevel);
      SELL_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position on ",symbol," the volume of ",DoubleToString(request.volume,4)," ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
      Sleep(5000);
     }
   else
     {
      Print(__FUNCTION__,"(): It's impossible to make a deal!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      Sleep(5000);
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Modify the long position                                 |
//+------------------------------------------------------------------+
bool BuyPositionModify
(
bool &Modify_Signal,        // enable modification flag
const string symbol,        // trading pair of a deal
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit              // take-profit in points
)
//BuyPositionModify(Modify_Signal,symbol,deviation,StopLoss,Takeprofit);
  {
//----
   if(!Modify_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;

//---- Checking for an open position
   if(!PositionSelect(symbol)) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;

//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);

   long digit;
   double point,Ask;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_SLTP;
   request.symbol = symbol;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price-dStopLoss,int(digit));
      if(request.sl<PositionGetDouble(POSITION_SL)) request.sl=PositionGetDouble(POSITION_SL);
     }
   else request.sl=PositionGetDouble(POSITION_SL);

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price+dTakeprofit,int(digit));
      if(request.tp<PositionGetDouble(POSITION_TP)) request.tp=PositionGetDouble(POSITION_TP);
     }
   else request.tp=PositionGetDouble(POSITION_TP);

//----   
   if(request.tp==PositionGetDouble(POSITION_TP) && request.sl==PositionGetDouble(POSITION_SL)) return(true);
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Modify the Buy position by ",symbol," ============ >>>");
   Print(comment);

//---- Modify the BUY position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Modify_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under",symbol," modified ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Modify the short position                                    |
//+------------------------------------------------------------------+
bool SellPositionModify
(
bool &Modify_Signal,        // enable modification flag
const string symbol,        // trading pair of a deal
uint deviation,             // slipping
int StopLoss,               // stop-loss in points
int Takeprofit              // take-profit in points
)
//SellPositionModify(Modify_Signal,symbol,deviation,StopLoss,Takeprofit);
  {
//----
   if(!Modify_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;

//---- Checking for an open position
   if(!PositionSelect(symbol)) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;

//----Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);
//----
   long digit;
   double point,Bid;
//----
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(true);
   if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_SELL;
   request.price  = Bid;
   request.action = TRADE_ACTION_SLTP;
   request.symbol = symbol;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss!=0)
     {
      if(!StopCorrect(symbol,StopLoss))return(false);
      double dStopLoss=StopLoss*point;
      request.sl=NormalizeDouble(request.price+dStopLoss,int(digit));
      double laststop=PositionGetDouble(POSITION_SL);
      if(request.sl>laststop && laststop) request.sl=PositionGetDouble(POSITION_SL);
     }
   else request.sl=PositionGetDouble(POSITION_SL);

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit!=0)
     {
      if(!StopCorrect(symbol,Takeprofit))return(false);
      double dTakeprofit=Takeprofit*point;
      request.tp=NormalizeDouble(request.price-dTakeprofit,int(digit));
      double lasttake=PositionGetDouble(POSITION_TP);
      if(request.tp>lasttake && lasttake) request.tp=PositionGetDouble(POSITION_TP);
     }
   else request.tp=PositionGetDouble(POSITION_TP);

//----   
   if(request.tp==PositionGetDouble(POSITION_TP) && request.sl==PositionGetDouble(POSITION_SL)) return(true);
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Modify Sell position on ",symbol," ============ >>>");
   Print(comment);

//---- Modify the SELL position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Modify_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position under ",symbol," modified ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Modify the long position                                 |
//+------------------------------------------------------------------+
bool dBuyPositionModify
(
bool &Modify_Signal,        // enable modification flag
const string symbol,        // trading pair of a deal
uint deviation,             // slipping
double StopLoss,            // stoploss in the absolute value of the price chart
double Takeprofit           // TakeProfit in the absolute value of the price chart
)
//dBuyPositionModify(Modify_Signal,symbol,deviation,StopLoss,Takeprofit);
  {
//----
   if(!Modify_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;

//---- Checking for an open position
   if(!PositionSelect(symbol)) return(true);
   if(PositionGetInteger(POSITION_TYPE)!=PosType) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;

//---- Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);
//----
   int digit=int(SymbolInfoInteger(symbol,SYMBOL_DIGITS));
   double point=SymbolInfoDouble(symbol,SYMBOL_POINT);
   double Ask=SymbolInfoDouble(symbol,SYMBOL_ASK);
   if(!digit || !point || !Ask) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_SLTP;
   request.symbol = symbol;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss)
     {
      int nStopLoss=int((Ask-StopLoss)/point);
      if(nStopLoss<0) return(false);
      if(!StopCorrect(symbol,nStopLoss))return(false);
      double dStopLoss=nStopLoss*point;
      request.sl=NormalizeDouble(request.price-dStopLoss,digit);
      if(request.sl<PositionGetDouble(POSITION_SL)) request.sl=PositionGetDouble(POSITION_SL);
     }
   else request.sl=PositionGetDouble(POSITION_SL);

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit)
     {
      int nTakeprofit=int((Takeprofit-Ask)/point);
      if(nTakeprofit<0) return(false);
      if(!StopCorrect(symbol,nTakeprofit))return(false);
      double dTakeprofit=nTakeprofit*point;
      request.tp=NormalizeDouble(request.price+dTakeprofit,digit);
      if(request.tp<PositionGetDouble(POSITION_TP)) request.tp=PositionGetDouble(POSITION_TP);
     }
   else request.tp=PositionGetDouble(POSITION_TP);

//----   
   if(request.tp==PositionGetDouble(POSITION_TP) && request.sl==PositionGetDouble(POSITION_SL)) return(true);
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Modify the Buy position by ",symbol," ============ >>>");
   Print(comment);

//---- Modify the BUY position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Modify_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy position under ",symbol," modified ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Modify the short position                              |
//+------------------------------------------------------------------+
bool dSellPositionModify
(
bool &Modify_Signal,        // enable modification flag
const string symbol,        // trading pair of a deal
uint deviation,             // slipping
double StopLoss,            // stoploss in the absolute value of the price chart
double Takeprofit           // TakeProfit in the absolute value of the price chart
)
//dSellPositionModify(Modify_Signal,symbol,deviation,StopLoss,Takeprofit);
  {
//----
   if(!Modify_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;

//---- Checking for an open position
   if(!PositionSelect(symbol)) return(true);
   if(PositionGetInteger(POSITION_TYPE)!=PosType) return(true);

//---- Declaring the structure of the trade request and the result of the trade request
   MqlTradeRequest request;
   MqlTradeResult result;

//---- Declaration of the structure of the result of the trade request verification
   MqlTradeCheckResult check;

//---- zeroing structures
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);
//----
   int digit=int(SymbolInfoInteger(symbol,SYMBOL_DIGITS));
   double point=SymbolInfoDouble(symbol,SYMBOL_POINT);
   double Ask=SymbolInfoDouble(symbol,SYMBOL_ASK);
   if(!digit || !point || !Ask) return(true);

//---- Initialization of MqlTradeRequest trade request structure for BUY position opening
   request.type   = ORDER_TYPE_SELL;
   request.price  = Ask;
   request.action = TRADE_ACTION_SLTP;
   request.symbol = symbol;

//---- Determination of the distance to stop-loss in units of the price schedule
   if(StopLoss!=0)
     {
      int nStopLoss=int((StopLoss-Ask)/point);
      if(nStopLoss<0) return(false);
      if(!StopCorrect(symbol,nStopLoss))return(false);
      double dStopLoss=nStopLoss*point;
      request.sl=NormalizeDouble(request.price+dStopLoss,digit);
      double laststop=PositionGetDouble(POSITION_SL);
      if(request.sl>laststop && laststop) request.sl=PositionGetDouble(POSITION_SL);
     }
   else request.sl=PositionGetDouble(POSITION_SL);

//---- Determination of the distance to take-profit units of the price schedule
   if(Takeprofit!=0)
     {
      int nTakeprofit=int((Ask-Takeprofit)/point);
      if(nTakeprofit<0) return(false);
      if(!StopCorrect(symbol,nTakeprofit))return(false);
      double dTakeprofit=nTakeprofit*point;
      request.tp=NormalizeDouble(request.price-dTakeprofit,digit);
      double lasttake=PositionGetDouble(POSITION_TP);
      if(request.tp>lasttake && lasttake) request.tp=PositionGetDouble(POSITION_TP);
     }
   else request.tp=PositionGetDouble(POSITION_TP);

//----   
   if(request.tp==PositionGetDouble(POSITION_TP) && request.sl==PositionGetDouble(POSITION_SL)) return(true);
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Checking the trade request for correctness
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Invalid data for the structure of the trade request!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Modify Sell position by ",symbol," ============ >>>");
   Print(comment);

//---- Modify the SELL position and check the result of the trade request
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): it is impossible to modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Modify_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell position under ",symbol," modified ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): Can not modify the position!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| GetTimeLevelName() function                                      |
//+------------------------------------------------------------------+
string GetTimeLevelName(string symbol,ENUM_POSITION_TYPE trade_operation)
  {
//----
   string G_Name_;
//----  
   if(MQL5InfoInteger(MQL5_TESTING)
      || MQL5InfoInteger(MQL5_OPTIMIZATION)
      || MQL5InfoInteger(MQL5_DEBUGGING))
      StringConcatenate(G_Name_,"TimeLevel_",AccountInfoInteger(ACCOUNT_LOGIN),"_",symbol,"_",trade_operation,"_Test_");
   else StringConcatenate(G_Name_,"TimeLevel_",AccountInfoInteger(ACCOUNT_LOGIN),"_",symbol,"_",trade_operation);
//----
   return(G_Name_);
  }
//+------------------------------------------------------------------+
//| TradeTimeLevelCheck() function                                   |
//+------------------------------------------------------------------+
bool TradeTimeLevelCheck
(
string symbol,
ENUM_POSITION_TYPE trade_operation,
datetime TradeTimeLevel
)
  {
//----
   if(TradeTimeLevel>0)
     {
      //---- Check for expiration of the time limit for the previous transaction
      if(TimeCurrent()<GlobalVariableGet(GetTimeLevelName(symbol,trade_operation))) return(false);
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| TradeTimeLevelSet() function                                     |
//+------------------------------------------------------------------+
void TradeTimeLevelSet
(
string symbol,
ENUM_POSITION_TYPE trade_operation,
datetime TradeTimeLevel
)
  {
//----
   GlobalVariableSet(GetTimeLevelName(symbol,trade_operation),TradeTimeLevel);
  }
//+------------------------------------------------------------------+
//| TradeTimeLevelSet() function                                     |
//+------------------------------------------------------------------+
datetime TradeTimeLevelGet
(
string symbol,
ENUM_POSITION_TYPE trade_operation
)
  {
//----
   return(datetime(GlobalVariableGet(GetTimeLevelName(symbol,trade_operation))));
  }
//+------------------------------------------------------------------+
//| TimeLevelGlobalVariableDel() function                            |
//+------------------------------------------------------------------+
void TimeLevelGlobalVariableDel
(
string symbol,
ENUM_POSITION_TYPE trade_operation
)
  {
//----
   if(MQL5InfoInteger(MQL5_TESTING)
      || MQL5InfoInteger(MQL5_OPTIMIZATION)
      || MQL5InfoInteger(MQL5_DEBUGGING))
      GlobalVariableDel(GetTimeLevelName(symbol,trade_operation));
//----
  }
//+------------------------------------------------------------------+
//| GlobalVariableDel_() function                                    |
//+------------------------------------------------------------------+
void GlobalVariableDel_(string symbol)
  {
//----
   TimeLevelGlobalVariableDel(symbol,POSITION_TYPE_BUY);
   TimeLevelGlobalVariableDel(symbol,POSITION_TYPE_SELL);
//----
  }
//+------------------------------------------------------------------+
//| Calculation of lot size for opening long                       |  
//+------------------------------------------------------------------+
/*                                                                   |
The external variable Margin_Mode determines the way of calculating the |
lot |
0 - MM on free funds on the account |
1 - MM on the balance of funds on the account |
2 - MM for losses from free funds on the account |
3 - MM for losses from the balance of funds on the account |
by default - MM by free funds on the account                  |
//+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+
if Money_Management is less than zero, then the trading function as |
The lot size uses rounded up to the nearest standard |
value is the absolute value of Money_Management.                    |
*///                                                                 |
//+------------------------------------------------------------------+
double BuyLotCount
(
string symbol,
double Money_Management,
int Margin_Mode,
int STOPLOSS,
uint Slippage_
)
// BuyLotCount_(string symbol, double Money_Management, int Margin_Mode, int STOPLOSS,Slippage_)
  {
//----
   double margin,Lot;

//---1+ CALCULATION OF LARGE SIZE TO OPEN THE POSITION
   if(Money_Management<0) Lot=MathAbs(Money_Management);
   else
   switch(Margin_Mode)
     {
      //---- Calculation of the lot from free funds on the account
      case  0:
         margin=AccountInfoDouble(ACCOUNT_FREEMARGIN)*Money_Management;
         Lot=GetLotForOpeningPos(symbol,POSITION_TYPE_BUY,margin);
         break;

         //---- Calculation of the lot from the balance of funds on the account
      case  1:
         margin=AccountInfoDouble(ACCOUNT_BALANCE)*Money_Management;
         Lot=GetLotForOpeningPos(symbol,POSITION_TYPE_BUY,margin);
         break;

         //---- Calculation of the lot for losses from free funds on the account          
      case  2:
        {
         if(STOPLOSS<=0)
           {
            Print(__FUNCTION__,": Неверный стоплосс!!!");
            STOPLOSS=0;
           }
         //---- 
         long digit;
         double point,price_open;
         //----   
         if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_ASK,price_open)) return(-1);

         //---- Determination of the distance to stop-loss in units of the price schedule
         if(!StopCorrect(symbol,STOPLOSS)) return(TRADE_RETCODE_ERROR);
         double price_close=NormalizeDouble(price_open-STOPLOSS*point,int(digit));

         double profit;
         if(!OrderCalcProfit(ORDER_TYPE_BUY,symbol,1,price_open,price_close,profit)) return(-1);
         if(!profit) return(-1);

         //---- Calculation of losses from free funds on the account
         double Loss=AccountInfoDouble(ACCOUNT_FREEMARGIN)*Money_Management;
         if(!Loss) return(-1);

         Lot=Loss/MathAbs(profit);
         break;
        }

      //---- Calculation of the lot for losses from the balance of funds on the account
      case  3:
        {
         if(STOPLOSS<=0)
           {
            Print(__FUNCTION__,": Неверный стоплосс!!!");
            STOPLOSS=0;
           }
         //---- 
         long digit;
         double point,price_open;
         //----   
         if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_ASK,price_open)) return(-1);

         //---- Determination of the distance to stop-loss in units of the price schedule
         if(!StopCorrect(symbol,STOPLOSS)) return(TRADE_RETCODE_ERROR);
         double price_close=NormalizeDouble(price_open-STOPLOSS*point,int(digit));

         double profit;
         if(!OrderCalcProfit(ORDER_TYPE_BUY,symbol,1,price_open,price_close,profit)) return(-1);
         if(!profit) return(-1);

         //---- Calculation of losses from the balance of funds on the account
         double Loss=AccountInfoDouble(ACCOUNT_BALANCE)*Money_Management;
         if(!Loss) return(-1);

         Lot=Loss/MathAbs(profit);
         break;
        }

      //---- Lot calculation without change
      case  4:
        {
         Lot=MathAbs(Money_Management);
         break;
        }

      //---- Calculation of the lot from free funds on the account by default
      default:
        {
         margin=AccountInfoDouble(ACCOUNT_FREEMARGIN)*Money_Management;
         Lot=GetLotForOpeningPos(symbol,POSITION_TYPE_BUY,margin);
        }
     }
//---1+    

//---- normalizing the lot size to the nearest standard value
   if(!LotCorrect(symbol,Lot,POSITION_TYPE_BUY)) return(-1);
//----
   return(Lot);
  }
//+------------------------------------------------------------------+
//| Calculation of the lot size for opening a short                        |  
//+------------------------------------------------------------------+
/*                                                                   |
The external variable Margin_Mode determines the way of calculating the |
lot |
0 - MM on free funds on the account |
1 - MM on the balance of funds on the account |
2 - MM for losses from free funds on the account |
3 - MM for losses from the balance of funds on the account |
by default - MM by free funds on the account                  |
//+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+
if Money_Management is less than zero, then the trading function as |
The lot size uses rounded up to the nearest standard |
value is the absolute value of Money_Management.                  |
*///                                                                 |
//+------------------------------------------------------------------+
double SellLotCount
(
string symbol,
double Money_Management,
int Margin_Mode,
int STOPLOSS,
uint Slippage_
)
// (string symbol, double Money_Management, int Margin_Mode, int STOPLOSS)
  {
//----
   double margin,Lot;

//---1+ CALCULATION OF LARGE SIZE TO OPEN THE POSITION
   if(Money_Management<0) Lot=MathAbs(Money_Management);
   else
   switch(Margin_Mode)
     {
      //---- Calculation of the lot from free funds on the account
      case  0:
         margin=AccountInfoDouble(ACCOUNT_FREEMARGIN)*Money_Management;
         Lot=GetLotForOpeningPos(symbol,POSITION_TYPE_SELL,margin);
         break;

         //---- Calculation of the lot from the balance of funds on the account
      case  1:
         margin=AccountInfoDouble(ACCOUNT_BALANCE)*Money_Management;
         Lot=GetLotForOpeningPos(symbol,POSITION_TYPE_SELL,margin);
         break;

         //---- Calculation of the lot for losses from free funds on the account             
      case  2:
        {
         if(STOPLOSS<=0)
           {
            Print(__FUNCTION__,": Invalid stoploss!");
            STOPLOSS=0;
           }
         //---- 
         long digit;
         double point,price_open;
         //----   
         if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_BID,price_open)) return(-1);

         //---- Determination of the distance to stop-loss in units of the price schedule
         if(!StopCorrect(symbol,STOPLOSS)) return(TRADE_RETCODE_ERROR);
         double price_close=NormalizeDouble(price_open+STOPLOSS*point,int(digit));

         double profit;
         if(!OrderCalcProfit(ORDER_TYPE_SELL,symbol,1,price_open,price_close,profit)) return(-1);
         if(!profit) return(-1);

         //---- Calculation of losses from free funds on the account
         double Loss=AccountInfoDouble(ACCOUNT_FREEMARGIN)*Money_Management;
         if(!Loss) return(-1);

         Lot=Loss/MathAbs(profit);
         break;
        }

      //---- Calculation of the lot for losses from the balance of funds on the account
      case  3:
        {
         if(STOPLOSS<=0)
           {
            Print(__FUNCTION__,": Invalid stoploss!");
            STOPLOSS=0;
           }
         //---- 
         long digit;
         double point,price_open;
         //----   
         if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(-1);
         if(!SymbolInfoDouble(symbol,SYMBOL_BID,price_open)) return(-1);

         //----Determination of the distance to stop-loss in units of the price schedule
         if(!StopCorrect(symbol,STOPLOSS)) return(TRADE_RETCODE_ERROR);
         double price_close=NormalizeDouble(price_open+STOPLOSS*point,int(digit));

         double profit;
         if(!OrderCalcProfit(ORDER_TYPE_SELL,symbol,1,price_open,price_close,profit)) return(-1);
         if(!profit) return(-1);

         //---- Calculation of losses from the balance of funds on the account
         double Loss=AccountInfoDouble(ACCOUNT_BALANCE)*Money_Management;
         if(!Loss) return(-1);

         Lot=Loss/MathAbs(profit);
         break;
        }

      //---- Lot calculation without change
      case  4:
        {
         Lot=MathAbs(Money_Management);
         break;
        }

      //---- Calculation of the lot from free funds on the account by default
      default:
        {
         margin=AccountInfoDouble(ACCOUNT_FREEMARGIN)*Money_Management;
         Lot=GetLotForOpeningPos(symbol,POSITION_TYPE_SELL,margin);
        }
     }
//---1+ 

//---- normalizing the lot size to the nearest standard value
   if(!LotCorrect(symbol,Lot,POSITION_TYPE_SELL)) return(-1);
//----
   return(Lot);
  }
//+------------------------------------------------------------------+
//| correction of the size of the pending order to the allowable value     |
//+------------------------------------------------------------------+
bool StopCorrect(string symbol,int &Stop)
  {
//----
   long Extrem_Stop;
   if(!SymbolInfoInteger(symbol,SYMBOL_TRADE_STOPS_LEVEL,Extrem_Stop)) return(false);
   if(Stop<Extrem_Stop) Stop=int(Extrem_Stop);
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| correction of the size of the pending order to the allowable value     |
//+------------------------------------------------------------------+
bool dStopCorrect
(
string symbol,
double &dStopLoss,
double &dTakeprofit,
ENUM_POSITION_TYPE trade_operation
)
// dStopCorrect(symbol,dStopLoss,dTakeprofit,trade_operation)
  {
//----
   if(!dStopLoss && !dTakeprofit) return(true);

   if(dStopLoss<0)
     {
      Print(__FUNCTION__,"(): Negative stop-loss value!");
      return(false);
     }

   if(dTakeprofit<0)
     {
      Print(__FUNCTION__,"(): Negative value of takeprofit!");
      return(false);
     }
//---- 
   int Stop;
   long digit;
   double point,dStop,ExtrStop,ExtrTake;

//---- get the minimum distance to the pending order
   Stop=0;
   if(!StopCorrect(symbol,Stop))return(false);
//----   
   if(!SymbolInfoInteger(symbol,SYMBOL_DIGITS,digit)) return(false);
   if(!SymbolInfoDouble(symbol,SYMBOL_POINT,point)) return(false);
   dStop=Stop*point;

//----correction of the size of the pending order for long
   if(trade_operation==POSITION_TYPE_BUY)
     {
      double Ask;
      if(!SymbolInfoDouble(symbol,SYMBOL_ASK,Ask)) return(false);

      ExtrStop=NormalizeDouble(Ask-dStop,int(digit));
      ExtrTake=NormalizeDouble(Ask+dStop,int(digit));

      if(dStopLoss>ExtrStop && dStopLoss) dStopLoss=ExtrStop;
      if(dTakeprofit<ExtrTake && dTakeprofit) dTakeprofit=ExtrTake;
     }

//---- correction of the size of the pending order for shorts
   if(trade_operation==POSITION_TYPE_SELL)
     {
      double Bid;
      if(!SymbolInfoDouble(symbol,SYMBOL_BID,Bid)) return(false);

      ExtrStop=NormalizeDouble(Bid+dStop,int(digit));
      ExtrTake=NormalizeDouble(Bid-dStop,int(digit));

      if(dStopLoss<ExtrStop && dStopLoss) dStopLoss=ExtrStop;
      if(dTakeprofit>ExtrTake && dTakeprofit) dTakeprofit=ExtrTake;
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| adjusting the lot size to the nearest acceptable value      |
//+------------------------------------------------------------------+
bool LotCorrect
(
string symbol,
double &Lot,
ENUM_POSITION_TYPE trade_operation
)
//LotCorrect(string symbol, double& Lot, ENUM_POSITION_TYPE trade_operation)
  {
//---- obtaining data for calculation   
   double Step,MaxLot,MinLot;
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_STEP,Step)) return(false);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MAX,MaxLot)) return(false);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MIN,MinLot)) return(false);

//---- normalizing the lot size to the nearest standard value
   Lot=Step*MathFloor(Lot/Step);

//---- check the lot for the minimum allowable value
   if(Lot<MinLot) Lot=MinLot;
//---- check the lot for the maximum allowable value       
   if(Lot>MaxLot) Lot=MaxLot;

//---- verification of funds for sufficiency
   if(!LotFreeMarginCorrect(symbol,Lot,trade_operation))return(false);
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| limit the size of the lot                |
//+------------------------------------------------------------------+
bool LotFreeMarginCorrect
(
string symbol,
double &Lot,
ENUM_POSITION_TYPE trade_operation
)
//(string symbol, double& Lot, ENUM_POSITION_TYPE trade_operation)
  {
//---- verification of funds for sufficiency
   double freemargin=AccountInfoDouble(ACCOUNT_FREEMARGIN);
   if(freemargin<=0) return(false);

//---- obtaining data for calculation 
   double Step,MaxLot,MinLot;
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_STEP,Step)) return(false);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MAX,MaxLot)) return(false);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MIN,MinLot)) return(false);

   double ExtremLot=GetLotForOpeningPos(symbol,trade_operation,freemargin);
//---- normalizing the lot size to the nearest standard value
   ExtremLot=Step*MathFloor(ExtremLot/Step);

   if(ExtremLot<MinLot) return(false); // not enough money even for a minimum lot!
   if(Lot>ExtremLot) Lot=ExtremLot; // we cut the size of the lot to what is on the deposit!
   if(Lot>MaxLot) Lot=MaxLot; // we cut the size of the lot to the maximum permissible
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| расчёт размер лота для открывания позиции с маржой lot_margin    |
//| calculation of lot size for opening a position with margin lot_margin |
//+------------------------------------------------------------------+
double GetLotForOpeningPos(string symbol,ENUM_POSITION_TYPE direction,double lot_margin)
  {
//----
   double price=0.0,n_margin;
   if(direction==POSITION_TYPE_BUY)  if(!SymbolInfoDouble(symbol,SYMBOL_ASK,price)) return(0);
   if(direction==POSITION_TYPE_SELL) if(!SymbolInfoDouble(symbol,SYMBOL_BID,price)) return(0);
   if(!price) return(NULL);

   if(!OrderCalcMargin(ENUM_ORDER_TYPE(direction),symbol,1,price,n_margin) || !n_margin) return(0);
   double lot=lot_margin/n_margin;

//---- получение торговых констант
//---- obtaining trading constants
   double LOTSTEP,MaxLot,MinLot;
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_STEP,LOTSTEP)) return(0);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MAX,MaxLot)) return(0);
   if(!SymbolInfoDouble(symbol,SYMBOL_VOLUME_MIN,MinLot)) return(0);

//---- нормирование величины лота до ближайшего стандартного значения 
//---- normalizing the lot size to the nearest standard value
   lot=LOTSTEP*MathFloor(lot/LOTSTEP);

//---- проверка лота на минимальное допустимое значение
//---- check the lot for the minimum allowable value
   if(lot<MinLot) lot=0;
//---- check the lot for the maximum allowable value 
//---- check the lot for the maximum allowable value     
   if(lot>MaxLot) lot=MaxLot;
//----
   return(lot);
  }
//+------------------------------------------------------------------+
//| возврат символа с заданными валютами залога и котировки          |
//| return of the symbol with the specified deposit currencies and quotes |
//+------------------------------------------------------------------+
string GetSymbolByCurrencies(string margin_currency,string profit_currency)
  {
//---- переберем в цикле все символы, которые представлены в окне "Обзор рынка"
//---- we will look through in a cycle all symbols which are presented in a window "the Market review"
   int total=SymbolsTotal(true);
   for(int numb=0; numb<total; numb++)
     {
      //---- получим имя символа по номеру в списке "Обзор рынка"
	  //---- get the name of the symbol by the number in the list "Market Overview"
      string symbolname=SymbolName(numb,true);

      //---- получим валюту залога
	  //---- we will receive the lien currency
      string m_cur=SymbolInfoString(symbolname,SYMBOL_CURRENCY_MARGIN);

      //---- получим валюту котировки (в чем измеряется прибыль при изменении цены)
	  //---  we obtain the quotation currency (in which the profit is measured when the price changes)
      string p_cur=SymbolInfoString(symbolname,SYMBOL_CURRENCY_PROFIT);

      //---- если символ подошел по обеим заданным валютам, вернем  имя символа
	  //---- if the symbol has been approached for both given currencies, return the name of the symbol
      if(m_cur==margin_currency && p_cur==profit_currency) return(symbolname);
     }
//----    
   return(NULL);
  }
//+------------------------------------------------------------------+
//| возврат стрингового результата торговой операции по его коду     |
//| return of the trading result of the string operation by its code |
//+------------------------------------------------------------------+
string ResultRetcodeDescription(int retcode)
  {
   string str;
//----
   switch(retcode)
     {
      case TRADE_RETCODE_REQUOTE: str="The rebellion"; break;
      case TRADE_RETCODE_REJECT: str="Request rejected"; break;
      case TRADE_RETCODE_CANCEL: str="Request canceled by trader"; break;
      case TRADE_RETCODE_PLACED: str="Order placed"; break;
      case TRADE_RETCODE_DONE: str="Application completed"; break;
      case TRADE_RETCODE_DONE_PARTIAL: str="The application was executed in part"; break;
      case TRADE_RETCODE_ERROR: str="Error processing request"; break;
      case TRADE_RETCODE_TIMEOUT: str="Request canceled after timeout";break;
      case TRADE_RETCODE_INVALID: str="Invalid request"; break;
      case TRADE_RETCODE_INVALID_VOLUME: str="Invalid request volume"; break;
      case TRADE_RETCODE_INVALID_PRICE: str="Неправильная цена в запросе"; break;
      case TRADE_RETCODE_INVALID_STOPS: str="Invalid price in request"; break;
      case TRADE_RETCODE_TRADE_DISABLED: str="Trade is prohibited"; break;
      case TRADE_RETCODE_MARKET_CLOSED: str="The market is closed"; break;
      case TRADE_RETCODE_NO_MONEY: str="There is not enough money to fulfill the request"; break;
      case TRADE_RETCODE_PRICE_CHANGED: str="Prices have changed"; break;
      case TRADE_RETCODE_PRICE_OFF: str="There are no quotes for processing the request"; break;
      case TRADE_RETCODE_INVALID_EXPIRATION: str="Invalid expiration date of the order in the request"; break;
      case TRADE_RETCODE_ORDER_CHANGED: str="The status of the order has changed"; break;
      case TRADE_RETCODE_TOO_MANY_REQUESTS: str="Too many requests"; break;
      case TRADE_RETCODE_NO_CHANGES: str="There are no changes to the request."; break;
      case TRADE_RETCODE_SERVER_DISABLES_AT: str="Autotrading is prohibited by the server"; break;
      case TRADE_RETCODE_CLIENT_DISABLES_AT: str="Autotrading is prohibited by the client terminal"; break;
      case TRADE_RETCODE_LOCKED: str="Request blocked for processing"; break;
      case TRADE_RETCODE_FROZEN: str="Order or position frozen"; break;
      case TRADE_RETCODE_INVALID_FILL: str="Order or position frozen..."; break;
      case TRADE_RETCODE_CONNECTION: str="No connection to the trading server"; break;
      case TRADE_RETCODE_ONLY_REAL: str="The operation is allowed only for real accounts"; break;
      case TRADE_RETCODE_LIMIT_ORDERS: str="Limit reached on the number of pending orders"; break;
      case TRADE_RETCODE_LIMIT_VOLUME: str="The limit on the volume of orders and positions for this symbol has been reached"; break;
      default: str="Unknown result";
     }
//----
   return(str);
  }
//+------------------------------------------------------------------+
//|                                                HistoryLoader.mqh |
//|                      Copyright © 2009, MetaQuotes Software Corp. |
//|                                       http://www.metaquotes.net/ |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| Загрузка истории для мультивалютного эксперта                    |
//| Download history for multicurrency expert						 |
//+------------------------------------------------------------------+
int LoadHistory(datetime StartDate,           // стартовая дата для подгрузки истории
											  // start date for history loading
                string LoadedSymbol,          // символ запрашиваемых исторических данных
											  // symbol of the requested historical data
                ENUM_TIMEFRAMES LoadedPeriod) // таймфрейм запрашиваемых исторических данных
											  // timeframe of requested historical data
  {
//----+ 
//Print(__FUNCTION__, ": Start load ", LoadedSymbol+ " , " + EnumToString(LoadedPeriod) + " from ", StartDate);
   int res=CheckLoadHistory(LoadedSymbol,LoadedPeriod,StartDate);
   switch(res)
     {
      case -1 : Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Unknown symbol ", LoadedSymbol);               break;
      case -2 : Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Requested bars more than max bars in chart "); break;
      case -3 : Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Program was stopped ");                        break;
      case -4 : Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Indicator shouldn't load its own data ");      break;
      case -5 : Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Load failed ");                                break;
      case  0 : /* Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Loaded OK ");  */                           break;
      case  1 : /* Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Loaded previously ");  */                   break;
      case  2 : /* Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Loaded previously and built ");  */         break;
      default : { /* Print(__FUNCTION__, "(", LoadedSymbol, " ", EnumToString(LoadedPeriod), "): Unknown result "); */}
     }
/* 
   if (res > 0)
    {   
     bars = Bars(LoadedSymbol, LoadedPeriod);
     Print(__FUNCTION__, "(", LoadedSymbol, " ", GetPeriodName(LoadedPeriod), "): First date ", first_date, " - ", bars, " bars");
    }
   */
//----+
   return(res);
  }
//+------------------------------------------------------------------+
//|  проверка истории для подгрузки                                  |
//+------------------------------------------------------------------+
int CheckLoadHistory(string symbol,ENUM_TIMEFRAMES period,datetime start_date)
  {
//----+
   datetime first_date=0;
   datetime times[100];
//--- check symbol & period
   if(symbol == NULL || symbol == "") symbol = Symbol();
   if(period == PERIOD_CURRENT)     period = Period();
//--- check if symbol is selected in the MarketWatch
   if(!SymbolInfoInteger(symbol,SYMBOL_SELECT))
     {
      if(GetLastError()==ERR_MARKET_UNKNOWN_SYMBOL) return(-1);
      if(!SymbolSelect(symbol,true)) Print(__FUNCTION__,"(): Не удалось добавить символ ",symbol," в окно MarketWatch!!!");
     }
//--- check if data is present
   SeriesInfoInteger(symbol,period,SERIES_FIRSTDATE,first_date);
   if(first_date>0 && first_date<=start_date) return(1);
//--- don't ask for load of its own data if it is an indicator
   if(MQL5InfoInteger(MQL5_PROGRAM_TYPE)==PROGRAM_INDICATOR && Period()==period && Symbol()==symbol)
      return(-4);
//--- second attempt
   if(SeriesInfoInteger(symbol,PERIOD_M1,SERIES_TERMINAL_FIRSTDATE,first_date))
     {
      //--- there is loaded data to build timeseries
      if(first_date>0)
        {
         //--- force timeseries build
         CopyTime(symbol,period,first_date+PeriodSeconds(period),1,times);
         //--- check date
         if(SeriesInfoInteger(symbol,period,SERIES_FIRSTDATE,first_date))
            if(first_date>0 && first_date<=start_date) return(2);
        }
     }
//--- max bars in chart from terminal options
   int max_bars=TerminalInfoInteger(TERMINAL_MAXBARS);
//--- load symbol history info
   datetime first_server_date=0;
   while(!SeriesInfoInteger(symbol,PERIOD_M1,SERIES_SERVER_FIRSTDATE,first_server_date) && !IsStopped())
      Sleep(5);
//--- fix start date for loading
   if(first_server_date>start_date) start_date=first_server_date;
   if(first_date>0 && first_date<first_server_date)
      Print(__FUNCTION__,"(): Warning: first server date ",first_server_date," for ",symbol,
            " does not match to first series date ",first_date);
//--- load data step by step
   int fail_cnt=0;
   while(!IsStopped())
     {
      //--- wait for timeseries build
      while(!SeriesInfoInteger(symbol,period,SERIES_SYNCHRONIZED) && !IsStopped())
         Sleep(5);
      //--- ask for built bars
      int bars=Bars(symbol,period);
      if(bars>0)
        {
         if(bars>=max_bars) return(-2);
         //--- ask for first date
         if(SeriesInfoInteger(symbol,period,SERIES_FIRSTDATE,first_date))
            if(first_date>0 && first_date<=start_date) return(0);
        }
      //--- copying of next part forces data loading
      int copied=CopyTime(symbol,period,bars,100,times);
      if(copied>0)
        {
         //--- check for data
         if(times[0]<=start_date) return(0);
         if(bars+copied>=max_bars) return(-2);
         fail_cnt=0;
        }
      else
        {
         //--- no more than 100 failed attempts
         fail_cnt++;
         if(fail_cnt>=100) return(-5);
         Sleep(10);
        }
     }
//----+ stopped
   return(-3);
  }
//+------------------------------------------------------------------+