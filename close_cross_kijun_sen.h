
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

