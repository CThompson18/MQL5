//+------------------------------------------------------------------+
//|                                                     Flow_php.mq4 |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_chart_window


#import "wininet.dll"
int InternetAttemptConnect (int x);
int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);

int InternetConnectA( int hInternet, string lpszServerName, int nServerPort,
string lpszUsername, string lpszPassword, int dwService=1, int dwFlags=134217728, int lContext = 1 );

bool FtpPutFileA(int hConnect, string lpszLocalFile, string lpszNewRemoteFile, int dwFlags, int lContext = 1);
int InternetCloseHandle(int hInet);

extern string File_Name="flow_multi";
extern string FTP_Path="/4xlounge.com/charts4/tb/";
extern string File_Path="c:\\Program Files\\FiboTBconsole\\experts\\files\\";
extern int    Update_Secconds=30;
extern string Server="69.175.33.242";
extern string UserName="riffster";
extern string Password="hummer";


extern int    Periods = 4;
extern bool   invert_strength =  0;

string pairs[17]={"GBPAUD","EURCAD","EURGBP","EURCHF","GBPCHF","CADJPY","NZDJPY","AUDJPY","EURJPY","GBPJPY","USDJPY","USDCAD","USDCHF","NZDUSD","AUDUSD","GBPUSD","EURUSD"};
datetime last_update=0;

int init()
  {
   return(0);
  }

int deinit()
  {

   return(0);
  }
int start()
  {
   string fn=File_Name+".php";
   string fs="";
   FileDelete(fn);
   int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
   fs="<?php\n $flow = array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   
   for (int kk=0;kk<17;kk++){
      fs="$flow[\""+pairs[kk]+"\"] = array();\n";
      int l = 0;
      for (int i=0;i<51;i++){
         double gr = iCustom(pairs[kk],15,"Heatmap_RSI_modified",Periods,invert_strength,2,i);
         double re = iCustom(pairs[kk],15,"Heatmap_RSI_modified",Periods,invert_strength,7,i);         
         int rez=0;
         if (gr!=0) {rez=1;}else if (re!=0) {rez=2;}
         fs=fs+"$flow[\""+pairs[kk]+"\"]["+i+"]="+rez+";\n";
         l++;
      }
      for (i=0;i<27;i++){
         gr = iCustom(pairs[kk],30,"Heatmap_RSI_modified",Periods,invert_strength,2,i);
         re = iCustom(pairs[kk],30,"Heatmap_RSI_modified",Periods,invert_strength,7,i);         
         rez=0;
         if (gr!=0) {rez=1;}else if (re!=0) {rez=2;}
         fs=fs+"$flow[\""+pairs[kk]+"\"]["+l+"]="+rez+";\n";
         l++;
      }
      for (i=0;i<15;i++){
         gr = iCustom(pairs[kk],60,"Heatmap_RSI_modified",Periods,invert_strength,2,i);
         re = iCustom(pairs[kk],60,"Heatmap_RSI_modified",Periods,invert_strength,7,i);         
         rez=0;
         if (gr!=0) {rez=1;}else if (re!=0) {rez=2;}
         fs=fs+"$flow[\""+pairs[kk]+"\"]["+l+"]="+rez+";\n";
         l++;
      }
      for (i=0;i<3;i++){
         gr = iCustom(pairs[kk],240,"Heatmap_RSI_modified",Periods,invert_strength,2,i);
         re = iCustom(pairs[kk],240,"Heatmap_RSI_modified",Periods,invert_strength,7,i);         
         rez=0;
         if (gr!=0) {rez=1;}else if (re!=0) {rez=2;}
         fs=fs+"$flow[\""+pairs[kk]+"\"]["+l+"]="+rez+";\n";
         l++;
      }      
      FileWriteString(handle,fs,StringLen(fs));
      FileFlush(handle);
   }
   fs="?>\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      
   FileClose(handle);
   
   if (TimeCurrent()>=last_update+Update_Secconds){
      int rw = InternetAttemptConnect(0);
      int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
      int hURL = InternetConnectA(hInternetSession, Server, 21, UserName, Password, 1, 134217728, 1);
   
      FtpPutFileA(hURL, File_Path+fn, FTP_Path+fn, 2, 1);
      InternetCloseHandle(hURL);
      InternetCloseHandle(hInternetSession);
   
      Print("Send FTP completed at : "+TimeToStr(Time[0])); 
      last_update=TimeCurrent();            
   }

   return(0);
  }

