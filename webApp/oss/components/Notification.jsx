import React, { Children, useRef } from 'react'
import { AiOutlineCloseCircle } from "react-icons/ai";
import { IoIosNotificationsOutline } from "react-icons/io";

export default function Notification(props) {
  const dialogRef=useRef();
  return (
    <>
      <div 
        className='Notification'
        onClick={()=>{
          dialogRef.current?.showModal();
        }}
      >
        <div>
          {props?.icon??<IoIosNotificationsOutline size={30}/>}
        </div>
        <div className='title'>
          {props?.title??"Notification"}
        </div>
      </div>
      <dialog
        ref={dialogRef}
        className='NotificationDialog'
        onClick={e=>{
          const dialogDimensions = e.currentTarget.getBoundingClientRect();
          if(
            e.clientX < dialogDimensions.left   ||
            e.clientX > dialogDimensions.right  ||
            e.clientY < dialogDimensions.top    ||
            e.clientY > dialogDimensions.bottom
          ){
            dialogRef?.current?.close();
          }
        }}
      >
        <div className='titleBar'>
          <div className='Notification'>
            <div>
              {props?.icon??<IoIosNotificationsOutline size={30}/>}
            </div>
            <div className='title'>
              {props?.title??"Notification"}
            </div>
          </div>
          <div onClick={()=>{
            dialogRef?.current?.close();
          }}>
            <AiOutlineCloseCircle size={30}/>
          </div>
        </div>
        {props.children}
      </dialog>
    </>
  )
}
