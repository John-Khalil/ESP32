import axios from "axios";
import WebSocket from "ws";

// --es-module-specifier-resolution=node




/*
	^ WRITE SIDE 
		* get the data 
		* wait for the SYNC REGISTER to update to the last sent sequence
		* wrape it inside the object
		* encode it
		* pass it to the threads
	^ READ SIDE
		* decode the data
		* check for packet type
		? if normal packet 
			* wrape it inside feedback object
			* send the feedback object
			* pass the packet to threads
		? if feedback packet 
			* update the SYNC REGISTER

*/



export default class globalLinker{
    readCallbackList=[];            //^ read data from realtime connection -- this would later be set by the user
    writeCallbackList=[];           //^ send the data through real time connection

    syncRegister=0;
    packetSequence=0;
    DEV_ID=255;

    encode(txData){
        return txData;
    }

    decode(rxData){
        return rxData;
    }
    
    async linkerSend(dataToList,devId=this.DEV_ID){


        devId=devId<<24;
        if(!syncRegister)
            syncRegister=devId;

        this.writeCallbackList.forEach(callBackFunction => {
            callBackFunction(dataToList);
        });
    }

    async linkerSet(dataToList){
        this.readCallbackList.forEach(callBackFunction => {
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
