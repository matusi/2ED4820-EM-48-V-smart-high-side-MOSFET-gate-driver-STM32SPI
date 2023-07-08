
  helpercmds.c
 
   Created on Jun 20, 2023
       Author jovani
 

#include helpercmds.h
void InfinionReadall (void){


	
	 initialise Regersters sturcts
	

	


	  
	    READ STDIAG REG the status  of each bit will be reflected of each bit member of the union

	   


	  stdiag.reg = structreadreg(STDIAG, stdiag.reg);



	  
	    READ CHDIAG REG the status  of each bit will be reflected of each bit member of the union

	   


	  chdiag.reg = structreadreg(CHDIAG, chdiag.reg);

	  
	    READ DIAG REG  the status  of each bit will be reflected of each bit member of the union

	   


	  diag.reg= structreadreg(DIAG, diag.reg);


	  
	    READ MOS_CHS_CTRL REG  the status  of each bit will be reflected of each bit member of the union

	  


	  mosctr.reg= structreadreg(MOS_CHS_CTRL, mosctr.reg);

	   
	      READ VDSTHA_B REG  the status  of each bit will be reflected of each bit member of the unio
	 


	  vdsth.reg= structreadreg(VDSTHA_B, vdsth.reg);



	 
	    READ MOSFLTBLKA_B REG  the status  of each bit will be reflected of each bit member of the union

	 


	  mosfltbl.reg= structreadreg(MOSFLTBLKA_B, mosfltbl.reg);


	 
	 READ CSAG_OCTH REG  the status  of each bit will be reflected of each bit member of the unio
	  

	 csag.reg= structreadreg(CSAG_OCTH, csag.reg);


	 
	 READ VBATOVUVRST REG  the status  of each bit will be reflected of each bit member of the unio
	  

	 vbatouv.reg= structreadreg(VBATOVUVRST, vbatouv.reg);


	 
	 READ RESETS REG  the status  of each bit will be reflected of each bit member of the unio
	  

	 resets.reg= structreadreg(RESETS, resets.reg);


	 

}





 SET CH A ON
 



void InfinionSetChAon(void){

	mosctr.bits.mosonch_a = 1;
	structwritereg(MOS_CHS_CTRL, mosctr.reg);
}




 SET CH A OFF
 


void InfinionSetChAoff(void){
	mosctr.bits.mosonch_a =0;

	structwritereg(MOS_CHS_CTRL, mosctr.reg);
}



 SET CH B ON
 


void InfinionSetChBon(void){


	mosctr.bits.mosonch_b= 1;
	structwritereg(MOS_CHS_CTRL, mosctr.reg);


}

 SET CH B OFF
 

void InfinionSetChBoff(void){

	mosctr.bits.mosonch_b =0;

	structwritereg(MOS_CHS_CTRL, mosctr.reg);



}



 CLEAR FAILURES
 


void InfinionClearfailures(void){

	fauls.reg =0xff;

	structwritereg(FAILURECLEAN, fauls.reg);

}


 Set Thresold CH A
 
void InfinionSetThreshold_ChA(uint8_t thresconf){
	vdsth.reg = thresconf;
	vdsth.bits.vdsa_ss = 1 ;
	structwritereg(VDSTHA_B, vdsth.reg);

}


 Set Thresold CH B
 
void InfinionSetThreshold_ChB(uint8_t thresconf){
	vdsth.reg = thresconf;
	vdsth.bits.vdsb_ss = 1 ;
	structwritereg(VDSTHA_B, vdsth.reg);

}



 Set Filter and blank time  CH  A
 
void InfinionSetFltBlktime_CHA(uint8_t fltconf,uint8_t blktimeconf){
	mosfltbl.reg = (fltconfblktimeconf) ;

	structwritereg(MOSFLTBLKA_B, mosfltbl.reg);

}


 Set Filter and blank time  CH B
 
void InfinionSetFltBlktime_CHB(uint8_t fltconf,uint8_t blktimeconf){
	mosfltbl.reg = (fltconfblktimeconf) ;

	structwritereg(MOSFLTBLKA_B, mosfltbl.reg);

}




 Set Overvoltage reset time VBatOVRst

 
void InfinionSetOverVlt(uint8_t overvltconf){
	vbatouv.reg = overvltconf ;

	structwritereg(VBATOVUVRST, vbatouv.reg);

}


Set Undervoltage reset VBatUVRst_1ms

 
void InfinionSetUnderVlt(uint8_t undervltconf){
	vbatouv.reg = undervltconf ;

	structwritereg(VBATOVUVRST, vbatouv.reg);

}

Set Curent sensor gain CSA_GAIN10VperV




void InfinionSetCurrsensGain(uint8_t currsensconfig){
	csag.reg = currsensconfig ;

	structwritereg(CSAG_OCTH, csag.reg);

}
