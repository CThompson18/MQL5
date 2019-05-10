//+------------------------------------------------------------------+
//|                                                  DanTest2_EA.mq4 |
//|                                                      4X Edge LLC |
//|                                                       4xedge.com |
//+------------------------------------------------------------------+
#property copyright "4X Edge LLC"
#property link      "4xedge.com"

extern int magic_number = 1234567;
extern int max_trades_per_hour = 2;

extern double pbt_up_level = 9.5;
extern double pbt_down_level = -9.5;

extern double base_up_level = 9.5;
extern double base_down_level = -9.5;

extern double cross_up_level = 9.5;
extern double cross_down_level = -9.5;

extern double global_trend_up_level = 6;
extern double global_trend_down_level = -6;

extern double SL_atr_multiplier = 1;
extern double TP_atr_multiplier = 1.5;
extern int SL_TP_atr_period = 14;

extern double Lots=0.1;
extern int Slippage=10;

//+------------------------------------------------------------------+
//| expert initialization function                                   |
//+------------------------------------------------------------------+
int init()
  {
//----
   start();
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| expert deinitialization function                                 |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   
//----
   return(0);
  }
  
string currency[8] ={"EUR","GBP","JPY","CHF","CAD","AUD","NZD","USD"};

//+------------------------------------------------------------------+
//| expert start function                                            |
//+------------------------------------------------------------------+
int start()
{
   if (TotalOrders(magic_number) == 0 && TotalOrdersInLastHour(magic_number) < max_trades_per_hour)
   {
      double nd0 = iCustom(Symbol(), 0, "newdash_danx_EA", 0, 0);
      double nd1 = iCustom(Symbol(), 0, "newdash_danx_EA", 0, 1);
      double nd2 = iCustom(Symbol(), 0, "newdash_danx_EA", 0, 2);
      double nd3 = iCustom(Symbol(), 0, "newdash_danx_EA", 0, 3);
      double nd4 = iCustom(Symbol(), 0, "newdash_danx_EA", 0, 4);
      double nd5 = iCustom(Symbol(), 0, "newdash_danx_EA", 0, 5);  

      double pbt = nd0 / 10;
      double scalping_trend = nd1 / 10;
   
      double xm0 = iCustom(Symbol(),0,"Xmeter_EA", 0,0);
      double xm1 = iCustom(Symbol(),0,"Xmeter_EA", 0,1);
      double xm2 = iCustom(Symbol(),0,"Xmeter_EA", 0,2);
      double xm3 = iCustom(Symbol(),0,"Xmeter_EA", 0,3);
      double xm4 = iCustom(Symbol(),0,"Xmeter_EA", 0,4);
      double xm5 = iCustom(Symbol(),0,"Xmeter_EA", 0,5);
      double xm6 = iCustom(Symbol(),0,"Xmeter_EA", 0,6);
   
      double atr_speed_1 = xm0 / 1.9;
      double atr_speed_2 = xm1 / 1.3;
   
      string base_currency=StringSubstr(Symbol(),0,3);
      string cross_currency=StringSubstr(Symbol(),3,3);
      int base_index = getIndex(currency, base_currency);
      int cross_index = getIndex(currency, cross_currency);
   
      double base = iCustom(Symbol(), 0, "BarometerDashboard_EA",base_index,0);
      double cross = iCustom(Symbol(), 0, "BarometerDashboard_EA",cross_index,0);
   
      double base_m = 0;
      double cross_m = 0;
      if (base <50) {
         base_m = ((-1)*(50-base)*2)+ xm1+xm2+xm3+xm4+xm5+xm6;
      } else {
         base_m = (2*(base-50))+ xm1+xm2+xm3+xm4+xm5+xm6;
      }

      if (cross <50) {
         cross_m = ((50-cross)*2)+ xm1+xm2+xm3+xm4+xm5+xm6;
      } else {
         cross_m = ((-1)*2*(cross-50))+ xm1+xm2+xm3+xm4+xm5+xm6;
      }
   
      double gv = MathRound(nd0*0.02+nd1*0.05+nd2*0.12+nd3*0.18+nd4*0.25+nd5*0.38);
      double global_trend_value =  NormalizeDouble((base_m + cross_m + gv)/25,1);
      
      double ask = MarketInfo(Symbol(),MODE_ASK);
      double bid = MarketInfo(Symbol(),MODE_BID);
      double sl_tp_atr = iATR(Symbol(),0,SL_TP_atr_period,0);
      double SL = 0, TP = 0;
      int i = 0;
   
      Comment("\nPBT: "+DoubleToStr(pbt, 1)+"\nGlobal Value: "+DoubleToStr(global_trend_value, 1));
      
      if (pbt >= pbt_up_level && global_trend_value < global_trend_up_level && 
           (base_m>=base_up_level || cross_m >= cross_up_level))
      {
         Print("Long conditions!!!: Part1 = "+DoubleToStr(pbt,1)+" Part2 = "+ DoubleToStr(global_trend_value,1)+" Part3= "+DoubleToStr((atr_speed_1+atr_speed_2+scalping_trend),1));
         SL = NormalizeDouble(ask-(sl_tp_atr * SL_atr_multiplier),Digits);
         TP = NormalizeDouble(ask+(sl_tp_atr * TP_atr_multiplier),Digits);
         if (OrderSend(Symbol(),OP_BUY,Lots,NormalizeDouble(ask,Digits),Slippage,SL,TP,"Cond1 trade",magic_number,0,Blue)==-1) {
               i=GetLastError();
               if (i!=1 && i!=0) Print("Long Trade Error :"+error(i)+" at "+Symbol()+" "+Period());                 
         }
      }
      else
      if (pbt <= pbt_down_level && global_trend_value > global_trend_down_level && 
           (base_m<=base_down_level || cross_m <= cross_down_level))
      {
         Print("Short conditions!!!: Part1 = "+DoubleToStr(pbt,1)+" Part2 = "+ DoubleToStr(global_trend_value,1)+" Part3= "+DoubleToStr((atr_speed_1+atr_speed_2+scalping_trend),1));
         SL = NormalizeDouble(bid+(sl_tp_atr * SL_atr_multiplier),Digits);
         TP = NormalizeDouble(bid-(sl_tp_atr * TP_atr_multiplier),Digits);
         if (OrderSend(Symbol(),OP_SELL,Lots,NormalizeDouble(bid,Digits),Slippage,SL,TP,"Cond1 trade",magic_number,0,Red)==-1) {
               i=GetLastError();
               if (i!=1 && i!=0) Print("Short Trade Error :"+error(i)+" at "+Symbol()+" "+Period());                 
         }
      }     
   }
   else
   {
      Comment("EA on pause until the current "+Symbol()+" trade closes \n and more than 1 hour passes since the last 2 trades!");
   }
   return(0);
}

int getIndex(string s[], string pair){
   for (int i = 0;i<8;i++){
      if (s[i] == pair) return (i);
   }
   return (-1);
}

int TotalOrders(int magic){
   int rez=0;
   for (int i=0;i<OrdersTotal();i++){
      OrderSelect(i,SELECT_BY_POS);
      if (OrderMagicNumber()==magic){
         rez++;
      }
   }   
   return(rez);
}

int TotalOrdersInLastHour(int magic){
   int rez=0;
   for (int i=0;i<OrdersTotal();i++){
      OrderSelect(i,SELECT_BY_POS);
      if (OrderMagicNumber()==magic && TimeCurrent()-OrderOpenTime()<3600){
         rez++;
      }
   }
   return(rez);
}


string error(int err){
switch (err){
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

