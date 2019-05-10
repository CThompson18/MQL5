//+------------------------------------------------------------------+
//|                                                   test_array.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window
#property indicator_buffers 1

extern string Currency="EURUSD";

extern int RSIPeriod=7;
extern double Level = 50;
extern int TradeBar = 0;

double ExtMapBuffer1[];


string currency[17] ={"EURUSD","GBPUSD","AUDUSD","NZDUSD","USDCHF","USDCAD","USDJPY",
                      "GBPJPY","EURJPY","AUDJPY","NZDJPY","CADJPY","GBPCHF","EURCHF",
                      "EURGBP","EURCAD","GBPAUD"};
string currency1[28]={"EURUSD","EURGBP","EURJPY","EURCHF","EURCAD","EURAUD","EURNZD",
                      "GBPUSD","GBPJPY","GBPCHF","GBPCAD","GBPAUD","GBPNZD","AUDUSD",
                      "AUDJPY","AUDCHF","AUDCAD","AUDNZD","NZDUSD","NZDJPY","NZDCHF",
                      "NZDCAD","CHFJPY","CADJPY","CADCHF","USDJPY","USDCHF","USDCAD"};
int TimeFrames[6] = {5,15,30,60,240,1440};
int init()
  {
   SetIndexStyle(0,DRAW_LINE,STYLE_SOLID,2);
   SetIndexBuffer(0,ExtMapBuffer1);
   return(0);
  }
int deinit()
  {
   return(0);
  }
int start()
  {
   string pair1=StringSubstr(Symbol(),0,3);
   string pair2=StringSubstr(Symbol(),3,3);
   int p0[10],p1[10];
   int s0[10],s1[10];
   getPairs(pair1,p0,s0);
   getPairs(pair2,p1,s1);

   for(int tf=0;tf<6;tf++)
   {
         double x1=0,x2=0;
         double val1=0,val2=0,gs=0;
         for (int jj=0;jj<7;jj++)
         {
            x1=x1+(s0[jj]* iRSI(p0[jj],TimeFrames[tf], RSIPeriod, PRICE_CLOSE, TradeBar));
            if (s0[jj]<0) x1=x1+100;
            x2=x2+(s1[jj]* iRSI(p1[jj],TimeFrames[tf], RSIPeriod, PRICE_CLOSE, TradeBar));
            if (s1[jj]<0) x2=x2+100;
         }
         if ((x1/7>=Level && x2/7<Level) || (x1/7>=Level && MathAbs(x1/7-Level)>MathAbs(x2/7-Level)) || (x2/7<Level && MathAbs(x2/7-Level)>MathAbs(x1/7-Level)))
         {
            val2=MathAbs(x1/7-x2/7);
            gs = val2*2;
            if (gs > 99) gs = 99;
         }
         else
         {
            val1=-1*MathAbs(x1/7-x2/7);
            gs = val1*2;
            if (gs < (-1)*99) gs = -99;
         }
         ExtMapBuffer1[tf] = NormalizeDouble(gs,0);
         //fs=fs+"$global_summary[\""+currency[j]+"\"]["+tf+"]["+kl+"] = "+DoubleToStr(gs,0)+";\n";
   }
   return(0);
  }
  
int getIndex(string s[], string pair){
   for (int i = 0;i<28;i++){
      if (s[i] == pair) return (i);
   }
   return (-1);
}
  
