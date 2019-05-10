struct CHANNEL {
  double    d_High;           // The price of the upper range border
  double    d_Low;            // The price of the lower range border
  datetime  t_From;           // The date/time of the first (oldest) bar of the channel
  datetime  t_To;             // The date/time of the last bar of the channel
  int       i_Highest_Offset; // The number of bars to the right of the High
  int       i_Lowest_Offset;  // The number of bars to the right of the Low
};
void f_Set(int i_Bars_Limit, int i_Newest_Bar_Shift = 1) {
  double da_Price_Array[]; // An auxiliary array for the High/Low prices of all bars of the channel
  
  // Determining the upper border of the channel:
  
  int i_Price_Bars = CopyHigh(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, da_Price_Array);
  int i_Bar = ArrayMaximum(da_Price_Array);
  d_High = da_Price_Array[i_Bar]; // The upper channel of the range is determined
  i_Highest_Offset = i_Price_Bars - i_Bar; // The age of the High (in bars)
  
  // Determining the lower border of the range:
  
  i_Price_Bars = CopyLow(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, da_Price_Array);
  i_Bar = ArrayMinimum(da_Price_Array);
  d_Low = da_Price_Array[i_Bar]; // The lower channel of the range is determined
  i_Lowest_Offset = i_Price_Bars - i_Bar; // The age of the Low (in bars)
  
  datetime ta_Time_Array[];
  i_Price_Bars = CopyTime(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, ta_Time_Array);
  t_From = ta_Time_Array[0];
  t_To = ta_Time_Array[i_Price_Bars - 1];
}
enum ENUM_LOG_LEVEL { // The list of logging levels
  LOG_LEVEL_NONE,     // Logging disabled
  LOG_LEVEL_ERR,      // Only error information
  LOG_LEVEL_INFO,     // Errors + robot's comments
  LOG_LEVEL_DEBUG     // Everything
};

void f_Set(int i_Bars_Limit, int i_Newest_Bar_Shift = 1) {
  double da_Price_Array[]; // An auxiliary array for the High/Low prices of all bars of the channel
  
  // Determining the upper border of the channel:
  
  int i_Price_Bars = CopyHigh(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, da_Price_Array);
  
  if(i_Price_Bars == WRONG_VALUE) {
    // Handling the CopyHigh function error
    if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyHigh: error #%u", __FUNCSIG__, _LastError);
    return;
  }
  
  if(i_Price_Bars < i_Bars_Limit) {
    // The CopyHigh function has not retrieved the required data amount
    if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyHigh: copied %u bars of %u", __FUNCSIG__, i_Price_Bars, i_Bars_Limit);
    return;
  }
  
  int i_Bar = ArrayMaximum(da_Price_Array);
  if(i_Bar == WRONG_VALUE) {
    // Handling the ArrayMaximum function error
    if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: ArrayMaximum: error #%u", __FUNCSIG__, _LastError);
    return;
  }
  
  d_High = da_Price_Array[i_Bar]; // The upper channel of the range is determined
  i_Highest_Offset = i_Price_Bars - i_Bar; // The age of the High (in bars)
  
  // Determining the lower border of the range:
  
  i_Price_Bars = CopyLow(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, da_Price_Array);
  
  if(i_Price_Bars == WRONG_VALUE) {
    // Handling the CopyLow function error
    if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyLow: error #%u", __FUNCSIG__, _LastError);
    return;
  }
  
  if(i_Price_Bars < i_Bars_Limit) {
    // The CopyLow function has not retrieved the required data amount
    if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyLow: copied %u bars of %u", __FUNCSIG__, i_Price_Bars, i_Bars_Limit);
    return;
  }
  
  i_Bar = ArrayMinimum(da_Price_Array);
  if(i_Bar == WRONG_VALUE) {
    // Handling the ArrayMinimum function error
    if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: ArrayMinimum: error #%u", __FUNCSIG__, _LastError);
    return;
  }
  d_Low = da_Price_Array[i_Bar]; // The lower channel of the range is determined
  i_Lowest_Offset = i_Price_Bars - i_Bar; // The age of the Low (in bars)
  
  datetime ta_Time_Array[];
  i_Price_Bars = CopyTime(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, ta_Time_Array);
  if(i_Price_Bars < 1) t_From = t_To = 0;
  else {
    t_From = ta_Time_Array[0];
    t_To = ta_Time_Array[i_Price_Bars - 1];
  }
}

