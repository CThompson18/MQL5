//+------------------------------------------------------------------+
//|                                          Barometer Dashboard.mq4 |
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

/*#import "kernel32.dll"
bool MoveFileA (string fromfile, string tofile);
*/
extern string File_Name="barometer_values_6";
extern string File_Path="C:\\Program Files\\FiboTBconsole2\\experts\\files\\";
extern int    Update_Secconds=15;
extern string FTP_Path="/data/";

extern string Server="10.10.10.2";
string UserName="riffster@4xedge.com";
extern string Password="pf4377dxzaQ~";

extern int Scalper_RSI_Period=14;
extern int Scalper_RSI_Price=0;
extern double Scalper_TF=PERIOD_M1;

extern int Short_RSI_Period=14;
extern int Short_RSI_Price=0;
extern double Short_TF=PERIOD_M5;

extern int Med_RSI_Period=7;
extern int Med_RSI_Price=0;
extern double Med_TF=PERIOD_H1;

extern int Long_RSI_Period=7;
extern int Long_RSI_Price=0;
extern double Long_TF=PERIOD_H4;

extern int Big_RSI_Period=7;
extern int Big_RSI_Price=0;
extern double Big_TF=PERIOD_D1;

extern int Bigger_RSI_Period=7;
extern int Bigger_RSI_Price=0;
extern double Bigger_TF=PERIOD_W1;

extern int Number_of_Bars=1;


extern int Precision=1;

datetime last_update=0;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
   return(0);
  }

