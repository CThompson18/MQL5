
//+------------------------------------------------------------------+
//|                                            1ClickCloseAllv02.mq4 |
//|                                Copyright 2016, Ozan Buyuksemerci |
//|                                             https://www.ozan.org |
//+------------------------------------------------------------------+
#property copyright "Copyright 2016, Ozan Buyuksemerci (grandaevus)"
#property link      "https://ozan.org"
#property version   "2.00"
#property strict

//--- input parameters
input bool CloseOnlyManualTrades = true;
input int MaxSlippage = 5;



//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- 
   
   ObjectCreate(0,"CloseButton",OBJ_BUTTON,0,0,0);
   ObjectSetInteger(0,"CloseButton",OBJPROP_XDISTANCE,25);
   ObjectSetInteger(0,"CloseButton",OBJPROP_YDISTANCE,100);
   ObjectSetInteger(0,"CloseButton",OBJPROP_XSIZE,100);
   ObjectSetInteger(0,"CloseButton",OBJPROP_YSIZE,50);

   ObjectSetString(0,"CloseButton",OBJPROP_TEXT,"Close All");
      
   
   ObjectSetInteger(0,"CloseButton",OBJPROP_COLOR, White);
   ObjectSetInteger(0,"CloseButton",OBJPROP_BGCOLOR, Red);
   ObjectSetInteger(0,"CloseButton",OBJPROP_BORDER_COLOR,Red);
   ObjectSetInteger(0,"CloseButton",OBJPROP_BORDER_TYPE,BORDER_FLAT);
   ObjectSetInteger(0,"CloseButton",OBJPROP_BACK,false);
   ObjectSetInteger(0,"CloseButton",OBJPROP_HIDDEN,true);
   ObjectSetInteger(0,"CloseButton",OBJPROP_STATE,false);
   ObjectSetInteger(0,"CloseButton",OBJPROP_FONTSIZE,12);

//---
   return(INIT_SUCCEEDED);
  }
  
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//---

   ObjectDelete(0,"CloseButton");
   
  }
  
  
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---
      
  }
  
  
//+------------------------------------------------------------------+
//| ChartEvent function                                              |
//+------------------------------------------------------------------+
void OnChartEvent(const int id,
                  const long &lparam,
                  const double &dparam,
                  const string &sparam)
  {
//---
            
   if(sparam== "CloseButton")
      {
      if(CloseOnlyManualTrades == false) CloseAllOpenPositionsV01(MaxSlippage);
      else if(CloseOnlyManualTrades == true) CloseAllOpenPositionsV02(MaxSlippage);
        
      ObjectSetInteger(0,"CloseButton",OBJPROP_STATE,false);    
      }
         
//---      
  }
//+------------------------------------------------------------------+


void CloseAllOpenPositionsV01(int intMaxSlippage)
  {
   bool checkOrderClose = true;        
   int index = OrdersTotal()-1;
   
   while (index >=0 && OrderSelect (index,SELECT_BY_POS,MODE_TRADES)==true)
      {
      if ((OrderType()==OP_BUY || OrderType()==OP_SELL) && OrderSymbol() == Symbol())
         {         
         checkOrderClose = OrderClose (OrderTicket(), OrderLots(), OrderClosePrice(), intMaxSlippage, CLR_NONE); 
         
         if(checkOrderClose == false)
            {
            int errorCode = GetLastError();
            
            if (errorCode == 1 || errorCode == 2 || errorCode == 5 || errorCode == 6 || errorCode == 64 || errorCode == 65 || errorCode == 132 || errorCode == 133 || errorCode == 139) break;
            else continue;        
            }          
         }     
      
      index--;
      }
   }      
      
      
