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
        Startup.controllerSetup();

        
        // Create serial port instance
        
    }
}

// string json = @"{ ""Name"": ""Alice"", ""Age"": 30, ""City"": ""Wonderland"" }";

// // Parse JSON into a JObject
// JObject jObject = JObject.Parse(json);

// // Loop through each property in the JObject
// foreach (var property in jObject.Properties())
// {
//     Console.WriteLine($"Key: {property.Name}, Value: {property.Value}");
// }