// Channel information and functions for collecting and updating it, in one structure
struct CHANNEL {
  // Variables
  double    d_High;           // The price of the upper range border
  double    d_Low;            // The price of the lower range border
  datetime  t_From;           // The date/time of the first (oldest) bar of the channel
  datetime  t_To;             // The date/time of the last bar of the channel
  int       i_Highest_Offset; // The number of bars to the right of the High
  int       i_Lowest_Offset;  // The number of bars to the right of the Low
  bool      b_Ready;          // Is the parameters update procedure over?
  bool      b_Updated;        // Have the channel parameters changed?
  string    s_Signature;      // The signature of the last known set of data
  
  // Functions:
  
  CHANNEL() {
    d_High = d_Low = 0;
    t_From = t_To = 0;
    b_Ready = b_Updated = false;
    s_Signature = "-";
    i_Highest_Offset = i_Lowest_Offset = WRONG_VALUE;
  }
  
  void f_Set(int i_Bars_Limit, int i_Newest_Bar_Shift = 1) {
    b_Ready = false; // Pitstop: set a service flag
    
    double da_Price_Array[]; // An auxiliary array for the High/Low prices of all bars of the channel
    
    // Determining the upper border of the channel:
    
    int i_Price_Bars = CopyHigh(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, da_Price_Array);
    if(i_Price_Bars == WRONG_VALUE) {
      // Handling the CopyHigh function error
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyHigh: error #%u", __FUNCSIG__, _LastError);
      return;
    }
    
    if(i_Price_Bars < i_Bars_Limit) {
      // The CopyHigh function has not retrieved the required data amount
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyHigh: copied %u bars of %u", __FUNCSIG__, i_Price_Bars, i_Bars_Limit);
      return;
    }
    
    int i_Bar = ArrayMaximum(da_Price_Array);
    if(i_Bar == WRONG_VALUE) {
      // Handling the ArrayMaximum function error
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: ArrayMaximum: error #%u", __FUNCSIG__, _LastError);
      return;
    }
    
    d_High = da_Price_Array[i_Bar]; // The upper channel of the range is determined
    i_Highest_Offset = i_Price_Bars - i_Bar; // The age of the High (in bars)
    
    // Determining the lower border of the range:
    
    i_Price_Bars = CopyLow(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, da_Price_Array);
    
    if(i_Price_Bars == WRONG_VALUE) {
      // Handling the CopyLow function error
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyLow: error #%u", __FUNCSIG__, _LastError);
      return;
    }
    
    if(i_Price_Bars < i_Bars_Limit) {
      // The CopyLow function has not retrieved the required data amount
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyLow: copied %u bars of %u", __FUNCSIG__, i_Price_Bars, i_Bars_Limit);
      return;
    }
    
    i_Bar = ArrayMinimum(da_Price_Array);
    if(i_Bar == WRONG_VALUE) {
      // Handling the ArrayMinimum function error
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: ArrayMinimum: error #%u", __FUNCSIG__, _LastError);
      return;
    }
    d_Low = da_Price_Array[i_Bar]; // The lower channel of the range is determined
    i_Lowest_Offset = i_Price_Bars - i_Bar; // The age of the Low (in bars)
    
    datetime ta_Time_Array[];
    i_Price_Bars = CopyTime(_Symbol, PERIOD_CURRENT, i_Newest_Bar_Shift, i_Bars_Limit, ta_Time_Array);
    if(i_Price_Bars < 1) t_From = t_To = 0;
    else {
      t_From = ta_Time_Array[0];
      t_To = ta_Time_Array[i_Price_Bars - 1];
    }
    
    string s_New_Signature = StringFormat("%.5f%.5f%u%u", d_Low, d_High, t_From, t_To);
    if(s_Signature != s_New_Signature) {
      // Channel data have changed
      b_Updated = true;
      if(Log_Level > LOG_LEVEL_ERR) PrintFormat("%s: Channel updated: %s .. %s / %s .. %s, min: %u max: %u ", __FUNCTION__, DoubleToString(d_Low, _Digits), DoubleToString(d_High, _Digits), TimeToString(t_From, TIME_DATE|TIME_MINUTES), TimeToString(t_To, TIME_DATE|TIME_MINUTES), i_Lowest_Offset, i_Highest_Offset);
      s_Signature = s_New_Signature;
    }
    
    b_Ready = true; // Data update successfully completed
  }
};



