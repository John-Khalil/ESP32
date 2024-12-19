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
export default function Home() {
  setupWebSocket();

  // appLinker.addListener("",data=>{
    
  // })
  // setTimeout(() => {
  //   appLinker.send("ws","this is test");
  // }, 500);
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
      
    </>
  );
}

// import React, { useRef } from 'react';

// const NativeDialog = () => {
//   const dialogRef = useRef(null);

//   const openDialog = () => {
//     dialogRef.current?.showModal();
//   };

//   const closeDialog = () => {
//     dialogRef.current?.close();
//   };

//   return (
//     <div className="p-4">
//       <button 
//         onClick={openDialog}
//         className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600"
//       >
//         Open Dialog
//       </button>

//       <dialog
//         ref={dialogRef}
//         className="p-6 rounded-lg shadow-lg backdrop:bg-gray-500/50 backdrop:backdrop-blur-sm"
//         onClick={(e) => {
//           const dialogDimensions = e.currentTarget.getBoundingClientRect();
//           if (
//             e.clientX < dialogDimensions.left ||
//             e.clientX > dialogDimensions.right ||
//             e.clientY < dialogDimensions.top ||
//             e.clientY > dialogDimensions.bottom
//           ) {
//             closeDialog();
//           }
//         }}
//       >
//         <div className="max-w-md">
//           <h2 className="text-xl font-bold mb-4">Native Dialog</h2>
//           <p className="mb-6">
//             This is a native HTML dialog element. It has built-in features like:
//             - Modal behavior
//             - Focus trapping
//             - Esc key to close
//             - Light-dismiss (click outside)
//           </p>
//           <div className="flex justify-end gap-2">
//             <button
//               onClick={closeDialog}
//               className="px-4 py-2 border rounded hover:bg-gray-100"
//             >
//               Cancel
//             </button>
//             <button
//               onClick={() => {
//                 console.log('Confirmed!');
//                 closeDialog();
//               }}
//               className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600"
//             >
//               Confirm
//             </button>
//           </div>
//         </div>
//       </dialog>
//     </div>
//   );
// };

// export default NativeDialog;