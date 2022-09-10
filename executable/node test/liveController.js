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

const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

const xtensaLinker=new globalLinker(hostServerConfig,globalUserCredentials);

xtensaLinker.linkerSetAdd((dataFromServer)=>{
    // console.log('read feedback  >> ',dataFromServer);
})

xtensaLinker.linkerSendAdd((dataFromServer)=>{
    // console.log(' write feedback  >> ',dataFromServer);
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
        xtensaLinker.linkerSend(JSON.stringify(liveLoad[superExecutableCounter++]));
    }
})

setTimeout(() => {
    xtensaLinker.linkerSet("MAIN-THREAD-LOAD");
    console.log("test")
}, 4000);



const serverConsoleCallBackID=MCU.newVariable();

xtensaLinker.linkerSetAdd((data)=>{
    if(jsonParse(data)[MCU.PACKAGE_IDENTIFIER]==serverConsoleCallBackID)
        console.log("MCU log >> ",jsonParse(data)[MCU.SERVER_DATA].consoleData||jsonParse(data)[MCU.SERVER_DATA]);
})

const serverConsole=(consoleData)=>{
    load(MCU.serverSend(serverConsoleCallBackID,(typeof consoleData=="string")?{consoleData}:consoleData));
}

MCU.logger=(consoleData)=>{
    return(MCU.serverSend(serverConsoleCallBackID,(typeof consoleData=="string")?{consoleData}:consoleData));
}

MCU.load=load;

const mcu=MCU;

mcu.load(mcu.logger('code just started'))


let x=40;

var counter=0;

// const increment=MCU.newVariable();
// mcu.load(mcu.memoryWrite(increment,0));

// // while(x--){
// //     mcu.load(mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))),
// //     mcu.load(mcu.logger(mcu.memoryRead(increment)))
// // }
// while(x--)
// mcu.load(mcu.loop(1,[
//     mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
//     mcu.logger(mcu.memoryRead(increment))
//     // mcu.executableStack(2,[
//     //     mcu.logger(`counter ${counter++}`),
//     //     mcu.executableStack(3,[
//     //         mcu.logger(`counter ${counter++}`)
//     //     ])
//     // ])
// ]))

// mcu.load(mcu.logger(`counter ${counter++}`))



const function1=MCU.newVariable();
const arguments1=MCU.newVariable();

const function2=MCU.newVariable();
const arguments2=MCU.newVariable();

const increment=MCU.newVariable();

mcu.load(mcu.memoryWrite(increment,0));

load(MCU.createFunction(function1,arguments1,()=>{
    // return mcu.logger(`counter  >> ${counter++}`);
    // return MCU.memoryWrite(arguments1,MCU.ALU(MCU.functionArgument('arg1'),'*',MCU.functionArgument('arg2')));
    return MCU.loop(1,[
        // MCU.logger(MCU.functionArgument('arg1')),
        // MCU.logger(MCU.functionArgument('arg2')),
        mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
        MCU.memoryWrite(arguments1,MCU.ALU(MCU.functionArgument('arg1'),'*',MCU.functionArgument('arg2'))),
        MCU.memoryWrite(arguments1,mcu.ALU(mcu.memoryRead(increment),'*',mcu.memoryRead(arguments1)))
        // MCU.callFunction(function1,{arg1:5,arg2:6})
        
    ])
},MCU.memoryRead(arguments1)))

// load(MCU.createFunction(function2,arguments2,()=>{
//     return MCU.executableStack(1,[
//         MCU.logger(MCU.functionArgument('arg1')),
//         MCU.logger(MCU.functionArgument('arg2')),
//         MCU.memoryWrite(arguments2,MCU.ALU(MCU.functionArgument('arg1'),'+',MCU.functionArgument('arg2'))),
//         // MCU.callFunction(function1,{arg1:5,arg2:6})
//         // serverConsole(MCU.functionArgument('arg1')),
//         // serverConsole(MCU.functionArgument('arg1'))
//     ])
// },MCU.memoryRead(arguments2)))


// let testVariable=7;
// while(testVariable--){
//     // load(MCU.delay(500));
//     serverConsole(MCU.ALU(5,'*',6))
//     serverConsole('string is string')
// }

// load(MCU.memoryWrite(1400,5));

// testVariable=7;
// while(testVariable--){
    

//     load(MCU.memoryWrite(1400,MCU.ALU(MCU.memoryRead(1400),'*',MCU.memoryRead(1400))));
    
//     serverConsole(MCU.memoryRead(1400))
// // }
// x=5;
// while(x--)
// serverConsole(MCU.callFunction(function1,{arg1:8,arg2:8}))
// serverConsole(MCU.callFunction(function1,{arg1:8,arg2:8}))

// load(MCU.controllerEventListener(1005,1006,MCU.digitalInput(69),MCU.callFunction(function1,{arg1:8,arg2:6})));

load(MCU.controllerEventListener(1005,1006,MCU.digitalInput(69),mcu.logger(MCU.callFunction(function1,{arg1:8,arg2:3}))));


serverConsole("hello world ");

// // // serverConsole(MCU.memoryRead(2008));

