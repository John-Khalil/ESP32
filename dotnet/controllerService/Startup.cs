using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;

public static class Startup
{
    public static void controllerSetup(){
        SerialPort serialPort = new SerialPort();
        
        // Basic configuration
        serialPort.PortName = "COM30";  // Change this to your port name
        serialPort.BaudRate = 921600;
        serialPort.DtrEnable = true;
        serialPort.RtsEnable = true; 

        
        try
        {
            // Open the port
            serialPort.Open();
            Console.WriteLine("Port opened successfully!");

            // Send some data

            Task.Run(async ()=>{
                while(true){
                    serialPort.Write("manga!\n");
                    Console.WriteLine("Sent: manga!");
                    await Task.Delay(10);
                }
            });

            Task.Run(()=>{
                while(true){
                    string response = serialPort.ReadLine();
                    Console.WriteLine($"Received: {response}");
                    // JObject jObject = JObject.Parse(response);
                    // foreach (var property in jObject.Properties())
                    // {
                    //     Console.WriteLine($"Key: {property.Name}, Value: {property.Value}");
                    // }
                }
            });

            // Close the port
            while (Console.ReadKey().Key != ConsoleKey.Enter);
            serialPort.Close();
            Console.WriteLine("Port closed!");
            return;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
}