enum ENUM_STRATEGY {     // The list of strategies
  TS_TURTLE_SOUP,        // Turtle Soup
  TS_TURTLE_SOUP_PLUS_1  // Turtle Soup Plus One
};
input ENUM_STRATEGY  Strategy_Type = TS_TURTLE_SOUP;  // Trading Strategy:

enum ENUM_ENTRY_SIGNAL {  // The list of entry signals
  ENTRY_BUY,              // A Buy signal
  ENTRY_SELL,             // A Sell signal
  ENTRY_NONE,             // No signal
  ENTRY_UNKNOWN           // An indefinite status
};
MqlTick go_Tick; // Information about the last known tick

ENUM_ENTRY_SIGNAL fe_Get_Entry_Signal(
  bool b_Wait_For_Bar_Close = false,
  int i_Extremum_Bars = 3
) {}

if(go_Channel.i_Highest_Offset > i_Extremum_Bars) // 1st condition
  if(go_Channel.d_High < d_Actual_Price) // 2nd condition
    return(ENTRY_SELL); // Both Sell conditions are met
if(go_Channel.i_Lowest_Offset > i_Extremum_Bars) // 1st condition
  if(go_Channel.d_Low > d_Actual_Price) { // 2nd condition
    return(ENTRY_BUY); // Both Buy conditions are met
double d_Actual_Price = go_Tick.bid; // The default price - for the Turtle Soup version
if(b_Wait_For_Bar_Close) { // for the Turtle Soup Plus One version
  double da_Price_Array[1]; // An auxiliary array
  CopyClose(_Symbol, PERIOD_CURRENT, 1, 1, da_Price_Array));
  d_Actual_Price = da_Price_Array[0];
}

ENUM_ENTRY_SIGNAL fe_Get_Entry_Signal(bool b_Wait_For_Bar_Close = false, int i_Extremum_Bars = 3) {
  double d_Actual_Price = go_Tick.bid; // The default price - for the Turtle Soup version
  if(b_Wait_For_Bar_Close) { // for the Turtle Soup Plus One version
    double da_Price_Array[1];
    CopyClose(_Symbol, PERIOD_CURRENT, 1, 1, da_Price_Array));
    d_Actual_Price = da_Price_Array[0];
  }
  
  // Upper limit:
  if(go_Channel.i_Highest_Offset > i_Extremum_Bars) // 1st condition
    if(go_Channel.d_High < d_Actual_Price) { // 2nd condition
      // The price has broken the upper limit
      return(ENTRY_SELL);
    }
  
  // lower limit:
  if(go_Channel.i_Lowest_Offset > i_Extremum_Bars) // 1st condition
    if(go_Channel.d_Low > d_Actual_Price) { // 2nd condition
      // The price has broken the lower limit
      return(ENTRY_BUY);
    }
  
  return(ENTRY_NONE);
}

