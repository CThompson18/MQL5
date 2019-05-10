extern double Lots=0.10;

extern int Entry_Max_Offset = 20;
extern bool Allow_Auto_Close = true;
extern string Symbol_Prefix = "";
extern string Symbol_Suffix = "";

extern bool EURUSD_RT = true;
extern bool GBPUSD_RT = true;
extern bool AUDUSD_RT = true;
extern bool NZDUSD_RT = true;
extern bool USDCHF_RT = true;
extern bool USDCAD_RT = true;
extern bool USDJPY_RT = true;
extern bool GBPJPY_RT = true;
extern bool EURJPY_RT = true;
extern bool AUDJPY_RT = true;
extern bool NZDJPY_RT = true;
extern bool CADJPY_RT = true;
extern bool GBPCHF_RT = true;
extern bool EURCHF_RT = true;
extern bool EURGBP_RT = true;
extern bool EURCAD_RT = true;
extern bool GBPAUD_RT = true;

extern bool EURUSD_ST = true;
extern bool GBPUSD_ST = true;
extern bool AUDUSD_ST = true;
extern bool NZDUSD_ST = true;
extern bool USDCHF_ST = true;
extern bool USDCAD_ST = true;
extern bool USDJPY_ST = true;
extern bool GBPJPY_ST = true;
extern bool EURJPY_ST = true;
extern bool AUDJPY_ST = true;
extern bool NZDJPY_ST = true;
extern bool CADJPY_ST = true;
extern bool GBPCHF_ST = true;
extern bool EURCHF_ST = true;
extern bool EURGBP_ST = true;
extern bool EURCAD_ST = true;
extern bool GBPAUD_ST = true;

extern bool EURUSD_LT = true;
extern bool GBPUSD_LT = true;
extern bool AUDUSD_LT = true;
extern bool NZDUSD_LT = true;
extern bool USDCHF_LT = true;
extern bool USDCAD_LT = true;
extern bool USDJPY_LT = true;
extern bool GBPJPY_LT = true;
extern bool EURJPY_LT = true;
extern bool AUDJPY_LT = true;
extern bool NZDJPY_LT = true;
extern bool CADJPY_LT = true;
extern bool GBPCHF_LT = true;
extern bool EURCHF_LT = true;
extern bool EURGBP_LT = true;
extern bool EURCAD_LT = true;
extern bool GBPAUD_LT = true;


extern int Slippage=3;
extern int Magic_Number=500;

string com="4xlounge_Manual";

int init(){
   return(0);
}
int ErrorTimeOut=30;
int TimeOut=30;
string currency[17]={"EURUSD","GBPUSD","AUDUSD","NZDUSD","USDCHF","USDCAD","USDJPY","GBPJPY","EURJPY","AUDJPY","NZDJPY","CADJPY","GBPCHF","EURCHF","EURGBP","EURCAD","GBPAUD"};
datetime last_t=0;  
bool access=false;
datetime last_e=0;

