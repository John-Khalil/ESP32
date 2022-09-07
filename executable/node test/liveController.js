import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser, { json } from 'body-parser';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';

console.clear();

const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

const xtensaLinker=new globalLinker(hostServerConfig,globalUserCredentials);

xtensaLinker.linkerSetAdd((dataFromServer)=>{
    // console.log('dataFromServer  >> ',dataFromServer);
})

xtensaLinker.linkerSendAdd((data)=>{
    // console.log(`\t\t executable @ - ${data.length}`)
})

const MCU=virtualController;

const superExecutableCallBackAddress=MCU.newVariable();

const liveLoad=[];

const load=(superExecutable)=>{
    liveLoad.push(superExecutable);
}

let superExecutableCounter=0;
xtensaLinker.linkerSetAdd((data)=>{
    if((JSON.parse(data)[MCU.PACKAGE_IDENTIFIER]==superExecutableCallBackAddress)&&superExecutableCounter<liveLoad.length){
        xtensaLinker.linkerSend(JSON.stringify(MCU.executableStack(1,[
            liveLoad[superExecutableCounter++],
            MCU.serverSend(superExecutableCallBackAddress,{executableCount:superExecutableCounter})
        ])));
    }
})

setTimeout(() => {
    xtensaLinker.linkerSet(JSON.stringify({[MCU.PACKAGE_IDENTIFIER]:superExecutableCallBackAddress}));
    // xtensaLinker.linkerSend(JSON.stringify(MCU.consoleLogger("live load test")));
}, 4000);


const serverConsoleCallBackID=MCU.newVariable();

xtensaLinker.linkerSetAdd((data)=>{
    if(JSON.parse(data)[MCU.PACKAGE_IDENTIFIER]==serverConsoleCallBackID)
        console.log("MCU log >> ",JSON.parse(data)[MCU.SERVER_DATA]);
})

const serverConsole=(cosnoleData)=>{
    load(MCU.serverSend(serverConsoleCallBackID,cosnoleData));
}


let testVariable=7;
while(testVariable--){
    load(MCU.delay(500));
    serverConsole({test:'this is test'});
}


