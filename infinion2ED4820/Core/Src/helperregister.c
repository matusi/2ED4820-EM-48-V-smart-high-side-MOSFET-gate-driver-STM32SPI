/*
 * helperRegisters.c
 *
 *  Created on: Jun 16, 2023
 *      Author: Gaming
 */




#include "helperRegisters.h"
//
////****************************************************************************
//// spiRead
////
//// sends read command for register adress
//// gets data of previous SPI access
////****************************************************************************
//int spiRead(uint8_t adr, uint8_t *data)
//{
//    uint8_t tx_buf[2]; // transmit buffer
//    uint8_t rx_buf[2]; // receive buffer
//
//    *data = 0;
//
//    // read only if driver is enabled (ENABLE pin high)
////    if(DIGITAL_IO_GetInput(&ENABLE))
////    {
//        if(adr >=0 && adr <= 10)
//        {
//            tx_buf[0] = adr; // R/W bit is 0 anyway with address <= 10
//            tx_buf[1] = 0;   // data is "don't care"
//            uint16_t packedData = CombineBytes(tx_buf[0], tx_buf[1]);
//            SplitBytes(packedData, &tx_buf[0], &tx_buf[1]);
////            SPI_MASTER_Transfer(&SPI, tx_buf, , 2);
//            HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, 1, HAL_MAX_DELAY);  // SPI Transceive
//
//            *data = rx_buf[1];
//            return 1;
//        }
//        else
//        {
//        	return 0;
////            uartPrintf("[ERROR]: Can't read register - driver is disabled");
//        }
////    }
//}
//
////****************************************************************************
//// spiWrite
////
//// writes 8 bit data to register adress
////****************************************************************************
//int spiWrite(uint8_t adr, uint8_t data)
//{
//    uint8_t tx_buf[2]; // transmit buffer
//
//    // write only if driver is enabled (ENABLE pin high)
////    if(DIGITAL_IO_GetInput(&ENABLE))
////    {
//
//        if(adr >=0 && adr <= 10)
//        {
//            tx_buf[0] = adr;
//            tx_buf[0] |= (1<<7); // set R/W bit to "1"
//            tx_buf[1] = data;
////            SPI_MASTER_Transmit(&SPI, tx_buf, 2);
//            uint16_t packedData = CombineBytes(tx_buf[0], tx_buf[1]);
//            SplitBytes(packedData, &tx_buf[0], &tx_buf[1]);
//
//            HAL_SPI_Transmit(&hspi1, tx_buf, 1, 10);
//            return 1 ;
//        }
////    }
//    else
//    {
//    	return 0;
////        uartPrintf("\r\n"); // new line
////        uartPrintf("[ERROR]: Can't write register - driver is disabled");
//    }
//}
//
//
//

uint8_t structwritereg (uint8_t regadr,uint8_t regbyte)

{

    Registers regist,verif;
    regist.regbits.reg = 0 ;
    verif.regbits.reg = 0 ;


    regist.regbits.reg = regbyte ;

    // write the register
    spiWrite(regadr, &regist.regbits.reg);



    spiRead(regadr, &verif.regbits.reg);
    return verif.regbits.reg ;

}


uint8_t structreadreg (uint8_t regadr,uint8_t regbyte)

{

    Registers regist;
    regist.regbits.reg = 0;

    spiRead(regadr, &regist.regbits.reg);


    return regist.regbits.reg ;




   }


