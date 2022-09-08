import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser, { json } from 'body-parser';
import { Console } from 'console';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';

console.clear();

const jsonParse=(jsonObject)=>{
    try {
        jsonObject=JSON.parse(jsonObject);
    } catch (error) {
        return "error parse json";
    }
    return jsonObject;
}

// const logger=new Console({ stdout: output, stderr: errorOutput,ignoreErrors: true, colorMode: true });

const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

const xtensaLinker=new globalLinker(hostServerConfig,globalUserCredentials);

xtensaLinker.linkerSetAdd((dataFromServer)=>{
    // console.log('dataFromServer  >> ',dataFromServer);
})



const MCU=virtualController;

const superExecutableCallBackAddress=MCU.newVariable();

const liveLoad=[];

const load=(superExecutable)=>{
    liveLoad.push(superExecutable);
}

let superExecutableCounter=0;
xtensaLinker.linkerSetAdd((data)=>{
    if((data=="MAIN-THREAD-LOAD")&&superExecutableCounter<liveLoad.length){
        // xtensaLinker.linkerSend(JSON.stringify(MCU.loop(1,[
        //     liveLoad[superExecutableCounter++],
        //     MCU.serverSend(superExecutableCallBackAddress,{executableCount:superExecutableCounter})
        // ])));
        xtensaLinker.linkerSend(JSON.stringify(liveLoad[superExecutableCounter++]));
    }
})

setTimeout(() => {
    xtensaLinker.linkerSet("MAIN-THREAD-LOAD");
    // xtensaLinker.linkerSend(JSON.stringify(MCU.serverSend(superExecutableCallBackAddress,MCU.ALU(5,'*',6))));


}, 4000);



const serverConsoleCallBackID=MCU.newVariable();

xtensaLinker.linkerSetAdd((data)=>{
    if(jsonParse(data)[MCU.PACKAGE_IDENTIFIER]==serverConsoleCallBackID)
        console.log("MCU log >> ",jsonParse(data)[MCU.SERVER_DATA].consoleData||jsonParse(data)[MCU.SERVER_DATA]);
})

const serverConsole=(consoleData)=>{
    load(MCU.serverSend(serverConsoleCallBackID,(typeof consoleData=="string")?{consoleData}:consoleData));
}


let testVariable=7;
while(testVariable--){
    // load(MCU.delay(500));
    serverConsole(MCU.ALU(5,'*',6))
    serverConsole('string is string')
}

load(MCU.memoryWrite(1400,5));

testVariable=7;
while(testVariable--){
    

    load(MCU.memoryWrite(1400,MCU.ALU(MCU.memoryRead(1400),'*',MCU.memoryRead(1400))));
    
    serverConsole(MCU.memoryRead(1400))
}

