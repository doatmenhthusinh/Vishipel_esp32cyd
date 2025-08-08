#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
#include <TimeLib.h>
#include <HardwareSerial.h>
HardwareSerial SerialPort(1);  //if using UART1

#define LVGL_TICK_PERIOD 20



// Set the screen resolution
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10]; 

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

//declare the function to create a group and input device for the rotary encoder
lv_group_t *group1 = NULL;
lv_indev_t *encoder_indev = NULL;

//configure the resolution
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}
String receivedText = "";
String gpsData = "";
String signalData = "";
String timeData = "";
const char *gps1;
const char *signal1;
const char *date1;

int cambien = 35;
int gtcambien;
float vol_out;
float vol_in;


unsigned long lastReceivedTime = 0;  // Thời gian nhận dữ liệu cuối cùng
const unsigned long timeout = 5000;  // Thời gian chờ 5 giây (5000ms)

void Datatimer(lv_timer_t * timer){
  if (SerialPort.available() > 0 ) {
    // Đọc tất cả dữ liệu có sẵn từ serial vào một chuỗi
    receivedText = SerialPort.readStringUntil('\n');  
    //receivedText = SerialPort.read();  // Đọc toàn bộ chuỗi dữ liệu
    Serial.println(receivedText);
    //SerialPort.println(receivedText);
    //  String tempReceivedText = "";
    // while (Serial.available()) {
    //   char c = Serial.read();
    //   tempReceivedText += c;
    // }
    //  receivedText = tempReceivedText;  // Lưu lại chuỗi nhận được
    // gps1 = receivedText.c_str();
    // lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
    // lv_obj_set_width(ui_Label4, 320); // Chỉnh theo kích thước LCD
    // lv_label_set_long_mode(ui_Label4, LV_LABEL_LONG_WRAP); // giới hạn text
    // lv_label_set_text(ui_Label4, gps1);  // Cập nhật giá trị vào Label
    // Serial.println(gps1);
    lastReceivedTime = millis();  // Reset thời gian khi có dữ liệu mới

    if (receivedText.indexOf("gps") >= 0) {
      // Tách dữ liệu GPS từ chuỗi nhận được
      gpsData = extractGpsData(receivedText);
      parseGpsReadable(gpsData);
      // Hiển thị dữ liệu GPS nếu có
      if (gpsData.length() > 0) {
        String gpsDisplay = parseGpsReadable(gpsData);  // Chuyển thành chuỗi hiển thị
         gps1 = gpsDisplay.c_str(); 
        //Serial.println("gpsData:");
        //Serial.println(gpsData);
        // lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
        // lv_obj_set_width(ui_Label4, 320); // Chỉnh theo kích thước LCD
        // lv_label_set_long_mode(ui_Label4, LV_LABEL_LONG_WRAP); // giới hạn text
        // lv_obj_set_width(ui_Label4, 150); // Set desired width in pixels
        lv_label_set_text(ui_Label4, gps1);  // Cập nhật giá trị vào Label
        Serial.println(gps1);


      } else {
        Serial.println("GPS not found");
        //lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label4, "Khong tim thay gps");  // Cập nhật giá trị vào Label

      }

     }
    //else {
    //   Serial.println("No GPS Data");
    //   lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
    //   lv_label_set_text(ui_Label4, "No GPS Data");  // Cập nhật giá trị vào Label

    //   }
       //##########################
       //##########################
              // Kiểm tra xem chuỗi có chứa từ "signal" hay không
    // if (receivedText.indexOf("signal") >= 0) {
    //   // Tách dữ liệu signal từ chuỗi nhận được
    //   signalData = extractsignaldata(receivedText);
    //   // Hiển thị dữ liệu GPS nếu có
    //   if (signalData.length() > 0) {
    //     Serial.println("Signal:");
    //     Serial.println(signalData);
    //     signal1 = signalData.c_str();
    //     //lv_obj_set_width(ui_Label5, 320); // Chỉnh theo kích thước LCD
    //     //lv_label_set_long_mode(ui_Label5, LV_LABEL_LONG_WRAP); // giới hạn text
    //     lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
    //     lv_label_set_text(ui_Label5, signal1);  // Cập nhật giá trị vào Label
    //     Serial.println(signal1);
    //   } else {
    //     Serial.println("Sinal not Found");
    //     lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
    //     lv_label_set_text(ui_Label5, "No signal");  // Cập nhật giá trị vào Label
    //   }
    //  }

    //else {
    //   // Nếu không có dữ liệu singal, in thông báo
    //     Serial.println("No Signal Data");
    //     lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
    //     lv_label_set_text(ui_Label5, "No Signal Data");  // Cập nhật giá trị vào Label
    //   }
        
    if (receivedText.indexOf("signal") >= 0) {
    String status;
    float dBm;

    bool success = extractSignalData(receivedText, status, dBm);

    if (success && status == "Active") {
      Serial.println("Signal:");
      Serial.println("Status: " + status);
      Serial.println("dBm: " + String(dBm, 2));

      //lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
      lv_label_set_text(ui_Label5, status.c_str());  // Label 5 hiển thị Status

      //lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
      lv_label_set_text(ui_Label7, String(dBm, 2).c_str()); // Label 6 hiển thị dBm

    } else {
      Serial.println("Signal not found or inactive");

      //lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
      lv_label_set_text(ui_Label5, "No signal");

      //lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
      lv_label_set_text(ui_Label7, "----");
    }
  }

    //   //############################
    //   //#########################

      if (receivedText.indexOf("outbox") >= 0) {
      // Tách dữ liệu signal từ chuỗi nhận được
      timeData = extractDateTime(receivedText);
      // Hiển thị dữ liệu GPS nếu có
      if (timeData.length() > 0) {
        Serial.println("outbox:");
        //Serial2.println(timeData);
        date1 = timeData.c_str();
        //lv_obj_set_width(ui_Label7, 320); // Chỉnh theo kích thước LCD
       // lv_label_set_long_mode(ui_Label7, LV_LABEL_LONG_WRAP); // giới hạn text
       // lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
        //lv_label_set_text(ui_Label7, date1);  // Cập nhật giá trị vào Label
        lv_textarea_set_text(ui_TextArea1, date1);
        Serial.println(date1);
      } else {
        Serial.println("khong tim thay");
        //lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
        //lv_label_set_text(ui_Label7, "khong tim thay");
        lv_textarea_set_text(ui_TextArea1, "khong tim thay");  // Cập nhật giá trị vào Label
      }
     }


    //else {
    //   Serial.println("No time Data");
    //   lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
    //   lv_label_set_text(ui_Label7, "No time Data");  // Cập nhật giá trị vào Label
    //   }
      
      receivedText = "";  // Làm sạch chuỗi sau mỗi lần xử lý 
    }

   else if (millis() - lastReceivedTime > timeout) {
    // Nếu không có dữ liệu mới sau 5 giây
        showWaitingMessage();

  }
 
     DC();
  }
