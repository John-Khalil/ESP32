import axios from "axios";
import WebSocket from "ws";



export default class globalLinker{
    readCallbackList=[];            //^ read data from realtime connection -- this would later be set by the user
    writeCallbackList=[];           //^ send the data through real time connection
    
    async globalLinkerSet(dataToList){
        readCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async globalLinkerGet(dataToList){
        writeCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async globalLinkerSetAdd(callBack){
        readCallbackList.push(callBack);
    }

    async globalLinkerGetAdd(callBack){
        writeCallbackList.push(callBack);
    }

    constructor(hostServerConfigUrl,globalUserCredentials){
        axios.get(hostServerConfigUrl).then((getResponse)=>{
            
            const encode64=(str)=>{
                return Buffer.from(str).toString('base64');
            }
            
            const decode64=(str)=>{
                return Buffer.from(str,'base64').toString('ascii');
            }

            let hostServerAddress=getResponse.data.dev;         //~ expected host-address:port
            const ws=new WebSocket(`ws://${hostServerAddress}`);
            ws.on('open',()=>{

            });
            ws.on('message',()=>{

            });
            ws.on('close',()=>{

            });
        }).catch((error)=>{
            console.error(error);
        })
    }
}