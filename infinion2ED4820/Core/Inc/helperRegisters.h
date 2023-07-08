/*
 * helperRegisters.h
 *
 *  Created on: Jun 16, 2023
 *      Author: Gaming
 */






#ifndef INC_HELPERREGISTERS_H_
#define INC_HELPERREGISTERS_H_


#include <stdio.h>
#include <stdint.h>






#include "registers.h"


typedef uint8_t regType;


/* register */
#define STDIAG          0x00


typedef union {
    struct {
        uint8_t vbat_ov : 1;// LSB : VBAT overvoltage failure (not latched)
        uint8_t vbat_uv : 1;// VBAT undervoltage failure (not latched)
        uint8_t vdd_uv : 1;// VDD undervoltage propagated in the first command and indicates an under voltage event
        uint8_t tsd : 1; // Chip in Overtemperature (latched)
        uint8_t ot_warning : 1;// Temperature warning
        uint8_t mem_fail : 1;// Error in the memory, trimming not possible (flag cannot be cleaned)
        uint8_t vcp_ready : 1;// Charge pump is ready
        uint8_t failure : 1;// MSB : Main Failure indication, 1 when there is a failure

    } bits;
    uint8_t reg;
} STDIAG_reg_cmds;



/* register */

#define CHDIAG          0x01

typedef union {
    struct {
        uint8_t vsource_a : 1;   // LSB: Source overvoltage - Channel_A
        uint8_t vdstrip_a : 1;   // Drain to Source overvoltage - Channel_A (latched)
        uint8_t vgst_h_a : 1;    // Gate-Source undervoltage - Channel_A (latched)
        uint8_t vsource_b : 1;   // Source overvoltage - Channel_B
        uint8_t vdstrip_b : 1;   // Drain to Source overvoltage - Channel_B (latched)
        uint8_t vgst_h_b : 1;    // Gate-Source undervoltage - Channel_B (latched)
        uint8_t itrip : 1;       // Overcurrent failure (latched)
        uint8_t vcp_uv : 1;      // MSB: Charge pump undervoltage
    } bits;
    uint8_t reg;
} CHDIAG_reg_cmds;






/* register */

#define DIAG            0x02
/* bits DIAG */
typedef union {
    struct {
        uint8_t safestaten : 1;    // LSB: SAFESTATEN
        uint8_t add_not_avail : 1; // Address not available
        uint8_t log_cp : 1;        // Loss of ground
        uint8_t log_d : 1;         // Loss of ground
        uint8_t log_a : 1;         // Loss of ground
        uint8_t d5 : 1;            // Not assigned
        uint8_t d6 : 1;            // Not assigned
        uint8_t d7 : 1;            // Not assigned
    } bits;
    uint8_t reg;
} DIAG_reg_cmds;






#define MOS_CHS_CTRL    0x03

typedef union {
    struct {
        uint8_t mosonch_a : 1;        // LSB: Switch on channel A
        uint8_t vdsa_cl : 1;          // Clear VDS flag Channel A
        uint8_t vgst_h_a_cl : 1;      // Clear VGS flag Channel A
        uint8_t mosonch_b : 1;        // Switch on channel B
        uint8_t vdsb_cl : 1;          // Clear VDS flag Channel B
        uint8_t vgst_h_b_cl : 1;      // Clear VGS flag Channel B
        uint8_t itrip_cl : 1;         // Clear ITRIP flag
        uint8_t chcrctrl : 1;         // MSB: Channel cross control activation
    } bits;
    uint8_t reg;
} MOS_CHS_CTRL_reg_cmds;




#define FAILURECLEAN    0x04
typedef union {
    struct {
        uint8_t vbat_uv_cl : 1;       // LSB: Clear VBAT undervoltage failure
        uint8_t vbat_ov_cl : 1;       // Clear VBAT overvoltage failure
        uint8_t vdd_uv_cl : 1;        // Clear Vdd undervoltage failure
        uint8_t tsd_cl : 1;           // Clear overtemperature failure
        uint8_t clear_int : 1;        // Clear interrupt
        uint8_t safestate_cl : 1;     // Safe State clear
        uint8_t d6 : 1;               // Not assigned
        uint8_t vcp_uv_cl : 1;        // MSB: VCP under voltage clear
    } bits;
    uint8_t reg;
} FAILURE_CLEAN_reg_cmds;





#define VDSTHA_B        0x05

typedef union {
    struct {
        uint8_t vdsth_a_0 : 1;       // LSB: Drain-Source overvoltage threshold Channel_A bit 0
        uint8_t vdsth_a_1 : 1;       // Drain-Source overvoltage threshold Channel_A bit 1
        uint8_t vdsth_a_2 : 1;       // Drain-Source overvoltage threshold Channel_A bit 2
        uint8_t vdsa_ss : 1;         // VDS Channel A safe state: ON when 0, OFF when 1 (default)
        uint8_t vdsth_b_0 : 1;       // Drain-Source overvoltage threshold Channel_B bit 0
        uint8_t vdsth_b_1 : 1;       // Drain-Source overvoltage threshold Channel_B bit 1
        uint8_t vdsth_b_2 : 1;       // Drain-Source overvoltage threshold Channel_B bit 2
        uint8_t vdsb_ss : 1;         // MSB: VDS Channel B safe state: ON when 0, OFF when 1 (default)
    } bits;
    uint8_t reg;
} VDSTHA_B_reg_cmds;