void showWaitingMessage() {
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
    lv_label_set_text(ui_Label4, "Dang doi du lieu...");
        lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
    lv_label_set_text(ui_Label5, "loading...");
        lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
    lv_label_set_text(ui_Label7, "loading...");
}


void setup() {
    Serial.begin(115200);
    // SerialPort.begin(9600, SERIAL_8N1, 22, 27); 
    SerialPort.begin(9600, SERIAL_8N1, 22, 21); 
    lv_init();
    pinMode(cambien, INPUT); // ESP32 hỗ trợ chế độ này
    tft.begin();
    tft.setRotation(1);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    //Initialize the display.
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    ui_init();

     lv_timer_t* timer = lv_timer_create(Datatimer,900,NULL); //Update  value after 1 second

    Serial.println("Setup done");
}

void loop() {
      // Đọc dữ liệu Serial ngay lập tức khi có sẵn
      // if (Serial.available()) {
      //     receivedText = Serial.readString();  // Đọc dữ liệu từ Serial
         
      // }
     lv_timer_handler();
    delay(5);
}
//get Gpsdata
String extractGpsData(String data) {
  String gps = "";
  int gpsIndex = data.indexOf("gps"); // Tìm vị trí bắt đầu của chuỗi "gps"

  if (gpsIndex >= 0) {
    // Lấy phần dữ liệu sau "gps"
    gps = data.substring(gpsIndex + 4);  // Bỏ qua "gps" và lấy phần còn lại của chuỗi

    // Duyệt qua từng ký tự trong chuỗi GPS cho đến khi gặp ký tự không phải số hoặc dấu ';'
    String tempGps = "";
    for (int i = 0; i < gps.length(); i++) {
      char c = gps.charAt(i);
      if ((c >= '0' && c <= '9') || c == ';') {
        tempGps += c; // Nếu là số 0-9 hoặc dấu ';', thêm vào bộ nhớ tạm
      } else {
        break; // Nếu gặp ký tự khác -> thoát vòng lặp
      }
    }
    // if(String tempGps = ""){
    //   tempGps = "No GPS";
    // }
    gps = tempGps; // Lưu lại chuỗi GPS đã tách
      
    
  }

  return gps;
}
 // get signalData