void InfinionStdDiag(uint8_t bitToTest)
{
    uint8_t data;

    // Read the STDIAG register
    if (spiRead(STDIAG, &data))
    {
        // Notify the result of each bit
        STDIAG_reg_cmds stdiag;
        stdiag.reg = data;

        if (bitToTest == 0)
        {
            // Test all bits
            if (stdiag.bits.vbat_ov)
            {
                // Perform action for VBAT OV failure
            }

            if (stdiag.bits.vbat_uv)
            {
                // Perform action for VBAT UV failure
            }

            if (stdiag.bits.vdd_uv)
            {
                // Perform action for VDD UV failure
            }

            if (stdiag.bits.tsd)
            {
                // Perform action for TSD (Overtemperature)
            }

            if (stdiag.bits.ot_warning)
            {
                // Perform action for OT Warning
            }

            if (stdiag.bits.mem_fail)
            {
                // Perform action for Memory Failure
            }

            if (stdiag.bits.vcp_ready)
            {
                // Perform action for VCP Ready
            }

            if (stdiag.bits.failure)
            {
                // Perform action for Main Failure indication
            }
        }
        else
        {
            // Test the specified bit
            switch (bitToTest)
            {
                case 0:
                    // Perform action for VBAT OV failure
                    break;
                case 1:
                    // Perform action for VBAT UV failure
                    break;
                case 2:
                    // Perform action for VDD UV failure
                    break;
                case 3:
                    // Perform action for TSD (Overtemperature)
                    break;
                case 4:
                    // Perform action for OT Warning
                    break;
                case 5:
                    // Perform action for Memory Failure
                    break;
                case 6:
                    // Perform action for VCP Ready
                    break;
                case 7:
                    // Perform action for Main Failure indication
                    break;
                default:
                    printf("Invalid bit to test\n");
                    break;
            }
        }
    }
    else
    {
        printf("Failed to read STDIAG register\n");
    }
}



//void InfinionCHDiag(uint8_t bitToTest)
//{
//    uint8_t data;
//
//    // Read the STDIAG register
//    if (spiRead(CHDIAG, &data))
//    {
//        // Notify the result of each bit
//    	CHDIAG_reg_cmds chdiag;
//    	chdiag.reg = data;
//
//        if (bitToTest == 0)
//        {
//            // Test all bits
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//
//            if ()
//            {
//
//            }
//        }
//        else
//        {
//            // Test the specified bit
//            switch (bitToTest)
//            {
//                case 0:
//
//                    break;
//                case 1:
//
//                    break;
//                case 2:
//
//                    break;
//                case 3:
//
//                    break;
//                case 4:
//
//                    break;
//                case 5:
//
//                    break;
//                case 6:
//
//                    break;
//                case 7:
//
//                    break;
//                default:
//                    printf("Invalid bit to test\n");
//                    break;
//            }
//        }
//    }
//    else
//    {
//        printf("Failed to read CHDIAG register\n");
//    }
//}



void InfinionCHDiag(uint8_t bitToTest)
{
    uint8_t data;

    // Read the CHDIAG register
    if (spiRead(CHDIAG, &data))
    {
        // Notify the result of each bit
        CHDIAG_reg_cmds chdiag;
        chdiag.reg = data;

        if (bitToTest == 0)
        {
            // Test all bits
            if (chdiag.bits.vsource_a)
            {
                // Perform action for Source overvoltage - Channel_A
            }

            if (chdiag.bits.vdstrip_a)
            {
                // Perform action for Drain to Source overvoltage - Channel_A
            }

            if (chdiag.bits.vgst_h_a)
            {
                // Perform action for Gate-Source undervoltage - Channel_A
            }

            if (chdiag.bits.vsource_b)
            {
                // Perform action for Source overvoltage - Channel_B
            }

            if (chdiag.bits.vdstrip_b)
            {
                // Perform action for Drain to Source overvoltage - Channel_B
            }

            if (chdiag.bits.vgst_h_b)
            {
                // Perform action for Gate-Source undervoltage - Channel_B
            }

            if (chdiag.bits.itrip)
            {
                // Perform action for Overcurrent failure
            }

            if (chdiag.bits.vcp_uv)
            {
                // Perform action for Charge pump undervoltage
            }
        }
        else
        {
            // Test the specified bit
            switch (bitToTest)
            {
                case 0:
                    // Perform action for Source overvoltage - Channel_A
                    break;
                case 1:
                    // Perform action for Drain to Source overvoltage - Channel_A
                    break;
                case 2:
                    // Perform action for Gate-Source undervoltage - Channel_A
                    break;
                case 3:
                    // Perform action for Source overvoltage - Channel_B
                    break;
                case 4:
                    // Perform action for Drain to Source overvoltage - Channel_B
                    break;
                case 5:
                    // Perform action for Gate-Source undervoltage - Channel_B
                    break;
                case 6:
                    // Perform action for Overcurrent failure
                    break;
                case 7:
                    // Perform action for Charge pump undervoltage
                    break;
                default:
                    printf("Invalid bit to test\n");
                    break;
            }
        }
    }
    else
    {
        printf("Failed to read CHDIAG register\n");
    }
}