ENUM_ENTRY_SIGNAL fe_Get_Entry_Signal(bool b_Wait_For_Bar_Close = false, int i_Extremum_Bars = 3) {
  static datetime st_Pause_End = 0; // The time of the next check
  if(st_Pause_End > go_Tick.time) return(ENTRY_NONE);
  st_Pause_End = 0;
  
  if(go_Channel.b_In_Process) {
    // Channel data are not prepared for use
    if(Log_Level == LOG_LEVEL_DEBUG) PrintFormat("%s: Channel parameters are not prepared", __FUNCTION__);
    return(ENTRY_UNKNOWN);
  }
  if(go_Channel.i_Lowest_Offset < i_Extremum_Bars && go_Channel.i_Highest_Offset < i_Extremum_Bars) {
    // The 1st condition is not met
    if(Log_Level == LOG_LEVEL_DEBUG) PrintFormat("%s: the 1st condition is not met", __FUNCTION__);
    
    // Pause until the channel is updated
    st_Pause_End = go_Tick.time + PeriodSeconds() - go_Tick.time % PeriodSeconds();
    
    return(ENTRY_NONE);
  }
  
  double d_Actual_Price = go_Tick.bid; // The default price - for the Turtle Soup version
  if(b_Wait_For_Bar_Close) { // for the Turtle Soup Plus One version
    double da_Price_Array[1];
    if(WRONG_VALUE == CopyClose(_Symbol, PERIOD_CURRENT, 1, 1, da_Price_Array)) {
      // Handling the error of the CopyClose function
      if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyClose: error #%u", __FUNCSIG__, _LastError);
      return(ENTRY_NONE);
    }
    d_Actual_Price = da_Price_Array[0];
  }
  
  // Upper limit:
  if(go_Channel.i_Highest_Offset > i_Extremum_Bars) // 1st condition
    if(go_Channel.d_High < d_Actual_Price) { // 2nd condition
      // The price has broken the upper limit
      if(Log_Level == LOG_LEVEL_DEBUG) PrintFormat("%s: Price (%s) has broken the upper limit (%s)", __FUNCTION__, DoubleToString(d_Actual_Price, _Digits), DoubleToString(go_Channel.d_High, _Digits));
      return(ENTRY_SELL);
    }
  
  // lower limit:
  if(go_Channel.i_Lowest_Offset > i_Extremum_Bars) // 1st condition
    if(go_Channel.d_Low > d_Actual_Price) { // 2nd condition
      // The price has broken the lower limit
      if(Log_Level == LOG_LEVEL_DEBUG) PrintFormat("%s: Price (%s) has broken the lower limit (%s)", __FUNCTION__, DoubleToString(d_Actual_Price, _Digits), DoubleToString(go_Channel.d_Low, _Digits));
      return(ENTRY_BUY);
    }
  
  // If the program has reached this line, then the price is inside the range, i.e. the second condition is not satisfied
  
  if(b_Wait_For_Bar_Close) // for the Turtle Soup Plus One version
    // Pause until the current bar close
    st_Pause_End = go_Tick.time + PeriodSeconds() - go_Tick.time % PeriodSeconds();
  
  return(ENTRY_NONE);
}

enum ENUM_STRATEGY {     // Strategy version
  TS_TURTLE_SOUP,        // Turtle Soup
  TS_TURTLE_SOUP_PLIS_1  // Turtle Soup Plus One
};

// Custom settings
input ENUM_STRATEGY  Turtle_Soup_Type = TS_TURTLE_SOUP;  // Turtle Soup: Strategy version
input uint           Turtle_Soup_Period_Length = 20;     // Turtle Soup: Extremum search depth (in bars)
input uint           Turtle_Soup_Extremum_Offset = 3;    // Turtle Soup: A pause after the last extremum (in bars)
input double         Turtle_Soup_Entry_Offset = 10;      // Turtle Soup: Entry: Offset from the extreme level (in points)
input double         Turtle_Soup_Exit_Offset = 1;        // Turtle Soup: Exit: Offset from an opposite extremum (in points)


//A Basic Expert Advisor for TS Testing


enum ENUM_LOG_LEVEL {  // The list of logging levels
  LOG_LEVEL_NONE,      // Logging disabled
  LOG_LEVEL_ERR,       // Only error information
  LOG_LEVEL_INFO,      // Errors + robot's comments
  LOG_LEVEL_DEBUG      // Everything
};
enum ENUM_ENTRY_SIGNAL {  // The list of entry signals
  ENTRY_BUY,              // A Buy signal
  ENTRY_SELL,             // A Sell signal
  ENTRY_NONE,             // No signal
  ENTRY_UNKNOWN           // An indefinite status
};

