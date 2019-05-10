//+------------------------------------------------------------------+
//|                                               CD_Trendsignal.mq4 |
//|                        Copyright 2015, CDFinancials.             |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "CKTFinancials"
#property link      "https://www.mql5.com"
#property version   "1.00"
#property strict
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_color1 Aqua
#property indicator_width1 3
#property indicator_color2 Blue
#property indicator_width2 3
//Multi-function variables
//*********************************************************************
extern double glbRisk = 11.25;
extern int glbCountBars = 300;
extern color glbText_Color = Yellow; 
extern int glbFontSize = 10;
//---- buffers
//*********************************************************************
double glbSellArray[];
double glbBuyArray[];
double Table_value2[];
//DataDisplay function global variables
//*********************************************************************
extern int glb_right_edge_shift = 175;
extern int glb_upper_edge_shift = 30;
string glb_ea_name = "CD Financials";
extern int glbMagic_Number = 1234;
extern double glbLot_Size_Percent = .75;
double glbBuySell = 0.00;
string strBuySell = "";
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
	{
		//---- indicator line
		IndicatorBuffers(3);
		SetIndexStyle(0,DRAW_ARROW);
		SetIndexArrow(0,234);
		SetIndexStyle(1,DRAW_ARROW);
		SetIndexArrow(1,233);
		SetIndexBuffer(0,glbSellArray);
		SetIndexBuffer(1,glbBuyArray);
		SetIndexBuffer(2,Table_value2);
	  
	return(INIT_SUCCEEDED);
	}
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,const int prev_calculated, const datetime &time[], const double &open[], const double &high[], const double &low[], const double &close[], const long &tick_volume[], const long &volume[], const int &spread[])
	{
		//---
		DrawArrow();
   
		//--- return value of prev_calculated for next call
	return(rates_total);
	}
//+------------------------------------------------------------------+
//| ChartEvent function                                              |
//+------------------------------------------------------------------+
void OnChartEvent(const int id, const long &lparam, const double &dparam, const string &sparam)
	{
		//---
 
   
	}
//+------------------------------------------------------------------+
void DrawArrow ()
	{
		int shift;
		int Counter,i1;
		double x1,x2,value10,value11;
		double value2,value3;
		double TrueCount,Range,AvgRange,MRO1,MRO2;


		value10 = 3 + glbRisk * 2;
		x1 = 67 + glbRisk;
		x2 = 33 - glbRisk;
		value11 = value10;
		//----
		shift = glbCountBars;
		while(shift >= 0)
			{
				Counter = shift;
				Range = 0.0;
				AvgRange = 0.0;
				
				for (Counter = shift; Counter <= shift + 9; Counter++) AvgRange = AvgRange + MathAbs(High[Counter]-Low[Counter]);
				 Range = AvgRange / 10;
				 Counter = shift;
				 TrueCount = 0;
    
				while (Counter < shift + 9 && TrueCount < 1)
					{
						if (MathAbs(Open[Counter] - Close[Counter + 1]) >= Range * 2.0) TrueCount = TrueCount + 1;
						Counter = Counter + 1;
					}
     
				if (TrueCount >= 1) {MRO1 = Counter;} else {MRO1 = -1;}
				
				Counter = shift;
				TrueCount = 0;
				while (Counter < shift + 6 && TrueCount < 1)
					{
						if (MathAbs(Close[Counter + 3] - Close[Counter])>= Range * 4.6) 
							TrueCount = TrueCount + 1;
							Counter=Counter+1;
					}
				 
				if (TrueCount >= 1) {MRO2 = Counter;} else {MRO2 = -1;}
				
				if (MRO1 > -1) {value11 = 3;} else {value11 = value10;}
				
				if (MRO2 >- 1) {value11 = 4;} else {value11 = value10;}
				
				value2 = 100 - MathAbs(iWPR(NULL,0,value11,shift)); // PercentR(value11=9)
				Table_value2[shift] = value2;
				glbSellArray[shift] = 0;
				glbBuyArray[shift] = 0;
				value3 = 0;
    
				if (value2 < x2)
					{
						i1 = 1;
						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1)
							{i1 ++;}
						if (Table_value2[shift + i1] > x1) 
							{
								value3 = High[shift] + Range * 0.5;
								glbSellArray[shift] = value3;
					         glbBuySell = value3;
					         strBuySell = "Sell";
							} 
					}
				if (value2 > x1)
					{
						i1 = 1;
						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1)
							{i1 ++;}
						if (Table_value2[shift + i1] < x2) 
							{
								value3 = Low[shift] - Range * 0.5;
								glbBuyArray[shift] = value3;
					         glbBuySell = value3;
					         strBuySell = "Buy";								 
							}
					}
