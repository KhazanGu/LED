# LED

## Arduino

Version: 1.8.19

Board - ESP32 Dev Module

partition scheme -  2M APP/2M SPIFFS

The project is an Arduino project. It uses the HomeSpan library.

### HomeSpan

Version: 2.1.1

[HomeSpan](https://github.com/HomeSpan/HomeSpan)

### Device Configure

SSID: Apple-LED  
Password: 12345678

Device code: 466-37-726


### Custom Website

FilePath:
`/Users/khazan/Documents/Arduino/libraries/HomeSpan/src/Network.cpp `

Content:

```
///////////////////////////////

void Network_HS::processRequest(char *body, char *formData){
  
  String responseHead="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n";
  
  String responseBody="<html><meta charset=\"utf-8\"><head><style>"
                        "p{font-size:300%; margin:1em}"
                        "label{font-size:300%; margin:1em}"
                        "input{font-size:250%; margin:1em}"
                        "button{font-size:250%; margin:1em}"
                      "</style></head>"
                      "<body style=\"background-color:lightyellow;\">"
                      "<center><p><b>连接Wi-Fi</b></p></center>";

  if(!strncmp(body,"POST /configure ",16) &&                              // POST CONFIGURE
     strstr(body,"Content-Type: application/x-www-form-urlencoded")){     // check that content is from a form

    LOG2(formData);                                                       // print form data
    LOG2("\n------------ END DATA! ------------\n");
               
    LOG1("In Post Configure...\n");

    getFormValue(formData,"network",wifiData.ssid,MAX_SSID);
    getFormValue(formData,"pwd",wifiData.pwd,MAX_PWD);

    STATUS_UPDATE(start(LED_WIFI_CONNECTING),HS_WIFI_CONNECTING)
        
    responseBody+="<meta http-equiv = \"refresh\" content = \"" + String(homeSpan.wifiTimeCounter/1000) + "; url = /wifi-status\" />"
                  "<p>正在启动 WiFi 连接：</p><p><b>" + String(wifiData.ssid) + "</b></p>"
                  "<p>(等待" + String((homeSpan.wifiTimeCounter++)/1000) + "秒检查响应)</p>";
                  
    WiFi.begin(wifiData.ssid,wifiData.pwd);              
  
  } else

  if(!strncmp(body,"POST /save ",11)){                                    // GET SAVE
    getFormValue(formData,"code",setupCode,8);

    if(allowedCode(setupCode)){
      responseBody+="<p><b>配置已保存!</b></p><p>设备重启中.</p><p>正在关闭页面...</p>";
      alarmTimeOut=millis()+2000;
      apStatus=1;
      
    } else {
    responseBody+="<meta http-equiv = \"refresh\" content = \"4; url = /wifi-status\" />"
                  "<p><b>Disallowed Setup Code - too simple!</b></p><p>Returning to configuration page...</p>";      
    }
    
  } else

  if(!strncmp(body,"GET /cancel ",12)){                                   // GET CANCEL
    responseBody+="<p><b>配置取消!</b></p><p>设备重启中.</p><p>正在关闭页面...</p>";
    alarmTimeOut=millis()+2000;
    apStatus=-1;
  } else

  if(!strncmp(body,"GET /wifi-status ",17)){                              // GET WIFI-STATUS

    LOG1("In Get WiFi Status...\n");

    if(WiFi.status()!=WL_CONNECTED){
      responseHead+="刷新: " + String(homeSpan.wifiTimeCounter/1000) + "\r\n";     
      responseBody+="<p>正在重连网络:</p><p><b>" + String(wifiData.ssid) + "</b></p>";
      responseBody+="<p>(等待" + String((homeSpan.wifiTimeCounter++)/1000) + "秒检查响应)</p>";
      responseBody+="<p>连接耗时" + String((alarmTimeOut-millis())/1000) + "秒.</p>";
      responseBody+="<center><button onclick=\"document.location='/hotspot-detect.html'\">Cancel</button></center>";
      WiFi.begin(wifiData.ssid,wifiData.pwd);
      
    } else {

      STATUS_UPDATE(start(LED_AP_CONNECTED),HS_AP_CONNECTED)
          
      responseBody+="<p>成功! 已连接到Wi-Fi:</p><p><b>" + String(wifiData.ssid) + "</b></p>";

      responseBody+="<form action=\"/save\" method=\"post\">"
                    "<label for=\"code\">Setup Code:</label>"
                    "<center><input size=\"32\" type=\"tel\" id=\"code\" name=\"code\" placeholder=\"12345678\" pattern=\"[0-9]{8}\" maxlength=8></center>"
                    "<center><input style=\"font-size:300%\" type=\"submit\" value=\"SAVE Settings\"></center>"
                    "</form>";
                    
      responseBody+="<center><button style=\"font-size:300%\" onclick=\"document.location='/cancel'\">CANCEL Configuration</button></center>";
    }
  
  } else                                                                

  if(!strstr(body,"wispr") && !strncmp(body,"GET /hotspot-detect.html ",25)){                             // GET LANDING-PAGE, but only if request does NOT contain "wispr" user agent

    LOG1("In Landing Page...\n");

    STATUS_UPDATE(start(LED_AP_CONNECTED),HS_AP_CONNECTED)
    homeSpan.wifiTimeCounter.reset();

    responseBody+="<p>确认指示灯双闪，输入Wi-Fi账户和密码.</p>"
                  "<form action=\"/configure\" method=\"post\">"
                  "<label for=\"ssid\">WiFi名称:</label>"
                  "<center><input size=\"32\" list=\"network\" name=\"network\" placeholder=\"Choose or Type\" required maxlength=" + String(MAX_SSID) + "></center>"
                  "<datalist id=\"network\">";

    for(int i=0;i<numSSID;i++)
        responseBody+="<option value=\"" + String(ssidList[i]) + "\">" + String(ssidList[i]) + "</option>";  
    
    responseBody+="</datalist><br><br>"
                  "<label for=\"pwd\">WiFi密码:</label>"
                  "<center><input size=\"32\" type=\"password\" id=\"pwd\" name=\"pwd\" required maxlength=" + String(MAX_PWD) + "></center>"
                  "<br><br>";
                  
    responseBody+="<center><input style=\"font-size:300%\" type=\"submit\" value=\"SUBMIT\"></center>"
                  "</form>";

    responseBody+="<center><button style=\"font-size:300%\" onclick=\"document.location='/cancel'\">CANCEL Configuration</button></center>";                  
                  
  }

  responseHead+="\r\n";               // add blank line between reponse header and body
  responseBody+="</body></html>";     // close out body and html tags

  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");
  LOG2(responseHead);
  LOG2(responseBody);
  LOG2("\n");
  client.print(responseHead);
  client.print(responseBody);
  LOG2("------------ SENT! --------------\n");
    
} // processRequest

//////////////////////////////////////

```


## User Guard

To enter Device Configuration Mode, press and hold the Control Button for 3 seconds, at which point the Status LED should begin to flash rapidly (10 times per second). Upon releasing the button the device will switch into the Device Configuration Mode.

Within Device Configuration Mode you can choose one of five possible Actions:

Exit Device Configuration Mode.

Reboot the device.

Launch HomeSpan’s temporary WiFi network.

Unpair the device from Apple HomeKit.

Erase stored WiFi Credentials. 

When the Device Configuration Mode is first started, the Status LED will blink once per second to indicate Action 1 has been selected. To select the Action 2, briefly press and release the Control Button. The Status LED will now exhibit a double-blink pattern with a one second pause between blinks, indicating Action 2 has been selected. Each press of the Control Button selects the next action and causes the Status LED to blink according to the Action number. Pressing the Control Button once you reach Action 5 brings the selection back to Action 1.

To execute the selected Action press and hold the Control Button for 3 seconds, at which time the Status LED will begin to flash rapidly (10 times a second). Upon releasing the button HomeSpan will perform the Action selected, and the Status LED will change patterns to reflect the new state of the device after the Action has been taken.

If no Action has been executed within 120 seconds (2 minutes) of starting the Device Configuration Mode, Action 1 is automatically executed and the Device Configuration Mode is exited.