#include <Trade\Trade.mqh> // Class for performing trading operations



input string  _ = "** Strategy settings:";  // .

#include <Expert\Signal\Signal_Turtle_Soup.mqh> // Signal module

                                                        
input string  __ = "** Position opening and management:"; // .
input double  Trade_Volume = 0.1;                  // Trade volume
input uint    Trail_Trigger = 100;                 // Trailing: Distance to enable trailing (in points)
input uint    Trail_Step = 5;                      // Trailing: SL moving step (in points)
input uint    Trail_Distance = 50;                 // Trailing: Max distance from the price to SL (in points)
input ENUM_LOG_LEVEL  Log_Level = LOG_LEVEL_INFO;  // Logging mode:

int
  gi_Try_To_Trade = 4, // The number of attempts to send a trade order
  gi_Connect_Wait = 2000 // A pause between attempts (in milliseconds)
;
double
  gd_Stop_Level, // StopLevel from the server settings converted to the symbol price
  gd_Lot_Step, gd_Lot_Min, gd_Lot_Max, // Lot value restrictions from the server settings
  gd_Entry_Offset, // Entry: Offset from the extremum in symbol prices
  gd_Exit_Offset, // Exit: Offset from the extremum in symbol prices
  gd_Trail_Trigger, gd_Trail_Step, gd_Trail_Distance // Trailing parameters converted to the symbol price
;
MqlTick go_Tick; // Information about the last known tick



int OnInit() {
  // Converting settings from points to symbol prices:
  double d_One_Point_Rate = pow(10, _Digits);
  gd_Entry_Offset = Turtle_Soup_Entry_Offset / d_One_Point_Rate;
  gd_Exit_Offset = Turtle_Soup_Exit_Offset / d_One_Point_Rate;
  gd_Trail_Trigger = Trail_Trigger / d_One_Point_Rate;
  gd_Trail_Step = Trail_Step / d_One_Point_Rate;
  gd_Trail_Distance = Trail_Distance / d_One_Point_Rate;
  gd_Stop_Level = SymbolInfoInteger(_Symbol, SYMBOL_TRADE_STOPS_LEVEL) / d_One_Point_Rate;
  // Initialization of lot limits:
  gd_Lot_Min = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_MIN);
  gd_Lot_Max = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_MAX);
  gd_Lot_Step = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_STEP);
  
  return(INIT_SUCCEEDED);
}

bool fb_Trailing_Stop(    // Trailing SL of the current symbol position
  double d_Trail_Trigger,  // The distance to enable trailing (in symbol prices
  double d_Trail_Step,    // SL trailing step (in symbol prices)
  double d_Trail_Distance  // min distance from the price to SL (in symbol prices)</s3>
) {
  if(!PositionSelect(_Symbol)) return(false); // No position, nothing to trail
  
  // The basic value for the calculation of the new SL level - current price value:
  double d_New_SL = PositionGetDouble(POSITION_PRICE_CURRENT);
  
  if(PositionGetInteger(POSITION_TYPE) == POSITION_TYPE_BUY) { // For a long position
    if(d_New_SL - PositionGetDouble(POSITION_PRICE_OPEN) < d_Trail_Trigger)
      return(false); // The price has not moved far enough to enable trailing
      
    if(d_New_SL - PositionGetDouble(POSITION_SL) < d_Trail_Distance + d_Trail_Step)
      return(false); // Price change less than the set SL trailing step
    
    d_New_SL -= d_Trail_Distance; // New SL level
  } else if(PositionGetInteger(POSITION_TYPE) == POSITION_TYPE_SELL) { // For a short position
    if(PositionGetDouble(POSITION_PRICE_OPEN) - d_New_SL < d_Trail_Trigger)
      return(false); // The price has not moved far enough to enable trailing
    
    if(PositionGetDouble(POSITION_SL) > 0.0) if(PositionGetDouble(POSITION_SL) - d_New_SL < d_Trail_Distance + d_Trail_Step)
      return(false); // The price has not moved far enough to enable trailing
    
    d_New_SL += d_Trail_Distance; // New SL level
  } else return(false);
  
  // Do server settings allow placing the new SL at this distance from the current price?
  if(!fb_Is_Acceptable_Distance(d_New_SL, PositionGetDouble(POSITION_PRICE_CURRENT))) return(false);
  
  CTrade Trade;
  Trade.LogLevel(LOG_LEVEL_ERRORS);
  // Move SL
  Trade.PositionModify(_Symbol, d_New_SL, PositionGetDouble(POSITION_TP));
  
  return(true);
}



