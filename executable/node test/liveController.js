import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser from 'body-parser';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';

console.clear();

const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

const xtensaLinker=new globalLinker(hostServerConfig,globalUserCredentials);

xtensaLinker.linkerSetAdd((dataFromServer)=>{
    console.log('dataFromServer  >> ',dataFromServer);
})

xtensaLinker.linkerSendAdd((data)=>{
    console.log(`\t\t executable @ - ${data.length}`)
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
        xtensaLinker.linkerSend(JSON.stringify(MCU.loop(1,[
            liveLoad[superExecutableCounter++],
            MCU.serverSend(superExecutableCallBackAddress,{executableCount:superExecutableCounter})
        ])));
    }
})

setTimeout(() => {
    xtensaLinker.linkerSet(JSON.stringify({[MCU.PACKAGE_IDENTIFIER]:superExecutableCallBackAddress}));
    // xtensaLinker.linkerSend(JSON.stringify(MCU.consoleLogger("live load test")));
}, 3000);


let testVariable=7;
while(testVariable--){
    load(MCU.delay(500));
    load(MCU.consoleLogger("this is test"));
}