void InfinionDiag(uint8_t bitToTest)
{
    uint8_t data;

    // Read the DIAG register
    if (spiRead(DIAG, &data))
    {
        // Notify the result of each bit
        DIAG_reg_cmds diag;
        diag.reg = data;

        if (bitToTest == 0)
        {
            // Test all bits
            if (diag.bits.safestaten)
            {
                // Perform action for SAFESTATEN
            }

            if (diag.bits.add_not_avail)
            {
                // Perform action for Address not available
            }

            if (diag.bits.log_cp)
            {
                // Perform action for Loss of ground (CP)
            }

            if (diag.bits.log_d)
            {
                // Perform action for Loss of ground (D)
            }

            if (diag.bits.log_a)
            {
                // Perform action for Loss of ground (A)
            }
        }
        else
        {
            // Test the specified bit
            switch (bitToTest)
            {
                case 0:
                    // Perform action for SAFESTATEN
                    break;
                case 1:
                    // Perform action for Address not available
                    break;
                case 2:
                    // Perform action for Loss of ground (CP)
                    break;
                case 3:
                    // Perform action for Loss of ground (D)
                    break;
                case 4:
                    // Perform action for Loss of ground (A)
                    break;
                default:
                    printf("Invalid bit to test\n");
                    break;
            }
        }
    }
    else
    {
        printf("Failed to read DIAG register\n");
    }
}



void InfinionMosChctrRead(uint8_t bitToTest)
{
    uint8_t data;

    // Read the MOS_CHS_CTRL register
    if (spiRead(MOS_CHS_CTRL, &data))
    {
        // Notify the result of each bit
        MOS_CHS_CTRL_reg_cmds mos_chs_ctrl;
        mos_chs_ctrl.reg = data;

        if (bitToTest == 0)
        {
            // Test all bits
            if (mos_chs_ctrl.bits.mosonch_a)
            {
                // Perform action for Switch on channel A
            }

            if (mos_chs_ctrl.bits.vdsa_cl)
            {
                // Perform action for Clear VDS flag Channel A
            }

            if (mos_chs_ctrl.bits.vgst_h_a_cl)
            {
                // Perform action for Clear VGS flag Channel A
            }

            if (mos_chs_ctrl.bits.mosonch_b)
            {
                // Perform action for Switch on channel B
            }

            if (mos_chs_ctrl.bits.vdsb_cl)
            {
                // Perform action for Clear VDS flag Channel B
            }

            if (mos_chs_ctrl.bits.vgst_h_b_cl)
            {
                // Perform action for Clear VGS flag Channel B
            }

            if (mos_chs_ctrl.bits.itrip_cl)
            {
                // Perform action for Clear ITRIP flag
            }

            if (mos_chs_ctrl.bits.chcrctrl)
            {
                // Perform action for Channel cross control activation
            }
        }
        else
        {
            // Test the specified bit
            switch (bitToTest)
            {
                case 0:
                    // Perform action for Switch on channel A
                    break;
                case 1:
                    // Perform action for Clear VDS flag Channel A
                    break;
                case 2:
                    // Perform action for Clear VGS flag Channel A
                    break;
                case 3:
                    // Perform action for Switch on channel B
                    break;
                case 4:
                    // Perform action for Clear VDS flag Channel B
                    break;
                case 5:
                    // Perform action for Clear VGS flag Channel B
                    break;
                case 6:
                    // Perform action for Clear ITRIP flag
                    break;
                case 7:
                    // Perform action for Channel cross control activation
                    break;
                default:
                    printf("Invalid bit to test\n");
                    break;
            }
        }
    }
    else
    {
        printf("Failed to read MOS_CHS_CTRL register\n");
    }
}


