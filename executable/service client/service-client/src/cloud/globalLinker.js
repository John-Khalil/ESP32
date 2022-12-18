import axios from "axios";
// import WebSocket from "ws";

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



const PACKET_SEQUENCE="MSS";
const PACKET_PAYLOAD="PP";
const FEEDBACK_TYPE="FT";


export default class globalLinker{
    readCallbackList=[];            //^ read data from realtime connection -- this would later be set by the user
    writeCallbackList=[];           //^ send the data through real time connection

    REAL_TIME_SYNC_REGISTER=0;
    packetSequence=0;
    DEV_ID=0;

    
    encode64=(str)=>{
        // return Buffer.from(str).toString('base64');
        return btoa(str);
    }
    
    decode64=(str)=>{
        // return Buffer.from(str,'base64').toString('utf-8');
        return atob(str);
    }

    encode(txData){
        return this.encode64(txData);
    }

    decode(rxData){
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

    DISABLE_FEEDBACK=1;
    
    async linkerSend(dataToList,devId=this.DEV_ID,recursiveCall=0,typeFeedback=this.DISABLE_FEEDBACK){     //! this needs to be cached
        dataToList=(Object.keys(this.jsonParse(dataToList)).length)?JSON.parse(dataToList):dataToList;
        
        // console.log("TX -> ",dataToList);
        // console.log(`waiting for data @ <${this.queueCounter}>`, this.linkerSendQueue)
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
            }):((typeof dataToList=='object')?JSON.stringify(dataToList):dataToList))
            this.writeCallbackList.forEach(callBackFunction => {
                callBackFunction(dataToList);
            });
        }
        else{
            setTimeout(() => {
                this.linkerSend(dataToList,devId,1);
            }, 3);
        }        
        
    }

    async linkerSet(dataToList,devId=this.DEV_ID,decodeMessage=0){
        devId=devId<<24;
        if(this.jsonParse(this.decode(dataToList))[PACKET_SEQUENCE]==undefined){
            if(decodeMessage)
                dataToList=this.decode(dataToList);

            dataToList=((dataToList.slice(0,1)=='\"')&&(dataToList.slice(dataToList.length-1,dataToList.length)=='\"'))?dataToList.slice(1,dataToList.length-1):dataToList;
            // console.log('direct no feedback --> ',dataToList);
            this.readCallbackList.forEach(callBackFunction => {
                callBackFunction(dataToList);
            });
            return;
        }
        dataToList=this.jsonParse(this.decode(dataToList))
        // console.log("RX -> ",dataToList)
        if(dataToList[FEEDBACK_TYPE]==true){
            if((dataToList[PACKET_SEQUENCE]>>24)==devId)
                this.REAL_TIME_SYNC_REGISTER=dataToList[PACKET_SEQUENCE];
        }
        else{
            this.linkerSend(JSON.stringify({
                [FEEDBACK_TYPE]:true,
                [PACKET_SEQUENCE]:devId|((dataToList[PACKET_SEQUENCE])&0xFFFFFF)
            }),this.DEV_ID,0,1);

            
            dataToList=(typeof(dataToList[PACKET_PAYLOAD])=='object')?JSON.stringify(dataToList[PACKET_PAYLOAD]):dataToList[PACKET_PAYLOAD];
            this.readCallbackList.forEach(callBackFunction => {
                callBackFunction(dataToList);
            });
        }

        
    }

    enableFeedback(forceFeedback){
        this.DISABLE_FEEDBACK=!forceFeedback;
    }


    async linkerSetAdd(callBack){
        this.readCallbackList.push(callBack);
    }

    async linkerSendAdd(callBack){
        this.writeCallbackList.push(callBack);
    }

    constructor(hostServerConfigUrl,globalUserCredentials,onConnectCallback=()=>{}){
        axios.get(hostServerConfigUrl).then((getResponse)=>{
            let hostServerAddress=getResponse.data.dev;         //~ expected host-address:port
            const ws=new WebSocket(`ws://${hostServerAddress}`);
            ws.onopen=()=>{
                console.log(`server connected @${globalUserCredentials}`)
                ws.send(JSON.stringify({auth:globalUserCredentials}));
                this.linkerSendAdd((dataToServer)=>{
                    try {
                        ws.send(dataToServer);
                    } catch (error) {
                        console.error(error);
                    }
                    
                });
                onConnectCallback();
            };
            ws.onmessage=(dataFromServer)=>{
                this.linkerSet(dataFromServer.data.toString(),this.DEV_ID,1);
            };
            ws.onclose=()=>{
              
            };
        }).catch((error)=>{
            console.error(error);
        })
    }
}
