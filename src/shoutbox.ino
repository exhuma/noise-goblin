
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include "config.h"
#define I2S_DOUT 15 // -> DIN
#define I2S_LRC 16 // -> LRC
#define I2S_BCLK 17 // -> BCLK

// Audio status callback functions
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
String ssid = WIFI_SSID;
String password = WIFI_PASSWORD;
Audio audio;
void setup() {
  Serial.begin(115200);
  Serial.println(F("Resetting WiFi"));
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.println(F("Connecting to WiFi"));
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(F("."));
      delay(1500);
  }
  Serial.println(F("Connected"));
  Serial.println(F("Connecting to audio stream"));
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10); // Setting to a lower value to avoid distortion

  // // Register all the callback functions
  // audio.setConnectionTimeout(10000, []() {
  //     Serial.println("Connection timeout - Failed to connect to stream");
  // });

  if(!audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3")) {
      Serial.println("Failed to connect to host");
  }
}
void loop()
{
  audio.loop();
}
