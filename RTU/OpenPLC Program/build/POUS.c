void LOGGER_init__(LOGGER *data__, BOOL retain) {
  __INIT_VAR(data__->EN,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->ENO,__BOOL_LITERAL(TRUE),retain)
  __INIT_VAR(data__->TRIG,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->MSG,__STRING_LITERAL(0,""),retain)
  __INIT_VAR(data__->LEVEL,LOGLEVEL__INFO,retain)
  __INIT_VAR(data__->TRIG0,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void LOGGER_body__(LOGGER *data__) {
  // Control execution
  if (!__GET_VAR(data__->EN)) {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(FALSE));
    return;
  }
  else {
    __SET_VAR(data__->,ENO,,__BOOL_LITERAL(TRUE));
  }
  // Initialise TEMP variables

  if ((__GET_VAR(data__->TRIG,) && !(__GET_VAR(data__->TRIG0,)))) {
    #define GetFbVar(var,...) __GET_VAR(data__->var,__VA_ARGS__)
    #define SetFbVar(var,val,...) __SET_VAR(data__->,var,__VA_ARGS__,val)

   LogMessage(GetFbVar(LEVEL),(char*)GetFbVar(MSG, .body),GetFbVar(MSG, .len));
  
    #undef GetFbVar
    #undef SetFbVar
;
  };
  __SET_VAR(data__->,TRIG0,,__GET_VAR(data__->TRIG,));

  goto __end;

__end:
  return;
} // LOGGER_body__() 





void METERING_init__(METERING *data__, BOOL retain) {
  __INIT_LOCATED(INT,__IW10,data__->PM01VA,retain)
  __INIT_LOCATED_VALUE(data__->PM01VA,0)
  __INIT_LOCATED(INT,__IW11,data__->PM01VB,retain)
  __INIT_LOCATED_VALUE(data__->PM01VB,0)
  __INIT_LOCATED(INT,__IW12,data__->PM01VC,retain)
  __INIT_LOCATED_VALUE(data__->PM01VC,0)
  __INIT_LOCATED(INT,__IW13,data__->PM01IA,retain)
  __INIT_LOCATED_VALUE(data__->PM01IA,0)
  __INIT_LOCATED(INT,__IW14,data__->PM01IB,retain)
  __INIT_LOCATED_VALUE(data__->PM01IB,0)
  __INIT_LOCATED(INT,__IW15,data__->PM01IC,retain)
  __INIT_LOCATED_VALUE(data__->PM01IC,0)
  __INIT_LOCATED(INT,__IW20,data__->PM02VA,retain)
  __INIT_LOCATED_VALUE(data__->PM02VA,0)
  __INIT_LOCATED(INT,__IW21,data__->PM02VB,retain)
  __INIT_LOCATED_VALUE(data__->PM02VB,0)
  __INIT_LOCATED(INT,__IW22,data__->PM02VC,retain)
  __INIT_LOCATED_VALUE(data__->PM02VC,0)
  __INIT_LOCATED(INT,__IW23,data__->PM02IA,retain)
  __INIT_LOCATED_VALUE(data__->PM02IA,0)
  __INIT_LOCATED(INT,__IW24,data__->PM02IB,retain)
  __INIT_LOCATED_VALUE(data__->PM02IB,0)
  __INIT_LOCATED(INT,__IW25,data__->PM02IC,retain)
  __INIT_LOCATED_VALUE(data__->PM02IC,0)
  __INIT_LOCATED(INT,__IW30,data__->PM03VA,retain)
  __INIT_LOCATED_VALUE(data__->PM03VA,0)
  __INIT_LOCATED(INT,__IW31,data__->PM03VB,retain)
  __INIT_LOCATED_VALUE(data__->PM03VB,0)
  __INIT_LOCATED(INT,__IW32,data__->PM03VC,retain)
  __INIT_LOCATED_VALUE(data__->PM03VC,0)
  __INIT_LOCATED(INT,__IW33,data__->PM03IA,retain)
  __INIT_LOCATED_VALUE(data__->PM03IA,0)
  __INIT_LOCATED(INT,__IW34,data__->PM03IB,retain)
  __INIT_LOCATED_VALUE(data__->PM03IB,0)
  __INIT_LOCATED(INT,__IW35,data__->PM03IC,retain)
  __INIT_LOCATED_VALUE(data__->PM03IC,0)
  __INIT_LOCATED(INT,__IW40,data__->PM04VA,retain)
  __INIT_LOCATED_VALUE(data__->PM04VA,0)
  __INIT_LOCATED(INT,__IW41,data__->PM04VB,retain)
  __INIT_LOCATED_VALUE(data__->PM04VB,0)
  __INIT_LOCATED(INT,__IW42,data__->PM04VC,retain)
  __INIT_LOCATED_VALUE(data__->PM04VC,0)
  __INIT_LOCATED(INT,__IW43,data__->PM04IA,retain)
  __INIT_LOCATED_VALUE(data__->PM04IA,0)
  __INIT_LOCATED(INT,__IW44,data__->PM04IB,retain)
  __INIT_LOCATED_VALUE(data__->PM04IB,0)
  __INIT_LOCATED(INT,__IW45,data__->PM04IC,retain)
  __INIT_LOCATED_VALUE(data__->PM04IC,0)
  __INIT_LOCATED(BOOL,__IX0_1,data__->PM01SECST,retain)
  __INIT_LOCATED_VALUE(data__->PM01SECST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX0_2,data__->PM02SECST,retain)
  __INIT_LOCATED_VALUE(data__->PM02SECST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX0_3,data__->PM03SECST,retain)
  __INIT_LOCATED_VALUE(data__->PM03SECST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX0_4,data__->PM04SECST,retain)
  __INIT_LOCATED_VALUE(data__->PM04SECST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX2_1,data__->CB01CST,retain)
  __INIT_LOCATED_VALUE(data__->CB01CST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX2_2,data__->CB02CST,retain)
  __INIT_LOCATED_VALUE(data__->CB02CST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX2_3,data__->CB03CST,retain)
  __INIT_LOCATED_VALUE(data__->CB03CST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__IX2_4,data__->CB04CST,retain)
  __INIT_LOCATED_VALUE(data__->CB04CST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX0_1,data__->CB01OPST,retain)
  __INIT_LOCATED_VALUE(data__->CB01OPST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX0_2,data__->CB02OPST,retain)
  __INIT_LOCATED_VALUE(data__->CB02OPST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX0_3,data__->CB03OPST,retain)
  __INIT_LOCATED_VALUE(data__->CB03OPST,__BOOL_LITERAL(FALSE))
  __INIT_LOCATED(BOOL,__QX0_4,data__->CB04OPST,retain)
  __INIT_LOCATED_VALUE(data__->CB04OPST,__BOOL_LITERAL(FALSE))
  __INIT_VAR(data__->CLOSE,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void METERING_body__(METERING *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->,CLOSE,,!(__GET_LOCATED(data__->CB01OPST,)));

  goto __end;

__end:
  return;
} // METERING_body__() 





