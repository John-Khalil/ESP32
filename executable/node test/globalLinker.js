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



const PACKET_SEQUENCE="MS";
const PACKET_PAYLOAD="PP";
const FEEDBACK_TYPE="FT";


export default class globalLinker{
    readCallbackList=[];            //^ read data from realtime connection -- this would later be set by the user
    writeCallbackList=[];           //^ send the data through real time connection

    REAL_TIME_SYNC_REGISTER=0;
    packetSequence=0;
    DEV_ID=126;

    
    encode64=(str)=>{
        return Buffer.from(str).toString('base64');
    }
    
    decode64=(str)=>{
        return Buffer.from(str,'base64').toString('utf-8');
    }

    encode(txData){
        return this.encode64(txData);
    }

    decode(rxData){
        console.log("test for data >*> ",this.decode64(rxData));
        return this.decode64(rxData);
    }

    jsonParse=(jsonObject)=>{
        try {
            jsonObject=JSON.parse(jsonObject);
        } catch (error) {
            return {};
        }
        return jsonObject;
    }

    linkerSendQueue=[];     // super empty array
    queueCounter=0;
    
    async linkerSend(dataToList,devId=this.DEV_ID,recursiveCall=0,typeFeedback=0){     //! this needs to be cached
        dataToList=(Object.keys(this.jsonParse(dataToList)).length)?JSON.parse(dataToList):dataToList;
        if(!recursiveCall&&(!typeFeedback)){
            devId=devId<<24;
            if(!this.REAL_TIME_SYNC_REGISTER)
                this.REAL_TIME_SYNC_REGISTER=devId;
            this.linkerSendQueue.push(dataToList);
        }
        if((this.REAL_TIME_SYNC_REGISTER==(devId|(this.packetSequence&0xFFFFFF)))||(typeFeedback)){
            // dataToList=typeFeedback?dataToList:this.linkerSendQueue[this.queueCounter++];
            dataToList=this.encode((!typeFeedback)?JSON.stringify({
                [PACKET_SEQUENCE]:devId|((++this.packetSequence)&0xFFFFFF),
                [PACKET_PAYLOAD]:this.linkerSendQueue[this.queueCounter++]
            }):dataToList)
            this.writeCallbackList.forEach(callBackFunction => {
                callBackFunction(dataToList);
            });
        }
        else{
            console.log(`waiting for data @ <${queueCounter}>`, this.linkerSendQueue);
            setTimeout(() => {
                this.linkerSend(dataToList,devId,1);
            }, 3);
        }        
        
    }

    async linkerSet(dataToList,devId=this.DEV_ID){
        devId=devId<<24;
        if(this.jsonParse(this.decode(dataToList))[PACKET_SEQUENCE]==undefined){
            this.readCallbackList.forEach(callBackFunction => {
                callBackFunction(dataToList);
            });
            return;
        }
        dataToList=this.jsonParse(this.decode(dataToList))
        console.log(dataToList)
        if(dataToList[FEEDBACK_TYPE]==true){
            if((dataToList[PACKET_SEQUENCE]>>24)==devId)
                REAL_TIME_SYNC_REGISTER=dataToList[PACKET_SEQUENCE];
        }
        else{
            this.linkerSend(JSON.stringify({
                [FEEDBACK_TYPE]:true,
                [PACKET_SEQUENCE]:devId|((dataToList[PACKET_SEQUENCE])&0xFFFFFF)
            }),this.DEV_ID,0,1);
            dataToList=JSON.stringify(dataToList[PACKET_PAYLOAD]);
            this.readCallbackList.forEach(callBackFunction => {
                callBackFunction(dataToList);
            });
        }

        
    }


    async linkerSetAdd(callBack){
        this.readCallbackList.push(callBack);
    }

    async linkerSendAdd(callBack){
        this.writeCallbackList.push(callBack);
    }

    constructor(hostServerConfigUrl,globalUserCredentials){
        axios.get(hostServerConfigUrl).then((getResponse)=>{
            let hostServerAddress=getResponse.data.dev;         //~ expected host-address:port
            const ws=new WebSocket(`ws://${hostServerAddress}`);
            ws.on('open',()=>{
                console.log(`server connected @${globalUserCredentials}`)
                ws.send(JSON.stringify({auth:globalUserCredentials}));
                this.linkerSendAdd((dataToServer)=>{
                    try {
                        ws.send(dataToServer);
                    } catch (error) {
                        console.error(error);
                    }
                    
                });
            });
            ws.on('message',(dataFromServer)=>{
                this.linkerSet(dataFromServer.toString());
            });
            ws.on('close',()=>{
              
            });
        }).catch((error)=>{
            console.error(error);
        })
    }
}
