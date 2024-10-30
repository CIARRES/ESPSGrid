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
  __INIT_VAR(data__->SM01VA,0,retain)
  __INIT_VAR(data__->SM01VB,0,retain)
  __INIT_VAR(data__->SM01VC,0,retain)
  __INIT_VAR(data__->SM01IA,0,retain)
  __INIT_VAR(data__->SM01IB,0,retain)
  __INIT_VAR(data__->SM01IC,0,retain)
  __INIT_VAR(data__->SM02VA,0,retain)
  __INIT_VAR(data__->SM02VB,0,retain)
  __INIT_VAR(data__->SM02VC,0,retain)
  __INIT_VAR(data__->SM02IA,0,retain)
  __INIT_VAR(data__->SM02IB,0,retain)
  __INIT_VAR(data__->SM02IC,0,retain)
  __INIT_VAR(data__->SM03VA,0,retain)
  __INIT_VAR(data__->SM03VB,0,retain)
  __INIT_VAR(data__->SM03VC,0,retain)
  __INIT_VAR(data__->SM03IA,0,retain)
  __INIT_VAR(data__->SM03IB,0,retain)
  __INIT_VAR(data__->SM03IC,0,retain)
  __INIT_VAR(data__->SM04VA,0,retain)
  __INIT_VAR(data__->SM04VB,0,retain)
  __INIT_VAR(data__->SM04VC,0,retain)
  __INIT_VAR(data__->SM04IA,0,retain)
  __INIT_VAR(data__->SM04IB,0,retain)
  __INIT_VAR(data__->SM04IC,0,retain)
  __INIT_VAR(data__->SM01SECST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->SM02SECST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->SM03SECST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->SM04SECST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB01CST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB02CST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB03CST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB04CST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB01OPST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB02OPST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB03OPST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CB04OPST,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->CLOSE,__BOOL_LITERAL(FALSE),retain)
}

// Code part
void METERING_body__(METERING *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->,CLOSE,,!(__GET_VAR(data__->CB01OPST,)));

  goto __end;

__end:
  return;
} // METERING_body__() 





