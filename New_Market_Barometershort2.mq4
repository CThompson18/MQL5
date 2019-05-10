//+------------------------------------------------------------------+
//|                                             currency_stregth.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window
#property indicator_buffers 8
#property indicator_color1 Chocolate
#property indicator_color2 Aqua
#property indicator_color3 Blue
#property indicator_color4 Red
#property indicator_color5 Yellow
#property indicator_color6 Violet
#property indicator_color7 Lime
#property indicator_color8 White

int RSI_Period=14;
int RSI_Price=0;
int CountBars=500;
string Indicator_Name="Short Term Barometer";
int Legend_X=400;
int Legend_Y=2;

int ErrorTimeOut=30;
color GBP_Color=indicator_color1;
color EUR_Color=indicator_color2;
color AUD_Color=indicator_color3;
color NZD_Color=indicator_color4;
color JPY_Color=indicator_color5;
color CHF_Color=indicator_color6;
color CAD_Color=indicator_color7;
color USD_Color=indicator_color8;
//---- buffers
double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double ExtMapBuffer5[];
double ExtMapBuffer6[];
double ExtMapBuffer7[];
double ExtMapBuffer8[];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_LINE);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexStyle(1,DRAW_LINE);
   SetIndexBuffer(1,ExtMapBuffer2);
   SetIndexStyle(2,DRAW_LINE);
   SetIndexBuffer(2,ExtMapBuffer3);
   SetIndexStyle(3,DRAW_LINE);
   SetIndexBuffer(3,ExtMapBuffer4);
   SetIndexStyle(4,DRAW_LINE);
   SetIndexBuffer(4,ExtMapBuffer5);
   SetIndexStyle(5,DRAW_LINE);
   SetIndexBuffer(5,ExtMapBuffer6);
   SetIndexStyle(6,DRAW_LINE);
   SetIndexBuffer(6,ExtMapBuffer7);
   SetIndexStyle(7,DRAW_LINE);
   SetIndexBuffer(7,ExtMapBuffer8);   
   SetIndexLabel(0,"GBP");
   SetIndexLabel(1,"EUR");
   SetIndexLabel(2,"AUD");
   SetIndexLabel(3,"NZD");
   SetIndexLabel(4,"JPY");
   SetIndexLabel(5,"CHF");
   SetIndexLabel(6,"CAD");   
   SetIndexLabel(7,"USD");   
   IndicatorShortName(Indicator_Name);
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   ObjectDelete("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectDelete("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));

//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
datetime last_t=0;  
bool access=false;
datetime last_e=0;
datetime lasttd=0;


