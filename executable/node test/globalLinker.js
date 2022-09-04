import axios from "axios";
import WebSocket from "ws";

// --es-module-specifier-resolution=node
export default class globalLinker{
    readCallbackList=[];            //^ read data from realtime connection -- this would later be set by the user
    writeCallbackList=[];           //^ send the data through real time connection
    
    async linkerSet(dataToList){
        this.readCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async linkerSend(dataToList){
        this.writeCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async linkerSetAdd(callBack){
        this.readCallbackList.push(callBack);
    }

    async linkerSendAdd(callBack){
        this.writeCallbackList.push(callBack);
    }

    constructor(hostServerConfigUrl,globalUserCredentials){
        axios.get(hostServerConfigUrl).then((getResponse)=>{
            
            const encode64=(str)=>{
                return Buffer.from(str).toString('base64');
            }
            
            const decode64=(str)=>{
                return Buffer.from(str,'base64').toString('utf-8');
            }

            let hostServerAddress=getResponse.data.dev;         //~ expected host-address:port
            const ws=new WebSocket(`ws://${hostServerAddress}`);
            ws.on('open',()=>{
                console.log(`server connected @${globalUserCredentials}`)
                ws.send(JSON.stringify({auth:globalUserCredentials}));
                this.linkerSendAdd((dataToServer)=>{
                    try {
                        ws.send(encode64(dataToServer));
                    } catch (error) {
                        console.error(error);
                    }
                    
                });
            });
            ws.on('message',(dataFromServer)=>{
                this.linkerSet(decode64(dataFromServer.toString()));
            });
            ws.on('close',()=>{
              
            });
        }).catch((error)=>{
            console.error(error);
        })
    }
}
