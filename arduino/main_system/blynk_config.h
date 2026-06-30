// Blynk IoT setup — optional. Reports free-slot count, optional manual gate
// trigger from the app. Sketch compiles and runs fine with this left at 0.
//
// Steps to enable:
//   1. Create a new template at https://blynk.cloud (or the Blynk IoT app)
//   2. Copy the Template ID and Template Name from the template's Info tab
//   3. Create a device from that template, copy its Auth Token (Device Info)
//   4. Paste all three below, fill in your WiFi SSID/password
//   5. Set ENABLE_BLYNK to 1
//   6. Install the "Blynk" library (by Volodymyr Shymanskyy) via Library Manager
//   7. In the Blynk app/web dashboard, add a "Labeled Value" widget bound to
//      V0 (free slot count), optionally a "Button" widget bound to V1 (manual
//      gate trigger)

#pragma once

#define ENABLE_BLYNK 0   // set to 1 once credentials below are filled in

#if ENABLE_BLYNK
  #define BLYNK_TEMPLATE_ID   "TMPLxxxxxx"
  #define BLYNK_TEMPLATE_NAME "Parking System"
  #define BLYNK_AUTH_TOKEN    "your-auth-token-here"

  const char* WIFI_SSID = "your-wifi-ssid";
  const char* WIFI_PASS = "your-wifi-password";
#endif
