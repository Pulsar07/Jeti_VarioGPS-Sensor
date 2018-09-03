/*
  -----------------------------------------------------------
            Menu handler for Jetibox
  -----------------------------------------------------------
*/


// Jetibox screen names
enum screenViews {
  aboutScreen,
  resetOffset,
  #ifdef SUPPORT_GPS
  setGpsMode,
  setDistanceMode,
  #endif
  detectedPressureSensor,
  setVarioSmoothingValue,
  setDeadzone,
  #ifdef SUPPORT_MPXV7002_MPXV5004
  setAirSpeedSensorState,
  #ifdef SUPPORT_AIRSPEED_JETIBOX
  setAirSpeedTubeCorrection,
  setAirSpeedSmoothing,
  #endif
  #endif
  #ifdef SUPPORT_TEC
  setTECmode,
  #endif
  #ifdef SUPPORT_MAIN_DRIVE
  setMainDrive,
  setCapacityMode,
  #endif
  #ifdef SUPPORT_RX_VOLTAGE
  enableRx1Voltage,
  enableRx2Voltage,
  #endif
  #ifdef SUPPORT_EXT_TEMP
  enableExternTemp,
  #endif
  #ifdef SPEEDVARIO
  setSpeedVarioMode,
  setNormalSpeed,
  setSpeedSpread,
  #endif
  saveSettings,
  defaultSettings
};


const char menuText[][17] PROGMEM=
{
  {"VarioGPS Sensor"},
  {"Reset offset"},
  #ifdef SUPPORT_GPS
  {"GPS mode:"},
  {"GPS distance:"},
  #endif
  {"Pressure sensor:"},
  {"Vario smoothing"},
  {"Vario deadzone:"},
  #ifdef SUPPORT_MPXV7002_MPXV5004
  {"AirSpeedState:"},
  #ifdef SUPPORT_AIRSPEED_JETIBOX
  {"AirSpTubeCorr:"},
  {"AirSpSmoothing:"},
  #endif
  #endif
  #ifdef SUPPORT_TEC
  {"TEC mode:"},
  #endif
  #ifdef SUPPORT_MAIN_DRIVE
  {"Main drive:"},
  {"Capacity reset:"},
  #endif
  #ifdef SUPPORT_RX_VOLTAGE
  {"Rx1 voltage:"},
  {"Rx2 voltage:"},
  #endif
  #ifdef SUPPORT_EXT_TEMP
  {"Ext. Temp:"},
  #endif
  #ifdef SPEEDVARIO
  {"SpeedVario mode:"},
  {"Normal Speed :"},
  {"Speed Spread F:"},
  #endif
  {"Save and restart"},
  {"Load defaults"},

};

const char aboutScreenText[17] PROGMEM= {VARIOGPS_VERSION};

#ifdef SPEEDVARIO
const char setSpeedVarioModeText[][16] PROGMEM=
{
  {" Base Vario"},
  {" Speed"},
  {" TEC-Vario AirS"},
  {" TEC-Vario GPS"},
};
#endif

const char setGpsModeText[][10] PROGMEM=
{
  {" Disabled"},
  {" Basic"},
  {" Extended"}
};

const char setDistanceModeText[][4] PROGMEM=
{
  {" 3D"},
  {" 2D"}
};

const char detectedPressureSensorText[][9] PROGMEM=
{
  {" unknown"},
  {" BMP280"},
  {" BME280"},
  {" MS5611"},
  {" LPS"}
};

#ifdef SUPPORT_TEC
const char setTECmodeText[][11] PROGMEM=
{
  {" Disabled"},
  {" Air speed"},
  {" GPS"}
};
#endif

const char setMainDriveText[][16] PROGMEM=
{
  {" Disabled"},
  {" AttoPilot 45"},
  {" AttoPilot 90"},
  {" AttoPilot 180"},
  {" APM2.5 90A/50V"},
  #if V_REF > 4500
  {" ACS712-05"},
  {" ACS712-20"},
  {" ACS712-30"},
  #endif
  {" ACS758-50B"},
  {" ACS758-100B"},
  {" ACS758-150B"},
  {" ACS758-200B"},
  {" ACS758-50U"},
  {" ACS758-100U"},
  {" ACS758-150U"},
  {" ACS758-200U"}
};

const char setCapacityModeText[][9] PROGMEM=
{
  {" Startup"},
  {" Auto"},
  {" Manual"}
};

const char enableText[][10] PROGMEM=
{
  {" Disabled"},
  {" Enabled"}
};


