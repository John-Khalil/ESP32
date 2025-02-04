'use client';

// const { default: appLinker } = require("./utils");
import appLinker from "./utils";
export default  function setupWebSocket(){

  
  const socket = new WebSocket('ws://localhost:8080');
  
  socket.addEventListener('open', (event) => {
    console.log('Connected to WebSocket server');
  });

  socket.addEventListener('message', (event) => {
    // console.log('Message from server:', event.data);
    Object.keys(event.data).forEach(key=>{
      appLinker.send(key,event?.data?.[key]);
    });
  });

  socket.addEventListener('error', (event) => {
    console.error('WebSocket error:', event);
  });

  socket.addEventListener('close', (event) => {
    console.log('Disconnected from WebSocket server');
    
      if (event.wasClean) {
        console.log(`Closed cleanly, code=${event.code}, reason=${event.reason}`);
      } else {
        console.log('Connection died');
      }
    });
    
    // function sendMessage(message) {
      //     if (socket.readyState === WebSocket.OPEN) {
        //         socket.send(message);
        //     } else {
          //         console.log('Connection not open');
  //     }
  // }

  // function closeConnection() {
    //     socket.close(1000, 'Closing connection normally');
    // }
    
  appLinker.addListener("ws",data=>{
    if (socket.readyState === WebSocket.OPEN) {
      // console.log("------------>",data);
      socket.send(data);
    } else {
      console.log('Connection not open');
    }
  })
}