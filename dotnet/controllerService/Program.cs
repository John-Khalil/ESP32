using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;

class Program
{
    public static AppLinker appLinker=new AppLinker();
    static void Main(){

        // var data = new
        // {
        //     reg = "value1",
        //     mask = "value2",
        //     delay = 100,
        //     clks = 1000
        // };

        // string jsonString = JsonConvert.SerializeObject(new{
        //     reg = "value1",
        //     mask = "value2",
        //     delay = 100,
        //     clks = 1000
        // });

        Startup.appLinker=appLinker;
        utils.appLinker=appLinker;
        Startup.controllerSetup();

        // appLinker[keys.InputPullDown].value=Pins.B12;
        appLinker[keys.Output].value=Pins.C13;
        appLinker[keys.Output].value=Pins.B13;
        appLinker[keys.Output].value=Pins.B14;
        appLinker[keys.Output].value=Pins.B12;

        appLinker[keys.ClearPin].value=Pins.B13;
        appLinker[keys.SetPin].value=Pins.B12;

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
        //         Console.WriteLine($" >> [{DateTime.Now.Hour:D2}:{DateTime.Now.Minute:D2}:{DateTime.Now.Second:D2}] {utils.readPin(Pins.B12)}");
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

         appLinker[keys.Clock].value=JsonConvert.SerializeObject(new{
            reg = Registers.PORTB,
            mask = 1<<13,
            delay = 100,
            clks = 10000
        });

        
        while (Console.ReadKey().Key != ConsoleKey.Enter);
    }
}