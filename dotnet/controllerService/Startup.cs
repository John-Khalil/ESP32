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
        
        serialPort.PortName = "COM31";
        serialPort.BaudRate = 921600;
        serialPort.DtrEnable = true;
        serialPort.RtsEnable = true;


        var serialSerialize=(int key,string value)=>{
            serialPort.WriteLine( $"{"{\""}{key}{"\":"}{value}{"}"}");
            // AppLinker.resolve(appLinker["ack"]);
            Thread.Sleep(10);
        };

        appLinker[keys.SerialSend].setAction((object data)=>{
            serialPort.WriteLine(data.ToString());
        });
        appLinker[keys.Input].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_INPUT,data.ToString());
        });
        appLinker[keys.InputPullUp].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_INPUT_PULLUP,data.ToString());
        });
        appLinker[keys.InputPullDown].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_INPUT_PULLDOWN,data.ToString());
        });
        appLinker[keys.Output].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_OUTPUT,data.ToString());
        });
        appLinker[keys.SetPin].setAction((object data)=>{
            serialSerialize(Registers.DW1,data.ToString());
        });
        appLinker[keys.ClearPin].setAction((object data)=>{
            serialSerialize(Registers.DW0,data.ToString());
        });
        appLinker[keys.ReadPin].setAction((object data)=>{
            serialSerialize(Registers.DR,data.ToString());
        });
        appLinker[keys.Clock].setAction((object data)=>{
            serialSerialize(Registers.CLOCK,data.ToString());
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
                    // Console.WriteLine($"Received: {response}");
                    if(IsValidJson(response)){
                        JObject jObject = JObject.Parse(response);
                        foreach (var property in jObject.Properties()){
                            // Console.WriteLine($"Key: {property.Name}, Value: {property.Value}");
                            appLinker[property.Name].value=property.Value;
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