import React, { useRef } from 'react'

export default function HomePage() {
  const distanceSensorRef=useRef(null);

  const displayTextRef=useRef(null);

  const socket = new WebSocket(`ws://192.168.1.12:80/`);

  socket.addEventListener('open',()=>{
    console.log("CONNECTED");
  });

  socket.addEventListener('message',(event)=>{
    distanceSensorRef.current.innerHTML=`Distance ${JSON.parse(event.data).distance} CM`;
  });

  socket.addEventListener('close',()=>{

  });

  socket.addEventListener('error',()=>{

  });


  return (
    <>
      <input className='rounded-md p-1 m-1 text-black' type="text" ref={displayTextRef}/>
      <br />

      <button className='bg-green-500 rounded-md p-1 m-1' onClick={()=>{
        socket.send(JSON.stringify({
          msg:displayTextRef.current.value
        }));
      }}>SUBMIT</button>

      <div ref={distanceSensorRef}>

      </div>
    </>
  )
}
