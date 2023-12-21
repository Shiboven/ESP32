/*
 * @Description: 
 * @Author: Shiboven
 * @Date: 2023-12-12 21:22:53
 * @LastEditTime: 2023-12-13 00:04:44
 * @LastEditors:  
 */
#include<Arduino.h>
#include<WiFi.h>                // 包含WiFi库
#include<PubSubClient.h>        // 包含PubSubClient库

const char*ssid ="Hide_2805";               //ESP32连接的WiFi账号
const char*password = "asdfghjkl";        //WiFi密码
const char*mqttServer = "t.yoyolife.fun";  //要连接到的服务器IP
const int mqttPort =1883;                 //要连接到的服务器端口号
const char*mqttUser = "75bdfb62a1c56065949702a3a6430e38";            //MQTT服务器账号
const char*mqttPassword = "123465";       //MQTT服务器密码
 
WiFiClient espClient;                     // 定义wifiClient实例
PubSubClient client(espClient);          // 定义PubSubClient的实例
 
void callback(char*topic, byte* payload, unsigned int length) 
{
    Serial.print("来自订阅的主题:");              //串口打印：来自订阅的主题:
    Serial.println(topic);                //串口打印订阅的主题
    Serial.print("信息：");              //串口打印：信息：
    for (int i = 0; i< length; i++)        //使用循环打印接收到的信息
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}
void setup() 
{
    Serial.begin(115200);                   //串口函数，波特率设置
    while (WiFi.status() != WL_CONNECTED)   //若WiFi接入成功WiFi.status()会返回 WL_CONNECTED
    {       
        Serial.println("连接wifi中");         //串口输出：连接wifi中
        WiFi.begin(ssid,password);          //接入WiFi函数（WiFi名称，密码）重新连接wif
        delay(2000);                         //若尚未连接WiFi，则进行重连WiFi的循环
    }
    Serial.println("wifi连接成功");             //连接wifi成功之后会跳出循环，串口并输出：wifi连接成功
    client.setServer(mqttServer,mqttPort);  //MQTT服务器连接函数（服务器IP，端口号）
    client.setCallback(callback);           //设定回调方式，当ESP32收到订阅消息时会调用此方法
    while (!client.connected())             //是否连接上MQTT服务器
    {
        Serial.println("连接服务器中");         //串口打印：连接服务器中
        if (client.connect("ESP32Client",mqttUser, mqttPassword ))  //如果服务器连接成功
        {
            Serial.println("服务器连接成功");          //串口打印：服务器连接成功
        }
        else 
        {
            Serial.print("连接服务器失败");            //串口打印：连接服务器失败
            Serial.print(client.state());       //重新连接函数
            delay(2000);                        
        }
    }
    client.subscribe("/iot/4432/wsy");                 //连接MQTT服务器后订阅主题
    Serial.print("已订阅主题，等待主题消息....");                  //串口打印：已订阅主题，等待主题消息
    client.publish("/iot/4432/waa","Hello from ESP32");//向服务器发送的信息(主题，内容)
}
 
void loop()   
{
  client.loop();                              //回旋接收函数  等待服务器返回的数据
}
