PROGRAM metering
  VAR
    PM01VA AT %IW10 : INT;
    PM01VB AT %IW11 : INT;
    PM01VC AT %IW12 : INT;
    PM01IA AT %IW13 : INT;
    PM01IB AT %IW14 : INT;
    PM01IC AT %IW15 : INT;
    PM02VA AT %IW20 : INT;
    PM02VB AT %IW21 : INT;
    PM02VC AT %IW22 : INT;
    PM02IA AT %IW23 : INT;
    PM02IB AT %IW24 : INT;
    PM02IC AT %IW25 : INT;
    PM03VA AT %IW30 : INT;
    PM03VB AT %IW31 : INT;
    PM03VC AT %IW32 : INT;
    PM03IA AT %IW33 : INT;
    PM03IB AT %IW34 : INT;
    PM03IC AT %IW35 : INT;
    PM04VA AT %IW40 : INT;
    PM04VB AT %IW41 : INT;
    PM04VC AT %IW42 : INT;
    PM04IA AT %IW43 : INT;
    PM04IB AT %IW44 : INT;
    PM04IC AT %IW45 : INT;
    PM01SecST AT %IX0.1 : BOOL;
    PM02SecST AT %IX0.2 : BOOL;
    PM03SecST AT %IX0.3 : BOOL;
    PM04SecST AT %IX0.4 : BOOL;
    CB01CST AT %IX2.1 : BOOL;
    CB02CST AT %IX2.2 : BOOL;
    CB03CST AT %IX2.3 : BOOL;
    CB04CST AT %IX2.4 : BOOL;
    CB01OpST AT %QX0.1 : BOOL;
    CB02OpST AT %QX0.2 : BOOL;
    CB03OpST AT %QX0.3 : BOOL;
    CB04OpST AT %QX0.4 : BOOL;
  END_VAR
  VAR
    Close : BOOL;
  END_VAR

  Close := NOT(CB01OpST);
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : metering;
  END_RESOURCE
END_CONFIGURATION

