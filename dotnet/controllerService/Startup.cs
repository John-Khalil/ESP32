using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;

public static class Startup
{
    public static AppLinker appLinker{get;set;}
    public static void controllerSetup(){

        SerialPort serialPort = new SerialPort();
        
        serialPort.PortName = "COM30";
        serialPort.BaudRate = 921600;
        serialPort.DtrEnable = true;
        serialPort.RtsEnable = true;

        appLinker[keys.SerialSend].setAction((object data)=>{
            serialPort.WriteLine(data.ToString());
        });


        var IsValidJson=(string input)=>{
            if (string.IsNullOrWhiteSpace(input))
                return false;
            try{
                var obj = JsonConvert.DeserializeObject<object>(input);
                return true;
            }
            catch (JsonException){
                return false;
            }
        };

        try{
            serialPort.Open();
            Task.Run(()=>{
                while(true){
                    string response = serialPort.ReadLine();
                    Console.WriteLine($"Received: {response}");
                    if(IsValidJson(response)){
                        JObject jObject = JObject.Parse(response);
                        foreach (var property in jObject.Properties()){
                            Console.WriteLine($"Key: {property.Name}, Value: {property.Value}");

                        }
                    }
                }
            });
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
}