void HandleMenu()
{
  static int  _nMenu = aboutScreen;
  static bool _bSetDisplay = true;
  static uint32_t LastKey;
  char _bufferLine1[17];
  char _bufferLine2[17];
  uint8_t c = jetiEx.GetJetiboxKey();

  enum
  {
    keyRight       = 0xe0,
    keyLeft        = 0x70,
    keyUp          = 0xd0,
    keyDown        = 0xb0,
    keyUpDown      = 0x90,
    keyLeftRight   = 0x60
  };

  if( c == 0 && !_bSetDisplay) return;

  if( millis() < LastKey )
    return;
  LastKey = millis() + 200;

  startHandleMenu:

  // Right
  if ( c == keyRight && _nMenu < defaultSettings)
  {
    _nMenu++;
    _bSetDisplay = true;
  }

  // Left
  if ( c == keyLeft &&  _nMenu > aboutScreen )
  {
    _nMenu--;
    _bSetDisplay = true;
  }

  // UP
  if ( c == keyUp )
  {
    switch ( _nMenu )
    {
      #ifdef SUPPORT_GPS
      case setGpsMode:
        if(gpsSettings.mode > GPS_disabled){
          gpsSettings.mode--;
        }
        break;
      #endif
      case setVarioSmoothingValue:
        if (pressureSensor.smoothingValue < 1.0) {
          pressureSensor.smoothingValue = (pressureSensor.smoothingValue*100+1)/100;
        }
        break;
      case setDeadzone:
        if (pressureSensor.deadzone < 100) {
          pressureSensor.deadzone++;
        }
        break;
      #ifdef SUPPORT_MPXV7002_MPXV5004
      case setAirSpeedSensorState:
        if(airSpeedSensor.state > airSpeed_disabled){
          airSpeedSensor.state--;
        }
        break;
      #ifdef SUPPORT_AIRSPEED_JETIBOX
      case setAirSpeedTubeCorrection:
        if(airSpeedSensor.tubeCorrection < 2.5){
          airSpeedSensor.tubeCorrection += .01;
        }
        break;
      case setAirSpeedSmoothing:
        if (airSpeedSensor.smoothingValue < 1.0) {
          airSpeedSensor.smoothingValue += .01;
        }
        break;
      #endif
      #endif
      #ifdef SUPPORT_TEC
      case setTECmode:
        if(airSpeedSensor.TECmode > TEC_disabled){
          airSpeedSensor.TECmode--;
        }
        //if(TECmode == TEC_airSpeed && airSpeedSensor == airSpeed_disabled)goto startHandleMenu;
        //if(TECmode == TEC_GPS && gpsSettings.mode == GPS_disabled)TECmode = TEC_disabled;
        break;
      #endif
      #ifdef SUPPORT_MAIN_DRIVE
      case setMainDrive:
        if (currentSensor > mainDrive_disabled) {
          currentSensor--;
        }
        break;
      case setCapacityMode:
        if(capacityMode > startup){
          capacityMode--;
        }
        break;
      #endif
      #ifdef SPEEDVARIO
      case setSpeedVarioMode:
        if(speedVarioPreset.mode > SV_BASIC_VARIO){
          speedVarioPreset.mode--;
        }
        break;
      case setNormalSpeed:
        if (speedVarioPreset.normalSpeed < 3000) {
          speedVarioPreset.normalSpeed += 100;
        }
        break;
      case setSpeedSpread:
        if (speedVarioPreset.speedSpread < 3.0) {
          speedVarioPreset.speedSpread += .1;
        }
        break;
      #endif
    }

    _bSetDisplay = true;
  }

  // DN
  if ( c == keyDown )
  {
    switch ( _nMenu )
    {
      case resetOffset:
        EEPROM.put(EEPROM_ADRESS_CAPACITY, 0.0f);                 // reset capacity in eeprom
        EEPROM.put(EEPROM_ADRESS_CAPACITY+sizeof(float), 0.0f);
        restartCPU();
      #ifdef SUPPORT_GPS
      case setGpsMode:
        if(gpsSettings.mode < GPS_extended){
          gpsSettings.mode++;
        }
        break;
      case setDistanceMode:
        gpsSettings.distance3D = !gpsSettings.distance3D;
        break;
      #endif
      case setVarioSmoothingValue:
        if (pressureSensor.smoothingValue > 0.0) {
          pressureSensor.smoothingValue = (pressureSensor.smoothingValue*100-1)/100;
        }
        break;
      case setDeadzone:
        if (pressureSensor.deadzone > 0) {
          pressureSensor.deadzone--;
        }
        break;
      #ifdef SUPPORT_MPXV7002_MPXV5004
      case setAirSpeedSensorState:
        if(airSpeedSensor.state < MPXV7002_MPXV5004){
          airSpeedSensor.state++;
        }
      #ifdef SUPPORT_AIRSPEED_JETIBOX
      case setAirSpeedTubeCorrection:
        if(airSpeedSensor.tubeCorrection > 0.0){
          airSpeedSensor.tubeCorrection -= .01;
        }
        break;
      case setAirSpeedSmoothing:
        if(airSpeedSensor.smoothingValue > 0.0){
          airSpeedSensor.smoothingValue -= .01;
        }
        break;
      #endif
      #endif
      #ifdef SUPPORT_TEC
      case setTECmode:
        if(airSpeedSensor.TECmode < TEC_GPS){
          airSpeedSensor.TECmode++;
        }
        //if(TECmode == TEC_airSpeed && airSpeedSensor == airSpeed_disabled)goto startHandleMenu;
        //if(TECmode == TEC_GPS && gpsSettings.mode == GPS_disabled)TECmode = TEC_disabled;
        break;
      #endif
      #ifdef SUPPORT_MAIN_DRIVE
      case setMainDrive:
        if (currentSensor < ACS758_200U) {
          currentSensor++;
        }
        break;
      case setCapacityMode:
        if(capacityMode < manual){
          capacityMode++;
        }
        break;
      #endif
      #ifdef SPEEDVARIO
      case setSpeedVarioMode:
        if(speedVarioPreset.mode < SV_TEC_VARIO_GPS){
          speedVarioPreset.mode++;
        }
      case setNormalSpeed:
        if (speedVarioPreset.normalSpeed > 0) {
          speedVarioPreset.normalSpeed -= 100;
        }
        break;
      case setSpeedSpread:
        if (speedVarioPreset.speedSpread > 0.0) {
          speedVarioPreset.speedSpread -= .1;
        }
        break;
      #endif
      #ifdef SUPPORT_RX_VOLTAGE
      case enableRx1Voltage:
        enableRx1 = !enableRx1;
        break;
      case enableRx2Voltage:
        enableRx2 = !enableRx2;
        break;
      #endif
      #ifdef SUPPORT_EXT_TEMP
      case enableExternTemp:
        enableExtTemp = !enableExtTemp;
        break;
      #endif
      case saveSettings:
        #ifdef SUPPORT_GPS
        EEPROM.write(P_GPS_MODE, gpsSettings.mode);
        EEPROM.write(P_GPS_3D, gpsSettings.distance3D);
        #endif

        #ifdef SUPPORT_MAIN_DRIVE
        EEPROM.write(P_CURRENT_SENSOR, currentSensor);
        EEPROM.write(P_CAPACITY_MODE, capacityMode);
        #endif

        #ifdef SUPPORT_RX_VOLTAGE
        EEPROM.write(P_ENABLE_RX1, enableRx1);
        EEPROM.write(P_ENABLE_RX2, enableRx2);
        #endif

        #ifdef SUPPORT_EXT_TEMP
        EEPROM.write(P_ENABLE_TEMP, enableExtTemp);
        #endif

        EEPROM.write(P_VARIO_SMOOTHING,int(pressureSensor.smoothingValue*100));
        EEPROM.write(P_VARIO_DEADZONE,pressureSensor.deadzone);

        #ifdef SUPPORT_MPXV7002_MPXV5004
        EEPROM.write(P_AIRSPEED_SENSOR,airSpeedSensor.state);
        #ifdef SUPPORT_AIRSPEED_JETIBOX
        EEPROM.write(P_AIRSPEED_TUBE_CORR,int(airSpeedSensor.tubeCorrection*100));
        EEPROM.write(P_AIRSPEED_SMOOTHING,int(airSpeedSensor.smoothingValue*100));
        #endif
        #endif

        #ifdef SUPPORT_TEC
        EEPROM.write(P_TEC_MODE,airSpeedSensor.TECmode);
        #endif

        #ifdef SPEEDVARIO
        EEPROM.write(P_SPEEDVARIO_NORMALSPEED,int(speedVarioPreset.normalSpeed/100));
        EEPROM.write(P_SPEEDVARIO_SPREADSPEED,int(speedVarioPreset.speedSpread*100));
        EEPROM.write(P_SPEEDVARIO_MODE,speedVarioPreset.mode);
        #endif
        restartCPU();
      case defaultSettings:
        for(int i=0; i < 255; i++){
          EEPROM.write(i, 0xFF);
        }
        EEPROM.put(EEPROM_ADRESS_CAPACITY, 0.0f);
        EEPROM.put(EEPROM_ADRESS_CAPACITY+sizeof(float), 0.0f);
        restartCPU();
    }

    _bSetDisplay = true;
  }

  if ( !_bSetDisplay )
    return;

  // clear buffer
  _bufferLine1[0] = 0;
  _bufferLine2[0] = 0;

  memcpy_P( _bufferLine1, &menuText[_nMenu], 16 );

  switch ( _nMenu )
  {
    case aboutScreen:
      memcpy_P( _bufferLine2, aboutScreenText, 16 );
      break;
    #ifdef SUPPORT_GPS
    case setGpsMode:
      memcpy_P( _bufferLine2, &setGpsModeText[gpsSettings.mode], 16 );
      break;
    case setDistanceMode:
      if(gpsSettings.mode == GPS_disabled)goto startHandleMenu;
      memcpy_P( _bufferLine2, &setDistanceModeText[gpsSettings.distance3D], 16 );
      break;
    #endif
    case detectedPressureSensor:
      memcpy_P( _bufferLine2, &detectedPressureSensorText[pressureSensor.type], 16 );
      break;
    case setVarioSmoothingValue:
      if(pressureSensor.type == unknown)goto startHandleMenu;
      sprintf( _bufferLine2, " %2d%%",int(pressureSensor.smoothingValue*100));
      break;
    case setDeadzone:
      if(pressureSensor.type == unknown)goto startHandleMenu;
      sprintf( _bufferLine2, " %2dcm",pressureSensor.deadzone);
      break;
    #ifdef SUPPORT_MPXV7002_MPXV5004
    case setAirSpeedSensorState:
      memcpy_P( _bufferLine2, &enableText[airSpeedSensor.state], 16 );
      break;
    #ifdef SUPPORT_AIRSPEED_JETIBOX
    case setAirSpeedSmoothing:
      sprintf( _bufferLine2, " %2d%%",int(airSpeedSensor.smoothingValue*100));
      break;
    case setAirSpeedTubeCorrection:
      sprintf(_bufferLine2, "%03d", int(airSpeedSensor.tubeCorrection*100));
      sprintf(_bufferLine2, "%c\.%c%c", _bufferLine2[0], _bufferLine2[1], _bufferLine2[2]);
      break;
    #endif
    #endif
    #ifdef SUPPORT_TEC
    case setTECmode:
      //if(airSpeedSensor == airSpeed_disabled && gpsSettings.mode == GPS_disabled)goto startHandleMenu;
      /*if(TECmode == TEC_airSpeed && airSpeedSensor == airSpeed_disabled)TECmode++;
      if(TECmode == TEC_GPS && gpsSettings.mode == GPS_disabled)TECmode++;
      if(TECmode > TEC_GPS){
        TECmode = TEC_disabled;
      }*/
      memcpy_P( _bufferLine2, &setTECmodeText[airSpeedSensor.TECmode], 16 );
      break;
    #endif
    #ifdef SUPPORT_MAIN_DRIVE
    case setMainDrive:
      memcpy_P( _bufferLine2, &setMainDriveText[currentSensor], 16 );
      break;
    case setCapacityMode:
      if(currentSensor == mainDrive_disabled)goto startHandleMenu;
      memcpy_P( _bufferLine2, &setCapacityModeText[capacityMode], 16 );
      break;
    #endif
    #ifdef SPEEDVARIO
    case setSpeedVarioMode:
      memcpy_P( _bufferLine2, &setSpeedVarioModeText[speedVarioPreset.mode], 16 );
      break;
    case setNormalSpeed:
      if(pressureSensor.type == unknown)goto startHandleMenu;
      sprintf( _bufferLine2, " %2dm/s",int(speedVarioPreset.normalSpeed/100));
      break;
    case setSpeedSpread:
      if(pressureSensor.type == unknown)goto startHandleMenu;
      // oh mann, was man zum Speicher sparen nicht alles treibt ;-))
      sprintf(_bufferLine2, "%03d", int(speedVarioPreset.speedSpread*100));
      sprintf(_bufferLine2, "%c.%c%c", _bufferLine2[0], _bufferLine2[1], _bufferLine2[2]);
      break;
    #endif
    #ifdef SUPPORT_RX_VOLTAGE
    case enableRx1Voltage:
      memcpy_P( _bufferLine2, &enableText[enableRx1], 16 );
      break;
    case enableRx2Voltage:
      memcpy_P( _bufferLine2, &enableText[enableRx2], 16 );
      break;
    #endif
    #ifdef SUPPORT_EXT_TEMP
    case enableExternTemp:
      memcpy_P( _bufferLine2, &enableText[enableExtTemp], 16 );
      break;
    #endif
  }

  jetiEx.SetJetiboxText( JetiExProtocol::LINE1, _bufferLine1 );
  jetiEx.SetJetiboxText( JetiExProtocol::LINE2, _bufferLine2 );

  _bSetDisplay = false;
}