int deinit(){return(0);}
int start()
  {
   int handle=FileOpen("4xlounge.ini",FILE_BIN|FILE_READ);
   if (handle<0) {
      if (TimeCurrent()-last_e>ErrorTimeOut) {       
         Print("Can not read login setings from 4xlounge.ini");
         last_e=TimeCurrent();         
      }
      return(0);
   }
   string s=FileReadString(handle,255);
   int i=StringFind(s,"Login=",0);
   int j=StringFind(s,"Password=");
   string login=StringSubstr(s,i+6,j-i-6);  
   login=StringTrimRight(login);
   string password=StringSubstr(s,j+9,FileSize(handle)-j-9);
   password=StringTrimRight(password);
   FileClose(handle);
   if (TimeCurrent()>last_t+TimeOut) {
       last_t=TimeCurrent();
       string w="";
       bool g=GrabWeb("http://4xlounge.com/XSystemTest/XsystemManual_EA_ver2.php?user="+login+"&pass="+password,w);
       if (!g) {         
          return(0);
       } 
       if (w=="Your account has expired!"){
          if (TimeCurrent()-last_e>ErrorTimeOut) {   
             Alert("Your subscription has expired!\nGoto Http://www.4xlounge.com to renew.");
             last_e=TimeCurrent();
          }
          return(0);
       }
      double op[100];
      datetime ot[100];
      string sym[100];
      string sys[100];
      int oty[100];
      double sl[100];
      double tp[100];
      int tk[100];
      int f1 = StringFind(w,"<success>",0);
      int ff1 = StringFind(w,"</finish>",f1+1);
      if (f1!=-1 && ff1!=-1){
         int f2 = StringFind(w,"</success>",f1);
         int nt = StrToInteger(StringSubstr(w,f1+9,f2-f1-9));
         int ni = 0;
         while (f1!=-1){
            f1 = StringFind(w,"<openprice>",f2);
            if (f1==-1) break;
            f2 = StringFind(w,"</openprice>",f1);
            op[ni] = StrToDouble(StringSubstr(w,f1+11,f2-f1-11));

            f1 = StringFind(w,"<opentime>",f2);
            f2 = StringFind(w,"</opentime>",f1);
            ot[ni] = StrToInteger(StringSubstr(w,f1+10,f2-f1-10));

            f1 = StringFind(w,"<symbol>",f2);
            f2 = StringFind(w,"</symbol>",f1);
            sym[ni] = StringSubstr(w,f1+8,f2-f1-8);

            f1 = StringFind(w,"<ordertype>",f2);
            f2 = StringFind(w,"</ordertype>",f1);
            oty[ni] = StrToInteger(StringSubstr(w,f1+11,f2-f1-11));         

            f1 = StringFind(w,"<stoploss>",f2);
            f2 = StringFind(w,"</stoploss>",f1);
            sl[ni] = StrToDouble(StringSubstr(w,f1+10,f2-f1-10));         

            f1 = StringFind(w,"<takeprofit>",f2);
            f2 = StringFind(w,"</takeprofit>",f1);
            tp[ni] = StrToDouble(StringSubstr(w,f1+12,f2-f1-12));       

            f1 = StringFind(w,"<ticket>",f2);
            f2 = StringFind(w,"</ticket>",f1);
            tk[ni] = StrToInteger(StringSubstr(w,f1+8,f2-f1-8));    

            f1 = StringFind(w,"<system>",f2);
            f2 = StringFind(w,"</system>",f1);
            sys[ni] = StringSubstr(w,f1+8,f2-f1-8);
            ni++;
         }   
         ni--;
         for (i=0;i<=ni;i++){
            if (!isActive(sym[i],sys[i])) continue;
            RefreshRates();
            double ask=MarketInfo(getSymbol(sym[i]),MODE_ASK);
            double bid=MarketInfo(getSymbol(sym[i]),MODE_BID);
            double point = MarketInfo(getSymbol(sym[i]),MODE_POINT);
            double digits = MarketInfo(getSymbol(sym[i]),MODE_DIGITS);
            int mult=1;
            if ( ((StringFind(getSymbol(sym[i]),"JPY",0)!=-1 || StringFind(getSymbol(sym[i]),"jpy",0)!=-1) && point<0.01) ||
                 ((StringFind(getSymbol(sym[i]),"JPY",0)==-1 || StringFind(getSymbol(sym[i]),"jpy",0)==-1) && point<0.0001)) 
               mult=10;        
            double TP=tp[i];
            double SL=sl[i];
            int tr = TradeExists(tk[i],Magic_Number,sys[i]);
            if (tr==0) {
               if (MathAbs(ask-op[i]) < Entry_Max_Offset*point*mult){
                  if (oty[i]==OP_BUY){
                     if (OrderSend(getSymbol(sym[i]),OP_BUY,Lots,NormalizeDouble(ask,digits),Slippage,0,0,sys[i]+" - "+DoubleToStr(tk[i],0),Magic_Number,0,Blue)==-1) {
                           j=GetLastError();
                           Print ("TP is "+TP+"\n");
                           Print ("TP is "+SL+"\n");
                           Print (sym[i]);
                           if (j!=1 && j!=0) Print("Buy Error :"+error(j)+" at "+getSymbol(sym[j])+" "+Period());                 
                     }
                  }
                  if (oty[i]==OP_SELL){
                     if (OrderSend(getSymbol(sym[i]),OP_SELL,Lots,NormalizeDouble(bid,digits),Slippage,0,0,sys[i]+" - "+DoubleToStr(tk[i],0),Magic_Number,0,Red)==-1) {
                           j=GetLastError();
                           Print ("TP is "+TP+"\n");
                           Print ("TP is "+SL+"\n");
                           Print (sym[j]);
                           if (j!=1 && j!=0) Print("Sell Error :"+error(j)+" at "+getSymbol(sym[j])+" "+Period());                 
                     }
                  }
                  if (oty[i]==OP_BUYSTOP){
                     if (OrderSend(getSymbol(sym[i]),OP_BUYSTOP,Lots,NormalizeDouble(op[i],digits),Slippage,0,0,sys[i]+" - "+DoubleToStr(tk[i],0),Magic_Number,0,Blue)==-1) {
                           j=GetLastError();
                           Print ("TP is "+TP+"\n");
                           Print ("TP is "+SL+"\n");
                           if (j!=1 && j!=0) Print("BuyStop Error :"+error(j)+" at "+getSymbol(sym[j])+" "+Period());                 
                     }
                  }
                  if (oty[i]==OP_BUYLIMIT){
                     if (OrderSend(getSymbol(sym[i]),OP_BUYLIMIT,Lots,NormalizeDouble(op[i],digits),Slippage,0,0,sys[i]+" - "+DoubleToStr(tk[i],0),Magic_Number,0,Blue)==-1) {
                           j=GetLastError();
                           if (j!=1 && j!=0) Print("BuyLimit Error :"+error(j)+" at "+getSymbol(sym[j])+" "+Period());                 
                     }
                  }
                  if (oty[i]==OP_SELLSTOP){
                     if (OrderSend(getSymbol(sym[i]),OP_SELLSTOP,Lots,NormalizeDouble(op[i],digits),Slippage,0,0,sys[i]+" - "+DoubleToStr(tk[i],0),Magic_Number,0,Red)==-1) {
                           j=GetLastError();
                           if (j!=1 && j!=0) Print("SellStop Error :"+error(j)+" at "+getSymbol(sym[j])+" "+Period());                 
                     }
                  }
                  if (oty[i]==OP_SELLLIMIT){
                     if (OrderSend(getSymbol(sym[i]),OP_SELLLIMIT,Lots,NormalizeDouble(op[i],digits),Slippage,0,0,sys[i]+" - "+DoubleToStr(tk[i],0),Magic_Number,0,Red)==-1) {
                           j=GetLastError();
                           if (j!=1 && j!=0) Print("SellLimit Error :"+error(j)+" at "+getSymbol(sym[j])+" "+Period());                 
                     }
                  }
                  
               }else {
                  Print ("Trade too far from the current price");
               }
            }else if (tr!=0 && TradeExistsInHistory(tk[i],Magic_Number,sys[i]) == 0) {
               if (OrderSelect(tr,SELECT_BY_TICKET)) {
                  if (OrderTakeProfit()!=TP || OrderStopLoss()!=SL) {
                     ask=MarketInfo(OrderSymbol(),MODE_ASK);
                     bid=MarketInfo(OrderSymbol(),MODE_BID);
                     digits = MarketInfo(OrderSymbol(),MODE_DIGITS);               
                     OrderModify(OrderTicket(),OrderOpenPrice(),NormalizeDouble(SL,digits),NormalizeDouble(TP,digits),0,Brown);
                  }
               }
            }
         }
         
         if (Allow_Auto_Close){
            bool gs=false;
            while (!gs){
               gs=true;
               for (int t=0;t<OrdersTotal();t++){
                  OrderSelect(t,SELECT_BY_POS);
                  ask=MarketInfo(OrderSymbol(),MODE_ASK);
                  bid=MarketInfo(OrderSymbol(),MODE_BID);
                  digits = MarketInfo(OrderSymbol(),MODE_DIGITS);
                  if (OrderMagicNumber()==Magic_Number && OrderType()==OP_BUY && !Opened(OrderComment(),nt,tk,sys)){
                     OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(bid,digits),Slippage,Pink);
                     gs=false;
                     break;
                  }
                  if (OrderMagicNumber()==Magic_Number && OrderType()==OP_SELL && !Opened(OrderComment(),nt,tk,sys)){
                     OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(ask,digits),Slippage,Pink);
                     gs=false;
                     break;
                  }
                  if (OrderMagicNumber()==Magic_Number && (OrderType()==OP_SELLSTOP || OrderType()==OP_SELLLIMIT || OrderType()==OP_BUYSTOP || OrderType()==OP_BUYLIMIT) && !Opened(OrderComment(),nt,tk,sys)){
                     OrderDelete(OrderTicket());
                     gs=false;
                     break;
                  }
               }
            }
         }
      }else if (f1==-1){
         Print(w);
      }
    }
    return(0);   
  }


