//+------------------------------------------------------------------+
//|                                                 Xtrend_part1.mq4 |
//|                                                     Dan Campbell |
//+------------------------------------------------------------------+
#property copyright "Dan Campbell"
#property link      "WWW.4XEDGE.COM"

#property indicator_chart_window
#property indicator_buffers 8
#property indicator_color1 C'53,74,51'
#property indicator_color2 C'117,51,51'
#property indicator_color3 DarkBlue
#property indicator_color4 CLR_NONE
#property indicator_color5 C'0,94,0'
#property indicator_color6 C'100,0,0'
#property indicator_color7 C'0,138,0'
#property indicator_color8 C'164,0,0'


int  Chart=PERIOD_M30;

color r1_p=DarkSlateGray;
color r1_r2=DarkSlateGray;
color r2_r3=DarkSlateGray;
color s1_p=DarkSlateGray;
color s1_s2=DarkSlateGray;
color s2_s3=DarkSlateGray;


int       Xtrend_Periods=121;
int       Xtrend_Shift=5;
int       Xtrend_ApplyTo=0;

int       T4x1_Periods1=125;
int       T4x1_Shift1=0;
int       T4x1_ApplyTo1=0;
int       T4x1_Bar_Width=5;

int       T4x2_Periods1=121;
int       T4x2_Shift1=5;
int       T4x2_ApplyTo1=0;
int       T4x2_Bar_Width=3;


int ErrorTimeOut=30;

double ExtMapBuffer5[];
double ExtMapBuffer6[];

double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double xtrend[];