int start()
  {

   WindowRedraw();
   int l=0;
   if (CountBars>Bars) {l=Bars;}else{l=CountBars;}
   for (int i=l;i>=0;i--){
      ExtMapBuffer1[i]=EMPTY_VALUE;
      ExtMapBuffer2[i]=EMPTY_VALUE;
      ExtMapBuffer3[i]=EMPTY_VALUE;
      ExtMapBuffer4[i]=EMPTY_VALUE;
      ExtMapBuffer5[i]=EMPTY_VALUE;
      ExtMapBuffer6[i]=EMPTY_VALUE;
      ExtMapBuffer7[i]=EMPTY_VALUE;
      ExtMapBuffer8[i]=EMPTY_VALUE;
   
      int j=iBarShift("GBPUSD",0,Time[i],false);
      double gbp=iRSI("GBPUSD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("EURUSD",0,Time[i],false);
      double eur=iRSI("EURUSD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("AUDUSD",0,Time[i],false);
      double aud=iRSI("AUDUSD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("NZDUSD",0,Time[i],false);
      double nzd=iRSI("NZDUSD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("USDJPY",0,Time[i],false);
      double jpy=iRSI("USDJPY",0,RSI_Period,RSI_Price,j);
      j=iBarShift("USDCHF",0,Time[i],false);
      double chf=iRSI("USDCHF",0,RSI_Period,RSI_Price,j);
      j=iBarShift("USDCAD",0,Time[i],false);
      double cad=iRSI("USDCAD",0,RSI_Period,RSI_Price,j);        
      
      j=iBarShift("EURGBP",0,Time[i],false);
      double eurgbp=iRSI("EURGBP",0,RSI_Period,RSI_Price,j);
      j=iBarShift("EURJPY",0,Time[i],false);
      double eurjpy=iRSI("EURJPY",0,RSI_Period,RSI_Price,j);
      j=iBarShift("EURCHF",0,Time[i],false);
      double eurchf=iRSI("EURCHF",0,RSI_Period,RSI_Price,j);
      j=iBarShift("EURCAD",0,Time[i],false);
      double eurcad=iRSI("EURCAD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("EURAUD",0,Time[i],false);
      double euraud=iRSI("EURAUD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("EURNZD",0,Time[i],false);
      double eurnzd=iRSI("EURNZD",0,RSI_Period,RSI_Price,j);
      
      j=iBarShift("GBPJPY",0,Time[i],false);
      double gbpjpy=iRSI("GBPJPY",0,RSI_Period,RSI_Price,j);
      j=iBarShift("GBPCHF",0,Time[i],false);
      double gbpchf=iRSI("GBPCHF",0,RSI_Period,RSI_Price,j);
      j=iBarShift("GBPCAD",0,Time[i],false);
      double gbpcad=iRSI("GBPCAD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("GBPAUD",0,Time[i],false);
      double gbpaud=iRSI("GBPAUD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("GBPNZD",0,Time[i],false);
      double gbpnzd=iRSI("GBPNZD",0,RSI_Period,RSI_Price,j);
      
      j=iBarShift("CHFJPY",0,Time[i],false);
      double chfjpy=iRSI("CHFJPY",0,RSI_Period,RSI_Price,j);
      j=iBarShift("CADJPY",0,Time[i],false);
      double cadjpy=iRSI("CADJPY",0,RSI_Period,RSI_Price,j);
      j=iBarShift("AUDJPY",0,Time[i],false);
      double audjpy=iRSI("AUDJPY",0,RSI_Period,RSI_Price,j);
      j=iBarShift("NZDJPY",0,Time[i],false);
      double nzdjpy=iRSI("NZDJPY",0,RSI_Period,RSI_Price,j);

      j=iBarShift("CADCHF",0,Time[i],false);
      double cadchf=iRSI("CADCHF",0,RSI_Period,RSI_Price,j);
      j=iBarShift("AUDCHF",0,Time[i],false);
      double audchf=iRSI("AUDCHF",0,RSI_Period,RSI_Price,j);
      j=iBarShift("NZDCHF",0,Time[i],false);
      double nzdchf=iRSI("NZDCHF",0,RSI_Period,RSI_Price,j);

      j=iBarShift("AUDCAD",0,Time[i],false);
      double audcad=iRSI("AUDCAD",0,RSI_Period,RSI_Price,j);
      j=iBarShift("NZDCAD",0,Time[i],false);
      double nzdcad=iRSI("NZDCAD",0,RSI_Period,RSI_Price,j);

      j=iBarShift("AUDNZD",0,Time[i],false);
      double audnzd=iRSI("AUDNZD",0,RSI_Period,RSI_Price,j);

      
      
      ExtMapBuffer1[i]=(gbp+100-eurgbp+gbpjpy+gbpchf+gbpcad+gbpaud+gbpnzd)/7;      
      ExtMapBuffer2[i]=(eur+eurgbp+eurjpy+eurchf+eurcad+euraud+eurnzd)/7;      
      ExtMapBuffer3[i]=(aud+100-euraud+100-gbpaud+audjpy+audchf+audcad+audnzd)/7;
      ExtMapBuffer4[i]=(nzd+100-eurnzd+100-gbpnzd+nzdjpy+nzdchf+nzdcad+100-audnzd)/7;      
      ExtMapBuffer5[i]=(100-jpy+100-eurjpy+100-gbpjpy+100-chfjpy+100-cadjpy+100-audjpy+100-nzdjpy)/7;      
      ExtMapBuffer6[i]=(100-chf+100-eurchf+100-gbpjpy+chfjpy+100-cadchf+100-audchf+100-nzdchf)/7;
      ExtMapBuffer7[i]=(100-cad+100-eurcad+100-gbpcad+cadjpy+cadchf+100-audcad+100-nzdcad)/7;            
      ExtMapBuffer8[i]=(100-eur+100-gbp+jpy+chf+cad+100-aud+100-nzd)/7;      
   }

   int w=WindowFind(Indicator_Name);
   ObjectDelete("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"GBP",14, "Arial Bold", GBP_Color);
   ObjectSet("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X);
   ObjectSet("GBP_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);
   
   ObjectDelete("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"EUR",14, "Arial Bold", EUR_Color);
   ObjectSet("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+50);
   ObjectSet("EUR_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);

   ObjectDelete("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"AUD",14, "Arial Bold", AUD_Color);
   ObjectSet("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+100);
   ObjectSet("AUD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);

   ObjectDelete("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"NZD",14, "Arial Bold", NZD_Color);
   ObjectSet("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+150);
   ObjectSet("NZD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);

   ObjectDelete("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"JPY",14, "Arial Bold", JPY_Color);
   ObjectSet("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+200);
   ObjectSet("JPY_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);
   
   ObjectDelete("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"CHF",14, "Arial Bold", CHF_Color);
   ObjectSet("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+250);
   ObjectSet("CHF_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);

   ObjectDelete("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"CAD",14, "Arial Bold", CAD_Color);
   ObjectSet("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+300);
   ObjectSet("CAD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);

   ObjectDelete("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0));
   ObjectCreate("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJ_LABEL,w,0,0);
   ObjectSetText("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),"USD",14, "Arial Bold", USD_Color);
   ObjectSet("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_CORNER, 0);
   ObjectSet("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_XDISTANCE, Legend_X+350);
   ObjectSet("USD_Strength_Color"+Symbol()+Period()+DoubleToStr(RSI_Period,0),OBJPROP_YDISTANCE, Legend_Y);

   return(0);
  }