bool fb_Is_Acceptable_Distance(double d_Level_To_Check, double d_Current_Price) {
  return(
    fabs(d_Current_Price - d_Level_To_Check)
    >
    fmax(gd_Stop_Level, go_Tick.ask - go_Tick.bid)
  );
}

if(PositionSelect(_Symbol)) { // There is an open position
  if(PositionGetDouble(POSITION_SL) == 0.) { // New position
    double
      d_SL = WRONG_VALUE, // SL level
      da_Price_Array[] // An auxiliary array
    ;
    
    // Calculate the StopLoss level:
    if(PositionGetInteger(POSITION_TYPE) == POSITION_TYPE_BUY) { // For a long position
      if(WRONG_VALUE == CopyLow(_Symbol, PERIOD_CURRENT, 0, 1 + (Turtle_Soup_Type == TS_TURTLE_SOUP_PLIS_1), da_Price_Array)) {
        // Handling the CopyLow function error
        if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyLow: error #%u", __FUNCTION__, _LastError);
        return;
      }
      d_SL = da_Price_Array[ArrayMinimum(da_Price_Array)] - gd_Exit_Offset;
      
      // Is the distance from the current price enough?
      if(!fb_Is_Acceptable_Distance(d_SL, go_Tick.bid)) {
        if(Log_Level > LOG_LEVEL_NONE) PrintFormat("Calculated SL level %s is replaced by the minimum allowable %s", DoubleToString(d_SL, _Digits), DoubleToString(go_Tick.bid + fmax(gd_Stop_Level, go_Tick.ask - go_Tick.bid), _Digits));
        d_SL = go_Tick.bid - fmax(gd_Stop_Level, go_Tick.ask - go_Tick.bid);
      }
      
    } else { // For a short position
      if(WRONG_VALUE == CopyHigh(_Symbol, PERIOD_CURRENT, 0, 1 + (Turtle_Soup_Type == TS_TURTLE_SOUP_PLIS_1), da_Price_Array)) {
        // Handling the CopyHigh function error
        if(Log_Level > LOG_LEVEL_NONE) PrintFormat("%s: CopyHigh: error #%u", __FUNCTION__, _LastError);
        return;
      }
      d_SL = da_Price_Array[ArrayMaximum(da_Price_Array)] + gd_Exit_Offset;
      
      // Is the distance from the current price enough?
      if(!fb_Is_Acceptable_Distance(d_SL, go_Tick.ask)) {
        if(Log_Level > LOG_LEVEL_NONE) PrintFormat("Calculated SL level %s is replaced by the minimum allowable %s", DoubleToString(d_SL, _Digits), DoubleToString(go_Tick.ask - fmax(gd_Stop_Level, go_Tick.ask - go_Tick.bid), _Digits));
        d_SL = go_Tick.ask + fmax(gd_Stop_Level, go_Tick.ask - go_Tick.bid);
      }
    }
    
    CTrade Trade;
    Trade.LogLevel(LOG_LEVEL_ERRORS);
    // Set SL
    Trade.PositionModify(_Symbol, d_SL, PositionGetDouble(POSITION_TP));
    return;
  }
  
  // Trailing
  fb_Trailing_Stop(gd_Trail_Trigger, gd_Trail_Step, gd_Trail_Distance);
  return;
}

