//+------------------------------------------------------------------+
//|                                         exp_breakout_signals.mq5 |
//|                                                         Tapochun |
//|                           https://www.mql5.com/ru/users/tapochun |
//+------------------------------------------------------------------+
#property copyright "Tapochun"
#property link      "https://www.mql5.com/ru/users/tapochun"
#property version   "1.00"
//+------------------------------------------------------------------+
//| Version |
//+------------------------------------------------------------------+
//--- 1.00: Working Version;
//+------------------------------------------------------------------+
//| Global Changes |
//+------------------------------------------------------------------+
enum ENUM_ADD_MODE      // Enumeration - process of adding objects
  {
   ADD_MODE_ALL,         // Add all lines
   ADD_MODE_MESSAGE      // Add lines  by request
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
enum ENUM_SIGNAL_MODE   // Enumeration - process of acquiring signal
  {
   SIGNAL_MODE_SOUND,   // Sound Signal
   SIGNAL_MODE_ALERT,   // On-Screen Alert
   SIGNAL_MODE_PUSH,// push-notifications on phone
   SIGNAL_MODE_MAIL      // email notifications
  };
//+------------------------------------------------------------------+
//| Input parameters |
//+------------------------------------------------------------------+
sinput   string             inpPrefix="bot_";               // prefix working lines
sinput   ENUM_ADD_MODE      inpAddMode=ADD_MODE_MESSAGE;       // Process to add lines
sinput   ENUM_SIGNAL_MODE    inpSignalMode=SIGNAL_MODE_SOUND;  // Signal Mode
sinput   string            inpSoundName="Alert2.wav";         // Name of sound file (for sound file process)
sinput   bool               inpDelObjects=false;               // Delete work lines when deleting an expert?
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- Verifying input parameters
   if(!CheckInputParameters())       // If the check is completed
      return(INIT_FAILED);             // Exit with mistake
//--- Underwrite event by creating a graph. Object
   ChartSetInteger(0,CHART_EVENT_OBJECT_CREATE,true);
//---
   return( INIT_SUCCEEDED );
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason) // Reason for code de-initialization
  {
//--- Check necessity to delete working lines
   if(reason==REASON_REMOVE && inpDelObjects) // If the expert is deleted from the chart and you need to delete the work lines
     {
      ObjectsDeleteAll( 0, inpPrefix, 0, OBJ_HLINE );   // Delete working horizontal lines
      ChartRedraw();                                    // Rewrite the chart
     }
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//--- Stored opening time of the bar
   static datetime savedTime=0;
//--- Check the formation of a new bar
   if(!IsNewBar(savedTime))         // If the bar is not formed
      return;                           // Exit
//--- Verify the signal
   CheckSignals();
  }
//+------------------------------------------------------------------+
//| Check signal creation |
//+------------------------------------------------------------------+
void CheckSignals()
  {
//--- The total number of lines in the main window
   const int total=ObjectsTotal(0,0,OBJ_HLINE);
   if(total<=0) // If there are no lines
      return;                              // Exit
//--- Receive opening and closing prices with the formation of candles
   MqlRates rates[2];                  // Massif - successor
//--- Received..
   if(CopyRates(_Symbol,_Period,0,2,rates)!=2) // If no data is received
     {
      Print(__FUNCTION__,": Error #",GetLastError(),": Two last given candles!");
      return;                                                // exit with error
     }
//--- string name
   string name;
//--- string text
   string text;
//--- price as double
   double price;
//--- Line cycle
   for(int i=0; i<total; i++)
     {
      //--- String name
      name=ObjectName(0,i,0,OBJ_HLINE);
      //--- Check whether the line refers to objects
      if(StringFind(name,inpPrefix)<0) // If the substring is not found
         continue;                              // Go to the next object
      //--- Determine line price
      price=ObjectGetDouble(0,name,OBJPROP_PRICE,0);
      //--- Check if the line crossed the previous candle
      if(( rates[0].high>price && rates[0].low<price) || // If there was a cross when forming the candle or...
         (rates[ 1 ].open > price && rates[ 0 ].open < price ) ||      // ..if there was a cross with a high gap or..
         ( rates[ 1 ].open < price && rates[ 0 ].open > price ) )      // ..if there was a cross with a low gap
        {
         //--- Form a text message
         text=_Symbol+": Line crossing! '"+name+"'";
         //--- The line was crossed! Signal!
         switch(inpSignalMode) // Depending on what type of signal
           {
            case SIGNAL_MODE_SOUND:                  // Sound signal
               if(!PlaySound(inpSoundName))
               Print(__FUNCTION__,": ERROR #",GetLastError(),": sound file '"+inpSoundName+"' No reproduction!");
               break;
            case SIGNAL_MODE_ALERT:                  // Alert
               Alert(text);
               break;
            case SIGNAL_MODE_PUSH:                     // Push-Notification
               if(!SendNotification(text))
               Print(__FUNCTION__,": ERROR #",GetLastError(),": push-notification not sent!");
               break;
            case SIGNAL_MODE_MAIL:                     // Mail Notification
               if(!SendMail("MT5: "+text,text))
               Print(__FUNCTION__,": ERROR #",GetLastError(),": Mail Notification not sent!");
               break;
            default:                                 // Unknown type of signal
               Print(__FUNCTION__,": ERROR! Unknown type of signal '"+EnumToString(inpSignalMode)+"'");
               return;
           }
        }
     }
  }