bool Opened(string s,int n, int s1[],string s2[]){
   for (int i=0;i<n;i++){
      if (s==s2[i]+" - "+s1[i]) return (true);
   }
   return (false);
}


int TradeExists(int tki, int m, string system){
   for (int i=0;i<OrdersTotal();i++){
      OrderSelect(i,SELECT_BY_POS);
      if (OrderMagicNumber()==m && StringFind(OrderComment(), system+" - "+DoubleToStr(tki,0)) != -1)  return (OrderTicket());
   }
   for (i=0;i<OrdersHistoryTotal();i++){
      OrderSelect(i,SELECT_BY_POS,MODE_HISTORY);
      if (OrderMagicNumber()==m && StringFind(OrderComment(), system+" - "+DoubleToStr(tki,0)) != -1)  return (-1*OrderTicket());
   }
   return (0);
}

int TradeExistsInHistory(int tki, int m, string system){
   for (int i=0;i<OrdersHistoryTotal();i++){
      OrderSelect(i,SELECT_BY_POS,MODE_HISTORY);
      if (OrderMagicNumber()==m && StringFind(OrderComment(), system+" - "+DoubleToStr(tki,0)) != -1)  return (-1*OrderTicket());
   }
   return (0);
}



/*string toUpperCase(string s){
   string rez=s;
   for (int i=0;i<StringLen(s);i++){
      if (StringGetChar(rez, i)>=97 && StringGetChar(rez, i)<=122) 
         rez = StringSetChar(rez,i,StringGetChar(rez, i)-32);
   }
   return (rez);
}*/

