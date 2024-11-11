using System;
using System.IO.Ports;

class Program
{
    static void Main()
    {
        SerialPort serialPort = new SerialPort("COM14", 9600, Parity.None, 8, StopBits.One);

        try
        {
            serialPort.Open();
            Console.WriteLine("Serial Port Opened");

            // Write to the serial port
            serialPort.WriteLine("Hello, Serial Port!");

            // Read from the serial port
            string response = serialPort.ReadLine();
            Console.WriteLine("Received: " + response);
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error: " + ex.Message);
        }
        finally
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                Console.WriteLine("Serial Port Closed");
            }
        }
    }
}
