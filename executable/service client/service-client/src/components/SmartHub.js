import React,{ useState,useEffect,useRef } from "react"
import globalLinker from "../cloud/globalLinker";

if(localStorage.getItem('devList')==null){
    localStorage.setItem('devList',JSON.stringify([]));
}



// devLinker.linkerSetAdd(data=>console.log("devLinker >> ",data));





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
    (deviceList.userDevices||[]).forEach((device,index) => {
        if(device.deleted)
            return;
        device.index=index;
        deviceRenderList.push(
            <>
                <div className="m-1 p-1 overflow-scroll text-left border border-gray-300 bg-black rounded-md">
                    <span>{`${device.name||'Device Name'}`}</span><br />
                    <button onClick={()=>{
                        deviceList.delete(device);
                    }} className="m-1 p-1 bg-[#FF0000] border hover:bg-[#ff00006c] border-[#ff00006c] rounded inline-block float-left">Delete</button>
                    <button onClick={()=>{
                        deviceList.playBack(device);
                    }} className="m-1 p-1 bg-[#059862] border hover:bg-[#05986295] border-[#05986295] rounded inline-block float-right">Play Back</button>
                </div>
            </>
        );
    });
    return(
        <>
            {deviceRenderList}
        </>
    )
}

export default function SmartHub() {
    const [deviceName,clearNameInput]=useState();
    const [userDevices,setUserDevices]=useState(JSON.parse(localStorage.getItem('devList')));

    const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
    const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

    const devLinker=new globalLinker(hostServerConfig,globalUserCredentials,()=>{
        // devLinker.linkerSend("connected\r\n")
    });
    
    let ADD_REQUEST="";


    devLinker.linkerSetAdd(deviceResponse=>{
        if((ADD_REQUEST!=="")&&(deviceResponse!=='CONNECTION-ALIVE-ACK')){
            let userDevList=JSON.parse(localStorage.getItem('devList'));        //^ array is expected
            userDevList.push({
                name:ADD_REQUEST,
                deleted:false,
                data:deviceResponse
            })
            localStorage.setItem('devList',JSON.stringify(userDevList));
            setUserDevices(JSON.parse(localStorage.getItem('devList')));
            ADD_REQUEST="";
        }
    })


    return (
        <>
            <AddDevice userDevice={{
                deviceName,
                addDevice:(userDeviceName)=>{
                    console.log(userDeviceName);
                    clearNameInput(userDeviceName);
                    ADD_REQUEST=userDeviceName;
                    devLinker.linkerSend('!get-key')
                }
            }}/>
            <DeviceList deviceList={{
                userDevices,
                delete:device=>{
                    // console.log(device);

                    let userDevList=JSON.parse(localStorage.getItem('devList'));        //^ array is expected
                    userDevList[device.index].deleted=true;
                    localStorage.setItem('devList',JSON.stringify(userDevList));
                    setUserDevices(JSON.parse(localStorage.getItem('devList')));

                },
                playBack:device=>{
                    // console.log(device);
                    devLinker.linkerSend(JSON.parse(localStorage.getItem('devList'))[device.index].data);
                }
            }}/>
        </>
    )
}