void getPairs(string pair1,int &p[],int &s[]){
   if (pair1 == "gbp" || pair1 == "GBP"){
      p[0]=getIndex(currency1,"GBPUSD");p[1]=getIndex(currency1,"EURGBP");p[2]=getIndex(currency1,"GBPJPY");p[3]=getIndex(currency1,"GBPCHF");p[4]=getIndex(currency1,"GBPCAD");p[5]=getIndex(currency1,"GBPAUD");p[6]=getIndex(currency1,"GBPNZD");
      s[0]=1;s[1]=-1;s[2]=1;s[3]=1;s[4]=1;s[5]=1;s[6]=1;
   }
   if (pair1 == "eur" || pair1 == "EUR"){
      p[0]=getIndex(currency1,"EURUSD");p[1]=getIndex(currency1,"EURGBP");p[2]=getIndex(currency1,"EURJPY");p[3]=getIndex(currency1,"EURCHF");p[4]=getIndex(currency1,"EURCAD");p[5]=getIndex(currency1,"EURAUD");p[6]=getIndex(currency1,"EURNZD");
      s[0]=1;s[1]=1;s[2]=1;s[3]=1;s[4]=1;s[5]=1;s[6]=1;
   }
   if (pair1 == "aud" || pair1 == "AUD"){
      p[0]=getIndex(currency1,"AUDUSD");p[1]=getIndex(currency1,"EURAUD");p[2]=getIndex(currency1,"GBPAUD");p[3]=getIndex(currency1,"AUDJPY");p[4]=getIndex(currency1,"AUDCHF");p[5]=getIndex(currency1,"AUDCAD");p[6]=getIndex(currency1,"AUDNZD");
      s[0]=1;s[1]=-1;s[2]=-1;s[3]=1;s[4]=1;s[5]=1;s[6]=1;
   }   
   if (pair1 == "nzd" || pair1 == "NZD"){
      p[0]=getIndex(currency1,"NZDUSD");p[1]=getIndex(currency1,"EURNZD");p[2]=getIndex(currency1,"GBPNZD");p[3]=getIndex(currency1,"NZDJPY");p[4]=getIndex(currency1,"NZDCHF");p[5]=getIndex(currency1,"NZDCAD");p[6]=getIndex(currency1,"AUDNZD");
      s[0]=1;s[1]=-1;s[2]=-1;s[3]=1;s[4]=1;s[5]=1;s[6]=-1;
   }
   if (pair1 == "jpy" || pair1 == "JPY"){
      p[0]=getIndex(currency1,"USDJPY");p[1]=getIndex(currency1,"EURJPY");p[2]=getIndex(currency1,"GBPJPY");p[3]=getIndex(currency1,"CHFJPY");p[4]=getIndex(currency1,"CADJPY");p[5]=getIndex(currency1,"AUDJPY");p[6]=getIndex(currency1,"NZDJPY");
      s[0]=-1;s[1]=-1;s[2]=-1;s[3]=-1;s[4]=-1;s[5]=-1;s[6]=-1;
   }
   if (pair1 == "chf" || pair1 == "CHF"){
      p[0]=getIndex(currency1,"USDCHF");p[1]=getIndex(currency1,"EURCHF");p[2]=getIndex(currency1,"GBPCHF");p[3]=getIndex(currency1,"CHFJPY");p[4]=getIndex(currency1,"CADCHF");p[5]=getIndex(currency1,"AUDCHF");p[6]=getIndex(currency1,"NZDCHF");
      s[0]=-1;s[1]=-1;s[2]=-1;s[3]=1;s[4]=-1;s[5]=-1;s[6]=-1;
   }
   if (pair1 == "cad" || pair1 == "CAD"){
      p[0]=getIndex(currency1,"USDCAD");p[1]=getIndex(currency1,"EURCAD");p[2]=getIndex(currency1,"GBPCAD");p[3]=getIndex(currency1,"CADJPY");p[4]=getIndex(currency1,"CADCHF");p[5]=getIndex(currency1,"AUDCAD");p[6]=getIndex(currency1,"NZDCAD");
      s[0]=-1;s[1]=-1;s[2]=-1;s[3]=1;s[4]=1;s[5]=-1;s[6]=-1;
   }
   if (pair1 == "usd" || pair1 == "USD"){
      p[0]=getIndex(currency1,"EURUSD");p[1]=getIndex(currency1,"GBPUSD");p[2]=getIndex(currency1,"USDJPY");p[3]=getIndex(currency1,"USDCHF");p[4]=getIndex(currency1,"USDCAD");p[5]=getIndex(currency1,"AUDUSD");p[6]=getIndex(currency1,"NZDUSD");
      s[0]=-1;s[1]=-1;s[2]=1;s[3]=1;s[4]=1;s[5]=-1;s[6]=-1;
   }
}