string getSymbol(string sym){
   return (Symbol_Prefix+sym+Symbol_Suffix);  
}

bool isActive(string currency, string system){
   if (system=="rt" && currency=="EURUSD" && !EURUSD_RT) return (false);
   if (system=="rt" && currency=="GBPUSD" && !GBPUSD_RT) return (false);
   if (system=="rt" && currency=="AUDUSD" && !AUDUSD_RT) return (false);
   if (system=="rt" && currency=="NZDUSD" && !NZDUSD_RT) return (false);
   if (system=="rt" && currency=="USDCHF" && !USDCHF_RT) return (false);
   if (system=="rt" && currency=="USDCAD" && !USDCAD_RT) return (false);
   if (system=="rt" && currency=="USDJPY" && !USDJPY_RT) return (false);
   if (system=="rt" && currency=="GBPJPY" && !GBPJPY_RT) return (false);
   if (system=="rt" && currency=="EURJPY" && !EURJPY_RT) return (false);
   if (system=="rt" && currency=="AUDJPY" && !AUDJPY_RT) return (false);
   if (system=="rt" && currency=="NZDJPY" && !NZDJPY_RT) return (false);
   if (system=="rt" && currency=="CADJPY" && !CADJPY_RT) return (false);
   if (system=="rt" && currency=="GBPCHF" && !GBPCHF_RT) return (false);
   if (system=="rt" && currency=="EURCHF" && !EURCHF_RT) return (false);
   if (system=="rt" && currency=="EURGBP" && !EURGBP_RT) return (false);
   if (system=="rt" && currency=="EURCAD" && !EURCAD_RT) return (false);
   if (system=="rt" && currency=="GBPAUD" && !GBPAUD_RT) return (false);   

   if (system=="st" && currency=="EURUSD" && !EURUSD_ST) return (false);
   if (system=="st" && currency=="GBPUSD" && !GBPUSD_ST) return (false);
   if (system=="st" && currency=="AUDUSD" && !AUDUSD_ST) return (false);
   if (system=="st" && currency=="NZDUSD" && !NZDUSD_ST) return (false);
   if (system=="st" && currency=="USDCHF" && !USDCHF_ST) return (false);
   if (system=="st" && currency=="USDCAD" && !USDCAD_ST) return (false);
   if (system=="st" && currency=="USDJPY" && !USDJPY_ST) return (false);
   if (system=="st" && currency=="GBPJPY" && !GBPJPY_ST) return (false);
   if (system=="st" && currency=="EURJPY" && !EURJPY_ST) return (false);
   if (system=="st" && currency=="AUDJPY" && !AUDJPY_ST) return (false);
   if (system=="st" && currency=="NZDJPY" && !NZDJPY_ST) return (false);
   if (system=="st" && currency=="CADJPY" && !CADJPY_ST) return (false);
   if (system=="st" && currency=="GBPCHF" && !GBPCHF_ST) return (false);
   if (system=="st" && currency=="EURCHF" && !EURCHF_ST) return (false);
   if (system=="st" && currency=="EURGBP" && !EURGBP_ST) return (false);
   if (system=="st" && currency=="EURCAD" && !EURCAD_ST) return (false);
   if (system=="st" && currency=="GBPAUD" && !GBPAUD_ST) return (false);   

   if (system=="lt" && currency=="EURUSD" && !EURUSD_LT) return (false);
   if (system=="lt" && currency=="GBPUSD" && !GBPUSD_LT) return (false);
   if (system=="lt" && currency=="AUDUSD" && !AUDUSD_LT) return (false);
   if (system=="lt" && currency=="NZDUSD" && !NZDUSD_LT) return (false);
   if (system=="lt" && currency=="USDCHF" && !USDCHF_LT) return (false);
   if (system=="lt" && currency=="USDCAD" && !USDCAD_LT) return (false);
   if (system=="lt" && currency=="USDJPY" && !USDJPY_LT) return (false);
   if (system=="lt" && currency=="GBPJPY" && !GBPJPY_LT) return (false);
   if (system=="lt" && currency=="EURJPY" && !EURJPY_LT) return (false);
   if (system=="lt" && currency=="AUDJPY" && !AUDJPY_LT) return (false);
   if (system=="lt" && currency=="NZDJPY" && !NZDJPY_LT) return (false);
   if (system=="lt" && currency=="CADJPY" && !CADJPY_LT) return (false);
   if (system=="lt" && currency=="GBPCHF" && !GBPCHF_LT) return (false);
   if (system=="lt" && currency=="EURCHF" && !EURCHF_LT) return (false);
   if (system=="lt" && currency=="EURGBP" && !EURGBP_LT) return (false);
   if (system=="lt" && currency=="EURCAD" && !EURCAD_LT) return (false);
   if (system=="lt" && currency=="GBPAUD" && !GBPAUD_LT) return (false);   

   return (true);
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

//=================================================================================================
//=================================================================================================
//====================================   GrabWeb Functions   ======================================
//=================================================================================================
//=================================================================================================
// Main Webscraping function
// ~~~~~~~~~~~~~~~~~~~~~~~~~
// bool GrabWeb(string strUrl, string& strWebPage)
// returns the text of any webpage. Returns false on timeout or other error
// 
// Parsing functions
// ~~~~~~~~~~~~~~~~~
// string GetData(string strWebPage, int nStart, string strLeftTag, string strRightTag, int& nPos)
// obtains the text between two tags found after nStart, and sets nPos to the end of the second tag
//
// void Goto(string strWebPage, int nStart, string strTag, int& nPos)
// Sets nPos to the end of the first tag found after nStart 

bool bWinInetDebug = false;

int hSession_IEType;
int hSession_Direct;
int Internet_Open_Type_Preconfig = 0;
int Internet_Open_Type_Direct = 1;
int Internet_Open_Type_Proxy = 3;
int Buffer_LEN = 13;

#import "wininet.dll"

#define INTERNET_FLAG_PRAGMA_NOCACHE    0x00000100 // Forces the request to be resolved by the origin server, even if a cached copy exists on the proxy.
#define INTERNET_FLAG_NO_CACHE_WRITE    0x04000000 // Does not add the returned entity to the cache. 
#define INTERNET_FLAG_RELOAD            0x80000000 // Forces a download of the requested file, object, or directory listing from the origin server, not from the cache.

int InternetOpenA(
	string 	sAgent,
	int		lAccessType,
	string 	sProxyName="",
	string 	sProxyBypass="",
	int 	lFlags=0
);

int InternetOpenUrlA(
	int 	hInternetSession,
	string 	sUrl, 
	string 	sHeaders="",
	int 	lHeadersLength=0,
	int 	lFlags=0,
	int 	lContext=0 
);

int InternetReadFile(
	int 	hFile,
	string 	sBuffer,
	int 	lNumBytesToRead,
	int& 	lNumberOfBytesRead[]
);

int InternetCloseHandle(
	int 	hInet
);
#import


int hSession(bool Direct)
{
	string InternetAgent;
	if (hSession_IEType == 0)
	{
		InternetAgent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; Q312461)";
		hSession_IEType = InternetOpenA(InternetAgent, Internet_Open_Type_Preconfig, "0", "0", 0);
		hSession_Direct = InternetOpenA(InternetAgent, Internet_Open_Type_Direct, "0", "0", 0);
	}
	if (Direct) 
	{ 
		return(hSession_Direct); 
	}
	else 
	{
		return(hSession_IEType); 
	}
}


bool GrabWeb(string strUrl, string& strWebPage)
{
	int 	hInternet;
	int		iResult;
	int 	lReturn[]={1};
	string 	sBuffer="x";
	int 	bytes;
	
	hInternet = InternetOpenUrlA(hSession(FALSE), strUrl, "0", 0, 
								INTERNET_FLAG_NO_CACHE_WRITE | 
								INTERNET_FLAG_PRAGMA_NOCACHE | 
								INTERNET_FLAG_RELOAD, 0);
								
	if (hInternet == 0) 
		return(false);

   //Print("Reading URL: " + strUrl);	   //added by MN	
	iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
	
	if (iResult == 0) 
		return(false);
	bytes = lReturn[0];

	strWebPage = StringSubstr(sBuffer, 0, lReturn[0]);
	
	// If there's more data then keep reading it into the buffer
	while (lReturn[0] != 0)
	{
		iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
		if (lReturn[0]==0) 
			break;
		bytes = bytes + lReturn[0];
		strWebPage = strWebPage + StringSubstr(sBuffer, 0, lReturn[0]);
	}

   //Print("Closing URL web connection");   //added by MN
	iResult = InternetCloseHandle(hInternet);
	if (iResult == 0) 
		return(false);
		
	return(true);
}