int
  i_Order_Ticket = WRONG_VALUE, // The ticket of a pending order
  i_Try = gi_Try_To_Trade, // The number of attempts to perform the operation
  i_Pending_Type = -10 // The type of the existing pending order
;
static int si_Last_Tick_Bar_Num = 0; // The number of the previous tick's bar (0 = beginning of calculation in MQL)

// Processing events linked to the beginning if a new day (bar):
if(si_Last_Tick_Bar_Num < int(floor(go_Tick.time / PeriodSeconds()))) {
  // Hello new day :)
  si_Last_Tick_Bar_Num = int(floor(go_Tick.time / PeriodSeconds()));
  
  // Is there an obsolete pending order?
  i_Pending_Type = fi_Get_Pending_Type(i_Order_Ticket);
  if(i_Pending_Type == ORDER_TYPE_SELL_STOP || i_Pending_Type == ORDER_TYPE_BUY_STOP) {
    // Delete the old order:
    if(Log_Level > LOG_LEVEL_ERR) Print("Deleting yesterday's pending order");
    
    CTrade o_Trade;
    o_Trade.LogLevel(LOG_LEVEL_ERRORS);
    while(i_Try-- > 0) { // Attempts to delete
      if(o_Trade.OrderDelete(i_Order_Ticket)) { // Attempt successful
        i_Try = -10; // The flag of a successful operation
        break;
      }
      // Attempt failed
      Sleep(gi_Connect_Wait); // A pause before the next attempt
    }
    
    if(i_Try == WRONG_VALUE) { // Deleting a pending order failed
      if(Log_Level > LOG_LEVEL_NONE) Print("Pending order deleting error");
      return; // Wait till the next tick
    }
  }
  
  // Updating the channel parameters:
  go_Channel.f_Set(Turtle_Soup_Period_Length, 1 + (Turtle_Soup_Type == TS_TURTLE_SOUP_PLIS_1));
}

int fi_Get_Pending_Type( // Detecting the presence of a pending order of the current symbol
  int& i_Order_Ticket // A reference to the ticket of the selected pending order
) {
  int
    i_Order = OrdersTotal(), // The total number of orders
    i_Order_Type = WRONG_VALUE // A variable for the order type
  ;
  i_Order_Ticket = WRONG_VALUE; // The default returned ticket value
  
  if(i_Order < 1) return(i_Order_Ticket); // No orders
  
  while(i_Order-- > 0) { // Checking existing orders
    i_Order_Ticket = int(OrderGetTicket(i_Order)); // Reading the ticket
    if(i_Order_Ticket > 0)
      if(StringCompare(OrderGetString(ORDER_SYMBOL), _Symbol, false) == 0) {
        i_Order_Type = int(OrderGetInteger(ORDER_TYPE));
        // We only need pending orders:
        if(i_Order_Type == ORDER_TYPE_BUY_LIMIT || i_Order_Type == ORDER_TYPE_BUY_STOP || i_Order_Type == ORDER_TYPE_SELL_LIMIT || i_Order_Type == ORDER_TYPE_SELL_STOP)
          break; // A pending order has been found
      }
    i_Order_Ticket = WRONG_VALUE; // Not found yet
  }
  
  return(i_Order_Type);
}

// Get the signal status:
ENUM_ENTRY_SIGNAL e_Signal = fe_Get_Entry_Signal(Turtle_Soup_Type == TS_TURTLE_SOUP_PLIS_1, Turtle_Soup_Extremum_Offset);
if(e_Signal > 1) return; // No signal


// Finding the type of the pending order and its ticket if we haven't done this yet:
if(i_Pending_Type == -10)
  i_Pending_Type = fi_Get_Pending_Type(i_Order_Ticket);

// Do we need a new pending order?
if(
  (e_Signal == ENTRY_SELL && i_Pending_Type == ORDER_TYPE_SELL_STOP)
  ||
  (e_Signal == ENTRY_BUY && i_Pending_Type == ORDER_TYPE_BUY_STOP)
) return; // There is a pending order in the direction of the signal

