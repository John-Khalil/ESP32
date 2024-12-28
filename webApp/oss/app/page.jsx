'use client';
import Image from "next/image";
import styles from "./page.module.css";
import NavBar from "@/components/NavBar";
import Notification from "@/components/Notification";
import Tbot from "@/assets/Tbot";
import ConveyorBelt from "@/assets/ConveyorBelt";
import BoxOpener from "@/assets/BoxOpener";
import appLinker from "@/utils/utils";
import setupWebSocket from "@/utils/webSocket";
import { MdOutlineErrorOutline } from "react-icons/md";
import { IoIosWarning } from "react-icons/io";
import { Box } from "@mui/material";
import TbotControls from "@/components/TbotControls";
import { useState } from "react";
export default function Home() {
  const [selectedApp,setSelectedApp]=useState(<TbotControls/>);

  setupWebSocket();


  // appLinker.addListener("",data=>{
    
  // })
  // setTimeout(() => {
  //   appLinker.send("ws","this is test");
  // }, 500);
  return (
    <Box sx={{
      display:'flex',
      flexDirection:'column',
    }}>
      <NavBar {...{
        navList:[
          {
            title:'T-Bot',
            icon:<Tbot {...{
              // x:50,
              // y:100,
              // active:true
            }}/>,
            onClick:()=>{
              setSelectedApp(<TbotControls/>);
            }
          },
          {
            title:'Conveyor Belt',
            icon:<ConveyorBelt/>,
            onClick:()=>{
              setSelectedApp("ConveyorBelt");
            }
          },
          {
            title:'Box Opener',
            icon:<BoxOpener/>,
            onClick:()=>{
              setSelectedApp("BoxOpener");
            }
          },
        ],
        notificationPanel:[
          <Notification {...{
            title:"Tbot Error",
            icon:<MdOutlineErrorOutline size={25} color="red"/>
          }}>
            <Tbot/>
            some random content
          </Notification>,
          <Notification {...{
            title:"Tbot Warning",
            icon:<IoIosWarning size={25} color="yellow"/>
          }}>
            
          </Notification>,
          <Notification {...{
            title:"Another Notification",
          }}>
          </Notification>,
          
        ]
      }} />
      <Box sx={{
        padding:' 10px 16px 10px 16px;'
      }}>
        {selectedApp}
      </Box>
      
    </Box>
  );
}
