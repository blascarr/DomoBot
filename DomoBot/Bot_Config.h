DomoBot bot(10);

void botloop(){
  bot.loop();
}

#if SERIAL_CONTROL
  void serial_loop(){
    bot.serial_loop();
  }
#endif