(*DBG:char md5[] = "6af44537f1cdb28cb67cb1cd5e6ac69d";*)
(*DBG:/**)
(*DBG: * This file is part of OpenPLC Runtime*)
(*DBG: **)
(*DBG: * Copyright (C) 2023 Autonomy, GP Orcullo*)
(*DBG: * Based on the work by GP Orcullo on Beremiz for uC*)
(*DBG: **)
(*DBG: * This program is free software; you can redistribute it and/or*)
(*DBG: * modify it under the terms of the GNU General Public License*)
(*DBG: * as published by the Free Software Foundation; either version 2*)
(*DBG: * of the License, or (at your option) any later version.*)
(*DBG: **)
(*DBG: * This program is distributed in the hope that it will be useful,*)
(*DBG: * but WITHOUT ANY WARRANTY; without even the implied warranty of*)
(*DBG: * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the*)
(*DBG: * GNU General Public License for more details.*)
(*DBG: **)
(*DBG: * You should have received a copy of the GNU General Public License*)
(*DBG: * along with this program; If not, see <http://www.gnu.org/licenses/>.*)
(*DBG: **)
(*DBG: */*)
(*DBG:*)
(*DBG:#include <stdbool.h>*)
(*DBG:*)
(*DBG:#include "iec_types_all.h"*)
(*DBG:#include "POUS.h"*)
(*DBG:*)
(*DBG:#define SAME_ENDIANNESS      0*)
(*DBG:#define REVERSE_ENDIANNESS   1*)
(*DBG:*)
(*DBG:uint8_t endianness;*)
(*DBG:*)
(*DBG:*)
(*DBG:extern METERING RES0__INSTANCE0;*)
(*DBG:*)
(*DBG:static const struct {*)
(*DBG:    void *ptr;*)
(*DBG:    __IEC_types_enum type;*)
(*DBG:} debug_vars[] = {*)
(*DBG:    {&(RES0__INSTANCE0.PM01VA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM01VB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM01VC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM01IA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM01IB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM01IC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02VA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02VB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02VC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02IA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02IB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02IC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03VA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03VB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03VC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03IA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03IB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03IC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04VA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04VB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04VC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04IA), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04IB), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04IC), INT_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM01SECST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM02SECST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM03SECST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.PM04SECST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB01CST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB02CST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB03CST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB04CST), BOOL_P_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB01OPST), BOOL_O_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB02OPST), BOOL_O_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB03OPST), BOOL_O_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB04OPST), BOOL_O_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CLOSE), BOOL_ENUM},*)
(*DBG:};*)
(*DBG:*)
(*DBG:#define VAR_COUNT               37*)
(*DBG:*)
(*DBG:uint16_t get_var_count(void)*)
(*DBG:{*)
(*DBG:    return VAR_COUNT;*)
(*DBG:}*)
(*DBG:*)
(*DBG:size_t get_var_size(size_t idx)*)
(*DBG:{*)
(*DBG:    if (idx >= VAR_COUNT)*)
(*DBG:    {*)
(*DBG:        return 0;*)
(*DBG:    }*)
(*DBG:    switch (debug_vars[idx].type) {*)
(*DBG:    case INT_P_ENUM:*)
(*DBG:        return sizeof(INT);*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:    case BOOL_O_ENUM:*)
(*DBG:    case BOOL_P_ENUM:*)
(*DBG:        return sizeof(BOOL);*)
(*DBG:    default:*)
(*DBG:        return 0;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void *get_var_addr(size_t idx)*)
(*DBG:{*)
(*DBG:    void *ptr = debug_vars[idx].ptr;*)
(*DBG:*)
(*DBG:    switch (debug_vars[idx].type) {*)
(*DBG:    case INT_P_ENUM:*)
(*DBG:        return (void *)((((__IEC_INT_p *) ptr)->flags & __IEC_FORCE_FLAG) *)
(*DBG:                        ? &(((__IEC_INT_p *) ptr)->fvalue) *)
(*DBG:                        : ((__IEC_INT_p *) ptr)->value);*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:        return (void *)&((__IEC_BOOL_t *) ptr)->value;*)
(*DBG:    case BOOL_O_ENUM:*)
(*DBG:    case BOOL_P_ENUM:*)
(*DBG:        return (void *)((((__IEC_BOOL_p *) ptr)->flags & __IEC_FORCE_FLAG) *)
(*DBG:                        ? &(((__IEC_BOOL_p *) ptr)->fvalue) *)
(*DBG:                        : ((__IEC_BOOL_p *) ptr)->value);*)
(*DBG:    default:*)
(*DBG:        return 0;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void force_var(size_t idx, bool forced, void *val)*)
(*DBG:{*)
(*DBG:    void *ptr = debug_vars[idx].ptr;*)
(*DBG:*)
(*DBG:    if (forced) {*)
(*DBG:        size_t var_size = get_var_size(idx);*)
(*DBG:        switch (debug_vars[idx].type) {*)
(*DBG:        case INT_P_ENUM: {*)
(*DBG:            memcpy(&((__IEC_INT_p *) ptr)->fvalue, val, var_size);*)
(*DBG:            //((__IEC_INT_p *) ptr)->fvalue = *((INT *) val);*)
(*DBG:            ((__IEC_INT_p *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case BOOL_ENUM: {*)
(*DBG:            memcpy(&((__IEC_BOOL_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_BOOL_t *) ptr)->value = *((BOOL *) val);*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case BOOL_O_ENUM: {*)
(*DBG:            memcpy((((__IEC_BOOL_p *) ptr)->value), val, var_size);*)
(*DBG:            //*(((__IEC_BOOL_p *) ptr)->value) = *((BOOL *) val);*)
(*DBG:            ((__IEC_BOOL_p *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case BOOL_P_ENUM: {*)
(*DBG:            memcpy(&((__IEC_BOOL_p *) ptr)->fvalue, val, var_size);*)
(*DBG:            //((__IEC_BOOL_p *) ptr)->fvalue = *((BOOL *) val);*)
(*DBG:            ((__IEC_BOOL_p *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        default:*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:    } else {*)
(*DBG:        switch (debug_vars[idx].type) {*)
(*DBG:        case INT_P_ENUM:*)
(*DBG:            ((__IEC_INT_p *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case BOOL_ENUM:*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case BOOL_O_ENUM:*)
(*DBG:        case BOOL_P_ENUM:*)
(*DBG:            ((__IEC_BOOL_p *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        default:*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void swap_bytes(void *ptr, size_t size) *)
(*DBG:{*)
(*DBG:    uint8_t *bytePtr = (uint8_t *)ptr;*)
(*DBG:    size_t i;*)
(*DBG:    for (i = 0; i < size / 2; ++i) *)
(*DBG:    {*)
(*DBG:        uint8_t temp = bytePtr[i];*)
(*DBG:        bytePtr[i] = bytePtr[size - 1 - i];*)
(*DBG:        bytePtr[size - 1 - i] = temp;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void trace_reset(void)*)
(*DBG:{*)
(*DBG:    for (size_t i=0; i < VAR_COUNT; i++) *)
(*DBG:    {*)
(*DBG:        force_var(i, false, 0);*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void set_trace(size_t idx, bool forced, void *val)*)
(*DBG:{*)
(*DBG:    if (idx >= 0 && idx < VAR_COUNT) *)
(*DBG:    {*)
(*DBG:        if (endianness == REVERSE_ENDIANNESS)*)
(*DBG:        {*)
(*DBG:            // Aaaaarghhhh... Stupid AVR is Big Endian.*)
(*DBG:            swap_bytes(val, get_var_size(idx));*)
(*DBG:        }*)
(*DBG:*)
(*DBG:        force_var(idx, forced, val);*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)
(*DBG:void set_endianness(uint8_t value)*)
(*DBG:{*)
(*DBG:    if (value == SAME_ENDIANNESS || value == REVERSE_ENDIANNESS)*)
(*DBG:    {*)
(*DBG:        endianness = value;*)
(*DBG:    }*)
(*DBG:}*)
(*DBG:*)