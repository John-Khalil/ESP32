import React, { Children, useRef } from 'react'

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
        test
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
        {props.children}
      </dialog>
    </>
  )
}
