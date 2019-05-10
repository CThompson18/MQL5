void AutoTrade()
	{
		int i = 0;
		double ask, bid;
		double SL = 0, TP = 0;
		bool buy = false;
		bool dn = false;
		double buy_trend = iCustom(Symbol(),Signal_Timeframe,"Dan\'s_Trendsignal_m",Signal_RISK,300,1,Trade_Bar);
		double sell_trend = iCustom(Symbol(),Signal_Timeframe,"Dan\'s_Trendsignal_m",Signal_RISK,300,0,Trade_Bar);
		double sl_tp_atr = iATR(Symbol(),0,SL_TP_atr_period,0);


		if (buy_trend != 0)
			{
				buy = true;
				dn = false;
			}
		if (sell_trend != 0){
			buy=false;
			dn=true;
		}

	   int mult=1;
	   if ( ((StringFind(Symbol(),"JPY",0)!=-1 || StringFind(Symbol(),"jpy",0)!=-1) && Point<0.01) ||
			((StringFind(Symbol(),"JPY",0)==-1 || StringFind(Symbol(),"jpy",0)==-1) && Point<0.0001))
		  mult=10;

	   if (TotalOrders(Magic_Number)==0 && Time[0]>last_t){
		  if (buy ){
			 RefreshRates();
			 ask=MarketInfo(Symbol(),MODE_ASK);
			 SL = NormalizeDouble(ask-(sl_tp_atr * SL_atr_multiplier),Digits);
			 TP = NormalizeDouble(ask+(sl_tp_atr * TP_atr_multiplier),Digits);
			 if (OrderSend(Symbol(),OP_BUY,Lots,NormalizeDouble(ask,Digits),Slippage,SL,TP,com+Period(),Magic_Number,0,Blue)==-1) {
				   i=GetLastError();
				   if (i!=1 && i!=0) Print("Buy Error :"+error(i)+" at "+Symbol()+" "+Period());                 
			 }else{
				last_t=Time[0];
				return(0);
			 }
		  }
		  if (dn ){
			 RefreshRates();
			 bid=MarketInfo(Symbol(),MODE_BID);
			 SL = NormalizeDouble(bid+(sl_tp_atr * SL_atr_multiplier),Digits);
			 TP = NormalizeDouble(bid-(sl_tp_atr * TP_atr_multiplier),Digits);
			 if (OrderSend(Symbol(),OP_SELL,Lots,NormalizeDouble(bid,Digits),Slippage,SL,TP,com+Period(),Magic_Number,0,Red)==-1) {
				   i=GetLastError();
				   if (i!=1 && i!=0) Print("Sell Error :"+error(i)+" at "+Symbol()+" "+Period());                 
			 }else{
				last_t=Time[0];
				return(0);
			 }
		  }         
	   }
	   
	   if (TotalOrders(Magic_Number)>0){
		  for (int t=0;t<OrdersTotal();t++){
			 OrderSelect(t,SELECT_BY_POS);
			 if (OrderType()==Magic_Number){
				break;
			 }
		  }
		  if (OrderType()==OP_BUY && dn){
			 OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Digits),Slippage,Pink);
		  }
		  if (OrderType()==OP_SELL && buy){
			 OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Digits),Slippage,Pink);
		  }
	   }
	   
	   return(0);   
  }