//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int start()
  { 
  if (TimeCurrent()>=last_update+Update_Secconds){
      string fn=File_Name+".php";
      string fs="";
      FileDelete(fn);
      int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
      fs="<?php\n $bd=array(); $time=array();\n";
      FileWriteString(handle,fs,StringLen(fs));
      FileFlush(handle);    
      int i;

   
   fs="$time[\"scalper\"]=array();\n";
   fs=fs+"$time[\"short\"]=array();\n";
   fs=fs+"$time[\"med\"]=array();\n";
   fs=fs+"$time[\"long\"]=array();\n";
   fs=fs+"$time[\"big\"]=array();\n";
   fs=fs+"$time[\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   

   fs="$bd[\"EUR\"]=array();\n";
   fs=fs+"$bd[\"EUR\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"EUR\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"EUR\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"EUR\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"EUR\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"EUR\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      
   
   fs="$bd[\"GBP\"]=array();\n";
   fs=fs+"$bd[\"GBP\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"GBP\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"GBP\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"GBP\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"GBP\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"GBP\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      

   fs="$bd[\"JPY\"]=array();\n";
   fs=fs+"$bd[\"JPY\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"JPY\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"JPY\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"JPY\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"JPY\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"JPY\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      

   fs="$bd[\"CHF\"]=array();\n";
   fs=fs+"$bd[\"CHF\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"CHF\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"CHF\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"CHF\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"CHF\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"CHF\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      

   fs="$bd[\"CAD\"]=array();\n";
   fs=fs+"$bd[\"CAD\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"CAD\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"CAD\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"CAD\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"CAD\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"CAD\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      

   fs="$bd[\"AUD\"]=array();\n";
   fs=fs+"$bd[\"AUD\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"AUD\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"AUD\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"AUD\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"AUD\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"AUD\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      

   fs="$bd[\"NZD\"]=array();\n";
   fs=fs+"$bd[\"NZD\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"NZD\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"NZD\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"NZD\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"NZD\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"NZD\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      

   fs="$bd[\"USD\"]=array();\n";
   fs=fs+"$bd[\"USD\"][\"scalper\"]=array();\n";
   fs=fs+"$bd[\"USD\"][\"short\"]=array();\n";
   fs=fs+"$bd[\"USD\"][\"med\"]=array();\n";
   fs=fs+"$bd[\"USD\"][\"long\"]=array();\n";
   fs=fs+"$bd[\"USD\"][\"big\"]=array();\n";
   fs=fs+"$bd[\"USD\"][\"bigger\"]=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);    
     
   for (i=50; i>=0; i--){
      double Scalper_gbp=iRSI("GBPUSD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_eur=iRSI("EURUSD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_aud=iRSI("AUDUSD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_nzd=iRSI("NZDUSD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_jpy=iRSI("USDJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_chf=iRSI("USDCHF",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_cad=iRSI("USDCAD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);             
      double Scalper_eurgbp=iRSI("EURGBP",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_eurjpy=iRSI("EURJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_eurchf=iRSI("EURCHF",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_eurcad=iRSI("EURCAD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_euraud=iRSI("EURAUD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_eurnzd=iRSI("EURNZD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_gbpjpy=iRSI("GBPJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_gbpchf=iRSI("GBPCHF",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_gbpcad=iRSI("GBPCAD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_gbpaud=iRSI("GBPAUD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_gbpnzd=iRSI("GBPNZD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_chfjpy=iRSI("CHFJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_cadjpy=iRSI("CADJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_audjpy=iRSI("AUDJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_nzdjpy=iRSI("NZDJPY",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_cadchf=iRSI("CADCHF",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_audchf=iRSI("AUDCHF",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_nzdchf=iRSI("NZDCHF",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_audcad=iRSI("AUDCAD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_nzdcad=iRSI("NZDCAD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);
      double Scalper_audnzd=iRSI("AUDNZD",Scalper_TF,Scalper_RSI_Period,Scalper_RSI_Price,i);  
      
      double gbp_scalper=(Scalper_gbp+100-Scalper_eurgbp+Scalper_gbpjpy+Scalper_gbpchf+Scalper_gbpcad+Scalper_gbpaud+Scalper_gbpnzd)/7;      
      double eur_scalper=(Scalper_eur+Scalper_eurgbp+Scalper_eurjpy+Scalper_eurchf+Scalper_eurcad+Scalper_euraud+Scalper_eurnzd)/7;      
      double aud_scalper=(Scalper_aud+100-Scalper_euraud+100-Scalper_gbpaud+Scalper_audjpy+Scalper_audchf+Scalper_audcad+Scalper_audnzd)/7;
      double nzd_scalper=(Scalper_nzd+100-Scalper_eurnzd+100-Scalper_gbpnzd+Scalper_nzdjpy+Scalper_nzdchf+Scalper_nzdcad+100-Scalper_audnzd)/7;      
      double jpy_scalper=(100-Scalper_jpy+100-Scalper_eurjpy+100-Scalper_gbpjpy+100-Scalper_chfjpy+100-Scalper_cadjpy+100-Scalper_audjpy+100-Scalper_nzdjpy)/7;      
      double chf_scalper=(100-Scalper_chf+100-Scalper_eurchf+100-Scalper_gbpjpy+Scalper_chfjpy+100-Scalper_cadchf+100-Scalper_audchf+100-Scalper_nzdchf)/7;
      double cad_scalper=(100-Scalper_cad+100-Scalper_eurcad+100-Scalper_gbpcad+Scalper_cadjpy+Scalper_cadchf+100-Scalper_audcad+100-Scalper_nzdcad)/7;            
      double usd_scalper=(100-Scalper_eur+100-Scalper_gbp+Scalper_jpy+Scalper_chf+Scalper_cad+100-Scalper_aud+100-Scalper_nzd)/7;      

      
      double Short_gbp=iRSI("GBPUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_eur=iRSI("EURUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_aud=iRSI("AUDUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_nzd=iRSI("NZDUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_jpy=iRSI("USDJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_chf=iRSI("USDCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_cad=iRSI("USDCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);             
      double Short_eurgbp=iRSI("EURGBP",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_eurjpy=iRSI("EURJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_eurchf=iRSI("EURCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_eurcad=iRSI("EURCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_euraud=iRSI("EURAUD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_eurnzd=iRSI("EURNZD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_gbpjpy=iRSI("GBPJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_gbpchf=iRSI("GBPCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_gbpcad=iRSI("GBPCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_gbpaud=iRSI("GBPAUD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_gbpnzd=iRSI("GBPNZD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_chfjpy=iRSI("CHFJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_cadjpy=iRSI("CADJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_audjpy=iRSI("AUDJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_nzdjpy=iRSI("NZDJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_cadchf=iRSI("CADCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_audchf=iRSI("AUDCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_nzdchf=iRSI("NZDCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_audcad=iRSI("AUDCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_nzdcad=iRSI("NZDCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);
      double Short_audnzd=iRSI("AUDNZD",Short_TF,Short_RSI_Period,Short_RSI_Price,i);  
      
      double gbp_short=(Short_gbp+100-Short_eurgbp+Short_gbpjpy+Short_gbpchf+Short_gbpcad+Short_gbpaud+Short_gbpnzd)/7;      
      double eur_short=(Short_eur+Short_eurgbp+Short_eurjpy+Short_eurchf+Short_eurcad+Short_euraud+Short_eurnzd)/7;      
      double aud_short=(Short_aud+100-Short_euraud+100-Short_gbpaud+Short_audjpy+Short_audchf+Short_audcad+Short_audnzd)/7;
      double nzd_short=(Short_nzd+100-Short_eurnzd+100-Short_gbpnzd+Short_nzdjpy+Short_nzdchf+Short_nzdcad+100-Short_audnzd)/7;      
      double jpy_short=(100-Short_jpy+100-Short_eurjpy+100-Short_gbpjpy+100-Short_chfjpy+100-Short_cadjpy+100-Short_audjpy+100-Short_nzdjpy)/7;      
      double chf_short=(100-Short_chf+100-Short_eurchf+100-Short_gbpjpy+Short_chfjpy+100-Short_cadchf+100-Short_audchf+100-Short_nzdchf)/7;
      double cad_short=(100-Short_cad+100-Short_eurcad+100-Short_gbpcad+Short_cadjpy+Short_cadchf+100-Short_audcad+100-Short_nzdcad)/7;            
      double usd_short=(100-Short_eur+100-Short_gbp+Short_jpy+Short_chf+Short_cad+100-Short_aud+100-Short_nzd)/7;      
      
     
      double Med_gbp=iRSI("GBPUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_eur=iRSI("EURUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_aud=iRSI("AUDUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_nzd=iRSI("NZDUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_jpy=iRSI("USDJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_chf=iRSI("USDCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_cad=iRSI("USDCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);             
      double Med_eurgbp=iRSI("EURGBP",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_eurjpy=iRSI("EURJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_eurchf=iRSI("EURCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_eurcad=iRSI("EURCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_euraud=iRSI("EURAUD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_eurnzd=iRSI("EURNZD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_gbpjpy=iRSI("GBPJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_gbpchf=iRSI("GBPCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_gbpcad=iRSI("GBPCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_gbpaud=iRSI("GBPAUD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_gbpnzd=iRSI("GBPNZD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_chfjpy=iRSI("CHFJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_cadjpy=iRSI("CADJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_audjpy=iRSI("AUDJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_nzdjpy=iRSI("NZDJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_cadchf=iRSI("CADCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_audchf=iRSI("AUDCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_nzdchf=iRSI("NZDCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_audcad=iRSI("AUDCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_nzdcad=iRSI("NZDCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);
      double Med_audnzd=iRSI("AUDNZD",Med_TF,Med_RSI_Period,Med_RSI_Price,i);  

      double gbp_med=(Med_gbp+100-Med_eurgbp+Med_gbpjpy+Med_gbpchf+Med_gbpcad+Med_gbpaud+Med_gbpnzd)/7;      
      double eur_med=(Med_eur+Med_eurgbp+Med_eurjpy+Med_eurchf+Med_eurcad+Med_euraud+Med_eurnzd)/7;      
      double aud_med=(Med_aud+100-Med_euraud+100-Med_gbpaud+Med_audjpy+Med_audchf+Med_audcad+Med_audnzd)/7;
      double nzd_med=(Med_nzd+100-Med_eurnzd+100-Med_gbpnzd+Med_nzdjpy+Med_nzdchf+Med_nzdcad+100-Med_audnzd)/7;      
      double jpy_med=(100-Med_jpy+100-Med_eurjpy+100-Med_gbpjpy+100-Med_chfjpy+100-Med_cadjpy+100-Med_audjpy+100-Med_nzdjpy)/7;      
      double chf_med=(100-Med_chf+100-Med_eurchf+100-Med_gbpjpy+Med_chfjpy+100-Med_cadchf+100-Med_audchf+100-Med_nzdchf)/7;
      double cad_med=(100-Med_cad+100-Med_eurcad+100-Med_gbpcad+Med_cadjpy+Med_cadchf+100-Med_audcad+100-Med_nzdcad)/7;            
      double usd_med=(100-Med_eur+100-Med_gbp+Med_jpy+Med_chf+Med_cad+100-Med_aud+100-Med_nzd)/7;      


      double Long_gbp=iRSI("GBPUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_eur=iRSI("EURUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_aud=iRSI("AUDUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_nzd=iRSI("NZDUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_jpy=iRSI("USDJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_chf=iRSI("USDCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_cad=iRSI("USDCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);             
      double Long_eurgbp=iRSI("EURGBP",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_eurjpy=iRSI("EURJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_eurchf=iRSI("EURCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_eurcad=iRSI("EURCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_euraud=iRSI("EURAUD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_eurnzd=iRSI("EURNZD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_gbpjpy=iRSI("GBPJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_gbpchf=iRSI("GBPCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_gbpcad=iRSI("GBPCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_gbpaud=iRSI("GBPAUD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_gbpnzd=iRSI("GBPNZD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_chfjpy=iRSI("CHFJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_cadjpy=iRSI("CADJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_audjpy=iRSI("AUDJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_nzdjpy=iRSI("NZDJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_cadchf=iRSI("CADCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_audchf=iRSI("AUDCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_nzdchf=iRSI("NZDCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_audcad=iRSI("AUDCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_nzdcad=iRSI("NZDCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);
      double Long_audnzd=iRSI("AUDNZD",Long_TF,Long_RSI_Period,Long_RSI_Price,i);  

      double gbp_long=(Long_gbp+100-Long_eurgbp+Long_gbpjpy+Long_gbpchf+Long_gbpcad+Long_gbpaud+Long_gbpnzd)/7;      
      double eur_long=(Long_eur+Long_eurgbp+Long_eurjpy+Long_eurchf+Long_eurcad+Long_euraud+Long_eurnzd)/7;      
      double aud_long=(Long_aud+100-Long_euraud+100-Long_gbpaud+Long_audjpy+Long_audchf+Long_audcad+Long_audnzd)/7;
      double nzd_long=(Long_nzd+100-Long_eurnzd+100-Long_gbpnzd+Long_nzdjpy+Long_nzdchf+Long_nzdcad+100-Long_audnzd)/7;      
      double jpy_long=(100-Long_jpy+100-Long_eurjpy+100-Long_gbpjpy+100-Long_chfjpy+100-Long_cadjpy+100-Long_audjpy+100-Long_nzdjpy)/7;      
      double chf_long=(100-Long_chf+100-Long_eurchf+100-Long_gbpjpy+Long_chfjpy+100-Long_cadchf+100-Long_audchf+100-Long_nzdchf)/7;
      double cad_long=(100-Long_cad+100-Long_eurcad+100-Long_gbpcad+Long_cadjpy+Long_cadchf+100-Long_audcad+100-Long_nzdcad)/7;            
      double usd_long=(100-Long_eur+100-Long_gbp+Long_jpy+Long_chf+Long_cad+100-Long_aud+100-Long_nzd)/7;          

      double Big_gbp=iRSI("GBPUSD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_eur=iRSI("EURUSD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_aud=iRSI("AUDUSD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_nzd=iRSI("NZDUSD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_jpy=iRSI("USDJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_chf=iRSI("USDCHF",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_cad=iRSI("USDCAD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);             
      double Big_eurgbp=iRSI("EURGBP",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_eurjpy=iRSI("EURJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_eurchf=iRSI("EURCHF",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_eurcad=iRSI("EURCAD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_euraud=iRSI("EURAUD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_eurnzd=iRSI("EURNZD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_gbpjpy=iRSI("GBPJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_gbpchf=iRSI("GBPCHF",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_gbpcad=iRSI("GBPCAD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_gbpaud=iRSI("GBPAUD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_gbpnzd=iRSI("GBPNZD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_chfjpy=iRSI("CHFJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_cadjpy=iRSI("CADJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_audjpy=iRSI("AUDJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_nzdjpy=iRSI("NZDJPY",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_cadchf=iRSI("CADCHF",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_audchf=iRSI("AUDCHF",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_nzdchf=iRSI("NZDCHF",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_audcad=iRSI("AUDCAD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_nzdcad=iRSI("NZDCAD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);
      double Big_audnzd=iRSI("AUDNZD",Big_TF,Big_RSI_Period,Big_RSI_Price,i);  

      double gbp_big=(Big_gbp+100-Big_eurgbp+Big_gbpjpy+Big_gbpchf+Big_gbpcad+Big_gbpaud+Big_gbpnzd)/7;      
      double eur_big=(Big_eur+Big_eurgbp+Big_eurjpy+Big_eurchf+Big_eurcad+Big_euraud+Big_eurnzd)/7;      
      double aud_big=(Big_aud+100-Big_euraud+100-Big_gbpaud+Big_audjpy+Big_audchf+Big_audcad+Big_audnzd)/7;
      double nzd_big=(Big_nzd+100-Big_eurnzd+100-Big_gbpnzd+Big_nzdjpy+Big_nzdchf+Big_nzdcad+100-Big_audnzd)/7;      
      double jpy_big=(100-Big_jpy+100-Big_eurjpy+100-Big_gbpjpy+100-Big_chfjpy+100-Big_cadjpy+100-Big_audjpy+100-Big_nzdjpy)/7;      
      double chf_big=(100-Big_chf+100-Big_eurchf+100-Big_gbpjpy+Big_chfjpy+100-Big_cadchf+100-Big_audchf+100-Big_nzdchf)/7;
      double cad_big=(100-Big_cad+100-Big_eurcad+100-Big_gbpcad+Big_cadjpy+Big_cadchf+100-Big_audcad+100-Big_nzdcad)/7;            
      double usd_big=(100-Big_eur+100-Big_gbp+Big_jpy+Big_chf+Big_cad+100-Big_aud+100-Big_nzd)/7;

      double Bigger_gbp=iRSI("GBPUSD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_eur=iRSI("EURUSD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_aud=iRSI("AUDUSD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_nzd=iRSI("NZDUSD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_jpy=iRSI("USDJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_chf=iRSI("USDCHF",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_cad=iRSI("USDCAD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);             
      double Bigger_eurgbp=iRSI("EURGBP",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_eurjpy=iRSI("EURJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_eurchf=iRSI("EURCHF",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_eurcad=iRSI("EURCAD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_euraud=iRSI("EURAUD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_eurnzd=iRSI("EURNZD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_gbpjpy=iRSI("GBPJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_gbpchf=iRSI("GBPCHF",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_gbpcad=iRSI("GBPCAD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_gbpaud=iRSI("GBPAUD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_gbpnzd=iRSI("GBPNZD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_chfjpy=iRSI("CHFJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_cadjpy=iRSI("CADJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_audjpy=iRSI("AUDJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_nzdjpy=iRSI("NZDJPY",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_cadchf=iRSI("CADCHF",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_audchf=iRSI("AUDCHF",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_nzdchf=iRSI("NZDCHF",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_audcad=iRSI("AUDCAD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_nzdcad=iRSI("NZDCAD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);
      double Bigger_audnzd=iRSI("AUDNZD",Bigger_TF,Bigger_RSI_Period,Bigger_RSI_Price,i);  

      double gbp_bigger=(Bigger_gbp+100-Bigger_eurgbp+Bigger_gbpjpy+Bigger_gbpchf+Bigger_gbpcad+Bigger_gbpaud+Bigger_gbpnzd)/7;      
      double eur_bigger=(Bigger_eur+Bigger_eurgbp+Bigger_eurjpy+Bigger_eurchf+Bigger_eurcad+Bigger_euraud+Bigger_eurnzd)/7;      
      double aud_bigger=(Bigger_aud+100-Bigger_euraud+100-Bigger_gbpaud+Bigger_audjpy+Bigger_audchf+Bigger_audcad+Bigger_audnzd)/7;
      double nzd_bigger=(Bigger_nzd+100-Bigger_eurnzd+100-Bigger_gbpnzd+Bigger_nzdjpy+Bigger_nzdchf+Bigger_nzdcad+100-Bigger_audnzd)/7;      
      double jpy_bigger=(100-Bigger_jpy+100-Bigger_eurjpy+100-Bigger_gbpjpy+100-Bigger_chfjpy+100-Bigger_cadjpy+100-Bigger_audjpy+100-Bigger_nzdjpy)/7;      
      double chf_bigger=(100-Bigger_chf+100-Bigger_eurchf+100-Bigger_gbpjpy+Bigger_chfjpy+100-Bigger_cadchf+100-Bigger_audchf+100-Bigger_nzdchf)/7;
      double cad_bigger=(100-Bigger_cad+100-Bigger_eurcad+100-Bigger_gbpcad+Bigger_cadjpy+Bigger_cadchf+100-Bigger_audcad+100-Bigger_nzdcad)/7;            
      double usd_bigger=(100-Bigger_eur+100-Bigger_gbp+Bigger_jpy+Bigger_chf+Bigger_cad+100-Bigger_aud+100-Bigger_nzd)/7;
   
   
   fs="$time[\"scalper\"]["+i+"]="+iTime(Symbol(),Scalper_TF,i)+";\n";
   fs=fs+"$time[\"short\"]["+i+"]="+iTime(Symbol(),Short_TF,i)+";\n";
   fs=fs+"$time[\"med\"]["+i+"]="+iTime(Symbol(),Med_TF,i)+";\n";
   fs=fs+"$time[\"long\"]["+i+"]="+iTime(Symbol(),Long_TF,i)+";\n";
   fs=fs+"$time[\"big\"]["+i+"]="+iTime(Symbol(),Big_TF,i)+";\n";
   fs=fs+"$time[\"bigger\"]["+i+"]="+iTime(Symbol(),Bigger_TF,i)+";\n";

   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   
         
   fs="$bd[\"EUR\"][\"short\"]["+i+"]="+DoubleToStr(eur_short,Precision)+";\n";
   fs=fs+"$bd[\"EUR\"][\"med\"]["+i+"]="+DoubleToStr(eur_med,Precision)+";\n";
   fs=fs+"$bd[\"EUR\"][\"long\"]["+i+"]="+DoubleToStr(eur_long,Precision)+";\n";
   fs=fs+"$bd[\"EUR\"][\"scalper\"]["+i+"]="+DoubleToStr(eur_scalper,Precision)+";\n";
   fs=fs+"$bd[\"EUR\"][\"big\"]["+i+"]="+DoubleToStr(eur_big,Precision)+";\n";
   fs=fs+"$bd[\"EUR\"][\"bigger\"]["+i+"]="+DoubleToStr(eur_bigger,Precision)+";\n";
   
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   
   
   fs="$bd[\"GBP\"][\"short\"]["+i+"]="+DoubleToStr(gbp_short,Precision)+";\n";
   fs=fs+"$bd[\"GBP\"][\"med\"]["+i+"]="+DoubleToStr(gbp_med,Precision)+";\n";
   fs=fs+"$bd[\"GBP\"][\"long\"]["+i+"]="+DoubleToStr(gbp_long,Precision)+";\n";
   fs=fs+"$bd[\"GBP\"][\"scalper\"]["+i+"]="+DoubleToStr(gbp_scalper,Precision)+";\n";
   fs=fs+"$bd[\"GBP\"][\"big\"]["+i+"]="+DoubleToStr(gbp_big,Precision)+";\n";
   fs=fs+"$bd[\"GBP\"][\"bigger\"]["+i+"]="+DoubleToStr(gbp_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);
   

   fs="$bd[\"JPY\"][\"short\"]["+i+"]="+DoubleToStr(jpy_short,Precision)+";\n";
   fs=fs+"$bd[\"JPY\"][\"med\"]["+i+"]="+DoubleToStr(jpy_med,Precision)+";\n";
   fs=fs+"$bd[\"JPY\"][\"long\"]["+i+"]="+DoubleToStr(jpy_long,Precision)+";\n";
   fs=fs+"$bd[\"JPY\"][\"scalper\"]["+i+"]="+DoubleToStr(jpy_scalper,Precision)+";\n";
   fs=fs+"$bd[\"JPY\"][\"big\"]["+i+"]="+DoubleToStr(jpy_big,Precision)+";\n";
   fs=fs+"$bd[\"JPY\"][\"bigger\"]["+i+"]="+DoubleToStr(jpy_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);



   fs="$bd[\"CHF\"][\"short\"]["+i+"]="+DoubleToStr(chf_short,Precision)+";\n";
   fs=fs+"$bd[\"CHF\"][\"med\"]["+i+"]="+DoubleToStr(chf_med,Precision)+";\n";
   fs=fs+"$bd[\"CHF\"][\"long\"]["+i+"]="+DoubleToStr(chf_long,Precision)+";\n";
   fs=fs+"$bd[\"CHF\"][\"scalper\"]["+i+"]="+DoubleToStr(chf_scalper,Precision)+";\n";
   fs=fs+"$bd[\"CHF\"][\"big\"]["+i+"]="+DoubleToStr(chf_big,Precision)+";\n";
   fs=fs+"$bd[\"CHF\"][\"bigger\"]["+i+"]="+DoubleToStr(chf_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   


   fs="$bd[\"CAD\"][\"short\"]["+i+"]="+DoubleToStr(cad_short,Precision)+";\n";
   fs=fs+"$bd[\"CAD\"][\"med\"]["+i+"]="+DoubleToStr(cad_med,Precision)+";\n";
   fs=fs+"$bd[\"CAD\"][\"long\"]["+i+"]="+DoubleToStr(cad_long,Precision)+";\n";
   fs=fs+"$bd[\"CAD\"][\"scalper\"]["+i+"]="+DoubleToStr(cad_scalper,Precision)+";\n";
   fs=fs+"$bd[\"CAD\"][\"big\"]["+i+"]="+DoubleToStr(cad_big,Precision)+";\n";
   fs=fs+"$bd[\"CAD\"][\"bigger\"]["+i+"]="+DoubleToStr(cad_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);  


   fs="$bd[\"AUD\"][\"short\"]["+i+"]="+DoubleToStr(aud_short,Precision)+";\n";
   fs=fs+"$bd[\"AUD\"][\"med\"]["+i+"]="+DoubleToStr(aud_med,Precision)+";\n";
   fs=fs+"$bd[\"AUD\"][\"long\"]["+i+"]="+DoubleToStr(aud_long,Precision)+";\n";
   fs=fs+"$bd[\"AUD\"][\"scalper\"]["+i+"]="+DoubleToStr(aud_scalper,Precision)+";\n";
   fs=fs+"$bd[\"AUD\"][\"big\"]["+i+"]="+DoubleToStr(aud_big,Precision)+";\n";
   fs=fs+"$bd[\"AUD\"][\"bigger\"]["+i+"]="+DoubleToStr(aud_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   


   fs="$bd[\"NZD\"][\"short\"]["+i+"]="+DoubleToStr(nzd_short,Precision)+";\n";
   fs=fs+"$bd[\"NZD\"][\"med\"]["+i+"]="+DoubleToStr(nzd_med,Precision)+";\n";
   fs=fs+"$bd[\"NZD\"][\"long\"]["+i+"]="+DoubleToStr(nzd_long,Precision)+";\n";
   fs=fs+"$bd[\"NZD\"][\"scalper\"]["+i+"]="+DoubleToStr(nzd_scalper,Precision)+";\n";
   fs=fs+"$bd[\"NZD\"][\"big\"]["+i+"]="+DoubleToStr(nzd_big,Precision)+";\n";
   fs=fs+"$bd[\"NZD\"][\"bigger\"]["+i+"]="+DoubleToStr(nzd_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);
   

   fs="$bd[\"USD\"][\"short\"]["+i+"]="+DoubleToStr(usd_short,Precision)+";\n";
   fs=fs+"$bd[\"USD\"][\"med\"]["+i+"]="+DoubleToStr(usd_med,Precision)+";\n";
   fs=fs+"$bd[\"USD\"][\"long\"]["+i+"]="+DoubleToStr(usd_long,Precision)+";\n";
   fs=fs+"$bd[\"USD\"][\"scalper\"]["+i+"]="+DoubleToStr(usd_scalper,Precision)+";\n";
   fs=fs+"$bd[\"USD\"][\"big\"]["+i+"]="+DoubleToStr(usd_big,Precision)+";\n";
   fs=fs+"$bd[\"USD\"][\"bigger\"]["+i+"]="+DoubleToStr(usd_bigger,Precision)+";\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);
  }   
   fs="?>\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      
   FileClose(handle);
   
   
      int rw = InternetAttemptConnect(0);
      int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
      int hURL = InternetConnectA(hInternetSession, Server, 21, UserName, Password, 1, 0, 1);
   
      bool x = FtpPutFileA(hURL, File_Path+fn, FTP_Path+fn, 2, 1);
      InternetCloseHandle(hURL);
      InternetCloseHandle(hInternetSession);
   
      if (!x) 
         Print("FAILED Send FTP at : "+TimeToStr(TimeCurrent()));
      /*FileDelete(File_Name+".php");
      MoveFileA(File_Path+fn, File_Path+File_Name+".php");*/
      last_update=TimeCurrent();            
   }

   return(0);
  }


