import React, { Children, useRef, useState } from 'react'
import { AiOutlineCloseCircle } from "react-icons/ai";
import { IoIosNotificationsOutline } from "react-icons/io";
import { Stack,Box,Paper,Card,CardActionArea,Typography,Modal} from '@mui/material'
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';


export function Notification2(props) {
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


export default function Notification(props) {
  const [open, setOpen] = useState(false);
  // const dialogRef=useRef();
  const Notification=()=>
  <>
    <Box 
      sx={{
        display: 'flex',
        justifyContent: 'flex-start',
        gap: '10px',
        paddingTop: '4px',
      }}
      onClick={()=>{
        setOpen(true);
      }}
    >
      <Box>
        {props?.icon??<IoIosNotificationsOutline size={30}/>}
      </Box>
      <Box>
        {props?.title??"Notification"}
      </Box>
    </Box>
  </>
  return (
    <>

      {Notification()}

      <Dialog
        open={open}
        onClose={()=>{
          setOpen(false);
        }}
        sx={{
          '& .MuiPaper-root': {
            // width: '80vw',
            // height: '85vh',

            borderRadius: '20px',
          },

          // border: 'none',
        }}
      >
        <DialogTitle sx={{ display: 'flex', justifyContent: 'space-between'}}>
          {Notification()}
          <Box 
            onClick={()=>{
            setOpen(false);
          }}>
            <AiOutlineCloseCircle size={30}/>
          </Box>
        </DialogTitle>
        <DialogContent>
          {props.children}
          {/* sample of content */}
        </DialogContent>
      </Dialog>
    </>
  )
}