double tma[];
int    weights[];
double divisor;
int  CountBars=1000;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
      SetIndexStyle(0,DRAW_LINE);
      SetIndexBuffer(0,ExtMapBuffer5);
      SetIndexStyle(1,DRAW_LINE);
      SetIndexBuffer(1,ExtMapBuffer6);
      SetIndexStyle(2,DRAW_LINE,EMPTY,4);
      SetIndexBuffer(2,xtrend);
      SetIndexShift(2,Xtrend_Shift);
      SetIndexStyle(3,DRAW_NONE);
      SetIndexStyle(4,DRAW_HISTOGRAM,EMPTY,T4x1_Bar_Width);
      SetIndexBuffer(4,ExtMapBuffer1);
      SetIndexStyle(5,DRAW_HISTOGRAM,EMPTY,T4x1_Bar_Width);
      SetIndexBuffer(5,ExtMapBuffer2);
      SetIndexStyle(6,DRAW_HISTOGRAM,EMPTY,T4x2_Bar_Width);
      SetIndexBuffer(6,ExtMapBuffer3);
      SetIndexStyle(7,DRAW_HISTOGRAM,EMPTY,T4x2_Bar_Width);
      SetIndexBuffer(7,ExtMapBuffer4);

   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   ObjectDelete("S1");
   ObjectDelete("S2");
   ObjectDelete("S3");
   ObjectDelete("R1");
   ObjectDelete("R2");
   ObjectDelete("R3");
   ObjectDelete("PIVOT");
   ObjectDelete("Support 1");
   ObjectDelete("Support 2");
   ObjectDelete("Support 3");
   ObjectDelete("Pivot level");
   ObjectDelete("Resistance 1");
   ObjectDelete("Resistance 2");
   ObjectDelete("Resistance 3");
   ObjectDelete("r1_p");
   ObjectDelete("r1_r2");
   ObjectDelete("r2_r3");
   ObjectDelete("s1_p");
   ObjectDelete("s1_s2");
   ObjectDelete("s2_s3");   
   Comment(" ");
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
datetime last_t=0;  
bool access=false;
datetime last_e=0;
int start()
  {
    int handle=FileOpen("4xedge.ini",FILE_BIN|FILE_READ);
    if (handle<0) {
      if (TimeCurrent()-last_e>ErrorTimeOut) {       
         Alert("Can not read login setings from 4xedge.ini");
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

   if (Period()!=Chart){
      if (TimeCurrent()-last_e>ErrorTimeOut) {   
         Alert("This indicator works only on "+Chart+" map.");
         last_e=TimeCurrent();
      }
      return(0);
   }
     if (Time[0]>last_t+86400 || !access) {
      last_t=Time[0];
      string w="";
      bool g=GrabWeb("https://4xedge.com/indicators/s_new1.php?user="+login+"&pass="+password,w);
      if (!g) {         
         return(0);
      }      
      if (w!=decript("*o]]a`")){
      if (TimeCurrent()-last_e>ErrorTimeOut) {   
         Alert("Your subscription has expired!\nGoto Http://www.4xedge.com to renew.");
         last_e=TimeCurrent();
      }
         return(0);
      }
      access=true;
   }   
//----

   double max=EMPTY_VALUE,min=EMPTY_VALUE;
   for (i=CountBars;i>=0;i--){      
      j=iBarShift(Symbol(),PERIOD_W1,Time[i],false);
      max=iHigh(Symbol(),PERIOD_W1,j+1);
      min=iLow(Symbol(),PERIOD_W1,j+1);      
      ExtMapBuffer5[i]=max;
      ExtMapBuffer6[i]=min;
   }

double rates[1][6],yesterday_close,yesterday_high,yesterday_low;
ArrayCopyRates(rates, Symbol(), PERIOD_D1);

if(DayOfWeek() == 1)
{
   if(TimeDayOfWeek(iTime(Symbol(),PERIOD_D1,1)) == 5)
   {
       yesterday_close = rates[1][4];
       yesterday_high = rates[1][3];
       yesterday_low = rates[1][2];
   }
   else
   {
      for(int d = 5;d>=0;d--)
      {
         if(TimeDayOfWeek(iTime(Symbol(),PERIOD_D1,d)) == 5)
         {
             yesterday_close = rates[d][4];
             yesterday_high = rates[d][3];
             yesterday_low = rates[d][2];
         }
         
      }  
   }
}
else
{
    yesterday_close = rates[1][4];
    yesterday_high = rates[1][3];
    yesterday_low = rates[1][2];
}


//---- Calculate Pivots

Comment("\nYesterday quotations:\nH ",yesterday_high,"\nL ",yesterday_low, "\nC ",yesterday_close);
double R = yesterday_high - yesterday_low;//range
double p = (yesterday_high + yesterday_low + yesterday_close)/3;// Standard Pivot
double r3 = p + (R * 1.000);
double r2 = p + (R * 0.618);
double r1 = p + (R * 0.382);
double s1 = p - (R * 0.382);
double s2 = p - (R * 0.618);
double s3 = p - (R * 1.000);

ObjectDelete("r1_p");
ObjectCreate("r1_p",16,0,Time[Bars],p,Time[0],r1);
ObjectSet("r1_p",6,r1_p);
ObjectDelete("r1_r2");
ObjectCreate("r1_r2",16,0,Time[Bars],r1,Time[0],r2);
ObjectSet("r1_r2",6,r1_r2);
ObjectDelete("r2_r3");
ObjectCreate("r2_r3",16,0,Time[Bars],r2,Time[0],r3);
ObjectSet("r2_r3",6,r2_r3);

ObjectDelete("s1_p");
ObjectCreate("s1_p",16,0,Time[Bars],p,Time[0],s1);
ObjectSet("s1_p",6,s1_p);
ObjectDelete("s1_s2");
ObjectCreate("s1_s2",16,0,Time[Bars],s1,Time[0],s2);
ObjectSet("s1_s2",6,s1_s2);
ObjectDelete("s2_s3");
ObjectCreate("s2_s3",16,0,Time[Bars],s2,Time[0],s3);
ObjectSet("s2_s3",6,s2_s3);


drawLine(r3,"R3", LightGray,0);
drawLabel("61.8",r3,LightGray);
drawLine(r2,"R2", LightGray,0);
drawLabel("50.0",r2,LightGray);
drawLine(r1,"R1", LightGray,0);
drawLabel("38.2",r1,LightGray);

drawLine(p,"PIVIOT",Gold,1);
drawLabel("Piviot level",p,Gold);

drawLine(s1,"S1",LightGray,0);
drawLabel("-38.2",s1,LightGray);
drawLine(s2,"S2",LightGray,0);
drawLabel("-50.0",s2,LightGray);
drawLine(s3,"S3",LightGray,0);
drawLabel("-61.8",s3,LightGray);



TMA(tma,Xtrend_Periods,Xtrend_ApplyTo);
TMA4x(ExtMapBuffer1,ExtMapBuffer2,T4x1_Periods1,T4x1_Shift1,T4x1_ApplyTo1);
TMA4x(ExtMapBuffer3,ExtMapBuffer4,T4x2_Periods1,T4x2_Shift1,T4x2_ApplyTo1);


//----
   return(0);
  }
//+------------------------------------------------------------------+
void drawLabel(string name,double lvl,color Color)
{
    if(ObjectFind(name) != 0)
    {
        ObjectCreate(name, OBJ_TEXT, 0, Time[10], lvl);
        ObjectSetText(name, name, 10, "Arial", Yellow);
        ObjectSet(name, OBJPROP_COLOR, Color);
    }
    else
    {
        ObjectMove(name, 0, Time[10], lvl);
    }
}


void drawLine(double lvl,string name, color Col,int type)
{
         if(ObjectFind(name) != 0)
         {
            ObjectCreate(name, OBJ_HLINE, 0, Time[0], lvl,Time[0],lvl);
            
            if(type == 1)
            ObjectSet(name, OBJPROP_STYLE, STYLE_SOLID);
            else
            ObjectSet(name, OBJPROP_STYLE, STYLE_DOT);
            
            ObjectSet(name, OBJPROP_COLOR, Col);
            ObjectSet(name,OBJPROP_WIDTH,1);
            
         }
         else
         {
            ObjectDelete(name);
            ObjectCreate(name, OBJ_HLINE, 0, Time[0], lvl,Time[0],lvl);
            
            if(type == 1)
            ObjectSet(name, OBJPROP_STYLE, STYLE_SOLID);
            else
            ObjectSet(name, OBJPROP_STYLE, STYLE_DOT);
            
            ObjectSet(name, OBJPROP_COLOR, Col);        
            ObjectSet(name,OBJPROP_WIDTH,1);
          
         }
}

void TMA4x(double &r[],double &r1[],int Periods1,int Shift1,int ApplyTo1)
  {
    int l=0;
    if (CountBars>Bars) {l=Bars;}else{l=CountBars;}
        double a[];
    ArrayResize(a,l);
    ArraySetAsSeries(a,true);
    TMA(a,Periods1,ApplyTo1);
    for (int i=l;i>=0;i--){
    double a1=a[i+1+Shift1];      
      if (Open[i+1]>a1 && Close[i+1]>a1) {
         r[i]=MathMax(Open[i],Close[i]);
         r1[i]=MathMin(Open[i],Close[i]);
      }else if (Open[i+1]<a1 && Close[i+1]<a1){
         r[i]=MathMin(Open[i],Close[i]);
         r1[i]=MathMax(Open[i],Close[i]);      
      }
    }
  }

void TMA(double &r[],int Periods,int ApplyTo){
  ArrayResize(weights,Periods);
  double d_Periods = Periods;
  int to=MathCeil(d_Periods/2.0);

  for(int i=0; i<to; i++)
  {
    weights[i]            = i+1;
    weights[Periods-1-i]  = i+1;
  }

  divisor = 0.0;
  for(int j=0; j<Periods; j++)
    divisor += weights[j];

   int l=0;
   if (CountBars>Bars) {l=Bars;}else{l=CountBars;}
   for(i=l-1; i>=0; i--){
    double tma_val = 0.0;
    for(j=0; j<Periods; j++)
    {
      double price = getPrice(ApplyTo, i+Periods-j);
      tma_val += price*weights[j];
    }
      r[i] = tma_val/divisor;
   }
}
double getPrice(int priceType, int index)
{
  double price = 0.0;
  
  switch(priceType)
  {
    case PRICE_OPEN    : price = Open[index];
                         break;
  
    case PRICE_HIGH    : price = High[index];
                         break;
  
    case PRICE_LOW     : price = Low[index];
                         break;
  
    case PRICE_MEDIAN  : price = (High[index]+Low[index])/2.0;
                         break;
  
    case PRICE_TYPICAL : price = (High[index]+Low[index]+Close[index])/3.0;
                         break;

    case PRICE_WEIGHTED: price = (High[index]+Low[index]+2*Close[index])/4.0;
                         break;

    
    case PRICE_CLOSE   : 
    default            : price = Close[index];
                         break;

  }
  
  return(price);
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
#define INTERNET_FLAG_SECURE            0x800000
#define INTERNET_OPEN_TYPE_PRECONFIG    0

int InternetOpenW(
	string 	sAgent,
	int		lAccessType,
	string 	sProxyName="",
	string 	sProxyBypass="",
	int 	lFlags=0
);

int InternetOpenUrlW(
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


bool GrabWeb(string strUrl, string& strWebPage)
{
	int 	hInternet;
	int		iResult;
	int 	lReturn[]={1};
	string 	sBuffer="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	int 	bytes;
	int hSession;
	string InternetAgent;

   InternetAgent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; Q312461)";
   hSession = InternetOpenW(InternetAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	
	hInternet = InternetOpenUrlW(hSession, strUrl, NULL, 0, 
								INTERNET_FLAG_NO_CACHE_WRITE | 
								INTERNET_FLAG_PRAGMA_NOCACHE | 
								INTERNET_FLAG_RELOAD |
								INTERNET_FLAG_SECURE, 0);
								
	if (hInternet == 0) 
		return(false);

   //Print("Reading URL: " + strUrl);	   //added by MN	
	iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
	
	if (iResult == 0) 
		return(false);
   bytes = lReturn[0];
   strWebPage = StringSubstr(ANSI2UNICODE(sBuffer), 0, lReturn[0]);
   while (lReturn[0] != 0)
   {
      iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
      if (lReturn[0]==0) 
	      break;
      bytes = bytes + lReturn[0];
      strWebPage = strWebPage + StringSubstr(ANSI2UNICODE(sBuffer), 0, lReturn[0]);
   }

   //Print("Closing URL web connection");   //added by MN
	iResult = InternetCloseHandle(hInternet);
	if (iResult == 0) 
		return(false);
		
	return(true);
}
string decript(string s){
   string rez="";
   for (int i=0;i<StringLen(s);i++){
      rez=StringSetChar(rez,i,StringGetChar(s,i)-i+10);
   }   
   return(StringTrimRight(rez));
}

string ANSI2UNICODE(string s)
  {
   ushort mychar;
   long m,d;
   double mm,dd;
   string img;    
   string res="";
   if (StringLen(s)>0)
     {
      string g=" ";
      for (int i=0;i<StringLen(s);i++)
         {          
          string f="  ";          
          mychar=ushort(StringGetCharacter(s,i));
          mm=MathMod(mychar,256);
          img=DoubleToString(mm,0);
          m=StringToInteger(img);
          dd=(mychar-m)/256;
          img=DoubleToString(dd,0);
          d=StringToInteger(img);
          if (m!=0)
            {
             StringSetCharacter(f,0,ushort(m));
             StringSetCharacter(f,1,ushort(d));
             res = StringConcatenate(res,f);
            }//if (m!=0)
          else
            break;                      
         }//for (int i=0;i<StringLen(s);i++)
      }//if (StringLen(s)>0)
   return(res);
  }
  
  string UNICODE2ANSI(string s)
  {
   int leng,ipos;
   uchar m,d;
   ulong big;
   leng=StringLen(s);
   string unichar;
   string res="";
   if (leng!=0)
     {    
      unichar=" ";
      ipos=0;      
      while (ipos<leng)
        { //uchar typecasted because each double byte char is actually one byte
         m=uchar(StringGetCharacter(s,ipos));
         if (ipos+1<leng)
           d=uchar(StringGetCharacter(s,ipos+1));
         else
           d=0;
         big=d*256+m;                
         StringSetCharacter(unichar,0,ushort(big));         
         res = StringConcatenate(res,unichar);    
         ipos=ipos+2;
        }
     }
   return(res);
  }
