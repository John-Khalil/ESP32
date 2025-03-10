﻿using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;
using EFCore;
using EFCore.Models;
using System.Dynamic;

class Program
{   
    public static string[] controllers=["COM5"];
    public static AppLinker appLinker=new AppLinker();
    static void Main(){
        Startup.appLinker=appLinker;
        utils.appLinker=appLinker;
        Startup.controllerSetup(controllers);
        Startup.webSocketServerStart();

        var bot=new Tbot(new{
            serialPortID=controllers[0],
            pickup=new Tbot.Point(50,50),
            placement=new Tbot.Point(50,10),
            feedRateD1=(uint)100,
            feedRateD2=(uint)100,
            vacuum=Pins.B12,
            roundCount=5,
            cycleComplete=new Action(()=>{

            })
        });

        appLinker[keys.ws.TbotParameters].setAction((dynamic data)=>{
            var parameters=JObject.Parse(data.message.ToString());
            // Console.WriteLine(JsonConvert.SerializeObject(parameters));
            bot.init(new{
                serialPortID=controllers[0],
                pickup=new Tbot.Point((int)parameters["Pickup D1"],(int)parameters["Pickup D2"]),
                placement=new Tbot.Point((int)parameters["Placement D1"],(int)parameters["Placement D2"]),
                feedRateD1=(uint)parameters["Feedrate D1"],
                feedRateD2=(uint)parameters["Feedrate D2"],
                vacuum=Pins.B12,
                roundCount=parameters["Round Count"],
                cycleComplete=new Action(()=>{

                })
            });
        });

        appLinker[keys.ws.TbotStart].setAction((object data)=>{

            bot.start();
        });



        // Console.WriteLine(JsonConvert.SerializeObject(new Dictionary<int,object>{[123]="value"}));
       



        // DB.Create(new Settings{
        //     Name="manga",
        // });

        // var entity=DB.GetById<Settings>(2);
        // entity.Name="new value";
        // DB.Update<Settings>(entity);

        // dynamic test=new{
        //     x=5
        // };

        // Console.WriteLine(JObject.FromObject(test).ContainsKey("y")?"found":"not found");

        // var list=DB.GetAll<Settings>();
        // foreach(var item in list)
        //     Console.WriteLine(JsonConvert.SerializeObject(item));

        
        // appLinker[keys.InputPullUp].value=new{
        //     port=controllers[0],
        //     value=Pins.B12
        // };
        // appLinker[keys.InputPullUp].value=new{
        //     port=controllers[0],
        //     value=Pins.B11
        // };
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
        //         Console.ForegroundColor=ConsoleColor.Red;
        //         // Console.WriteLine($">> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}]-{(int)utils.analogRaed(controllers[0],Pins.A0)}");
        //         Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.readPin(controllers[0],Pins.B12)}");
        //         // await Task.Delay(1);
        //     }
        // });
        // Task.Run(async ()=>{
        //     while(true){
        //         // appLinker[keys.SerialSend].value="manga";
        //         Console.ForegroundColor=ConsoleColor.Green;
        //         // Console.WriteLine($">> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}]-{(int)utils.analogRaed(controllers[0],Pins.A7)}");
        //         Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.readPin(controllers[0],Pins.B12)}");
        //         // await Task.Delay(1);
        //     }
        // });
        // Task.Run(async ()=>{
        //     while(true){
        //         // appLinker[keys.SerialSend].value="manga";
        //         Console.ForegroundColor=ConsoleColor.Blue;
        //         // Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.analogRaed(controllers[0],Pins.A0)}");
        //         Console.WriteLine($">> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {(int)utils.readPin(controllers[0],Pins.B12)}");
        //         // await Task.Delay(1);
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
        // // });

        
        // Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.tbotPosition(controllers[0])}");

        // appLinker[keys.TbotMove].value=new{
        //     port=controllers[0],
        //     value=new{
        //         d1 = 800,
        //         d2 = 500,
        //         feedRate = 100,
        //     }
        // };


        // Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.tbotPosition(controllers[0])}");

        // appLinker[keys.TbotMove].value=new{
        //     port=controllers[0],
        //     value=new{
        //         d1 = 500,
        //         d2 = -700,
        //         feedRate = 1000,
        //     }
        // };


        // Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.tbotPosition(controllers[0])}");

        
        while (Console.ReadKey().Key != ConsoleKey.Enter);
    }
}