using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;
using System.Dynamic;

public static class Startup
{
    public static AppLinker appLinker{get;set;}


    public static void controllerSetup(string[] SerialPorts){
         
        var serialPortList=new List<SerialPort>();

        var serialPortResolve=(dynamic data)=>{
            return JObject.FromObject(data).ContainsKey("port")?(serialPortList.Where(p=>p?.PortName==data?.port).FirstOrDefault()??serialPortList[0]):serialPortList[0];
        };

        var serialSerialize=(int key,dynamic data)=>{
            if(!serialPortList.Any())
                return;
            try{
                var value=JObject.FromObject(data).ContainsKey("value")?data?.value:data;
                serialPortResolve(data).WriteLine(JsonConvert.SerializeObject(new Dictionary<int,object>{[key]=value}));
                // Console.WriteLine(JsonConvert.SerializeObject(new Dictionary<int,object>{[key]=value}));
                // AppLinker.resolve(appLinker["ack"]);
                // Thread.Sleep(10);
                // Task.Delay(10);
            }
            catch (Exception ex){
                Console.WriteLine($"Error : {ex.Message}");
            }
        };

        appLinker[keys.SerialSend].setAction((object data)=>{
            // serialPort.WriteLine(data.ToString());
        });
        appLinker[keys.Input].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_INPUT,data);
        });
        appLinker[keys.InputPullUp].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_INPUT_PULLUP,data);
        });
        appLinker[keys.InputPullDown].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_INPUT_PULLDOWN,data);
        });
        appLinker[keys.Output].setAction((object data)=>{
            serialSerialize(Registers.DIGITAL_OUTPUT,data);
        });
        appLinker[keys.SetPin].setAction((object data)=>{
            serialSerialize(Registers.DW1,data);
        });
        appLinker[keys.ClearPin].setAction((object data)=>{
            serialSerialize(Registers.DW0,data);
        });
        appLinker[keys.ReadPin].setAction((object data)=>{
            serialSerialize(Registers.DR,data);
        });
        appLinker[keys.Clock].setAction((object data)=>{
            serialSerialize(Registers.CLOCK,data);
        });
        appLinker[keys.AnalogRead].setAction((object data)=>{
            serialSerialize(Registers.ANALOG_READ,data);
        });
        appLinker[keys.TbotMove].setAction((object data)=>{
            Console.WriteLine(JsonConvert.SerializeObject(data));
            serialSerialize(Registers.TBOT_MOVE,data);
        });
        appLinker[keys.TbotPosition].setAction((object data)=>{
            serialSerialize(Registers.TBOT_POS,data);
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

        foreach (string port in SerialPorts){
            

            SerialPort serialPort = new SerialPort();
            
            serialPort.PortName = port;
            serialPort.BaudRate = 921600;
            serialPort.DtrEnable = true;
            serialPort.RtsEnable = true;

            try{
                serialPort.Open();
                Task.Run(async ()=>{
                    while(true){
                        string response = serialPort.ReadLine();
                        // Console.WriteLine($"Received: {response}");
                        if(IsValidJson(response)){
                            JObject jObject = JObject.Parse(response);
                            foreach (var property in jObject.Properties()){
                                // Console.WriteLine($"Key: {property.Name}, Value: {property.Value}");
                                // Console.Write("-");
                                await Task.Delay(1);

                                appLinker[property.Name].value=property.Value;                              // set general property
                                appLinker[$"{property.Name}-{serialPort.PortName}"].value=property.Value;   // tie it to a specific comm port

                                // await Task.Delay(20);
                            }
                        }
                    }
                });
                serialPortList.Add(serialPort);
            }
            catch (Exception ex){
                Console.WriteLine($"Error: {ex.Message} @ {port}");
            }
        }
    }

    public static void WebSocketRegisters(){
        utils.appLinker[keys.WebSocket].setAction(async (object data)=>{
            var dataObj=JObject.Parse(data?.ToString()??"{}");
			string clientId = dataObj["clientId"]?.ToString();
            dataObj.Remove("clientId");
            // Console.WriteLine($" --> {JsonConvert.SerializeObject(dataObj)}");
            await utils.webSocket.SendToClient(Guid.Parse(clientId??""),JsonConvert.SerializeObject(dataObj));
        });


        // Console.WriteLine(keys.ws.TbotPosition);
        // appLinker[keys.ws.TbotPosition].value=(new{
        //     clientId="c694800b-5023-4150-863c-09ea05c4a9c2",
        //     message="TbotPosition"
        // });
        
        appLinker[keys.ws.TbotManualControl].setAction((dynamic data)=>{
            Console.WriteLine(JsonConvert.SerializeObject(data));
            appLinker[keys.ws.TbotPosition].value=(new{
                clientId=data?.clientId,
                message="TbotPosition"
            });
        
        });
    }

    public static void webSocketServerStart(){
        var server = new WebSocketServer("http://localhost:8080/");
        utils.webSocket=server;
        utils.appLinker[keys.WebSocketBroadCast].setAction(async (object data)=>{
            await server.BroadcastMessage(JsonConvert.SerializeObject(data));
        });
        _ = server.StartAsync();
        WebSocketRegisters();
    }
}