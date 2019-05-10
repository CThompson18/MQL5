//+------------------------------------------------------------------+
//|                                             Power_Bars_part1.mq4 |
//+------------------------------------------------------------------+
#property copyright "4xedge"
#property link      "http://www.4xedge.com"

#property indicator_chart_window
#property indicator_buffers 8
#property indicator_color3 C'64,0,0'
#property indicator_color4 C'0,60,0'
#property indicator_color5 C'145,0,0'
#property indicator_color6 C'0,125,0'
#property indicator_color7 C'255,45,45'
#property indicator_color8 C'21,255,80'
#property indicator_color1 Aqua
#property indicator_color2 CLR_NONE
//---- buffers

int  Chart=PERIOD_H4;

int  TMA1_Periods=21;
int  TMA1_Shift=5;
int  TMA1_ApplyTo=7;


int PeriodWATR1=11;
double Kwatr1=1.1000;
int HighLow1=0;
int NumberOfBarsToCalculate1 = 1000;
int Bars_width1=5;

int PeriodWATR2=7;
double Kwatr2=0.7;
int HighLow2=0;
int NumberOfBarsToCalculate2 = 1000;
int Bars_width2=3;

int PeriodWATR3=18;
double Kwatr3=1.8;
int HighLow3=0;
int NumberOfBarsToCalculate3 = 1000;
int Bars_width3=1;

int CountBars=1000;

int ErrorTimeOut=30;

double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double ExtMapBuffer5[];
double ExtMapBuffer6[];
double ExtMapBuffer7[];



int    weights[];
double divisor;
datetime last_e=0;

int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_LINE,STYLE_SOLID,2);
   SetIndexBuffer(0,ExtMapBuffer7);
   SetIndexStyle(1,DRAW_NONE);
   SetIndexStyle(2,DRAW_HISTOGRAM,STYLE_SOLID,Bars_width1);
   SetIndexBuffer(2,ExtMapBuffer1);
   SetIndexStyle(3,DRAW_HISTOGRAM,STYLE_SOLID,Bars_width1);
   SetIndexBuffer(3,ExtMapBuffer2);
   SetIndexStyle(4,DRAW_HISTOGRAM,STYLE_SOLID,Bars_width2);
   SetIndexBuffer(4,ExtMapBuffer3);
   SetIndexStyle(5,DRAW_HISTOGRAM,STYLE_SOLID,Bars_width2);
   SetIndexBuffer(5,ExtMapBuffer4);
   SetIndexStyle(6,DRAW_HISTOGRAM,STYLE_SOLID,Bars_width3);
   SetIndexBuffer(6,ExtMapBuffer5);
   SetIndexStyle(7,DRAW_HISTOGRAM,STYLE_SOLID,Bars_width3);
   SetIndexBuffer(7,ExtMapBuffer6);
   
//----

   SetIndexDrawBegin(2,PeriodWATR1);
   SetIndexDrawBegin(3,PeriodWATR1);
   SetIndexDrawBegin(4,PeriodWATR2);
   SetIndexDrawBegin(5,PeriodWATR2);

   
   SetIndexShift(0,TMA1_Shift);
  


    
   return(0);
  }
int deinit()
  {
   return(0);
  }
datetime last_t=0;
bool access=false;

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

    TMA(ExtMapBuffer7,TMA1_Periods,TMA1_ApplyTo);
    hist(ExtMapBuffer1,ExtMapBuffer2,PeriodWATR1,Kwatr1,HighLow1,NumberOfBarsToCalculate1);
    hist(ExtMapBuffer3,ExtMapBuffer4,PeriodWATR2,Kwatr2,HighLow2,NumberOfBarsToCalculate2);
    hist(ExtMapBuffer5,ExtMapBuffer6,PeriodWATR3,Kwatr3,HighLow3,NumberOfBarsToCalculate3);
   return(0);
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