// String extractsignaldata(String data) {
//   int signalIndex = data.indexOf("signal");

//   if (signalIndex >= 0) {
//     // Lấy phần sau "signal "
//     String sub = data.substring(signalIndex + 7); // Bỏ "signal "

//     int firstSep = sub.indexOf(';');
//     if (firstSep != -1) {
//       String status = sub.substring(0, firstSep);

//       // Nếu trạng thái là Active
//       if (status == "Active") {
//         int secondSep = sub.indexOf(';', firstSep + 1);
//         if (secondSep != -1) {
//           // Lấy chuỗi số sau dấu ';' thứ hai
//           String rawValue = sub.substring(secondSep + 1);
//           float dBm = rawValue.toFloat() / 100.0;

//           // Trả kết quả theo định dạng yêu cầu
//           return status + ", dBm: " + String(dBm, 2);
//         }
//       } else {
//         return "No signal";
//       }
//     }
//   }

//   return "no signal";
// }


bool extractSignalData(String data, String &status, float &dBm) {
  int signalIndex = data.indexOf("signal");

  if (signalIndex >= 0) {
    String sub = data.substring(signalIndex + 7); // Bỏ "signal "
    int firstSep = sub.indexOf(';');
    if (firstSep != -1) {
      status = sub.substring(0, firstSep);

      int secondSep = sub.indexOf(';', firstSep + 1);
      if (secondSep != -1) {
        String rawValue = sub.substring(secondSep + 1);
        dBm = rawValue.toFloat() / 100.0;
        return true;
      }
    }
  }

  status = "No signal";
  dBm = 0.0;
  return false;
}

// get outbox data

String extractDateTime(String data) {
  String dateTime = "";
  int tmpIndex = data.indexOf("outbox"); // Tìm vị trí của từ "outbox"

  if (tmpIndex >= 0) {
    // Lấy phần dữ liệu sau "outbox "
    String dateTimePart = data.substring(tmpIndex + 7);  // Bỏ qua "outbox " và lấy phần còn lại của chuỗi

    // Tìm dấu '#' và lấy phần trước nó (phần dữ liệu)
    int hashIndex = dateTimePart.indexOf('#');
    if (hashIndex >= 0) {
      // Nếu có dấu # → lấy đến trước #
      dateTime = dateTimePart.substring(0, hashIndex);
    }else {
      // Nếu không có dấu # → lấy đến trước dấu cách đầu tiên
      int spaceIndex = dateTimePart.indexOf(' ');
      if (spaceIndex >= 0) {
        dateTime = dateTimePart.substring(0, spaceIndex);
      } 
    }

  //   // Duyệt qua từng ký tự trong phần ngày giờ để giữ lại các ký tự hợp lệ
  //   String tempDateTime = "";
  //   for (int i = 0; i < dateTimePart.length(); i++) {
  //     char c = dateTimePart.charAt(i);  // Lấy ký tự tại vị trí i
  //     // Kiểm tra nếu ký tự là số hoặc dấu phân cách '/' và ':'
  //     if ((c >= '0' && c <= '9') || c == '/' || c == ':' || c == ' ') {
  //       tempDateTime += c;  // Nếu ký tự hợp lệ (số, '/', ':', hoặc dấu cách), thêm vào chuỗi tạm
  //     } else {
  //       break;  // Nếu gặp ký tự không hợp lệ, dừng lại
  //     }
  //   }
  //   dateTime = tempDateTime;  // Lưu lại chuỗi ngày giờ đã tách
  }
   
  return dateTime;  // Trả về dữ liệu ngày giờ đã tách
}
// chyển kinh độ, vỹ độ
String decimalToDMS(float decimal, bool isLat) {
  char direction = (isLat)
                     ? ((decimal >= 0) ? 'N' : 'S')
                     : ((decimal >= 0) ? 'E' : 'W');

  decimal = fabs(decimal);
  int degrees = int(decimal);
  float minutesDecimal = (decimal - degrees) * 60;
  int minutes = int(minutesDecimal);
  int seconds = int((minutesDecimal - minutes) * 60);

  char buffer[20];
  sprintf(buffer, "%d°%d'%d\" %c", degrees, minutes, seconds, direction);
  return String(buffer);
}

