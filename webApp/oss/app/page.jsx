'use client';
import Image from "next/image";
import styles from "./page.module.css";
import NavBar from "@/components/NavBar";
import Tbot from "@/assets/Tbot";
import ConveyorBelt from "@/assets/ConveyorBelt";
import BoxOpener from "@/assets/BoxOpener";
import appLinker from "@/utils/utils";
import setupWebSocket from "@/utils/webSocket";

export default function Home() {
  setupWebSocket();

  appLinker.addListener("",data=>{
    
  })
  appLinker.send("ws","this is test");
  return (
    <>
      <NavBar {...{
        navList:[
          {
            title:'T-Bot',
            icon:<Tbot/>,
            onclick:()=>{

            }
          },
          {
            title:'Conveyor Belt',
            icon:<ConveyorBelt/>,
            onclick:()=>{

            }
          },
          {
            title:'Box Opener',
            icon:<BoxOpener/>,
            onclick:()=>{

            }
          },
        ],
        notificationPanel:[
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
          <div>test</div>,
        ]
      }} />
    </>
  );
}