/*					
				if (glbSellArray[shift+1]!=0 || glbBuyArray[shift+1]!=0)
					{
						ObjectDelete("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0));
						ObjectCreate("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0),21,0,Time[shift],Low[shift]-5*Point);       
						ObjectSetText("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0),DoubleToStr(Open[shift],Digits),glbFontSize,"Arial",glbText_Color);
					}
*/				
			DataDisplay();  
			shift --;
			}
	}
 
//DataDisplay
//***************************************************************************************************************************************************
//void DataDisplay(const int dshift, const int dCounter, const int di1, const double dvalue2, const double dvalue3, const int dvalue10, const int dvalue11, const double dx1, const double dx2, const double dTrueCount, const double dRange, const double dAvgRange, const double dMRO1, const double dMRO2)
void DataDisplay()
	{
		double One_Lot = MarketInfo(Symbol(),MODE_MARGINREQUIRED);
		string text = "";
		int xAxis = (int)(ChartGetInteger(ChartID(),CHART_WIDTH_IN_PIXELS,0) - glb_right_edge_shift);
		int yAxis = glb_upper_edge_shift;

		text = "***************************************************";
		createObject(1,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);

		text= strBuySell + " = "+ DoubleToString(glbBuySell ,2);
		createObject(2,OBJ_LABEL,0,xAxis,yAxis,text);
		
		text = "***************************************************";
		createObject(3,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);		
					  
		//text = "EA id = " + glb_ea_name + "  " + (string)Magic_Number;
		text="Spread = "+DoubleToString(MarketInfo(_Symbol,MODE_SPREAD)/ 10 ,2);
		createObject(4,OBJ_LABEL,0,xAxis,yAxis,text);
    
		text = "***************************************************";
		createObject(5,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);	
  
		text = "Max Lot Size % = " + DoubleToString((AccountBalance()/ One_Lot) * glbLot_Size_Percent,6);
		createObject(6,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 2,text);
    
		text = "***************************************************";
		createObject(7,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 3,text);
      
		text="DayHigh = "+DoubleToString(MarketInfo(_Symbol,MODE_HIGH));
		createObject(8,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 4,text);
    }
 /*            
		text = "Account Max Lot = " + DoubleToString(AccountBalance()/ One_Lot,6);
		createObject(4,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 3,text); 
    
		text = "Account Balance = " + DoubleToString(AccountBalance(),2);
		createObject(5,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 4,text);                      
		text = "Lot Size Percent = " + DoubleToString(glbLot_Size_Percent,4);
		createObject(6,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 5,text);
    
		text = "Max Lot Size % = " + DoubleToString((AccountBalance()/ One_Lot) * glbLot_Size_Percent,6);
		createObject(7,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 6,text);    
 
     
		text = "value3 = " + DoubleToString(dvalue3,6);
		createObject(3,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 2,text); 
		text = "***************************************************";
		createObject(4,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 3,text);      
		text = "value2 = " + DoubleToString(dvalue2,6);
		createObject(5,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 4,text);
    
		text = "i1 = " + IntegerToString(di1);
		createObject(6,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 5,text); 
    
		text = "***************************************************";
		createObject(7,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 6,text);    
  
		text = "shift = " + IntegerToString(dshift);
		createObject(8,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 7,text);
    
		text = "Counter = " + IntegerToString(dCounter);
		createObject(9,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 8,text);
    
		text = "Range = " + DoubleToString(dRange,6);
		createObject(10,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 9,text); 
  
		text = "x1 = " + DoubleToString(dx1,4);
		createObject(11,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 10,text);              
		text = "x2 = " + DoubleToString(dx2,4);
		createObject(12,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 11,text);
    
		text = "***************************************************";
		createObject(13,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 12,text); 
    
		text = "TrueCount = " + DoubleToString(dTrueCount,6);
		createObject(14,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 13,text);          
    
		text = "MRO1 = " + DoubleToString(dMRO1,4);
		createObject(15,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 14,text);
    
		text = "MRO2 = " + DoubleToString(dMRO2,4);
		createObject(16,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 15,text);
    
		text = "value10 = " + IntegerToString(dvalue10);
		createObject(17,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 16,text); 
    
		text = "value11 = " + IntegerToString(dvalue11);
		createObject(18,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 17,text); 
    

      
   }
*/   
//createObject
//***************************************************************************************************************************************************
void createObject(int st_ID,ENUM_OBJECT obj,int window,int xA,int yA,string txt="")
	{
		color text_color = Red;
		int text_size = 10;
		ObjectCreate(glb_ea_name + "_" + IntegerToString(st_ID),obj,window,xA,yA);
		ObjectSet(glb_ea_name + "_" + IntegerToString(st_ID),OBJPROP_XDISTANCE,xA);
		ObjectSet(glb_ea_name + "_" + IntegerToString(st_ID),OBJPROP_YDISTANCE,yA);
		ObjectSetText(glb_ea_name + "_" + IntegerToString(st_ID),txt,text_size,"TimesNewRoman",text_color);
	} 