void CloseAllOpenPositionsV02(int intMaxSlippage)
  {
   bool checkOrderClose = true;        
   int index = OrdersTotal()-1;
   
   while (index >=0 && OrderSelect (index,SELECT_BY_POS,MODE_TRADES)==true)
      {
      if ((OrderType()==OP_BUY || OrderType()==OP_SELL) && OrderSymbol() == Symbol() && OrderMagicNumber() ==0)
         {         
         checkOrderClose = OrderClose (OrderTicket(), OrderLots(), OrderClosePrice(), intMaxSlippage, CLR_NONE); 
         
         if(checkOrderClose == false)
            {
            int errorCode = GetLastError();
            
            if (errorCode == 1 || errorCode == 2 || errorCode == 5 || errorCode == 6 || errorCode == 64 || errorCode == 65 || errorCode == 132 || errorCode == 133 || errorCode == 139) break;
            else continue;        
            }       
         }     
      
      index--;
      }            
   
   
   
//+------------------------------------------------------------------+
//|                                                     AutoSLTP.mq4 |
//|                        Copyright 2015, MetaQuotes Software Corp. |
//|                           http://free-bonus-deposit.blogspot.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2015, dXerof"
#property link      "http://free-bonus-deposit.blogspot.com"
#property version   "1.00"
/*
This EA will help you set Stop Loss and Take Profit on your open position (manual order).

EA can set SL/TP to all pairs, and you just need attach the EA on one pair only.

In the input settings, you can set single pair or all pairs. If AllPairs = True, it means EA will manage all pairs. Just attach on one chart, EA will work with all pairs.
*/
input bool   AllPairs=True;
input double TakeProfit=400; //40-->4 digits; 400-->5 digits
input double StopLoss=150; //15-->4 digits; 150-->5 digits
input double DurasiTime=60;
//---
int ticket;
double poen;
string pair="";
double iTakeProfit,iStopLoss;
double slbuy;
double tpbuy;
double slsell;
double tpsell;
double stoplevel;
double digi;
double poin;
//+------------------------------------------------------------------+
//| expert initialization function                                   |
//+------------------------------------------------------------------+
int init()
  {
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| expert deinitialization function                                 |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| expert start function                                            |
//+------------------------------------------------------------------+
int start()
  {
//----
   for(int cnt=0; cnt<OrdersTotal(); cnt++)
     {
      if(OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES))
        {
         if(OrderTicket()>0 && OrderMagicNumber()==0 && OrderTakeProfit()==0 && TimeCurrent()-OrderOpenTime()<=DurasiTime)
           {
            if(AllPairs) pair=OrderSymbol(); else pair=Symbol();
            //---
            stoplevel=MarketInfo(pair,MODE_STOPLEVEL);
            digi=MarketInfo(pair,MODE_DIGITS);
            poin=MarketInfo(pair,MODE_POINT);

            iStopLoss=StopLoss;
            iTakeProfit=TakeProfit;
            if(StopLoss<stoplevel) iStopLoss=stoplevel;
            if(TakeProfit<stoplevel) iTakeProfit=stoplevel;
            //---
            slbuy=NormalizeDouble(OrderOpenPrice()-iStopLoss*poin,digi);
            tpbuy=NormalizeDouble(OrderOpenPrice()+iTakeProfit*poin,digi);
            slsell=NormalizeDouble(OrderOpenPrice()+iStopLoss*poin,digi);
            tpsell=NormalizeDouble(OrderOpenPrice()-iTakeProfit*poin,digi);
            //---
            if(OrderSymbol()==pair && (OrderType()==OP_BUY || OrderType()==OP_BUYLIMIT || OrderType()==OP_BUYSTOP))
              {
               ticket=OrderModify(OrderTicket(),OrderOpenPrice(),slbuy,tpbuy,0,Blue);
              }
            if(OrderSymbol()==pair && (OrderType()==OP_SELL || OrderType()==OP_SELLLIMIT || OrderType()==OP_SELLSTOP))
              {
               ticket=OrderModify(OrderTicket(),OrderOpenPrice(),slsell,tpsell,0,Red);
              }
           }
        }
     }
//----
   return(0);
  }
//+------------------------------------------------------------------+


   
   //+------------------------------------------------------------------+
//|                                                     Close@Profit |
//|                                        Copyright 2012, ForexBold |
//|                                         http://www.forexbold.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2014, ForexBold Software Corp."
#property link      "http://www.forexbold.com"
#property version   "1.00"
#property strict
/*
Description

Close@Profit is an EA that close orders after defined profit or loss.

You can define if to close only trades on current symbol or to close all orders on all symbols, to close only opened trades or also delete pending orders.

Based on Close at Profit idea from Matus German but including Commissions, Swaps and changes in UI.
Input variables 
◾UseProfitToClose = true; - if it is true the EA will close trades when the profit is greater than defined profit to close
◾ProfitToClose = 100; - the defined profit in account currency, when close trades
◾UseLossToClose = false; - if it is true the EA will close trades when the loss is greater than defined loss to close
◾LossToClose = 100; - the defined loss in account currency, when close trades
◾AllSymbols = true; - if it is true the EA closes trades on all symbols, if false- only on current symbol where EA is attached
◾PendingOrders = true; - if it is true the EA closes also pending orders, on all symbols or current symbol depends on AllSymbols setting
◾MaxSlippage = 3; - maximal allowed slippage in pip
◾LegendColor = White; - color of label text
◾VariablesColor = Gold; - color of variables
◾FontSize = 8; - Font size

*/
extern bool      UseProfitToClose       = true;
extern double    ProfitToClose          = 100;

extern bool      UseLossToClose         = false;
extern double    LossToClose            = 100;

extern bool      AllSymbols             = true;
extern bool      PendingOrders          = true;

extern double    MaxSlippage=3;

extern color     LegendColor            = White;
extern color     VariableColor          = Gold;

extern int       FontSize=8;

int RowCount=1;
int Tick=0;
double Profit=0;
double ProfitSymbol=0;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
   ObjectsDeleteAll();
   EventSetTimer(1);
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   ObjectsDeleteAll();
   EventKillTimer();
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick()
  {

   FindMyOrders();

   if(Profit>=ProfitToClose && AllSymbols==true)
     {
      CloseOrders();
      Print("Closing All Symbols @ Profit ",Profit);
     }

   if(ProfitSymbol>=ProfitToClose && AllSymbols==false)
     {
      CloseOrders();
      Print("Closing "+Symbol()+" @ Profit ",ProfitSymbol);
     }

   if(Profit<=LossToClose*-1 && AllSymbols==true && UseLossToClose)
     {
      CloseOrders();
      Print("Closing All Symbols @ Loss ",Profit);
     }

   if(ProfitSymbol<=LossToClose*-1 && AllSymbols==false && UseLossToClose)
     {
      CloseOrders();
      Print("Closing "+Symbol()+" @ Loss ",ProfitSymbol);
     }

  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTimer()
  {

   FindMyOrders();
   DrawMenu();

  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void DrawMenu()
  {

   color foo1 = VariableColor;
   color foo2 = VariableColor;

   if(!AllSymbols)
     {
      foo1=Gray;
        } else {
      foo2=Gray;
     }

   DrawItem("totalProfit","AllSymbols Profit",DoubleToStr(Profit,2),foo1);
   DrawItem("symbolProfit",Symbol()+" Profit",DoubleToStr(ProfitSymbol,2),foo2);
   DrawItem("AllSymbols","AllSymbols",BoolToStr(AllSymbols),BoolToColor(AllSymbols));

   DrawItem("sep1","","",0);

   DrawItem("UseProfitToClose","UseProfitToClose",BoolToStr(UseProfitToClose),BoolToColor(UseProfitToClose));
   DrawItem("ProfitToClose","ProfitToClose",DoubleToStr(ProfitToClose,2),BoolToColor(UseProfitToClose));

   DrawItem("sep2","","",0);

   color lossColor=VariableColor;
   if(!UseLossToClose)
     {
      lossColor=Gray;
     }

   DrawItem("UseLossToClose","UseLossToClose",BoolToStr(UseLossToClose),BoolToColor(UseLossToClose));
   DrawItem("LossToClose","LossToClose",DoubleToStr(LossToClose*-1,2),lossColor);

   DrawItem("sep3","","",0);

   DrawItem("PendingOrders","PendingOrders",BoolToStr(PendingOrders),BoolToColor(PendingOrders));
   DrawItem("OrdersTotal","OrdersTotal",IntegerToString(OrdersTotal()),VariableColor);
   DrawItem("Lots","Lots",DoubleToStr(LotsSize(),2),VariableColor);
   RowCount=1;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void FindMyOrders()
  {

   Profit=0;
   ProfitSymbol=0;

   for(int l_pos_0=OrdersTotal()-1; l_pos_0>=0; l_pos_0--)
     {

      bool order=OrderSelect(l_pos_0,SELECT_BY_POS,MODE_TRADES);

      if(!order)
        {
         continue;
        }

      if(OrderType()==OP_BUY || OrderType()==OP_SELL)
        {

         double order_profit=OrderProfit()+OrderSwap()+OrderCommission();
         Profit+=order_profit;

         if(OrderSymbol()==Symbol())
           {
            ProfitSymbol+=order_profit;
           }

        }
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
string BoolToStr(bool val)
  {
   if(val) { return "True"; }
   return "False";
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
color BoolToColor(bool val)
  {
   if(val) { return VariableColor; }
   return Gray;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void DrawItem(string name,string label,string value,color valueColor)
  {

   if(UseLossToClose==false && UseProfitToClose==false)
     {
      valueColor=Gray;
     }

   RowCount++;
   string col1 = name+"Legend";
   string col2 = name+"Value";

   ObjectDelete(col1);
   ObjectDelete(col2);

   ObjectCreate(col1,OBJ_LABEL,0,0,0,0,0);
   ObjectCreate(col2,OBJ_LABEL,0,0,0,0,0);

   ObjectSetText(col1,label,FontSize,"Arial",LegendColor);
   ObjectSet(col1,OBJPROP_XDISTANCE,10*FontSize);
   ObjectSet(col1,OBJPROP_YDISTANCE,FontSize*RowCount*1.5);
   ObjectSet(col1,OBJPROP_CORNER,1);

   ObjectSetText(col2,value,FontSize,"Arial",valueColor);
   ObjectSet(col2,OBJPROP_XDISTANCE,FontSize*3);
   ObjectSet(col2,OBJPROP_YDISTANCE,FontSize*RowCount*1.5);
   ObjectSet(col2,OBJPROP_CORNER,1);

  }
//+------------------------------------------------------------------+

double LotsSize()
  {
   int total=OrdersTotal();
   double lots=0;
   for(int cnt=total-1; cnt>=0; cnt--)
     {
      bool order=OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES);
      if(!order)
        {
         continue;
        }
      if(AllSymbols)
        {
         if(PendingOrders)
            lots+=OrderLots();
         if(!PendingOrders)
            if(OrderType()==OP_BUY || OrderType()==OP_SELL)
               lots+=OrderLots();
        }
      if(!AllSymbols)
        {
         if(OrderSymbol()==Symbol())
           {
            if(PendingOrders)
               lots+=OrderLots();
            if(!PendingOrders)
               if(OrderType()==OP_BUY || OrderType()==OP_SELL)
                  lots+=OrderLots();
           }
        }
     }
   return (lots);
  }
//+------------------------------------------------------------------+

void CloseOrders()
  {

   int cnt;
   RefreshRates();

   for(cnt=OrdersTotal()-1; cnt>=0; cnt--)
     {

      OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES);

      if(AllSymbols==false && OrderSymbol()!=Symbol()) 
        {
         continue;
        }

      if(OrderType()==OP_BUY)
        {
         OrderClose(OrderTicket(),OrderLots(),Bid,MaxSlippage,Violet);
        }
      if(OrderType()==OP_SELL)
        {
         OrderClose(OrderTicket(),OrderLots(),Ask,MaxSlippage,Violet);
        }

      if(PendingOrders)
        {
         if(OrderType()==OP_BUYLIMIT)
           {
            OrderDelete(OrderTicket());
           }
         if(OrderType()==OP_SELLLIMIT)
           {
            OrderDelete(OrderTicket());
           }
         if(OrderType()==OP_BUYSTOP)
           {
            OrderDelete(OrderTicket());
           }
         if(OrderType()==OP_SELLSTOP)
           {
            OrderDelete(OrderTicket());
           }
        }

     } // for

  }
//+------------------------------------------------------------------+



//+------------------------------------------------------------------------+
//|                              Copyright 2015, MetaQuotes Software Corp. |
//|                                                   https://www.mql5.com |
//| Close Cross Kijun Sen 3.00 : File45 Aug 2015                           |
//| https://www.mql5.com/en/users/file45/publications                      |
//|                                                                        |
//| !IsExpertEnabled and !IsTradeAllowed code adapted from :               |
//| http://www.jimdandyforex.com/mql4-tutorial-video-enabling-autotrading/ |
//+------------------------------------------------------------------------+
#property copyright "Copyright file45."
#property link      "https://www.mql5.com"
#property version   "3.00"
#property description "The EA will close all relevant open orders across all charts corresponding to the EA chart symbol when Bid or Ask cross the Kijun Sen." 
#property description " "
#property description  "* Enable/Disable Order Close function : Click on chart --> | close on ^ | close off ."
#property strict
/*
Stealth Mode: The EA will close all open orders across all charts corresponding to the EA chart symbol when Bid or Ask is equal to or crosses the Kijun Sen. For Buy orders the condition is met when Bid is equal or less than Kijun Sen. For Sell orders the condition is met when Ask is equal or greater than Kijun Sen.

Price gaps in Stealth Mode: If current price gaps past the Kijun Sen, the EA will close all relevant open orders at the first available price.

Spread Mode: The EA will close all open orders across all charts corresponding to the EA chart symbol when Kijun Sen price falls between the Bid-Ask spread. In this condition, Bid is lower than the Kijun Sen price and simultaneously Ask is higher than the Kijun Sen price. This condition is true for both Buy and Sell orders.

Price gaps in Spread Mode: If current price gaps past the Kijun Sen, both Bid and Ask will either be greater than Kijun Sen or lesser than Kijun Sen. All relevant open orders will remain open until the current price retraces back to the Kijun Sen and the Kijun Sen price falls between the Bid-Ask spread or until relevant open orders are closed manually.
◾Note: Check that the chart Kijun Sen and EA parameters match each other exactly.
◾Changed parameters can be saved, named and loaded for re-use from the Save & Load buttons. See EA Properties Inputs Tab (F7 → Inputs Tab → Save, Load Buttons).
◾Chart PopUp and Sound alert and Push Notification for mobile options.

◾A set of Kijun Sen Indicators with various default periods is included in Kijun Zip. See - Download.

Enable/Disable Order Close function : Click on chart --> | close on ^ | close off.

Close Order on/off 


Limitations
◾The Experts button has been renamed to AutoTrading and also works in a different way. See - https://www.youtube.com/watch?v=OOoV5prZy3M for more information.

◾In order for an EA to discriminate control over any particular order and actions of other EAs, it must have access to the order's assigned magic number. It is not possible to assign magic numbers to order opened manually. As orders are opened manually and only closed by the EA the absence of magic number results in EA function limitations as highlighted below.
◾For more information see - Magic Number: "Magic" Identifier of the Order - https://www.mql5.com/en/articles/1359. Quote from article/1359: when an order is being placed (a position is being opened), one can assign a unique number to it. This number will consequently be used to distinguish the above order from others. There is no use (or even possibility) applying this feature when trading manually.

◾The EA will close all open trades for the symbol of the chart to which it is attached and all open trades on charts of the same symbol to which is not attached.
◾Do not use the EA on more than one chart of the same symbol at the same time. This will frequently result in all open trades on all same symbol charts being closed by the first EA to meet any OrderClose conditions.
Note: The EA will close all open positions across all charts only of the same symbol to which the EA is attached. Open positions of different symbols will remain open.
Push Notification: iOS and Android
1.Push Notifications are fast, fail safe and easy to set up.
2.First install MetaTrader 4 on Mobile Device.
3.Locate MetaQuotes ID. See - Settings and Messages in MetaTrader 4 Mobile.
4.Enter MetaQuotes ID in Client Terminal of MetaTrader 4 on computer (Tools → Options → Notifications).
5.Note: The same MetaQuotes ID can be used with multiple Client Terminals (different Brokers).
6.Click the Test Button.
7.Open up MetaTrader 4 Terminal (Ctrl + T) and select the Journal Tab and check under Messages if the the Test was successful.
8.Check that the test message has been received on the mobile device.
9.Once a successful Test is obtained Push Notification Alerts can be set in the EA.
10.Important: Ensure that your mobile device has sound turned on for notifications and also ensure MetaTrader 4 mobile is included in your phone Notifications or Messaging setting.
11.If further clarification is required, google and youtube "MT4 push notification."

*/
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
/*enum EAOffOn 
{
   EA_Off = 0, // Off 
   EA_On = 1   // On
};*/
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
enum AorSorNA
  {
   Alrt = 0,   // PopUp Alert
   Snd  = 1,   // Sound Only
   No_Alrt = 3 // No Alerts
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
enum StealthorSpread
  {
   Stealth = 0, // Stealth: Bid <= Kijun Sen or Ask >= Kijun Sen
   Straddle = 1 // Spread: Bid < Kijun Sen and Ask > Kijun Sen
  };

//////////// Default Inputs : Start
//input EAOffOn EA_Off_On        = 1;  // EA Off or On
input string on_off_ea="Click on chart --> | close on ^ | close off "; // * Enable/Disable Order Close function
input StealthorSpread S_or_S=0;  // Close Mode -->
input int Kijun_Period         = 50; // Kijun Sen Period
input int Points_to_cross      = 0;  // Points to Cross 
input int Slippage             = 10; // Slippage
input bool Close_Marker        = false; // Order Close Marker -->
input AorSorNA A_or_S_or_NA    = 0;     // Alerts 
input bool Push_Notification   = false; // Push Notification -->
input color Font_Color=DodgerBlue; // Font Color
input int Font_Size=8;    // Font Size 
input bool Font_Bold           = false; // Font Bold -->
input int Left_Right           = 15;    // Left <-> Right
input int Up_Down=15;   // Up <-> Down
input ENUM_BASE_CORNER Corner=1;     // Corner -->
                                     //////////// Default Inputs : End
string TM,SS,AP,FS;
string  KS=" Kijun Sen ";
color CM;
int BarToTest=1;
color Font_Color_SS=Green;
string FO=" | close on ";
int EA_Off_On= 1,UpDnss,UpDncl;
bool switchz = true;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int OnInit()
  {
   if(EA_Off_On==1)
     {
      if(!IsExpertEnabled())
        {
         MessageBox("You need to enable AutoTrading! "+" Check your Settings!");
         return(INIT_FAILED);
        }

      if(!IsTradeAllowed())
        {
         MessageBox("You need to Allow Live Trading! "+" Check your Settings!"+"\n"+
                    "Common tab -> Live Trading -> Allow live trading");
         return(INIT_FAILED);
        }
     }

   if(A_or_S_or_NA==0 && Push_Notification==false)
     {
      AP="A";
     }
   else if(A_or_S_or_NA==1 && Push_Notification==false)
     {
      AP="S";
     }
   else if(A_or_S_or_NA==3 && Push_Notification==true)
     {
      AP="P";
     }
   else if(A_or_S_or_NA==0 && Push_Notification==true)
     {
      AP="AP";
     }
   else if(A_or_S_or_NA==1 && Push_Notification==true)
     {
      AP="SP";
     }
   else
     {
      AP="  ";
     }

   switch(Font_Bold)
     {
      case 1: FS = "Arial Bold"; break;
      case 0: FS = "Arial";      break;
     }

   switch(Period())
     {
      case 1:     TM = "M1";  break;
      case 2:     TM = "M2";  break;
      case 3:     TM = "M3";  break;
      case 4:     TM = "M4";  break;
      case 5:     TM = "M5";  break;
      case 6:     TM = "M6";  break;
      case 7:     TM = "M7";  break;
      case 8:     TM = "M8";  break;
      case 9:     TM = "M9";  break;
      case 10:    TM = "M10"; break;
      case 11:    TM = "M11"; break;
      case 12:    TM = "M12"; break;
      case 13:    TM = "M13"; break;
      case 14:    TM = "M14"; break;
      case 15:    TM = "M15"; break;
      case 20:    TM = "M20"; break;
      case 25:    TM = "M25"; break;
      case 30:    TM = "M30"; break;
      case 40:    TM = "M40"; break;
      case 45:    TM = "M45"; break;
      case 50:    TM = "M50"; break;
      case 60:    TM = "H1";  break;
      case 120:   TM = "H2";  break;
      case 180:   TM = "H3";  break;
      case 240:   TM = "H4";  break;
      case 300:   TM = "H5";  break;
      case 360:   TM = "H6";  break;
      case 420:   TM = "H7";  break;
      case 480:   TM = "H8";  break;
      case 540:   TM = "H9";  break;
      case 600:   TM = "H10"; break;
      case 660:   TM = "H11"; break;
      case 720:   TM = "H12"; break;
      case 1440:  TM = "D1";  break;
      case 10080: TM = "W1";  break;
      case 43200: TM = "MN";  break;
     }

   switch(Close_Marker)
     {
      case 0 : CM = CLR_NONE; break;
      case 1 : CM = Red;
     }

/*switch(EA_Off_On)
   {
      case 0: FO = " (off) "; break;
      case 1: FO = " (on) ";
   }*/

   switch(S_or_S)
     {
      case 0: SS = " Stealth mode"; break;
      case 1: SS = " Spread mode";
     }

   switch(Corner)
     {
      case 0: UpDnss = Up_Down+ Font_Size*2;UpDncl = Up_Down; break;
      case 1: UpDnss = Up_Down+ Font_Size*2;UpDncl = Up_Down; break;
      case 2: UpDnss = Up_Down;UpDncl = Up_Down + Font_Size*2; break;
      case 3: UpDnss = Up_Down;UpDncl = Up_Down + Font_Size*2;
     }

   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   ObjectDelete("CLKS");
   ObjectDelete("SSKS");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick()
  {
   if(EA_Off_On==1)
     {
      if(!IsExpertEnabled())
        {
         MessageBox("You need to enable AutoTrading! "+" Check your Settings!");
         return;
        }

      if(!IsTradeAllowed())
        {
         MessageBox("You need to Allow Live Trading! "+" Check your Settings!"+"\n"+
                    "Common tab -> Live Trading -> Allow live trading");
         return;
        }
     }

   ObjectCreate(0,"SSKS",OBJ_LABEL,0,0,0);
   ObjectSetText("SSKS",SS+FO,Font_Size,FS,Font_Color_SS);
   ObjectSet("SSKS",OBJPROP_CORNER,Corner);
   ObjectSet("SSKS",OBJPROP_XDISTANCE,Left_Right);
   ObjectSet("SSKS",OBJPROP_YDISTANCE,UpDnss);
   ObjectSet("SSKS", OBJPROP_SELECTABLE, false);

   ObjectCreate(0,"CLKS",OBJ_LABEL,0,0,0);
   ObjectSetText("CLKS",KS+IntegerToString(Kijun_Period)+" "+AP+" "+" ",Font_Size,FS,Font_Color);
   ObjectSet("CLKS",OBJPROP_CORNER,Corner);
   ObjectSet("CLKS",OBJPROP_XDISTANCE,Left_Right);
   ObjectSet("CLKS",OBJPROP_YDISTANCE,UpDncl);
   ObjectSet("CLKS", OBJPROP_SELECTABLE, false);

   double kijun_sen=iIchimoku(NULL,0,9,Kijun_Period,52,MODE_KIJUNSEN,BarToTest);

   int total= OrdersTotal();
   for(int i=total-1;i>=0;i--)
     {
      if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES)==true)
        {
         if(OrderType()==OP_BUY)
           {
            if(((Bid<=kijun_sen -(Points_to_cross*Point)) && OrderSymbol()==Symbol() && EA_Off_On==1 && S_or_S==0) || 
               ((Bid<kijun_sen -(Points_to_cross*Point)) && (Ask>kijun_sen -(Points_to_cross*Point)) && OrderSymbol()==Symbol() && EA_Off_On==1 && S_or_S==1))
              {
               if(OrderClose(OrderTicket(),OrderLots(),Bid,Slippage,CM))
                 {
                  if(A_or_S_or_NA==0)
                    {
                     Alert(Symbol()+" Buy closed "+KS+IntegerToString(Kijun_Period)+SS+" - "+AccountCompany());
                    }
                  else if(A_or_S_or_NA==1)
                    {
                     PlaySound("Alert.wav");
                    }
                  if(Push_Notification==true)
                    {
                     SendNotification("Buy closed "+KS+IntegerToString(Kijun_Period)+SS+"\n"+
                                      Symbol()+" "+TM+" "+TimeToStr(TimeCurrent(),TIME_SECONDS)+"  "+TimeToStr(TimeCurrent(),TIME_DATE)+"  "+AccountCompany());
                    }
                 }
               else
                 {
                  Alert("Buy "+KS+IntegerToString(Kijun_Period)+SS+" : Order ",OrderTicket()," failed to close. Error:",GetLastError());
                  Sleep(3000);
                  if(Push_Notification==true)
                    {
                     SendNotification("Buy "+KS+IntegerToString(Kijun_Period)+SS+" : Order "+DoubleToString(OrderTicket())+" failed to close. Error:"+IntegerToString(GetLastError())+"\n"+
                                      Symbol()+" "+TM+" "+TimeToStr(TimeCurrent(),TIME_SECONDS)+"  "+TimeToStr(TimeCurrent(),TIME_DATE)+"  "+AccountCompany());
                    }
                 }
              }
           }
         else if(OrderType()==OP_SELL)
           {
            if(((Ask>=kijun_sen -(Points_to_cross*Point)) && OrderSymbol()==Symbol() && EA_Off_On==1 && S_or_S==0) || 
               ((Bid<kijun_sen -(Points_to_cross*Point)) && (Ask>kijun_sen -(Points_to_cross*Point)) && OrderSymbol()==Symbol() && EA_Off_On==1 && S_or_S==1))
              {
               if(OrderClose(OrderTicket(),OrderLots(),Ask,Slippage,CM))
                 {
                  if(A_or_S_or_NA==0)
                    {
                     Alert(Symbol()+" Sell closed "+KS+IntegerToString(Kijun_Period)+SS+" - "+AccountCompany());
                    }
                  else if(A_or_S_or_NA==1)
                    {
                     PlaySound("Alert.wav");
                    }
                  if(Push_Notification==true)
                    {
                     SendNotification("Sell closed "+KS+IntegerToString(Kijun_Period)+SS+"\n"+
                                      Symbol()+" "+TM+" "+TimeToStr(TimeCurrent(),TIME_SECONDS)+"  "+TimeToStr(TimeCurrent(),TIME_DATE)+"  "+AccountCompany());
                    }
                 }
               else
                 {
                  Alert("Sell "+KS+IntegerToString(Kijun_Period)+SS+" : Order ",OrderTicket()," failed to close. Error:",GetLastError());
                  Sleep(3000);
                  if(Push_Notification==true)
                    {
                     SendNotification("Sell "+KS+IntegerToString(Kijun_Period)+SS+" : Order "+DoubleToString(OrderTicket())+" failed to close. Error:"+IntegerToString(GetLastError())+"\n"+
                                      Symbol()+" "+TM+" "+TimeToStr(TimeCurrent(),TIME_SECONDS)+"  "+TimeToStr(TimeCurrent(),TIME_DATE)+"  "+AccountCompany());
                    }
                 }
              }
           }
        }
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnChartEvent(const int id,         // Event identifier  
                  const long& lparam,   // Event parameter of long type
                  const double& dparam, // Event parameter of double type
                  const string& sparam) // Event parameter of string type
  {
//if(id==CHARTEVENT_OBJECT_CLICK && (sparam == "SSKS" || sparam == "CLKS"))
   if(id==CHARTEVENT_OBJECT_CLICK && sparam=="SSKS")
     {

      switchz=!switchz;
      EA_Off_On=switchz;

      switch(EA_Off_On)
        {
         case 0: FO = " | close off "; Font_Color_SS = Red;break;
         case 1: FO = " | close on ";  Font_Color_SS = Green;
        }
     }
  }
//+------------------------------------------------------------------+


//+------------------------------------------------------------------+
//|  Copyright, Programmed by: Khaled Mohamed Abdalla                |
//|  https://www.mql5.com/en/users/khaled.mohamed                    |
//+------------------------------------------------------------------+
#property copyright "Programmed by: Khaled Mohamed Abdalla"
#property link      "https://www.mql5.com/en/users/khaled.mohamed"
#property strict
#property description "An EA to close the all orders once the drawdoen reach a specific percentage ."
/*
This code has some functions to close all opened orders once the drawdown reaches a specific percentage of the account balance.

You will just enter the magic number of the orders (enter 0 for control all orders), and the maximum percentage of drawdown that will allow the code logic to close the order once it is reached.

Only call the function DD_close to use this code.
*/
/*
// To use this option, just you need to call the function : ( DD_close ) 
//+------------------------------------------------------------------+
//|                          Global scope                            |
//+------------------------------------------------------------------+
bool Close_All_V;
//+------------------------------------------------------------------+
//|                         Main function                            |
//+------------------------------------------------------------------+
// DD:               Here it is the DD percentage, 100 means never close any order .
// Magic_Number:     Your EA magic number, enter 0 to control the all orders .
void DD_close(int DD,int Magic_Number)
  {
   if(DD(Magic_Number)>=DD)
      Close_All_V=true;
   if(Close_All_V)
      Close_All(Magic_Number);
  }
//+------------------------------------------------------------------+
//|                          Check close                             |
//+------------------------------------------------------------------+
void Check_Close(int Check_Number) // check close order
  {
   if(Check_Number<0) Print("OrderClose failed with error: ",ErrorDescription(GetLastError()));
   else Close_All_V=false;
  }
//+------------------------------------------------------------------+
//|                          Close all                               |
//+------------------------------------------------------------------+
void Close_All(int M_N)
  {
   int Loop=0;
   for(int i=0; Loop<OrdersTotal(); i++)
      if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES))
        {
         Loop++;
         if(OrderSymbol()==Symbol())
            if(OrderMagicNumber()==M_N || OrderMagicNumber()==0)
              {
               if(OrderType()==OP_BUY)
                  Check_Close(OrderClose(OrderTicket(),OrderLots(),Bid,100,clrNONE));
               if(OrderType()==OP_SELL)
                  Check_Close(OrderClose(OrderTicket(),OrderLots(),Ask,100,clrNONE));
              }
        }
  }
//+------------------------------------------------------------------+
//|                          Calculate loss                          |
//+------------------------------------------------------------------+
double Loss(int M_N)
  {
   double re=0;
   int Loop=0;
   for(int i=0; Loop<OrdersTotal(); i++)
      if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES))
        {
         Loop++;
         if(OrderSymbol()==Symbol())
            if(OrderMagicNumber()==M_N || OrderMagicNumber()==0)
               re=re+OrderProfit();
        }
   return re * -1;
  }
//+------------------------------------------------------------------+
//|                  Calculate drawdown percentage                   |
//+------------------------------------------------------------------+
double DD(int M_N)
  {
   return ( 100 / AccountBalance ( ) ) * Loss ( M_N );
  }
//+------------------------------------------------------------------+
*/
// Header files for the errors description:
#include <stderror.mqh>
#include <stdlib.mqh>

//+------------------------------------------------------------------+
//|                             Inputs                               |
//+------------------------------------------------------------------+
// Enter 0 for control the all orders .
extern const string Description_1="The magic number, enter 0 to control the all orders .";
extern int Magic_number=0;
//-----------------
extern const string Space_1="";
//-----------------
// The max drawdown persantge that allow the EA to close the all orders when it reached, enter 100 to disable the EA .
extern const string Description_2 = "The max drawdown persantge that allow the EA";
extern const string Description_3 = " to close the all orders when it reached,";
extern const string Description_4 = "( enter 100 to disable the EA )";
extern int Max_drawdown_persantge = 100;
//-----------------
extern const string Space_2="";
//-----------------
extern string Copyrights="Programmed by: Khaled Mohamed";

// To use this option, just you need to call the function : ( DD_close ) 
//+------------------------------------------------------------------+
//|                          Global scop                             |
//+------------------------------------------------------------------+
bool Close_All_V;
//+------------------------------------------------------------------+
//|                         OnInit function                          |
//+------------------------------------------------------------------+
void OnInit()
  {
   Close_All_V=false;
  }
//+------------------------------------------------------------------+
//|                         OnTick function                          |
//+------------------------------------------------------------------+
void OnTick()
  {
   DD_close(Max_drawdown_persantge,Magic_number);
  }
//+------------------------------------------------------------------+
//|                         Main function                            |
//+------------------------------------------------------------------+
// DD:               Here it is the DD persantge, 100 means never close any order .
// Magic_Number:     Your EA magic number, enter 0 to control the all orders .
void DD_close(int DD,int Magic_Number)
  {
   if(DD(Magic_Number)>=DD)
      Close_All_V=true;
   if(Close_All_V)
      Close_All(Magic_Number);
  }
//+------------------------------------------------------------------+
//|                          Check close                             |
//+------------------------------------------------------------------+
void Check_Close(int Check_Number) // check close order
  {
   if(Check_Number<0) Print("OrderClose failed with error: ",ErrorDescription(GetLastError()));
   else Close_All_V=false;
  }
//+------------------------------------------------------------------+
//|                          Close all                               |
//+------------------------------------------------------------------+
void Close_All(int M_N)
  {
   int Loop=0;
   for(int i=0; Loop<OrdersTotal(); i++)
      if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES))
        {
         Loop++;
         if(OrderSymbol()==Symbol())
            if(OrderMagicNumber()==M_N || OrderMagicNumber()==0)
              {
               if(OrderType()==OP_BUY)
                  Check_Close(OrderClose(OrderTicket(),OrderLots(),Bid,100,clrNONE));
               if(OrderType()==OP_SELL)
                  Check_Close(OrderClose(OrderTicket(),OrderLots(),Ask,100,clrNONE));
              }
        }
  }
//+------------------------------------------------------------------+
//|                          Calculate loss                          |
//+------------------------------------------------------------------+
double Loss(int M_N)
  {
   double re=0;
   int Loop=0;
   for(int i=0; Loop<OrdersTotal(); i++)
      if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES))
        {
         Loop++;
         if(OrderSymbol()==Symbol())
            if(OrderMagicNumber()==M_N || OrderMagicNumber()==0)
               re=re+OrderProfit();
        }
   return re * -1;
  }
//+------------------------------------------------------------------+
//|                  Calculate drawdown persantage                   |
//+------------------------------------------------------------------+
double DD(int M_N)
  {
   return ( 100 / AccountBalance ( ) ) * Loss ( M_N );
  }
//+------------------------------------------------------------------+


//+------------------------------------------------------------------+
//|                                               Equity Guardian    |
//|                              Copyright © 2015, Yogie  Pratama    |
//|                                        droneox01@gmail.com       |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2015, Yogie Pratama"
#property link      "droneox01@gmail.com"
#property version   "1.00"
#property strict

#include <WinUser32.mqh>

extern string EAName="Equity Guardian";
extern string Copyright="Droneox";
extern bool CloseOrder=TRUE; //Close All Order
extern bool disableexpert=TRUE; //Disable Expert Advisor
extern double EquityTarget=999999; //Equity Take Profit (USD)
extern double EquityStop=0; //Equity Stop Loss (USD)
//+------------------------------------------------------------------+
//| check live trading and dll                                       |
//+------------------------------------------------------------------+
int OnInit()
  {
   if(!IsTradeAllowed() && !IsDllsAllowed())
     {
      Alert("Please Allow Live Trading and DLL Import");
      return(1);
     }
   else if(!IsTradeAllowed())
     {
      Alert("Please Allow Live Trading");
      return(1);
     }
   else if(!IsDllsAllowed())
     {
      Alert("Please Allow DLL Import");
      return(1);
     }
   else
     {
      return(0);
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {

  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int start()
  {
   double equity=AccountEquity();
   double accbalance=AccountBalance();

   int total=OrdersTotal();

   if(IsExpertEnabled())
     {
      if(equity<=EquityStop)
        {
         if(total!=0 && CloseOrder==TRUE)
           {
            CloseAllTrade();
           }
         if(disableexpert)
           {
            DisableEA();
           }
         Print("Equity Guardian reach equity stop level");
        }

      if(equity>=EquityTarget)
        {
         if(total!=0 && CloseOrder==TRUE)
           {
            CloseAllTrade();
           }
         if(disableexpert)
           {
            DisableEA();
           }
         Print("Equity Guardian reach equity Target level");
        }
     }
   return(0);
  }
//disable autotrading
void DisableEA() 
  {
   keybd_event(17,0,0,0);
   keybd_event(69,0,0,0);
   keybd_event(69,0,2,0);
   keybd_event(17,0,2,0);
  }
//close all open trade
int CloseAllTrade() 
  {
   int total=OrdersTotal();
   int t;
   int cnt=0;
   for(cnt=0; cnt<=total; cnt++)
     {
      bool s=OrderSelect(0,SELECT_BY_POS,MODE_TRADES);
      if(OrderType()==OP_BUY)
         t=OrderClose(OrderTicket(),OrderLots(),MarketInfo(OrderSymbol(),MODE_BID),5,Violet);
      if(OrderType()==OP_SELL)
         t=OrderClose(OrderTicket(),OrderLots(),MarketInfo(OrderSymbol(),MODE_ASK),5,Violet);
      if(OrderType()>OP_SELL) //pending orders
         t=OrderDelete(OrderTicket());
     }
   return(0);
  }
//+------------------------------------------------------------------+


//+------------------------------------------------------------------+
//|                              Copyright © 2015, Khlystov Vladimir |
//|                                         http://cmillion.narod.ru |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2015, cmillion@narod.ru"
#property link      "http://cmillion.ru"
#property strict
#property description "The Expert Advisor moves the Stop Loss in the direction of the price movement using various methods"
#property description "by candlesticks, by fractals, by ATR MA Parabolic indicators, by profit percent..."
/*

I present you the Trailing Stop that can do everything expected from a trailing stop.
◾It can trail both real and virtual.
◾By points, as usual, by SAR, ATR and MA indicators, by candlesticks and by fractals.
◾It can trail percent of the profit.
◾It can do all of that based on the net breakeven as well as on each order separately.




In the strategy tester the EA opens the order pairs by itself, so that its operation could be seen and understood.

It is even possible to run optimization and develop a new strategy based on it.
Check, test, and if you see something still missing, it will be added! 


*/
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
enum t
  {
   b=1,     // by extremums of candlesticks
   c=2,     // by fractals
   d=3,     // by ATR indicator
   e=4,     // by Parabolic indicator
   f=5,     // by MA indicator
   g=6,     // by profit % 
   i=7,     // by points
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
enum tf
  {
   af=0,     // current
   bf=1,     // 1 minute 
   cf=2,     // 5 minutes
   df=3,     // 15 minutes
   ef=4,     // 30 minutes
   ff=5,     // 1 hour
   gf=6,     // 4 hours
   hf=7,     // 1 day
  };
//+------------------------------------------------------------------+
extern bool    VirtualTrailingStop=false;  // Virtual trailing stop
input t        parameters_trailing=i;      // Trailing method
extern int     delta=50;        // Offset from the stop loss calculation level
input tf       TF_Tralling=af;  // Timeframe of the indicators (0-current)
extern int     StepTrall=1;     // Stop loss movement step
extern int     StartTrall=1;    // Minimal profit of trailing stop in points
extern bool    GeneralNoLoss=true; // Trailing from breakeven point
sinput  int     Magic=-1; // Which magic to trail (-1 all)
color   text_color=Lime;  // Information output color
sinput string Advanced_Options="";
input int     period_ATR=14;// ATR period (method 3)
input double Step=0.02;   // Parabolic Step (method 4)
input double Maximum=0.2; // Parabolic Maximum (method 4)
input int ma_period=34; // MA period (method 5)
input ENUM_MA_METHOD ma_method=MODE_SMA; // Averaging method (method 5)
input ENUM_APPLIED_PRICE applied_price=PRICE_CLOSE; // Price type (method 5)
input double PercetnProfit=50; // Percent of profit (method 6)
//---
int TF[10]={0,1,5,15,30,60,240,1440,10080,43200};
int STOPLEVEL;
string val;
double SLB=0,SLS=0;
int slippage=100;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int OnInit()
  {
   val=AccountCurrency();
   DrawLABEL(2,"cm Copyright","Copyright © 2015, http://cmillion.ru",5,10,clrGray);
   string txt;
   switch(parameters_trailing)
     {
      case 1: // by extremums of candlesticks
         txt=StringConcatenate("by candlesticks ",StrPer(TF[TF_Tralling])," +- ",delta);
         break;
      case 2: // by fractals
         txt=StringConcatenate("by fractals ",StrPer(TF[TF_Tralling])," ",StrPer(TF[TF_Tralling])," +- ",delta);
         break;
      case 3: // by ATR indicator
         txt=StringConcatenate("by ATR (",delta,") ",StrPer(TF[TF_Tralling]),"+- ",delta);
         break;
      case 4: // by Parabolic indicator
         txt=StringConcatenate("by Parabolic (",DoubleToStr(Step,2)," ",DoubleToStr(Maximum,2),") ",StrPer(TF[TF_Tralling])," +- ",delta);
         break;
      case 5: // by MA indicator
         txt=StringConcatenate("by MA (",ma_period," ",ma_method," ",applied_price,") ",StrPer(TF[TF_Tralling])," +- ",delta);
         break;
      case 6: // by profit %
         txt=StringConcatenate(" ",DoubleToStr(PercetnProfit,2),"% of profit)");
         break;
      default: // by points
         txt=StringConcatenate("by points ",delta," p");
         break;
     }
   if(VirtualTrailingStop)
     {
      if(GeneralNoLoss) DrawLABEL(3,"cm 3",StringConcatenate("Virtual trailing from beakeven ",txt),5,10,text_color);
      else DrawLABEL(3,"cm 3",StringConcatenate("Virtual trailing ",txt),5,10,text_color);
     }
   else
     {
      if(GeneralNoLoss) DrawLABEL(3,"cm 3",StringConcatenate("Trailing from breakeven ",txt),5,10,text_color);
      else DrawLABEL(3,"cm 3",StringConcatenate("Trailing ",txt),5,10,text_color);
     }

   if(Magic==-1) DrawLABEL(3,"cm 2","All Magic",5,25,text_color);
   else DrawLABEL(3,"cm 2",StringConcatenate("Magic ",Magic,"\n"),5,15,text_color);

   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick()
  {
   if(!IsTradeAllowed())
     {
      Comment("Trading forbidden ",TimeToStr(TimeCurrent(),TIME_MINUTES));
      return;
     }
   int b=0,s=0;
   double ProfitB=0,ProfitS=0,OOP,price_b=0,price_s=0,lot=0,NLb=0,NLs=0,LS=0,LB=0;
   for(int j=0; j<OrdersTotal(); j++)
     {
      if(OrderSelect(j,SELECT_BY_POS,MODE_TRADES)==true)
        {
         if((Magic==OrderMagicNumber() || Magic==-1) && OrderSymbol()==Symbol())
           {
            OOP = OrderOpenPrice();
            lot = OrderLots();
            if(OrderType()==OP_BUY ) {ProfitB+=OrderProfit()+OrderSwap()+OrderCommission();price_b += OOP*lot; LB+=lot; b++;}
            if(OrderType()==OP_SELL) {ProfitS+=OrderProfit()+OrderSwap()+OrderCommission();price_s += OOP*lot; LS+=lot; s++;}
           }
        }
     }
//----
   if(b!=0)
     {
      NLb=price_b/LB;
      ARROW("cm_NL_Buy",NLb,6,clrAqua);
     }
   if(s!=0)
     {
      NLs=price_s/LS;
      ARROW("cm_NL_Sell",NLs,6,clrRed);
     }
   DrawLABEL(1,"cm Balance",StringConcatenate("Balance ",DoubleToStr(AccountBalance(),2),val),5,20,Lime);
   DrawLABEL(1,"cm Equity",StringConcatenate("Equity ",DoubleToStr(AccountEquity(),2),val),5,35,Lime);
   DrawLABEL(1,"cm OrdersB",StringConcatenate(b," Buy ",DoubleToStr(LB,2)," ",DoubleToStr(ProfitB,2),val),5,50,Color(ProfitB>0,Lime,Red));
   DrawLABEL(1,"cm OrdersS",StringConcatenate(s," Sell ",DoubleToStr(LS,2)," ",DoubleToStr(ProfitS,2),val),5,65,Color(ProfitS>0,Lime,Red));
//----
   if(!VirtualTrailingStop) STOPLEVEL=(int)MarketInfo(Symbol(),MODE_STOPLEVEL);
   int tip,Ticket;
   bool error;
   double SL,OSL;
   int n=0;
   if(b==0) SLB=0;
   if(s==0) SLS=0;
   for(int i=OrdersTotal(); i>=0; i--)
     {
      if(OrderSelect(i,SELECT_BY_POS)==true)
        {
         tip=OrderType();
         if(tip<2 && (OrderSymbol()==Symbol()) && (OrderMagicNumber()==Magic || Magic==-1))
           {
            OSL    = OrderStopLoss();
            OOP    = OrderOpenPrice();
            Ticket = OrderTicket();
            n++;
            if(tip==OP_BUY)
              {
               if(GeneralNoLoss)
                 {
                  SL=SlLastBar(OP_BUY,Bid,NLb);
                  if(SL<NLb+StartTrall*Point) continue;
                 }
               else
                 {
                  SL=SlLastBar(OP_BUY,Bid,OOP);
                  if(SL<OOP+StartTrall*Point) continue;
                 }
               //if (OSL  >= OOP && only_NoLoss) continue;
               if(SL>=OSL+StepTrall*Point && (Bid-SL)/Point>STOPLEVEL)
                 {
                  if(VirtualTrailingStop)
                    {
                     if(SLB<SL) SLB=SL;
                     if(SLB!=0 && Bid<=SLB)
                       {
                        if(OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Digits),slippage,clrNONE)) continue;
                       }
                    }
                  else
                    {
                     error=OrderModify(Ticket,OOP,SL,OrderTakeProfit(),0,White);
                     if(!error) Comment("TrailingStop Error ",GetLastError(),"  order ",Ticket,"   SL ",SL);
                     else Comment("TrailingStop ",Ticket," ",TimeToStr(TimeCurrent(),TIME_MINUTES));
                    }
                 }
              }
            if(tip==OP_SELL)
              {
               if(GeneralNoLoss)
                 {
                  SL=SlLastBar(OP_SELL,Ask,NLs);
                  if(SL>NLs-StartTrall*Point) continue;
                 }
               else
                 {
                  SL=SlLastBar(OP_SELL,Ask,OOP);
                  if(SL>OOP-StartTrall*Point) continue;
                 }
               //if (OSL  <= OOP && only_NoLoss) continue;
               if((SL<=OSL-StepTrall*Point || OSL==0) && (SL-Ask)/Point>STOPLEVEL)
                 {
                  if(VirtualTrailingStop)
                    {
                     if(SLS==0 || SLS>SL) SLS=SL;
                     if(SLS!=0 && Ask>=SLS)
                       {
                        if(OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Digits),slippage,clrNONE)) continue;
                       }
                    }
                  else
                    {
                     error=OrderModify(Ticket,OOP,SL,OrderTakeProfit(),0,White);
                     if(!error) Comment("TrailingStop Error ",GetLastError(),"  order ",Ticket,"   SL ",SL);
                     else Comment("TrailingStop ",Ticket," ",TimeToStr(TimeCurrent(),TIME_MINUTES));
                    }
                 }
              }
           }
        }
     }
//---
   if(IsTesting())
     {
      if(b==0 || s==0)
        {
         if(AccountFreeMarginCheck(Symbol(),OP_BUY,0.1)+AccountFreeMarginCheck(Symbol(),OP_SELL,0.1)>0)
           {
            if(OrderSend(Symbol(),OP_BUY,0.1,NormalizeDouble(Ask,Digits),slippage,0,0,NULL,0,0,clrBlue)==-1)
               Print("Error OrderSend ",GetLastError());
            if(OrderSend(Symbol(),OP_SELL,0.1,NormalizeDouble(Bid,Digits),slippage,0,0,NULL,0,0,clrRed)==-1)
               Print("Error OrderSend ",GetLastError());
           }
        }
     }
   return;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   if(IsTesting()) return;
   string PN="cm";
   for(int i=ObjectsTotal()-1; i>=0; i--)
     {
      string Obj_Name=ObjectName(i);
      if(StringFind(Obj_Name,PN,0)!=-1)
        {
         ObjectDelete(Obj_Name);
        }
     }
   Comment("");
   return;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
double SlLastBar(int tip,double price,double OOP)
  {
   double prc=0;
   int i;
   switch(parameters_trailing)
     {
      case 1: // by extremums of candlesticks
         if(tip==OP_BUY)
           {
            for(i=1; i<500; i++)
              {
               prc=NormalizeDouble(iLow(Symbol(),TF[TF_Tralling],i)-delta*Point,Digits);
               if(prc!=0) if(price-STOPLEVEL*Point>prc) break;
               else prc=0;
              }
            ARROW("cm_SL_Buy",prc,4,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy candle ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            for(i=1; i<500; i++)
              {
               prc=NormalizeDouble(iHigh(Symbol(),TF[TF_Tralling],i)+delta*Point,Digits);
               if(prc!=0) if(price+STOPLEVEL*Point<prc) break;
               else prc=0;
              }
            ARROW("cm_SL_Sell",prc,4,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell candle ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;

      case 2: // by fractals
         if(tip==OP_BUY)
           {
            for(i=1; i<100; i++)
              {
               prc=iFractals(Symbol(),TF[TF_Tralling],MODE_LOWER,i);
               if(prc!=0)
                 {
                  prc=NormalizeDouble(prc-delta*Point,Digits);
                  if(price-STOPLEVEL*Point>prc) break;
                 }
               else prc=0;
              }
            ARROW("cm_SL_Buy",prc,218,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy Fractals ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            for(i=1; i<100; i++)
              {
               prc=iFractals(Symbol(),TF[TF_Tralling],MODE_UPPER,i);
               if(prc!=0)
                 {
                  prc=NormalizeDouble(prc+delta*Point,Digits);
                  if(price+STOPLEVEL*Point<prc) break;
                 }
               else prc=0;
              }
            ARROW("cm_SL_Sell",prc,217,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell Fractals ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;
      case 3: // by ATR indicator
         if(tip==OP_BUY)
           {
            prc=NormalizeDouble(Bid-iATR(Symbol(),TF[TF_Tralling],period_ATR,0)-delta*Point,Digits);
            ARROW("cm_SL_Buy",prc,4,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy ATR ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            prc=NormalizeDouble(Ask+iATR(Symbol(),TF[TF_Tralling],period_ATR,0)+delta*Point,Digits);
            ARROW("cm_SL_Sell",prc,4,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell ATR ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;

      case 4: // by Parabolic indicator
         prc=iSAR(Symbol(),TF[TF_Tralling],Step,Maximum,0);
         if(tip==OP_BUY)
           {
            prc=NormalizeDouble(prc-delta*Point,Digits);
            if(price-STOPLEVEL*Point<prc) prc=0;
            ARROW("cm_SL_Buy",prc,4,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy Parabolic ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            prc=NormalizeDouble(prc+delta*Point,Digits);
            if(price+STOPLEVEL*Point>prc) prc=0;
            ARROW("cm_SL_Sell",prc,4,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell Parabolic ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;

      case 5: // by MA indicator
         prc=iMA(NULL,TF[TF_Tralling],ma_period,0,ma_method,applied_price,0);
         if(tip==OP_BUY)
           {
            prc=NormalizeDouble(prc-delta*Point,Digits);
            if(price-STOPLEVEL*Point<prc) prc=0;
            ARROW("cm_SL_Buy",prc,4,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy MA ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            prc=NormalizeDouble(prc+delta*Point,Digits);
            if(price+STOPLEVEL*Point>prc) prc=0;
            ARROW("cm_SL_Sell",prc,4,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell MA ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;
      case 6: // % of profit
         if(tip==OP_BUY)
           {
            prc=NormalizeDouble(OOP+(price-OOP)/100*PercetnProfit,Digits);
            ARROW("cm_SL_Buy",prc,4,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy % ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            prc=NormalizeDouble(OOP-(OOP-price)/100*PercetnProfit,Digits);
            ARROW("cm_SL_Sell",prc,4,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell % ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;
      default: // by points
         if(tip==OP_BUY)
           {
            prc=NormalizeDouble(price-delta*Point,Digits);
            ARROW("cm_SL_Buy",prc,4,clrAqua);
            DrawLABEL(1,"cm SL Buy",StringConcatenate("SL Buy pips ",DoubleToStr(prc,Digits)),5,100,text_color);
           }
         if(tip==OP_SELL)
           {
            prc=NormalizeDouble(price+delta*Point,Digits);
            ARROW("cm_SL_Sell",prc,4,clrRed);
            DrawLABEL(1,"cm SL Sell",StringConcatenate("SL Sell pips ",DoubleToStr(prc,Digits)),5,120,text_color);
           }
         break;
     }
   return(prc);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
string StrPer(int per)
  {
   if(per == 0) per=Period();
   if(per == 1) return("M1");
   if(per == 5) return("M5");
   if(per == 15) return("M15");
   if(per == 30) return("M30");
   if(per == 60) return("H1");
   if(per == 240) return("H4");
   if(per == 1440) return("D1");
   if(per == 10080) return("W1");
   if(per == 43200) return("MN1");
   return("timeframe error");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void ARROW(string Name,double Price,int ARROWCODE,color c)
  {
   ObjectDelete(Name);
   ObjectCreate(Name,OBJ_ARROW,0,Time[0],Price,0,0,0,0);
   ObjectSetInteger(0,Name,OBJPROP_ARROWCODE,ARROWCODE);
   ObjectSetInteger(0,Name,OBJPROP_SELECTABLE,false);
   ObjectSetInteger(0,Name,OBJPROP_SELECTED,false);
   ObjectSetInteger(0,Name,OBJPROP_COLOR,c);
   ObjectSetInteger(0,Name,OBJPROP_WIDTH,1);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void DrawLABEL(int c,string name,string Name,int X,int Y,color clr)
  {
   if(ObjectFind(name)==-1)
     {
      ObjectCreate(name,OBJ_LABEL,0,0,0);
      ObjectSet(name,OBJPROP_CORNER,c);
      ObjectSet(name,OBJPROP_XDISTANCE,X);
      ObjectSet(name,OBJPROP_YDISTANCE,Y);
      ObjectSetInteger(0,name,OBJPROP_SELECTABLE,false);
      ObjectSetInteger(0,name,OBJPROP_SELECTED,false);
     }
   ObjectSetText(name,Name,10,"Arial",clr);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
color Color(bool P,color a,color b)
  {
   if(P) return(a);
   return(b);
  }
//+------------------------------------------------------------------+


//+------------------------------------------------------------------+
//|                                        virtual_trailing_stop.mq4 |
//|                                         Copyright 2015, cmillion |
//|                                               http://cmillion.ru |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2015, cmillion@narod.ru"
#property link      "http://cmillion.ru"
#property strict
//---
input int     Stoploss      = 0;    // Стоп-лосс
input int     Takeprofit    = 0;    // Тейк-профит
input int     TrailingStop  = 5;    // Длина трала
input int     TrailingStart = 5;    // Минимальная прибыль для старта
input int     TrailingStep  = 1;    // Шаг трала
double   TrallB = 0;
double   TrallS = 0;
int      slippage=30;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int start()
  {
   double OOP,SL;
   int b=0,s=0,tip,TicketB=0,TicketS=0;
   for(int i=0; i<OrdersTotal(); i++)
     {
      if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES))
        {
         if(OrderSymbol()==Symbol())
           {
            tip = OrderType();
            OOP = NormalizeDouble(OrderOpenPrice(),Digits);
            if(tip==OP_BUY)
              {
               b++;
               TicketB=OrderTicket();
               if(Stoploss!=0   && Bid<=OOP - Stoploss   * Point) {if(OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Digits),slippage,clrNONE)) continue;}
               if(Takeprofit!=0 && Bid>=OOP + Takeprofit * Point) {if(OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Digits),slippage,clrNONE)) continue;}
               if(TrailingStop>0)
                 {
                  SL=NormalizeDouble(Bid-TrailingStop*Point,Digits);
                  if(SL>=OOP+TrailingStart*Point && (TrallB==0 || TrallB+TrailingStep*Point<SL)) TrallB=SL;
                 }
              }
            if(tip==OP_SELL)
              {
               s++;
               if(Stoploss!=0   && Ask>=OOP + Stoploss   * Point) {if(OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Digits),slippage,clrNONE)) continue;}
               if(Takeprofit!=0 && Ask<=OOP - Takeprofit * Point) {if(OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Digits),slippage,clrNONE)) continue;}
               TicketS=OrderTicket();
               if(TrailingStop>0)
                 {
                  SL=NormalizeDouble(Ask+TrailingStop*Point,Digits);
                  if(SL<=OOP-TrailingStart*Point && (TrallS==0 || TrallS-TrailingStep*Point>SL)) TrallS=SL;
                 }
              }
           }
        }
     }
   if(b!=0)
     {
      if(b>1) Comment("Трал корректно работает только с 1 ордером");
      else
      if(TrallB!=0)
        {
         Comment("Тралим ордер ",TicketB);
         DrawHline("SL Buy",TrallB,clrBlue,1);
         if(Bid<=TrallB)
           {
            if(OrderSelect(TicketB,SELECT_BY_TICKET))
               if(OrderProfit()>0)
                  if(!OrderClose(TicketB,OrderLots(),NormalizeDouble(Ask,Digits),slippage,clrRed))
                     Comment("Ошибка закрытия ордера ",GetLastError());
           }
        }
     }
   else {TrallB=0;ObjectDelete("SL Buy");}
//---
   if(s!=0)
     {
      if(s>1) Comment("Трал корректно работает только с 1 ордером");
      else
      if(TrallS!=0)
        {
         Comment("Тралим ордер ",TicketS);
         DrawHline("SL Sell",TrallS,clrRed,1);
         if(Ask>=TrallS)
           {
            if(OrderSelect(TicketS,SELECT_BY_TICKET))
               if(OrderProfit()>0)
                  if(!OrderClose(TicketS,OrderLots(),NormalizeDouble(Ask,Digits),slippage,clrRed))
                     Comment("Ошибка закрытия ордера ",GetLastError());
           }
        }
     }
   else {TrallS=0;ObjectDelete("SL Sell");}
//---
   int err;
   if(IsTesting() && OrdersTotal()==0)
     {
      double Lot=0.1;
      err=OrderSend(Symbol(),OP_BUY,Lot,NormalizeDouble(Ask,Digits),slippage,0,0,"тест",0);
      err=OrderSend(Symbol(),OP_SELL,Lot,NormalizeDouble(Bid,Digits),slippage,0,0,"тест",0);
      return(0);
     }
   return(0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void DrawHline(string name,double P,color clr,int WIDTH)
  {
   if(ObjectFind(name)!=-1) ObjectDelete(name);
   ObjectCreate(name,OBJ_HLINE,0,0,P,0,0,0,0);
   ObjectSet(name,OBJPROP_COLOR,clr);
   ObjectSet(name,OBJPROP_STYLE,2);
   ObjectSet(name,OBJPROP_WIDTH,WIDTH);
  }
//+------------------------------------------------------------------+


//+------------------------------------------------------------------+
//|                                                   Zakryvator.mq4 |
//|                                 Copyright 2015-2016, Assurkov.ru |
//|                                           http://www.assurkov.ru
/*The EA closes orders, opened in the terminal, when certain loss is reached by them in the account currency. Including the orders opened for all other pairs on which the adviser is not installed. It can be put on any currency pair and it will keep track of all open orders.

It is possible to vary the loss by the volume of the order. For example, by default the maximum loss equals 4 (min_001_002=4) for orders with the volume of 0,01-0,02 lots (if the account is in dollars, then $4), and equals 30 (min_from1=30) at order volumes greater than 1 lot.

Attention: possible slippage depending on the market conditions, as well as the broker!

Input Parameters:
◾min_001_002 — The maximum loss per orders with volumes starting from 0,01 (inclusive) to 0,02 lots (inclusive);
◾min_002_005 — The maximum loss per orders with volumes starting from 0,02 (inclusive) to 0,05 lots (inclusive);
◾min_005_01 — The maximum loss per orders with volumes starting from 0,05 (inclusive) to 0,1 lots (inclusive);
◾min_01_03 — The maximum loss per orders with volumes starting from 0,1 (inclusive) to 0,3 lots (inclusive);
◾min_03_05 — The maximum loss per orders with volumes starting from 0,3 (inclusive) to 0,5 lots (inclusive);
◾min_05_1 — The maximum loss per orders with volumes starting from 0,5 (inclusive) to 1 lot (inclusive);
◾min_from1 — The maximum loss per orders with volumes greater than 1 lot;
 */
 |
//+------------------------------------------------------------------+
#property strict
#property copyright "Copyright © 2015-2016, www.assurkov.ru, Alexey Surkov"
#property link      "http://www.assurkov.ru/"
#property version     "1.00"

input double min_001_002 = 4;
input double min_002_005 = 8;
input double min_005_01  = 10;
input double min_01_03   = 15;
input double min_03_05   = 20;
input double min_05_1    = 25;
input double min_from1   = 30;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick()
  {
   int _GetLastError=0,Dig,_OrdersTotal=OrdersTotal();
   for(int z=_OrdersTotal-1; z>=0; z --)
     {
      if(!OrderSelect(z,SELECT_BY_POS))
        {
         _GetLastError=GetLastError();
         Print("OrderSelect( ",z,", SELECT_BY_POS ) - Error #",_GetLastError);
         continue;
        }

      //   min_001_002  
      if(OrderLots()<=0.02 && OrderProfit()<=min_001_002*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_002_005
      if((OrderLots()>0.02 && OrderLots()<=0.05) && OrderProfit()<=min_002_005*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_005_01
      if((OrderLots()>0.05 && OrderLots()<=0.10) && OrderProfit()<=min_005_01*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_01_03
      if((OrderLots()>0.10 && OrderLots()<=0.30) && OrderProfit()<=min_01_03*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_03_05
      if((OrderLots()>0.30 && OrderLots()<=0.50) && OrderProfit()<=min_03_05*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),10,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_05_1
      if((OrderLots()>0.50 && OrderLots()<=1) && OrderProfit()<=min_05_1*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_from1
      if((OrderLots()>1) && OrderProfit()<=min_from1*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }
     }
  }
//+------------------------------------------------------------------+