//+------------------------------------------------------------------+
//| ChartEvent function                                              |
//+------------------------------------------------------------------+
void OnChartEvent(const int id,
                  const long &lparam,
                  const double &dparam,
                  const string &sparam)
  {
   if(id==CHARTEVENT_OBJECT_CREATE)
     {
      //--- Check graph type. Object
      if(ObjectGetInteger(0,sparam,OBJPROP_TYPE)!=OBJ_HLINE) // If the line created is not horizontal
         return;                                                         // Exit
      //--- New Graph Name. Object
      const string name=inpPrefix+sparam;
      //--- Check, which method is needed to add object
      if( inpAddMode == ADD_MODE_ALL )                        // If while working it adds all horizontal lines
         ObjectSetString( 0,sparam,OBJPROP_NAME,name );      // Add to the string name postfix
      else if( inpAddMode == ADD_MODE_MESSAGE )               // If while working it adds horizontal lines by request
        {
         //--- Set parameters for text and header for window
         const string text="Добавить линию '"+sparam+"' к списку рабочих?";
         const string caption="Добавление линии";
         //--- Launch box. Receive answer
         const int answer=MessageBox(text,caption,MB_YESNO|MB_ICONQUESTION);
         if( answer == IDYES )                                 // If you press yes
            ObjectSetString( 0,sparam,OBJPROP_NAME, name );   // Add to the string name postfix
        }
      //--- Rewrite chart
      ChartRedraw();
     }
  }
//+------------------------------------------------------------------+
//| Verifying input parameters |
//+------------------------------------------------------------------+
bool CheckInputParameters()
  {
//--- Verifiying permissions for some of the singal modes
   switch(inpSignalMode) // Depending on which signal
     {
      case SIGNAL_MODE_SOUND:            // If sound notification
         if(inpSoundName=="") // If the name of the file is not shown
           {
            Print(__FUNCTION__,": ERROR! You need to select a sound file name!");
            return(false);               // Exit from error
           }
         else                            // If the name is shown
         return(true);               // Return true
      case SIGNAL_MODE_ALERT:            // If notification with alert
         return( true );                  // Return true
      case SIGNAL_MODE_PUSH:               // If notifications push-notifications
         if(!TerminalInfoInteger(TERMINAL_NOTIFICATIONS_ENABLED))   // If push-notification are disconnected
           {
            Print(__FUNCTION__,": ERROR! You need to allow push notifications on telephone!");
            return(false);               // Exit with error
           }
         else                            // If push notifications are allowed
         return(true );               // Return true
      case SIGNAL_MODE_MAIL:               // If mail notifications
         if(!TerminalInfoInteger(TERMINAL_EMAIL_ENABLED))            // If mail notifications are turned off
           {
            Print(__FUNCTION__,": ERROR! You need to allow mail notifications!");
            return(false);               // Exit from error
           }
         else                            // If mail notifications are turned on
         return(true);               // Return true
      default:                            // If this is an unknown signal mode
         Print(__FUNCTION__,": ERROR! Unknown signal mode '"+EnumToString(inpSignalMode)+"'");
         return(false);                  // Exit from error
     }
//--- If all checks are completed
   return(true);                        // Return true
  }
//+------------------------------------------------------------------+
//| Verification of new bar formation |
//+------------------------------------------------------------------+
bool IsNewBar(datetime &savedTime) // Stored opening time. (out)
  {
//--- Receive time to open last bar by symbol/period
   datetime currentTime=(datetime)SeriesInfoInteger(_Symbol,_Period,SERIES_LASTBAR_DATE);
//---
   if(savedTime==0) // If first launch
     {
      savedTime = currentTime;            // Set time for bar formation
      return(false );                     // Return false
     }
   else                                  // If not the first launch
     {
      if(savedTime!=currentTime) // If stored time isnt the same time of opening current candles
        {
         savedTime = currentTime;         // Save time as opening current candle
         return( true );                  // Return true
        }
      else                               // If the saved time coincides with the current time
      return(false);                  // return false
     }
  }
//+------------------------------------------------------------------+