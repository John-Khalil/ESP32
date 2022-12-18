import React,{ useState,useEffect,useRef } from "react"


const AddDevice=({userDevice})=>{
    const deviceName=useRef();
    useEffect(()=>{
        deviceName.current.value="";
    },[userDevice.deviceName])
    return(
        <>
            <div className='m-1 p-1 text-left border border-sky-800 rounded max-w-max'>
                Add New Device <br />
                <input className="text-black" type="text" placeholder={`${userDevice.placeholder||'Device Name'}`} ref={deviceName}/>
                <button onClick={()=>{
                    userDevice.deviceName=deviceName.current.value;
                    userDevice.addDevice(deviceName.current.value);
                }} className='m-1 p-1 bg-sky-800 border hover:bg-sky-500 border-sky-800 rounded'>ADD</button>
            </div>
        </>
    );
}

const DeviceList=({deviceList})=>{
    const deviceRenderList=[];
    (deviceList.userDevices||[]).forEach(device => {
        deviceRenderList.push(
            <>
            
            </>
        );
    });
    return(
        <>
            <div className="m-1 p-1 overflow-scroll text-left border border-sky-800 rounded h-96" >
                {deviceRenderList}
            </div>
        </>
    )
}

export default function SmartHub() {
    const [deviceName,clearNameInput]=useState();
    return (
        <>
            <AddDevice userDevice={{
                deviceName,
                addDevice:(userDeviceName)=>{
                    console.log(userDeviceName);
                    clearNameInput(userDeviceName);
                }
            }}/>
            <DeviceList deviceList={{
                
            }}/>
        </>
    )
}