void hist(double &r1[],double &r2[],int PeriodWATR,double Kwatr,int HighLow,int NumberOfBarsToCalculate)
  {
   double LineMinBuffer[];
   double LineMidBuffer[];
   ArrayResize(LineMinBuffer,NumberOfBarsToCalculate);
   ArrayResize(LineMidBuffer,NumberOfBarsToCalculate);
   int      i,shift,TrendMin,TrendMax,TrendMid;
   double   SminMin0,SmaxMin0,SminMin1,SmaxMin1,SumRange,dK,WATR0,WATRmax,WATRmin,WATRmid;
   double   SminMax0,SmaxMax0,SminMax1,SmaxMax1,SminMid0,SmaxMid0,SminMid1,SmaxMid1;
   double   linemin,linemax,linemid,Stoch1,Stoch2,bsmin,bsmax;
   //raduga : Variable theDelta - has a differense between two bsmin/max lines
   double theDelta;
   
   double prev_y = 0,prev_b = 0;
   	
   for(shift=NumberOfBarsToCalculate-1;shift>=0;shift--)
   {	
	SumRange=0;
	//raduga: initialization because of ZERO-DEVIDE ERROR
	//===============================
	Stoch1 = 0.0;
	Stoch2 = 0.0;
	//===============================
	for (i=PeriodWATR-1;i>=0;i--)
	    { 
       dK = 1+1.0*(PeriodWATR-i)/PeriodWATR;
       //raduga: MathAbs has no sense ragarding High - Low... Probably Close - open !!!!
       //SumRange+= dK*MathAbs(High[i+shift]-Low[i+shift]);
       SumRange+= dK*(High[i+shift]-Low[i+shift]);
       }
	WATR0 = SumRange/PeriodWATR;
	
	WATRmax=MathMax(WATR0,WATRmax);
	if (shift==NumberOfBarsToCalculate-1-PeriodWATR) WATRmin=WATR0;
	WATRmin=MathMin(WATR0,WATRmin);
	
	int StepSizeMin=MathRound(Kwatr*WATRmin/Point);
	int StepSizeMax=MathRound(Kwatr*WATRmax/Point);
	int StepSizeMid=MathRound(Kwatr*0.5*(WATRmax+WATRmin)/Point);
		
	if (HighLow>0)
	  {
	  SmaxMin0=Low[shift]+2*StepSizeMin*Point;
	  SminMin0=High[shift]-2*StepSizeMin*Point;
	  
	  SmaxMax0=Low[shift]+2*StepSizeMax*Point;
	  SminMax0=High[shift]-2*StepSizeMax*Point;
	  
	  SmaxMid0=Low[shift]+2*StepSizeMid*Point;
	  SminMid0=High[shift]-2*StepSizeMid*Point;
	  
	  if(Close[shift]>SmaxMin1) TrendMin=1; 
	  if(Close[shift]<SminMin1) TrendMin=-1;
	  
	  if(Close[shift]>SmaxMax1) TrendMax=1; 
	  if(Close[shift]<SminMax1) TrendMax=-1;
	  
	  if(Close[shift]>SmaxMid1) TrendMid=1; 
	  if(Close[shift]<SminMid1) TrendMid=-1;
	  }
	 
	if (HighLow == 0)
	  {
	  SmaxMin0=Close[shift]+2*StepSizeMin*Point;
	  SminMin0=Close[shift]-2*StepSizeMin*Point;
	  
	  SmaxMax0=Close[shift]+2*StepSizeMax*Point;
	  SminMax0=Close[shift]-2*StepSizeMax*Point;
	  
	  SmaxMid0=Close[shift]+2*StepSizeMid*Point;
	  SminMid0=Close[shift]-2*StepSizeMid*Point;
	  
	  if(Close[shift]>SmaxMin1) TrendMin=1; 
	  if(Close[shift]<SminMin1) TrendMin=-1;
	  
	  if(Close[shift]>SmaxMax1) TrendMax=1; 
	  if(Close[shift]<SminMax1) TrendMax=-1;
	  
	  if(Close[shift]>SmaxMid1) TrendMid=1; 
	  if(Close[shift]<SminMid1) TrendMid=-1;
	  }
	 	
	  if(TrendMin>0 && SminMin0<SminMin1) SminMin0=SminMin1;
	  if(TrendMin<0 && SmaxMin0>SmaxMin1) SmaxMin0=SmaxMin1;
		
	  if(TrendMax>0 && SminMax0<SminMax1) SminMax0=SminMax1;
	  if(TrendMax<0 && SmaxMax0>SmaxMax1) SmaxMax0=SmaxMax1;
	  
	  if(TrendMid>0 && SminMid0<SminMid1) SminMid0=SminMid1;
	  if(TrendMid<0 && SmaxMid0>SmaxMid1) SmaxMid0=SmaxMid1;
	  
	  
	  if (TrendMin>0) linemin=SminMin0+StepSizeMin*Point;
	  if (TrendMin<0) linemin=SmaxMin0-StepSizeMin*Point;
	  
	  if (TrendMax>0) linemax=SminMax0+StepSizeMax*Point;
	  if (TrendMax<0) linemax=SmaxMax0-StepSizeMax*Point;
	  
	  if (TrendMid>0) linemid=SminMid0+StepSizeMid*Point;
	  if (TrendMid<0) linemid=SmaxMid0-StepSizeMid*Point;
	  
	  bsmin=linemax-StepSizeMax*Point;
	  bsmax=linemax+StepSizeMax*Point;
	  //raduga: ZERO-DEVIDE-ERROR
	  //=============================
	  theDelta = bsmax-bsmin;
	  if(theDelta != 0) 
	  {
	     Stoch1=NormalizeDouble((linemin-bsmin)/theDelta,6);
	     Stoch2=NormalizeDouble((linemid-bsmin)/theDelta,6);
	  }
	  //==============================   
	  prev_y = (Stoch1 - Stoch2);
	  if(prev_y<0.0){
	     LineMinBuffer[shift] = prev_y;
	     LineMidBuffer[shift] = 0;
	     r1[shift]=MathMax(Open[shift],Close[shift]);
	     r2[shift]=MathMin(Close[shift],Open[shift]);
	     }
	  else if(prev_y>0.0){
	     LineMidBuffer[shift] = prev_y;
	     LineMinBuffer[shift] = 0;
	     r1[shift]=MathMin(Open[shift],Close[shift]);
	     r2[shift]=MathMax(Close[shift],Open[shift]);
	     }
	  
	  //prev_y = Stoch1;
	  //prev_b = Stoch2;
	  
	  
	  SminMin1=SminMin0;
	  SmaxMin1=SmaxMin0;
	  
	  SminMax1=SminMax0;
	  SmaxMax1=SmaxMax0;
	  
	  SminMid1=SminMid0;
	  SmaxMid1=SmaxMid0;
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
   InternetCloseHandle(hSession);
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
