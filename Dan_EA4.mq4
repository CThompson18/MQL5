//+------------------------------------------------------------------+
//|                                                      Dan_EA1.mq4 |
//|                                                      4X Edge LLC |
//|                                                       4xedge.com |
//+------------------------------------------------------------------+
#property copyright "4X Edge LLC"
#property link      "4xedge.com"

extern int magic_number = 123;

extern int StopLoss= 500;
extern int TakeProfit= 8000;
extern double Lots=0.1;
extern int Slippage=0;

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
  
//+------------------------------------------------------------------+
//| expert start function                                            |
//+------------------------------------------------------------------+
datetime last_bar = 0; 
int start()
{
      double up_signal = iCustom(Symbol(),0,"dantrend2",0,1);
      double dn_signal = iCustom(Symbol(),0,"dantrend2",1,1);
      
      double ask = MarketInfo(Symbol(),MODE_ASK);
      double bid = MarketInfo(Symbol(),MODE_BID);
      double SL = 0, TP = 0;
      int i = 0;
      
      MathSrand(TimeLocal());
      if (up_signal != EMPTY_VALUE && up_signal != 0)
      {
         Print("Long conditions!!!");
         
         if (TotalOrders(magic_number) == 1)
         {
            OrderSelect(0, SELECT_BY_POS);
            if (OrderType()==OP_SELL)
            {
               OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(ask,Digits),3,Red);
            }
         }

         if (TotalOrders(magic_number) == 0)
         {
            SL = NormalizeDouble(ask-(StopLoss * Point),Digits);
            TP = NormalizeDouble(ask+(TakeProfit * Point),Digits);
            if (OrderSend(Symbol(),OP_BUY,Lots,NormalizeDouble(ask,Digits),Slippage,SL,TP,"Up trade",magic_number,0,Blue)==-1) {
                  i=GetLastError();
                  if (i!=1 && i!=0) Print("Long Trade Error :"+error(i)+" at "+Symbol()+" "+Period());                 
            }
            WindowScreenShot("Dan_EA4_Long_"+TimeCurrent()+"_"+MathRand()+".gif",640,480);
         }
      }
      else
      if (dn_signal != EMPTY_VALUE && dn_signal != 0)
      {
         Print("Short conditions!!!");
       
         if (TotalOrders(magic_number) == 1)
         {
            OrderSelect(0, SELECT_BY_POS);
            if (OrderType()==OP_BUY)
            {
               OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(bid,Digits),3,Red);
            }
         }
       
         if (TotalOrders(magic_number) == 0)
         {
            SL = NormalizeDouble(bid+(StopLoss * Point),Digits);
            TP = NormalizeDouble(bid-(TakeProfit * Point),Digits);
            if (OrderSend(Symbol(),OP_SELL,Lots,NormalizeDouble(bid,Digits),Slippage,SL,TP,"Down trade",magic_number,0,Red)==-1) {
                  i=GetLastError();
                  if (i!=1 && i!=0) Print("Short Trade Error :"+error(i)+" at "+Symbol()+" "+Period());                 
            }
         }
         WindowScreenShot("Dan_EA4_Short_"+TimeCurrent()+"_"+MathRand()+".gif",640,480);
      }     
   return(0);
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