//Chuyển ngày giờ từ Uint time stamp
String epochToTime(uint32_t epoch) {
  // Nếu cần UTC+7, cộng thêm 7 * 3600
  time_t raw = epoch + 7 * 3600;

  char buffer[10];
  sprintf(buffer, "%02d:%02d:%02d", hour(raw), minute(raw), second(raw));
  return String(buffer);
}
//tach du lieu tu rawGPS
String parseGpsReadable(String rawData) {
  long values[6];
  int index = 0;

  while (index < 6 && rawData.length() > 0) {
    int sep = rawData.indexOf(';');
    String token;
    if (sep != -1) {
      token = rawData.substring(0, sep);
      rawData = rawData.substring(sep + 1);
    } else {
      token = rawData;
      rawData = "";
    }
    values[index++] = token.toInt();
  }

  // Ép kiểu chính xác
  float lon = values[1] / 60000.0; // vì ví dụ 1082136 → 108.2136
  float lat = values[0] / 60000.0;
  int velocity = values[2];
  int heading = values[3];
  uint32_t timestamp = values[4]; // có thể dùng trung bình giữa 4 và 5 nếu muốn chính xác hơn

  String latDMS = decimalToDMS(lat, true);
  String lonDMS = decimalToDMS(lon, false);
  String timeStr = epochToTime(timestamp);

  String result = latDMS + "\n" + lonDMS + "\n";
  //result += "Huong: " + String(heading) + "°  VT: " + String(velocity) + " ly\n";
  result += "Time: " + timeStr;

  return result;
}
//Đo điện áp pin
void DC(){
  gtcambien = analogRead(cambien);
  //Serial.print("Cảm biến: "); Serial.print(gtcambien); Serial.print("   ");
  
  vol_out = ((gtcambien * 3.3) / 4096.0); // Chuyển từ analog sang V; 0-4095(ADC 12bit) ~ 0-3.3v
  //Serial.print("V out: "); Serial.print(vol_out); Serial.print("   ");
  
  vol_in = float(vol_out / float(2300.0/float(8220.0 + 2300.0))); // Tính vol nguồn cần đo
  //Serial.print("V nguồn: "); Serial.println(vol_in);
  /*
   * Thevenin Norton
   * 
   * Vth = (Vcc x R2) / R1 + R2
   * Vout = (Vin x R2) / R1 + R2 => Vin = Vout / (R2/(R1+R2));
   */
   int voltage = batteryLevel(vol_in);
   lv_label_set_text(ui_Label10,(String(voltage)+"%" + String(vol_in)+"V" ).c_str());

}
int batteryLevel(float voltage) {
  // Tuỳ loại pin, bạn có thể điều chỉnh dải này.
  if (voltage >= 13.6) return 100;
  else if (voltage >= 13.40) return 99;
  else if (voltage >= 13.30) return 90;
  else if (voltage >= 13.20) return 70;
  else if (voltage >= 13.10) return 40;
  else if (voltage >= 13.00) return 30;
  else if (voltage >= 12.90) return 20;
  else if (voltage >= 12.80) return 17;
  else if (voltage >= 12.50) return 14;
  else if (voltage >= 12.00) return 9;
  else return 0;
}
