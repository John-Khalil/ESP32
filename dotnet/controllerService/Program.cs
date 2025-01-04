using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;
using EFCore;
using EFCore.Models;

class Program
{
    public static AppLinker appLinker=new AppLinker();
    static void Main(){
        Startup.appLinker=appLinker;
        utils.appLinker=appLinker;
        Startup.controllerSetup(["COM31"]);
        Startup.webSocketServerStart();



        // DB.Create(new Settings{
        //     Name="manga",
        // });

        // var entity=DB.GetById<Settings>(2);
        // entity.Name="new value";
        // DB.Update<Settings>(entity);

        dynamic test=new{
            x=5
        };

        Console.WriteLine(JObject.FromObject(test).ContainsKey("y")?"found":"not found");

        var list=DB.GetAll<Settings>();
        foreach(var item in list)
            Console.WriteLine(JsonConvert.SerializeObject(item));

        utils.appLinker[keys.WebSocket].setAction(async (object data)=>{
            Console.WriteLine($" --> {data.ToString()}");
            var dataObj=JObject.Parse(data?.ToString()??"{}");
			var clientID = dataObj["clientId"]?.ToString();
            await utils.webSocket.SendToClient(Guid.Parse(clientID??""),"this is test");
        });

        // appLinker[keys.InputPullDown].value=Pins.B12;
        // appLinker[keys.Output].value=Pins.C13;
        // appLinker[keys.Output].value=Pins.B13;
        // appLinker[keys.Output].value=Pins.B14;
        // appLinker[keys.Output].value=Pins.B12;

        // appLinker[keys.ClearPin].value=Pins.B14;
        // appLinker[keys.ClearPin].value=Pins.B13;

        // appLinker[keys.Clock].value=JsonConvert.SerializeObject(new{
        //     reg = Registers.PORTB,
        //     mask = 1<<14,
        //     delay = 100,
        //     clks = 1000
        // });

        // Console.WriteLine(JsonConvert.SerializeObject(new{
        //     reg = Registers.PORTB,
        //     mask = 1<<14,
        //     delay = 100,
        //     clks = 1000
        // }));

        // appLinker[keys.Output].value=Pins.C13;

        // Task.Run(async ()=>{
        //     while(true){
        //         // appLinker[keys.SerialSend].value="manga";
        //         Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.analogRaed(Pins.A0)}");
        //         await Task.Delay(10);
        //     }
        // });
        // Task.Run(async ()=>{
        //     while(true){
        //         // appLinker[keys.SerialSend].value="manga";
        //         // Console.WriteLine($" >> {utils.readPin(Pins.B12)}");
        //         appLinker[keys.SetPin].value=Pins.B14;
        //         await Task.Delay(100);
        //         appLinker[keys.ClearPin].value=Pins.B14;
        //         await Task.Delay(100);
        //     }
        // });

        //  Task.Run(async ()=>{
        //     while(true){
        //         // appLinker[keys.SerialSend].value="manga";
        //         // Console.WriteLine($" >> {utils.readPin(Pins.B12)}");
        //         appLinker[keys.SetPin].value=Pins.C13;
        //         await Task.Delay(100);
        //         appLinker[keys.ClearPin].value=Pins.C13;
        //         await Task.Delay(100);
        //     }
        // });

        //  appLinker[keys.Clock].value=JsonConvert.SerializeObject(new{
        //     reg = Registers.PORTB,
        //     mask = 1<<12,
        //     delay = 100,
        //     clks = 10000
        // });

        appLinker[keys.TbotMove].value=JsonConvert.SerializeObject(new{
            d1 = 800,
            // d2 = 500,
            feedRate = 100,
        });

        
        while (Console.ReadKey().Key != ConsoleKey.Enter);
    }
}