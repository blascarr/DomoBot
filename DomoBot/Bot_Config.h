DomoBot bot(10);

void botloop(){
  bot.loop();
}

#if SERIAL_CONTROL
  void serial_loop(){
    bot.serial_loop();
  }
#endif


#if IMU_ENABLE
  void imu_loop(){
    bot.imu_loop();
  }
#endif
