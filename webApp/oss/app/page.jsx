'use client';
import Image from "next/image";
import styles from "./page.module.css";
import NavBar from "@/components/NavBar";
import Tbot from "@/assets/Tbot";
import ConveyorBelt from "@/assets/ConveyorBelt";

export default function Home() {
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
            title:'T-Bot',
            icon:<Tbot/>,
            onclick:()=>{

            }
          },
        ],
        notificationPanel:[

        ]
      }} />
    </>
  );
}
