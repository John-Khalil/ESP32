import axios from "axios";
import WebSocket from "ws";



export default class globalLinker{
    readCallbackList=[];            //^ read data from realtime connection -- this would later be set by the user
    writeCallbackList=[];           //^ send the data through real time connection
    
    async linkerSet(dataToList){
        readCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async linkerSend(dataToList){
        writeCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async linkerSetAdd(callBack){
        readCallbackList.push(callBack);
    }

    async linkerSendAdd(callBack){
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

            let webSocketSend;

            let hostServerAddress=getResponse.data.dev;         //~ expected host-address:port
            const ws=new WebSocket(`ws://${hostServerAddress}`);
            ws.on('open',()=>{
                console.log(`server connected @${globalUserCredentials}`)
                ws.send(JSON.stringify({auth:globalUserCredentials}));
                webSocketSend=(dataToServer)=>{
                    ws.send(encode64(dataToServer));
                }
                linkerSendAdd(webSocketSend);
            });
            ws.on('message',(dataFromServer)=>{
                linkerSet(decode64(dataFromServer));
            });
            ws.on('close',()=>{
                webSocketSend=(dataToServer)=>{
                    console.log(`cannot send data @${dataToServer} SERVER-DISCONNECTED`);
                }
            });
        }).catch((error)=>{
            console.error(error);
        })
    }
}