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
        Startup.appLinker=appLinker;
        utils.appLinker=appLinker;
        Startup.controllerSetup();

        Task.Run(async ()=>{
            while(true){
                appLinker[keys.SerialSend].value="manga";
                Console.WriteLine("Sent: manga!");
                await Task.Delay(10);
            }
        });

        
        while (Console.ReadKey().Key != ConsoleKey.Enter);
    }
}