//
#define MOSFLTBLKA_B    0x06
typedef union {
    struct {
        uint8_t mosflt_a_0 : 1;      // LSB: MOS voltage filter Channel_A bit 0
        uint8_t mosflt_a_1 : 1;      // MOS voltage filter Channel_A bit 1
        uint8_t mosblk_a_0 : 1;      // MOS voltage blank time Channel_A bit 0
        uint8_t mosblk_a_1 : 1;      // MOS voltage blank time Channel_A bit 1
        uint8_t mosflt_b_0 : 1;      // MOS voltage filter Channel_B bit 0
        uint8_t mosflt_b_1 : 1;      // MOS voltage filter Channel_B bit 1
        uint8_t mosblk_b_0 : 1;      // MOS voltage blank time Channel_B bit 0
        uint8_t mosblk_b_1 : 1;      // MSB: MOS voltage blank time Channel_B bit 1
    } bits;
    uint8_t reg;
} MOSFLTBLKA_B_reg_cmds;






#define CSAG_OCTH       0x07
typedef union {
    struct {
        uint8_t csag_0 : 1;           // LSB: Current sense amplifier gain GDIFF bit 0
        uint8_t csag_1 : 1;           // Current sense amplifier gain GDIFF bit 1
        uint8_t csag_2 : 1;           // Current sense amplifier gain GDIFF bit 2
        uint8_t octh_0 : 1;           // Over current detection thresholds bit 0
        uint8_t octh_1 : 1;           // Over current detection thresholds bit 1
        uint8_t csa_hss : 1;          // Control signal to select the CSA configuration LSS or HSS (or bidirectional)
        uint8_t csa_coutsel : 1;      // Configures opamp output stage vs load cap
        uint8_t d7 : 1;               // MSB: Not assigned
    } bits;
    uint8_t reg;
} CSAG_OCTH_reg_cmds;





#define VBATOVUVRST     0x08
typedef union {
    struct {
        uint8_t vbatovarst_0 : 1;      // LSB: VBAT overvoltage auto-restart time bit 0
        uint8_t vbatovarst_1 : 1;      // VBAT overvoltage auto-restart time bit 1
        uint8_t vbatuvarst_0 : 1;      // Over VBAT undervoltage auto-restart time bit 0
        uint8_t vbatuvarst_1 : 1;      // Over VBAT undervoltage auto-restart time bit 1
        uint8_t d4 : 1;                // Not assigned
        uint8_t d5 : 1;                // Not assigned
        uint8_t d6 : 1;                // Not assigned
        uint8_t d7 : 1;                // Not assigned
    } bits;
    uint8_t reg;
} VBATOVUVRST_reg_cmds;



#define RESETS          0x09
typedef union {
    struct {
        uint8_t sft_rst_0 : 1;      // LSB: Software reset 0
        uint8_t fail_rst_0 : 1;     // Fail reset 0
        uint8_t gen_interrpt : 1;   // Generate interrupt signal
        uint8_t reserved : 1;       // Reserved bit
        uint8_t sft_rst_1 : 1;      // Software reset 1
        uint8_t fail_rst_1 : 1;     // Fail reset 1
        uint8_t reserved2 : 2;      // Reserved bits
    } bits;
    uint8_t reg;
} RESETS_reg_cmds;

#define SPARE_REG       0x0A
typedef union {
    struct {
        uint8_t spare0 : 1;
        uint8_t spare1 : 1;
        uint8_t spare2 : 1;
        uint8_t spare3 : 1;
        uint8_t spare4 : 1;
        uint8_t spare5 : 1;
        uint8_t spare6 : 1;
        uint8_t spare7 : 1;
    } bits;
    uint8_t reg;
} SPARE_reg_cmds;

typedef struct {
    STDIAG_reg_cmds stdiagreg;
    CHDIAG_reg_cmds chdiagreg;
    DIAG_reg_cmds diagreg;
    MOS_CHS_CTRL_reg_cmds moschsctrreg;
	FAILURE_CLEAN_reg_cmds failurecleanreg;
	VDSTHA_B_reg_cmds	vdsthabreg;
	MOSFLTBLKA_B_reg_cmds mosfltblk_breg;
	CSAG_OCTH_reg_cmds	csag_octhreg;
	VBATOVUVRST_reg_cmds	vbatovuv_rstreg;
	RESETS_reg_cmds			resets_reg;
	SPARE_reg_cmds		sparereg;
} Registers;




int spiRead(uint8_t adr, uint8_t *data);

int spiWrite(uint8_t adr, uint8_t data);

void InfinionStdDiag(uint8_t bitToTest);

void InfinionCHDiag(uint8_t bitToTest);

void InfinionDiag(uint8_t bitToTest);
void InfinionMosChctrRead(uint8_t bitToTest);

void writeMOS_CHS_CTRL_bit(uint8_t bitIndex, uint8_t value);
void InfinionMosChctrbitWrite(uint8_t bitToTest);

#endif /* INC_HELPERREGISTERS_H_ */
