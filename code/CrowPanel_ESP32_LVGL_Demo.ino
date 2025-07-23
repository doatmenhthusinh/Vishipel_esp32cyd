#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
//#include <Adafruit_Sensor.h>

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
bool isDataReady = false;  // Flag để kiểm tra xem có dữ liệu mới từ Serial hay không

void Datatimer(lv_timer_t * timer){
  if (Serial.available()) {
    // Đọc tất cả dữ liệu có sẵn từ serial vào một chuỗi
    receivedText = Serial.readString();  // Đọc toàn bộ chuỗi dữ liệu
    //  String tempReceivedText = "";
    // while (Serial.available()) {
    //   char c = Serial.read();
    //   tempReceivedText += c;
    // }
    //  receivedText = tempReceivedText;  // Lưu lại chuỗi nhận được

    if (receivedText.indexOf("gps") >= 0) {
      // Tách dữ liệu GPS từ chuỗi nhận được
      gpsData = extractGpsData(receivedText);
      
      // Hiển thị dữ liệu GPS nếu có
      if (gpsData.length() > 0) {

        Serial.println("gpsData:");
        //Serial.println(gpsData);
        gps1 = gpsData.c_str();
        lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_obj_set_width(ui_Label4, 320); // Chỉnh theo kích thước LCD
        lv_label_set_long_mode(ui_Label4, LV_LABEL_LONG_WRAP); // giới hạn text
        lv_label_set_text(ui_Label4, gps1);  // Cập nhật giá trị vào Label
        Serial.println(gps1);


      } else {
        Serial.println("GPS not true");
        lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label4, "GPS False");  // Cập nhật giá trị vào Label

      }

    }else {
      Serial.println("No GPS Data");
      lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);// căn giữa dòng text
      lv_label_set_text(ui_Label4, "No GPS Data");  // Cập nhật giá trị vào Label

      }
       //##########################
              // Kiểm tra xem chuỗi có chứa từ "signal" hay không
    if (receivedText.indexOf("signal") >= 0) {
      // Tách dữ liệu signal từ chuỗi nhận được
      signalData = extractsignaldata(receivedText);
      // Hiển thị dữ liệu GPS nếu có
      if (signalData.length() > 0) {
        Serial.println("Signal:");
        Serial.println(signalData);
        signal1 = signalData.c_str();
        //lv_obj_set_width(ui_Label5, 320); // Chỉnh theo kích thước LCD
        //lv_label_set_long_mode(ui_Label5, LV_LABEL_LONG_WRAP); // giới hạn text
        lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label5, signal1);  // Cập nhật giá trị vào Label
        Serial.println(signal1);
      } else {
        Serial.println("Sinal not true");
        lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label5, "Singal False");  // Cập nhật giá trị vào Label
      }


    }else {
      // Nếu không có dữ liệu singal, in thông báo
        Serial.println("No Signal Data");
        lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label5, "No Signal Data");  // Cập nhật giá trị vào Label
      }

      //############################
      //#########################

      if (receivedText.indexOf("tmp") >= 0) {
      // Tách dữ liệu signal từ chuỗi nhận được
      timeData = extractDateTime(receivedText);
      // Hiển thị dữ liệu GPS nếu có
      if (timeData.length() > 0) {
        Serial.println("Date:");
        //Serial2.println(timeData);
        date1 = timeData.c_str();
        //lv_obj_set_width(ui_Label7, 320); // Chỉnh theo kích thước LCD
       // lv_label_set_long_mode(ui_Label7, LV_LABEL_LONG_WRAP); // giới hạn text
        lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label7, date1);  // Cập nhật giá trị vào Label
        Serial.println(date1);
      } else {
        Serial.println("Date not true");
        lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
        lv_label_set_text(ui_Label7, "Date False");  // Cập nhật giá trị vào Label
      }


    }else {
      Serial.println("No time Data");
      lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);// căn giữa dòng text
      lv_label_set_text(ui_Label7, "No time Data");  // Cập nhật giá trị vào Label
      }
      
      //receivedText = "";  // Làm sạch chuỗi sau mỗi lần xử lý 
    }
  }


void setup() {
    Serial.begin(9600);

    lv_init();
    
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
    //     Datatimer(NULL);  // Gọi hàm xử lý dữ liệu ngay khi có dữ liệu mới
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
String extractsignaldata(String data){
  String signal = "";
  int signalIndex = data.indexOf("signal");

    if (signalIndex >= 0) {
    // Lấy phần dữ liệu sau "gps"
    signal = data.substring(signalIndex + 7);  // Bỏ qua "signal " và lấy phần còn lại của chuỗi

    // Duyệt qua từng ký tự trong chuỗi GPS cho đến khi gặp ký tự không phải số hoặc dấu ';'
    String tempSignal = "";
    for (int i = 0; i < signal.length(); i++) {
      char c = signal.charAt(i);
      if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ';') {
        tempSignal += c; // Nếu là số 0-9 hoặc dấu ';' hoặc chữ cái thêm vào bộ nhớ tạm
      } else {
        break; // Nếu gặp ký tự khác ->thoát vòng lặp
      }
    }
    signal = tempSignal; // Lưu lại chuỗi GPS đã tách
  }

  return signal;
}
// get time data

String extractDateTime(String data) {
  String dateTime = "";
  int tmpIndex = data.indexOf("tmp"); // Tìm vị trí của từ "tmp"

  if (tmpIndex >= 0) {
    // Lấy phần dữ liệu sau "tmp"
    String dateTimePart = data.substring(tmpIndex + 7);  // Bỏ qua "tmp;98;" và lấy phần còn lại của chuỗi

    // Tìm dấu '#' và lấy phần trước nó (phần ngày giờ)
    int hashIndex = dateTimePart.indexOf('#');
    if (hashIndex >= 0) {
      dateTimePart = dateTimePart.substring(0, hashIndex);  // Lấy phần trước dấu '#'
    }

    // Duyệt qua từng ký tự trong phần ngày giờ để giữ lại các ký tự hợp lệ
    String tempDateTime = "";
    for (int i = 0; i < dateTimePart.length(); i++) {
      char c = dateTimePart.charAt(i);  // Lấy ký tự tại vị trí i
      // Kiểm tra nếu ký tự là số hoặc dấu phân cách '/' và ':'
      if ((c >= '0' && c <= '9') || c == '/' || c == ':' || c == ' ') {
        tempDateTime += c;  // Nếu ký tự hợp lệ (số, '/', ':', hoặc dấu cách), thêm vào chuỗi tạm
      } else {
        break;  // Nếu gặp ký tự không hợp lệ, dừng lại
      }
    }
    dateTime = tempDateTime;  // Lưu lại chuỗi ngày giờ đã tách
  }

  return dateTime;  // Trả về dữ liệu ngày giờ đã tách
}