// Do we need to delete the pending order?
if(
  (e_Signal == ENTRY_SELL && i_Pending_Type == ORDER_TYPE_BUY_STOP)
  ||
  (e_Signal == ENTRY_BUY && i_Pending_Type == ORDER_TYPE_SELL_STOP)
) { // The direction of the pending order does not match the direction of the signal
  if(Log_Level > LOG_LEVEL_ERR) Print("The direction of the pending order does not correspond to the direction of the signal");
    
  i_Try = gi_Try_To_Trade;
  while(i_Try-- > 0) { // Attempts to delete
    if(o_Trade.OrderDelete(i_Order_Ticket)) { // Attempt successful
      i_Try = -10; // The flag of a successful operation
      break;
    }
    // Attempt failed
    Sleep(gi_Connect_Wait); // A pause before the next attempt
  }
  
  if(i_Try == WRONG_VALUE) { // Deleting a pending order failed
    if(Log_Level > LOG_LEVEL_NONE) Print("Pending order deleting error");
    return; // Wait till the next tick
  }
}

double d_Entry_Level = WRONG_VALUE; // The level for placing a pending order
if(e_Signal == ENTRY_BUY) { // For a pending Buy order
  // Checking the possibility to place an order:
  d_Entry_Level = go_Channel.d_Low + gd_Entry_Offset; // Order placing level
  if(!fb_Is_Acceptable_Distance(d_Entry_Level, go_Tick.ask)) {
    // The distance from the current price is not enough 
    if(Log_Level > LOG_LEVEL_ERR)
      PrintFormat("BuyStop cannot be placed at the %s level. Bid: %s Ask: %s StopLevel: %s",
        DoubleToString(d_Entry_Level, _Digits),
        DoubleToString(go_Tick.bid, _Digits),
        DoubleToString(go_Tick.ask, _Digits),
        DoubleToString(gd_Stop_Level, _Digits)
      );
    
    return; // Wait until the current price changes
  }
} else {
  // Checking the possibility to place an order:
  d_Entry_Level = go_Channel.d_High - gd_Entry_Offset; // Order placing level
  if(!fb_Is_Acceptable_Distance(d_Entry_Level, go_Tick.bid)) {
    // The distance from the current price is not enough 
    if(Log_Level > LOG_LEVEL_ERR)
      PrintFormat("SellStop cannot be placed at the %s level. Bid: %s Ask: %s StopLevel: %s",
        DoubleToString(d_Entry_Level, _Digits),
        DoubleToString(go_Tick.bid, _Digits),
        DoubleToString(go_Tick.ask, _Digits),
        DoubleToString(gd_Stop_Level, _Digits)
      );
    
    return; // Wait until the current price changes
  }
}

// Lot in accordance with the server requirements:
double d_Volume = fd_Normalize_Lot(Trade_Volume);

// Place a pending order:
i_Try = gi_Try_To_Trade;

if(e_Signal == ENTRY_BUY) {
  while(i_Try-- > 0) { // Attempts to place BuyStop
    if(o_Trade.BuyStop(
      d_Volume,
      d_Entry_Level,
      _Symbol
    )) { // Successful attempt
      Alert("A pending Buy order has been placed!");
      i_Try = -10; // The flag of a successful operation
      break;
    }
    // Failed
    Sleep(gi_Connect_Wait); // A pause before the next attempt
  }
} else {
  while(i_Try-- > 0) { // Attempts to place SellStop
    if(o_Trade.SellStop(
      d_Volume,
      d_Entry_Level,
      _Symbol
    )) { // Successful attempt
      Alert("A pending Sell order has been placed!");
      i_Try = -10; // The flag of a successful operation
      break;
    }
    // Failed
    Sleep(gi_Connect_Wait); // A pause before the next attempt
  }
}

if(i_Try == WRONG_VALUE) // Placing a pending order failed
  if(Log_Level > LOG_LEVEL_NONE) Print("Pending order placing error");
	