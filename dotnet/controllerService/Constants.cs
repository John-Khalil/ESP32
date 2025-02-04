using System.Drawing;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Dynamic;

namespace Constants
{
    public static class Registers
    {
        // Base addresses
        public const int OUTPUT_REGISTERS_BASE = 0x100;
        public const int INPUT_REGISTERS_BASE = 0x200;
        public const int READ_REGISTERS_BASE = 0x400;

        // Digital I/O
        public static readonly int DIGITAL_INPUT = OUTPUT_REGISTERS_BASE + 0x01;
        public static readonly int DIGITAL_INPUT_PULLUP = OUTPUT_REGISTERS_BASE + 0x02;
        public static readonly int DIGITAL_INPUT_PULLDOWN = OUTPUT_REGISTERS_BASE + 0x03;
        public static readonly int DIGITAL_OUTPUT = OUTPUT_REGISTERS_BASE + 0x04;

        // Other registers
        public static readonly int DW1 = OUTPUT_REGISTERS_BASE + 0x05;
        public static readonly int DW0 = OUTPUT_REGISTERS_BASE + 0x06;
        public static readonly int DR = OUTPUT_REGISTERS_BASE + 0x07;
        public static readonly int TBOT_MOVE = OUTPUT_REGISTERS_BASE + 0x08;
        public static readonly int TBOT_STEPS_PER_UNIT = OUTPUT_REGISTERS_BASE + 0x09;
        public static readonly int TBOT_SETUP = OUTPUT_REGISTERS_BASE + 0x0A;
        public static readonly int CLOCK = OUTPUT_REGISTERS_BASE + 0x0B;
        public static readonly int ANALOG_READ = OUTPUT_REGISTERS_BASE + 0x0C;
        public static readonly int TBOT_POS = OUTPUT_REGISTERS_BASE+0x0E;
        public static readonly int PORTA =1;
        public static readonly int PORTB =2;
        public static readonly int PORTC =3;
    }

    public static class Pins
    {
        // Pin mappings
        public const int A0 = 192;
        public const int A1 = 193;
        public const int A2 = 194;
        public const int A3 = 195;
        public const int A4 = 196;
        public const int A5 = 197;
        public const int A6 = 198;
        public const int A7 = 199;
        public const int A8 = 12;
        public const int A9 = 11;
        public const int A10 = 10;
        public const int A11 = 9;
        public const int A12 = 8;
        public const int A13 = 33;
        public const int A14 = 34;
        public const int A15 = 7;
        public const int B0 = 200;
        public const int B1 = 201;
        public const int B2 = 32;
        public const int B3 = 6;
        public const int B4 = 5;
        public const int B5 = 4;
        public const int B6 = 3;
        public const int B7 = 2;
        public const int B8 = 1;
        public const int B9 = 0;
        public const int B10 = 30;
        public const int B11 = 31;
        public const int B12 = 16;
        public const int B13 = 15;
        public const int B14 = 14;
        public const int B15 = 13;
        public const int C13 = 17;
        public const int C14 = 18;
        public const int C15 = 19;
    }

    public static class keys{
        public static string SerialSend{get;set;}
        public static string Input{get;set;}
        public static string InputPullUp{get;set;}
        public static string InputPullDown{get;set;}
        public static string Output{get;set;}
        public static string SetPin{get;set;}
        public static string ClearPin{get;set;}
        public static string ReadPin{get;set;}
        public static string Clock{get;set;}
        public static string AnalogRead{get;set;}

        public static string WebSocket{get;set;}
        public static string WebSocketBroadCast{get;set;}
        public static string WebSocketClients{get;set;}
        
        public static string TbotMove{get;set;}
        public static string TbotPosition{get;set;}

        public static class ws{
            public static string TbotPosition{get;set;}
            public static string TbotManualControl{get;set;}
    
            static ws(){
                var properties = typeof(ws).GetProperties();
                foreach (var property in properties){
                    property.SetValue(null,$"ws-{property.Name}");
                    // Console.WriteLine(property.GetValue(null).ToString());
                    utils.appLinker[property.GetValue(null).ToString()].setAction((dynamic data)=>{
                        dynamic expando = new ExpandoObject();
                        var expandoDict = (IDictionary<string, object>)expando;

                        expando.clientId = data?.clientId;
                        expandoDict[property?.GetValue(null)?.ToString()] = data?.message;
                        utils.appLinker[keys.WebSocket].value = JsonConvert.SerializeObject(expando);
                    });
                }
            }
        }

        static keys(){
            var properties = typeof(keys).GetProperties();
            foreach (var property in properties){
                property.SetValue(null,property.Name);
            }
        }

    }

    public static class utils{
        public static AppLinker appLinker{get;set;}
        public static WebSocketServer webSocket{get;set;}
        public static dynamic controllerRead(string key,object value,string portID){
            appLinker[key].value=new{
                port=portID,
                value,
            };
            return AppLinker.resolve(appLinker[$"{value.ToString()}-{portID}"]);
        }

        public static dynamic readPin(string portID,uint value){
            return controllerRead(keys.ReadPin,Registers.INPUT_REGISTERS_BASE|value,portID);
        }
        public static dynamic analogRaed(string portID,uint value){
            return controllerRead(keys.AnalogRead,Registers.INPUT_REGISTERS_BASE|value,portID);
        }
        public static dynamic tbotPosition(string portID){
            return controllerRead(keys.TbotPosition,Registers.READ_REGISTERS_BASE|Registers.TBOT_POS,portID);
        }
    }
}
