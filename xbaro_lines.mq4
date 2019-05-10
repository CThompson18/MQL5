//+------------------------------------------------------------------+
//|                                                  xbaro_lines.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window

#import "wininet.dll"
int InternetAttemptConnect (int x);
int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);

int InternetConnectA( int hInternet, string lpszServerName, int nServerPort,
string lpszUsername, string lpszPassword, int dwService=1, int dwFlags=134217728, int lContext = 1 );

bool FtpPutFileA(int hConnect, string lpszLocalFile, string lpszNewRemoteFile, int dwFlags, int lContext = 1);
int InternetCloseHandle(int hInet);

extern string File_Name="xbaro_line";
extern string FTP_Path="/4xlounge.com/charts4/baro/";
extern string File_Path="c:\\Program Files\\FIBO Group2\\experts\files\\";
extern int    Update_Secconds=10;
extern string Server="67.15.97.27";
extern string UserName="riffster";
extern string Password="hummer";


extern int CountBars=30;
extern int Scalp_RSI_Period=7;
extern int Scalp_RSI_Price=0;
extern int Scalp_TF=1;

extern int Short_RSI_Period=14;
extern int Short_RSI_Price=0;
extern int Short_TF=5;

extern int Med_RSI_Period=7;
extern int Med_RSI_Price=0;
extern int Med_TF=60;

extern int Long_RSI_Period=7;
extern int Long_RSI_Price=0;
extern int Long_TF=240;
extern int Precision=3;

int ErrorTimeOut=30;
datetime last_update=0;

int init()
  {
     return(0);
  }
int deinit()
  {
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int start()
  {
   int tf[4];
   tf[0]=Scalp_TF;tf[1]=Short_TF;tf[2]=Med_TF;tf[3]=Long_TF;
   int rsi[4];
   rsi[0]=Scalp_RSI_Period;rsi[1]=Short_RSI_Period;rsi[2]=Med_RSI_Period;rsi[3]=Long_RSI_Period;
   int rsip[4];
   rsip[0]=Scalp_RSI_Price;rsip[1]=Short_RSI_Price;rsip[2]=Med_RSI_Price;rsip[3]=Long_RSI_Price;
   
   string fn1=File_Name+"_scalp.php";
   string fn2=File_Name+"_short.php";
   string fn3=File_Name+"_med.php";
   string fn4=File_Name+"_long.php";
   
   string fs="";
   FileDelete(fn1);FileDelete(fn2);FileDelete(fn3);FileDelete(fn4);
   int handle1=FileOpen(fn1,FILE_WRITE|FILE_BIN);
   int handle2=FileOpen(fn2,FILE_WRITE|FILE_BIN);
   int handle3=FileOpen(fn3,FILE_WRITE|FILE_BIN);
   int handle4=FileOpen(fn4,FILE_WRITE|FILE_BIN);
   fs="<?php\n";
   FileWriteString(handle1,fs,StringLen(fs));
   FileFlush(handle1);    
   FileWriteString(handle2,fs,StringLen(fs));
   FileFlush(handle2);    
   FileWriteString(handle3,fs,StringLen(fs));
   FileFlush(handle3);    
   FileWriteString(handle4,fs,StringLen(fs));
   FileFlush(handle4);    

   fs="$xb_eur = array();\n$xb_usd = array();\n$xb_gbp = array();\n$xb_chf = array();\n$xb_jpy = array();\n$xb_cad = array();\n$xb_aud = array();\n$xb_nzd = array();\n";
   FileWriteString(handle1,fs,StringLen(fs));
   FileFlush(handle1);    
   FileWriteString(handle2,fs,StringLen(fs));
   FileFlush(handle2);    
   FileWriteString(handle3,fs,StringLen(fs));
   FileFlush(handle3);    
   FileWriteString(handle4,fs,StringLen(fs));
   FileFlush(handle4);    

   int handle;
   for (int j=0;j<4;j++){
      if (j==0) handle=handle1;
      if (j==1) handle=handle2;
      if (j==2) handle=handle3;
      if (j==3) handle=handle4;
      for (int i=CountBars-1;i>=0;i--){
         double v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],0,i);
         fs="$xb_gbp["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],1,i);
         fs="$xb_eur["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],2,i);
         fs="$xb_aud["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],3,i);
         fs="$xb_nzd["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],4,i);
         fs="$xb_jpy["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],5,i);
         fs="$xb_chf["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],6,i);
         fs="$xb_cad["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
         v = iCustom(Symbol(),tf[j],"New_Market_Barometer72",rsi[j],rsip[j],7,i);
         fs="$xb_usd["+(CountBars-1-i)+"] = "+DoubleToStr(v,Precision)+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
      }
   }

   fs="?>\n";
   FileWriteString(handle1,fs,StringLen(fs));
   FileFlush(handle1);      
   FileClose(handle1);
   FileWriteString(handle2,fs,StringLen(fs));
   FileFlush(handle2);      
   FileClose(handle2);
   FileWriteString(handle3,fs,StringLen(fs));
   FileFlush(handle3);      
   FileClose(handle3);
   FileWriteString(handle4,fs,StringLen(fs));
   FileFlush(handle4);      
   FileClose(handle4);

   if (TimeCurrent()>=last_update+Update_Secconds){
      int rw = InternetAttemptConnect(0);
      int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
      int hURL = InternetConnectA(hInternetSession, Server, 21, UserName, Password, 1, 134217728, 1);
   
      FtpPutFileA(hURL, File_Path+fn1, FTP_Path+fn1, 2, 1);
      FtpPutFileA(hURL, File_Path+fn2, FTP_Path+fn2, 2, 1);
      FtpPutFileA(hURL, File_Path+fn3, FTP_Path+fn3, 2, 1);
      FtpPutFileA(hURL, File_Path+fn4, FTP_Path+fn4, 2, 1);

      InternetCloseHandle(hURL);
      InternetCloseHandle(hInternetSession);
   
      Print("Send FTP completed at : "+TimeToStr(Time[0])); 
      last_update=TimeCurrent();            
   }
   
   return(0);
  }
//+------------------------------------------------------------------+