PROGRAM metering
  VAR
    SM01VA AT %IW10 : INT;
    SM01VB AT %IW11 : INT;
    SM01VC AT %IW12 : INT;
    SM01IA AT %IW13 : INT;
    SM01IB AT %IW14 : INT;
    SM01IC AT %IW15 : INT;
    SM02VA AT %IW20 : INT;
    SM02VB AT %IW21 : INT;
    SM02VC AT %IW22 : INT;
    SM02IA AT %IW23 : INT;
    SM02IB AT %IW24 : INT;
    SM02IC AT %IW25 : INT;
    SM03VA AT %IW30 : INT;
    SM03VB AT %IW31 : INT;
    SM03VC AT %IW32 : INT;
    SM03IA AT %IW33 : INT;
    SM03IB AT %IW34 : INT;
    SM03IC AT %IW35 : INT;
    SM04VA AT %IW40 : INT;
    SM04VB AT %IW41 : INT;
    SM04VC AT %IW42 : INT;
    SM04IA AT %IW43 : INT;
    SM04IB AT %IW44 : INT;
    SM04IC AT %IW45 : INT;
    SM01SecST AT %IX0.1 : BOOL;
    SM02SecST AT %IX0.2 : BOOL;
    SM03SecST AT %IX0.3 : BOOL;
    SM04SecST AT %IX0.4 : BOOL;
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

(*DBG:char md5[] = "f0bc2d97bfc4b07ea70e931a6e667bf4";*)
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
(*DBG:    {&(RES0__INSTANCE0.SM01VA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM01VB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM01VC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM01IA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM01IB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM01IC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02VA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02VB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02VC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02IA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02IB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02IC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03VA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03VB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03VC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03IA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03IB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03IC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04VA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04VB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04VC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04IA), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04IB), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04IC), INT_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM01SECST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM02SECST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM03SECST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.SM04SECST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB01CST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB02CST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB03CST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB04CST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB01OPST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB02OPST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB03OPST), BOOL_ENUM},*)
(*DBG:    {&(RES0__INSTANCE0.CB04OPST), BOOL_ENUM},*)
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
(*DBG:    case INT_ENUM:*)
(*DBG:        return sizeof(INT);*)
(*DBG:    case BOOL_ENUM:*)
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
(*DBG:    case INT_ENUM:*)
(*DBG:        return (void *)&((__IEC_INT_t *) ptr)->value;*)
(*DBG:    case BOOL_ENUM:*)
(*DBG:        return (void *)&((__IEC_BOOL_t *) ptr)->value;*)
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
(*DBG:        case INT_ENUM: {*)
(*DBG:            memcpy(&((__IEC_INT_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_INT_t *) ptr)->value = *((INT *) val);*)
(*DBG:            ((__IEC_INT_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        case BOOL_ENUM: {*)
(*DBG:            memcpy(&((__IEC_BOOL_t *) ptr)->value, val, var_size);*)
(*DBG:            //((__IEC_BOOL_t *) ptr)->value = *((BOOL *) val);*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags |= __IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:        default:*)
(*DBG:            break;*)
(*DBG:        }*)
(*DBG:    } else {*)
(*DBG:        switch (debug_vars[idx].type) {*)
(*DBG:        case INT_ENUM:*)
(*DBG:            ((__IEC_INT_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
(*DBG:            break;*)
(*DBG:        case BOOL_ENUM:*)
(*DBG:            ((__IEC_BOOL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;*)
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