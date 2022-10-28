#if SERIAL_DEBUG
  #define  DUMPS(s)    {  SERIALDEBUG.print(F(s)); }
  #define  DUMPSLN(s)    {  SERIALDEBUG.println(F(s)); }
  #define  DUMPV(v)    {  SERIALDEBUG.print(v); }
  #define  DUMPVHEX(v)    {  SERIALDEBUG.print(v, HEX); }
  #define  DUMPVLN(v)    {  SERIALDEBUG.println(v); }
  #define  DUMPPRINTLN() { SERIALDEBUG.println();}
  
  #define  DUMP(s, v)  { DUMPS(s); DUMPV(v); }
  #define  DUMPLN(s, v)  { DUMPS(s); DUMPV(v); DUMPPRINTLN(); }
  #define  DUMPHEX(s, v)  { DUMPS(s); DUMPVHEX(v); }
  #define  DUMPF(s,v)    {  SERIALDEBUG.printf(s,v); }
  #define  DUMP_CMD(s, v)  { SERIALDEBUG.print(DEBUG_STARTCMD); DUMPS(s); DUMPV(v); SERIALDEBUG.print(DEBUG_ENDCMD); }
  #define  DUMPS_CMD(s)    { SERIALDEBUG.print(DEBUG_STARTCMD); DUMPS(s);; SERIALDEBUG.print(DEBUG_ENDCMD);}
  
#else
  #define  DUMPS(s)
  #define  DUMPSLN(s)
  #define  DUMPV(v)
  #define  DUMPVHEX(v)
  #define  DUMPVLN(v)
  #define  DUMPPRINTLN() 
  
  #define  DUMP(s, v)
  #define  DUMPLN(s, v)
  #define  DUMPHEX(s, v)
  #define  DUMPF(s,v)
  #define  DUMP_CMD(s, v) 
  #define  DUMPS_CMD(s)
#endif