void writeMOS_CHS_CTRL_bit(uint8_t bitIndex, uint8_t value)
{
    uint8_t data;

    // Read the MOS_CHS_CTRL register
    if (spiRead(MOS_CHS_CTRL, &data))
    {
        MOS_CHS_CTRL_reg_cmds mos_chs_ctrl;
        mos_chs_ctrl.reg = data;

        // Set or clear the specified bit
        if (value)
        {
            mos_chs_ctrl.reg |= (1 << bitIndex);
        }
        else
        {
            mos_chs_ctrl.reg &= ~(1 << bitIndex);
        }

        // Write the updated data back to the MOS_CHS_CTRL register
        spiWrite(MOS_CHS_CTRL, mos_chs_ctrl.reg);
    }
    else
    {
        printf("Failed to read MOS_CHS_CTRL register\n");
    }
}

void InfinionMOS_CHS_CTRL_bit(uint8_t bitToTest)
{
    uint8_t data;

    // Read the MOS_CHS_CTRL register
    if (spiRead(MOS_CHS_CTRL, &data))
    {
        // Notify the result of each bit
        MOS_CHS_CTRL_reg_cmds mos_chs_ctrl;
        mos_chs_ctrl.reg = data;

        if (bitToTest == 0)
        {
            // Test all bits
            if (mos_chs_ctrl.bits.mosonch_a)
            {
                // Perform action for Switch on channel A
            }

            if (mos_chs_ctrl.bits.vdsa_cl)
            {
                // Perform action for Clear VDS flag Channel A
            }

            if (mos_chs_ctrl.bits.vgst_h_a_cl)
            {
                // Perform action for Clear VGS flag Channel A
            }

            if (mos_chs_ctrl.bits.mosonch_b)
            {
                // Perform action for Switch on channel B
            }

            if (mos_chs_ctrl.bits.vdsb_cl)
            {
                // Perform action for Clear VDS flag Channel B
            }

            if (mos_chs_ctrl.bits.vgst_h_b_cl)
            {
                // Perform action for Clear VGS flag Channel B
            }

            if (mos_chs_ctrl.bits.itrip_cl)
            {
                // Perform action for Clear ITRIP flag
            }

            if (mos_chs_ctrl.bits.chcrctrl)
            {
                // Perform action for Channel cross control activation
            }
        }
        else
        {
            // Test the specified bit
            switch (bitToTest)
            {
                case 0:
                    // Perform action for Switch on channel A
                    writeMOS_CHS_CTRL_bit(0, 1);  // Set the bit to 1
                    break;
                case 1:
                    // Perform action for Clear VDS flag Channel A
                    writeMOS_CHS_CTRL_bit(1, 1);  // Set the bit to 1
                    break;
                case 2:
                    // Perform action for Clear VGS flag Channel A
                    writeMOS_CHS_CTRL_bit(2, 1);  // Set the bit to 1
                    break;
                case 3:
                    // Perform action for Switch on channel B
                    writeMOS_CHS_CTRL_bit(3, 1);  // Set the bit to 1
                    break;
                case 4:
                    // Perform action for Clear VDS flag Channel B
                    writeMOS_CHS_CTRL_bit(4, 1);  // Set the bit to 1
                    break;
                case 5:
                    // Perform action for Clear VGS flag Channel B
                    writeMOS_CHS_CTRL_bit(5, 1);  // Set the bit to 1
                    break;
                case 6:
                    // Perform action for Clear ITRIP flag
                    writeMOS_CHS_CTRL_bit(6, 1);  // Set the bit to 1
                    break;
                case 7:
                    // Perform action for Channel cross control activation
                    writeMOS_CHS_CTRL_bit(7, 1);  // Set the bit to 1
                    break;
                default:
                    printf("Invalid bit to test\n");
                    break;
            }
        }
    }
    else
    {
        printf("Failed to read MOS_CHS_CTRL register\n");
    }
}
void InfinionVdsthaRead(uint8_t bitToTest)
{
    uint8_t data;

    // Read the MOS_CHS_CTRL register
    if (spiRead(MOS_CHS_CTRL, &data))
    {
        // Notify the result of each bit
        VDSTHA_B_reg_cmds vdth;
        vdth.reg = data;

        if (bitToTest == 0)
        {
//            // Test all bits
//            if (mos_chs_ctrl.bits.mosonch_a)
//            {
//                // Perform action for Switch on channel A
//            }
//
//            if (mos_chs_ctrl.bits.vdsa_cl)
//            {
//                // Perform action for Clear VDS flag Channel A
//            }
//
//            if (mos_chs_ctrl.bits.vgst_h_a_cl)
//            {
//                // Perform action for Clear VGS flag Channel A
//            }
//
//            if (mos_chs_ctrl.bits.mosonch_b)
//            {
//                // Perform action for Switch on channel B
//            }
//
//            if (mos_chs_ctrl.bits.vdsb_cl)
//            {
//                // Perform action for Clear VDS flag Channel B
//            }
//
//            if (mos_chs_ctrl.bits.vgst_h_b_cl)
//            {
//                // Perform action for Clear VGS flag Channel B
//            }
//
//            if (mos_chs_ctrl.bits.itrip_cl)
//            {
//                // Perform action for Clear ITRIP flag
//            }
//
//            if (mos_chs_ctrl.bits.chcrctrl)
//            {
//                // Perform action for Channel cross control activation
//            }
        }
        else
        {
            // Test the specified bit
            switch (bitToTest)
            {
                case 0:
                    // Perform action for Switch on channel A
                    break;
                case 1:
                    // Perform action for Clear VDS flag Channel A
                    break;
                case 2:
                    // Perform action for Clear VGS flag Channel A
                    break;
                case 3:
                    // Perform action for Switch on channel B
                    break;
                case 4:
                    // Perform action for Clear VDS flag Channel B
                    break;
                case 5:
                    // Perform action for Clear VGS flag Channel B
                    break;
                case 6:
                    // Perform action for Clear ITRIP flag
                    break;
                case 7:
                    // Perform action for Channel cross control activation
                    break;
                default:
                    printf("Invalid bit to test\n");
                    break;
            }
        }
    }
    else
    {
        printf("Failed to read MOS_CHS_CTRL register\n");
    }
}
void InfinionMosFltBlkRead(uint8_t bitToTest)
{  MOSFLTBLKA_B_reg_cmds fltblk;


    fltblk.reg =0 ;
    // Read the MOSFLTBLKA_B register
    spiRead(MOSFLTBLKA_B, &fltblk.reg );

        // Notify the result of each bit



    }

    void InfinionMosCsagRead(uint8_t bitToTest)
    {  CSAG_OCTH_reg_cmds csag;


    	csag.reg =0 ;
        // Read the CSAG_OCTH register
        spiRead(CSAG_OCTH, &csag.reg );

            // Notify the result of each bit



        }

    void InfinionMosVbatUORstRead(uint8_t bitToTest)
     {  VBATOVUVRST_reg_cmds vbat;


     	vbat.reg =0 ;
         // Read the CSAG_OCTH register
         spiRead(VBATOVUVRST, &vbat.reg );

             // Notify the result of each bit



         }


    void InfinionMosResetsRead(uint8_t bitToTest)
       {  RESETS_reg_cmds rst;


       	rst.reg =0 ;
           // Read the CSAG_OCTH register
           spiRead(RESETS, &rst.reg );

               // Notify the result of each bit
              }




