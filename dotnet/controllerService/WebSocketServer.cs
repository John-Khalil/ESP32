using System;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Concurrent;

class WebSocketServer
{
    private HttpListener httpListener;
    private CancellationTokenSource cancellationTokenSource;
    private ConcurrentDictionary<Guid, WebSocket> clients;

    public WebSocketServer(string url)
    {
        httpListener = new HttpListener();
        httpListener.Prefixes.Add(url);
        cancellationTokenSource = new CancellationTokenSource();
        clients = new ConcurrentDictionary<Guid, WebSocket>();
    }

    public async Task StartAsync()
    {
        httpListener.Start();
        Console.WriteLine($"WebSocket server started. Listening on {string.Join(", ", httpListener.Prefixes)}");

        while (!cancellationTokenSource.Token.IsCancellationRequested)
        {
            try
            {
                var context = await httpListener.GetContextAsync();
                if (context.Request.IsWebSocketRequest)
                {
                    // Handle each new connection in a separate task
                    _ = ProcessWebSocketRequest(context);
                }
                else
                {
                    context.Response.StatusCode = 400;
                    context.Response.Close();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }
    }

    private async Task ProcessWebSocketRequest(HttpListenerContext context)
    {
        try
        {
            HttpListenerWebSocketContext webSocketContext = await context.AcceptWebSocketAsync(null);
            WebSocket webSocket = webSocketContext.WebSocket;

            var clientId = Guid.NewGuid();
            clients.TryAdd(clientId, webSocket);

            Console.WriteLine($"Client connected. Total clients: {clients.Count}");

            await HandleWebSocketConnection(clientId, webSocket);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"WebSocket error: {ex.Message}");
            context.Response.StatusCode = 500;
            context.Response.Close();
        }
    }

    private async Task HandleWebSocketConnection(Guid clientId, WebSocket webSocket)
    {
        var buffer = new byte[1024 * 4];

        try
        {
            while (webSocket.State == WebSocketState.Open)
            {
                var result = await webSocket.ReceiveAsync(
                    new ArraySegment<byte>(buffer),
                    cancellationTokenSource.Token);

                if (result.MessageType == WebSocketMessageType.Text)
                {
                    string message = Encoding.UTF8.GetString(buffer, 0, result.Count);
                    Console.WriteLine($"Received from client {clientId}: {message}");

                    // Broadcast message to all clients
                    await BroadcastMessage($"Client {clientId}: {message}");
                }
                else if (result.MessageType == WebSocketMessageType.Close)
                {
                    await DisconnectClient(clientId);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error handling WebSocket connection: {ex.Message}");
        }
        finally
        {
            await DisconnectClient(clientId);
        }
    }

    private async Task BroadcastMessage(string message)
    {
        var tasks = new List<Task>();
        var disconnectedClients = new List<Guid>();
        var messageBytes = Encoding.UTF8.GetBytes(message);

        foreach (var client in clients)
        {
            if (client.Value.State == WebSocketState.Open)
            {
                try
                {
                    tasks.Add(client.Value.SendAsync(
                        new ArraySegment<byte>(messageBytes),
                        WebSocketMessageType.Text,
                        true,
                        cancellationTokenSource.Token));
                }
                catch
                {
                    disconnectedClients.Add(client.Key);
                }
            }
            else
            {
                disconnectedClients.Add(client.Key);
            }
        }

        // Clean up disconnected clients
        foreach (var clientId in disconnectedClients)
        {
            await DisconnectClient(clientId);
        }

        await Task.WhenAll(tasks);
    }

    private async Task DisconnectClient(Guid clientId)
    {
        if (clients.TryRemove(clientId, out WebSocket webSocket))
        {
            try
            {
                if (webSocket.State == WebSocketState.Open)
                {
                    await webSocket.CloseAsync(
                        WebSocketCloseStatus.NormalClosure,
                        "Closing",
                        cancellationTokenSource.Token);
                }
                webSocket.Dispose();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error disconnecting client {clientId}: {ex.Message}");
            }
            Console.WriteLine($"Client {clientId} disconnected. Total clients: {clients.Count}");
        }
    }

    public async Task SendToClient(Guid clientId, string message)
    {
        if (clients.TryGetValue(clientId, out WebSocket webSocket))
        {
            var messageBytes = Encoding.UTF8.GetBytes(message);
            await webSocket.SendAsync(
                new ArraySegment<byte>(messageBytes),
                WebSocketMessageType.Text,
                true,
                cancellationTokenSource.Token);
        }
    }

    public async Task Stop()
    {
        // Disconnect all clients
        var disconnectTasks = clients.Keys.Select(clientId => DisconnectClient(clientId));
        await Task.WhenAll(disconnectTasks);

        cancellationTokenSource.Cancel();
        httpListener.Stop();
        httpListener.Close();
    }
}