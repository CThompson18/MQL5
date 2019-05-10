//+------------------------------------------------------------------+
//|                                          Barometer Dashboard.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window
#property indicator_buffers 8

extern int Short_RSI_Period=14;
extern int Short_RSI_Price=0;
extern double Short_TF=PERIOD_M5;

extern int Med_RSI_Period=7;
extern int Med_RSI_Price=0;
extern double Med_TF=PERIOD_H1;

extern int Long_RSI_Period=7;
extern int Long_RSI_Price=0;
extern double Long_TF=PERIOD_H4;

extern int Trade_Bar=0;
extern int Precision=1;


double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double ExtMapBuffer5[];
double ExtMapBuffer6[];
double ExtMapBuffer7[];
double ExtMapBuffer8[];

datetime last_update=0;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
   SetIndexStyle(0,DRAW_NONE);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexLabel(0, "EUR"); 
   SetIndexStyle(1,DRAW_NONE);
   SetIndexBuffer(1,ExtMapBuffer2);
   SetIndexLabel(1, "GBP");
   SetIndexStyle(2,DRAW_NONE);
   SetIndexBuffer(2,ExtMapBuffer3);
   SetIndexLabel(2, "JPY");
   SetIndexStyle(3,DRAW_NONE);
   SetIndexBuffer(3,ExtMapBuffer4);
   SetIndexLabel(3, "CHF");
   SetIndexStyle(4,DRAW_NONE);
   SetIndexBuffer(4,ExtMapBuffer5);
   SetIndexLabel(4, "CAD");
   SetIndexStyle(5,DRAW_NONE);
   SetIndexBuffer(5,ExtMapBuffer6);
   SetIndexLabel(5, "AUD");
   SetIndexStyle(6,DRAW_NONE);
   SetIndexBuffer(6,ExtMapBuffer7);
   SetIndexLabel(6, "NZD");
   SetIndexStyle(7,DRAW_NONE);
   SetIndexBuffer(7,ExtMapBuffer8);
   SetIndexLabel(7, "USD");
   return(0);
  }
int deinit()
  {
   return(0);
  }

int start()
  { 
    
      double Short_gbp=iRSI("GBPUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_eur=iRSI("EURUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_aud=iRSI("AUDUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_nzd=iRSI("NZDUSD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_jpy=iRSI("USDJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_chf=iRSI("USDCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_cad=iRSI("USDCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);             
      double Short_eurgbp=iRSI("EURGBP",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_eurjpy=iRSI("EURJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_eurchf=iRSI("EURCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_eurcad=iRSI("EURCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_euraud=iRSI("EURAUD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_eurnzd=iRSI("EURNZD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_gbpjpy=iRSI("GBPJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_gbpchf=iRSI("GBPCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_gbpcad=iRSI("GBPCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_gbpaud=iRSI("GBPAUD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_gbpnzd=iRSI("GBPNZD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_chfjpy=iRSI("CHFJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_cadjpy=iRSI("CADJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_audjpy=iRSI("AUDJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_nzdjpy=iRSI("NZDJPY",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_cadchf=iRSI("CADCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_audchf=iRSI("AUDCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_nzdchf=iRSI("NZDCHF",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_audcad=iRSI("AUDCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_nzdcad=iRSI("NZDCAD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);
      double Short_audnzd=iRSI("AUDNZD",Short_TF,Short_RSI_Period,Short_RSI_Price,Trade_Bar);  
      
      double gbp_short=(Short_gbp+100-Short_eurgbp+Short_gbpjpy+Short_gbpchf+Short_gbpcad+Short_gbpaud+Short_gbpnzd)/7;      
      double eur_short=(Short_eur+Short_eurgbp+Short_eurjpy+Short_eurchf+Short_eurcad+Short_euraud+Short_eurnzd)/7;      
      double aud_short=(Short_aud+100-Short_euraud+100-Short_gbpaud+Short_audjpy+Short_audchf+Short_audcad+Short_audnzd)/7;
      double nzd_short=(Short_nzd+100-Short_eurnzd+100-Short_gbpnzd+Short_nzdjpy+Short_nzdchf+Short_nzdcad+100-Short_audnzd)/7;      
      double jpy_short=(100-Short_jpy+100-Short_eurjpy+100-Short_gbpjpy+100-Short_chfjpy+100-Short_cadjpy+100-Short_audjpy+100-Short_nzdjpy)/7;      
      double chf_short=(100-Short_chf+100-Short_eurchf+100-Short_gbpjpy+Short_chfjpy+100-Short_cadchf+100-Short_audchf+100-Short_nzdchf)/7;
      double cad_short=(100-Short_cad+100-Short_eurcad+100-Short_gbpcad+Short_cadjpy+Short_cadchf+100-Short_audcad+100-Short_nzdcad)/7;            
      double usd_short=(100-Short_eur+100-Short_gbp+Short_jpy+Short_chf+Short_cad+100-Short_aud+100-Short_nzd)/7;      
      
     
      double Med_gbp=iRSI("GBPUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_eur=iRSI("EURUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_aud=iRSI("AUDUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_nzd=iRSI("NZDUSD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_jpy=iRSI("USDJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_chf=iRSI("USDCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_cad=iRSI("USDCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);             
      double Med_eurgbp=iRSI("EURGBP",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_eurjpy=iRSI("EURJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_eurchf=iRSI("EURCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_eurcad=iRSI("EURCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_euraud=iRSI("EURAUD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_eurnzd=iRSI("EURNZD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_gbpjpy=iRSI("GBPJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_gbpchf=iRSI("GBPCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_gbpcad=iRSI("GBPCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_gbpaud=iRSI("GBPAUD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_gbpnzd=iRSI("GBPNZD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_chfjpy=iRSI("CHFJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_cadjpy=iRSI("CADJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_audjpy=iRSI("AUDJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_nzdjpy=iRSI("NZDJPY",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_cadchf=iRSI("CADCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_audchf=iRSI("AUDCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_nzdchf=iRSI("NZDCHF",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_audcad=iRSI("AUDCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_nzdcad=iRSI("NZDCAD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);
      double Med_audnzd=iRSI("AUDNZD",Med_TF,Med_RSI_Period,Med_RSI_Price,Trade_Bar);  

      double gbp_med=(Med_gbp+100-Med_eurgbp+Med_gbpjpy+Med_gbpchf+Med_gbpcad+Med_gbpaud+Med_gbpnzd)/7;      
      double eur_med=(Med_eur+Med_eurgbp+Med_eurjpy+Med_eurchf+Med_eurcad+Med_euraud+Med_eurnzd)/7;      
      double aud_med=(Med_aud+100-Med_euraud+100-Med_gbpaud+Med_audjpy+Med_audchf+Med_audcad+Med_audnzd)/7;
      double nzd_med=(Med_nzd+100-Med_eurnzd+100-Med_gbpnzd+Med_nzdjpy+Med_nzdchf+Med_nzdcad+100-Med_audnzd)/7;      
      double jpy_med=(100-Med_jpy+100-Med_eurjpy+100-Med_gbpjpy+100-Med_chfjpy+100-Med_cadjpy+100-Med_audjpy+100-Med_nzdjpy)/7;      
      double chf_med=(100-Med_chf+100-Med_eurchf+100-Med_gbpjpy+Med_chfjpy+100-Med_cadchf+100-Med_audchf+100-Med_nzdchf)/7;
      double cad_med=(100-Med_cad+100-Med_eurcad+100-Med_gbpcad+Med_cadjpy+Med_cadchf+100-Med_audcad+100-Med_nzdcad)/7;            
      double usd_med=(100-Med_eur+100-Med_gbp+Med_jpy+Med_chf+Med_cad+100-Med_aud+100-Med_nzd)/7;


      double Long_gbp=iRSI("GBPUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_eur=iRSI("EURUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_aud=iRSI("AUDUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_nzd=iRSI("NZDUSD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_jpy=iRSI("USDJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_chf=iRSI("USDCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_cad=iRSI("USDCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);             
      double Long_eurgbp=iRSI("EURGBP",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_eurjpy=iRSI("EURJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_eurchf=iRSI("EURCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_eurcad=iRSI("EURCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_euraud=iRSI("EURAUD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_eurnzd=iRSI("EURNZD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_gbpjpy=iRSI("GBPJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_gbpchf=iRSI("GBPCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_gbpcad=iRSI("GBPCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_gbpaud=iRSI("GBPAUD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_gbpnzd=iRSI("GBPNZD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_chfjpy=iRSI("CHFJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_cadjpy=iRSI("CADJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_audjpy=iRSI("AUDJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_nzdjpy=iRSI("NZDJPY",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_cadchf=iRSI("CADCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_audchf=iRSI("AUDCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_nzdchf=iRSI("NZDCHF",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_audcad=iRSI("AUDCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_nzdcad=iRSI("NZDCAD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);
      double Long_audnzd=iRSI("AUDNZD",Long_TF,Long_RSI_Period,Long_RSI_Price,Trade_Bar);  

      double gbp_long=(Long_gbp+100-Long_eurgbp+Long_gbpjpy+Long_gbpchf+Long_gbpcad+Long_gbpaud+Long_gbpnzd)/7;      
      double eur_long=(Long_eur+Long_eurgbp+Long_eurjpy+Long_eurchf+Long_eurcad+Long_euraud+Long_eurnzd)/7;      
      double aud_long=(Long_aud+100-Long_euraud+100-Long_gbpaud+Long_audjpy+Long_audchf+Long_audcad+Long_audnzd)/7;
      double nzd_long=(Long_nzd+100-Long_eurnzd+100-Long_gbpnzd+Long_nzdjpy+Long_nzdchf+Long_nzdcad+100-Long_audnzd)/7;      
      double jpy_long=(100-Long_jpy+100-Long_eurjpy+100-Long_gbpjpy+100-Long_chfjpy+100-Long_cadjpy+100-Long_audjpy+100-Long_nzdjpy)/7;      
      double chf_long=(100-Long_chf+100-Long_eurchf+100-Long_gbpjpy+Long_chfjpy+100-Long_cadchf+100-Long_audchf+100-Long_nzdchf)/7;
      double cad_long=(100-Long_cad+100-Long_eurcad+100-Long_gbpcad+Long_cadjpy+Long_cadchf+100-Long_audcad+100-Long_nzdcad)/7;            
      double usd_long=(100-Long_eur+100-Long_gbp+Long_jpy+Long_chf+Long_cad+100-Long_aud+100-Long_nzd)/7;          
      
      double trend_gbp=(gbp_short+gbp_med+gbp_long)/3;
      double trend_eur=(eur_short+eur_med+eur_long)/3;
      double trend_aud=(aud_short+aud_med+aud_long)/3;
      double trend_nzd=(nzd_short+nzd_med+nzd_long)/3;
      double trend_jpy=(jpy_short+jpy_med+jpy_long)/3;
      double trend_chf=(chf_short+chf_med+chf_long)/3;
      double trend_cad=(cad_short+cad_med+cad_long)/3;
      double trend_usd=(usd_short+usd_med+usd_long)/3;
   

      ExtMapBuffer1[3] = NormalizeDouble(eur_short,Precision);
      ExtMapBuffer1[2] = NormalizeDouble(eur_med,Precision);
      ExtMapBuffer1[1] = NormalizeDouble(eur_long,Precision);
      ExtMapBuffer1[0] = NormalizeDouble(trend_eur,Precision);
      
      ExtMapBuffer2[3] = NormalizeDouble(gbp_short,Precision);
      ExtMapBuffer2[2] = NormalizeDouble(gbp_med,Precision);
      ExtMapBuffer2[1] = NormalizeDouble(gbp_long,Precision);
      ExtMapBuffer2[0] = NormalizeDouble(trend_gbp,Precision);

      ExtMapBuffer3[3] = NormalizeDouble(jpy_short,Precision);
      ExtMapBuffer3[2] = NormalizeDouble(jpy_med,Precision);
      ExtMapBuffer3[1] = NormalizeDouble(jpy_long,Precision);
      ExtMapBuffer3[0] = NormalizeDouble(trend_jpy,Precision);

      ExtMapBuffer4[3] = NormalizeDouble(chf_short,Precision);
      ExtMapBuffer4[2] = NormalizeDouble(chf_med,Precision);
      ExtMapBuffer4[1] = NormalizeDouble(chf_long,Precision);
      ExtMapBuffer4[0] = NormalizeDouble(trend_chf,Precision);

      ExtMapBuffer5[3] = NormalizeDouble(cad_short,Precision);
      ExtMapBuffer5[2] = NormalizeDouble(cad_med,Precision);
      ExtMapBuffer5[1] = NormalizeDouble(cad_long,Precision);
      ExtMapBuffer5[0] = NormalizeDouble(trend_cad,Precision);

      ExtMapBuffer6[3] = NormalizeDouble(aud_short,Precision);
      ExtMapBuffer6[2] = NormalizeDouble(aud_med,Precision);
      ExtMapBuffer6[1] = NormalizeDouble(aud_long,Precision);
      ExtMapBuffer6[0] = NormalizeDouble(trend_aud,Precision);

      ExtMapBuffer7[3] = NormalizeDouble(nzd_short,Precision);
      ExtMapBuffer7[2] = NormalizeDouble(nzd_med,Precision);
      ExtMapBuffer7[1] = NormalizeDouble(nzd_long,Precision);
      ExtMapBuffer7[0] = NormalizeDouble(trend_nzd,Precision);

      ExtMapBuffer8[3] = NormalizeDouble(usd_short,Precision);
      ExtMapBuffer8[2] = NormalizeDouble(usd_med,Precision);
      ExtMapBuffer8[1] = NormalizeDouble(usd_long,Precision);
      ExtMapBuffer8[0] = NormalizeDouble(trend_usd,Precision);

   return(0);
  }


