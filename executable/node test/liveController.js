import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser, { json } from 'body-parser';
import { Console } from 'console';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';


console.clear();

const deviceIP='192.168.1.15';

const app =express();
app.use(cors());
app.use(bodyParser.json());
const port =766;
app.listen(port,()=>{
    console.log(`-------- server started @ port ${port}`);
});

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

MCU.postLogger=(consoleData)=>{
    return MCU.fetch(`http://${deviceIP}:${port}/upload`,(typeof consoleData=="string")?{consoleData}:consoleData,serverConsoleCallBackID);
}

xtensaLinker.linkerSetAdd((data)=>{
    if(jsonParse(data)[MCU.REQUEST_PARAM]==serverConsoleCallBackID)
        console.log("MCU post log >> ",jsonParse(data)[MCU.POST_BODY].consoleData||jsonParse(data)[MCU.POST_BODY]);
})

MCU.load=load;

const mcu=MCU;

mcu.led=ledValue=>{
    return mcu.digitalOutput(1,14,0,[ledValue]);
}

mcu.inputPin=()=>{
    return mcu.digitalInput(1,0,0)
}


console.log(" >> >> ",mcu.digitalOutput(5,4,2,[255,459,789]))





const increment=MCU.newVariable();

const newFunction=mcu.newVariable();
const newFunctionParams=mcu.newVariable();

const testRunner=()=>{
    // mcu.load(mcu.logger(mcu.digitalInput(24,0,2,[255,459,789])));

    // const increment=MCU.newVariable();
    // mcu.load(mcu.memoryWrite(increment,0));

    // mcu.memoryDelete(increment);

    mcu.load(mcu.led(1));

    mcu.load(mcu.logger('code started'));

    // mcu.load(mcu.createFunction(newFunction,newFunctionParams,()=>{
    //     return mcu.executableStack(1,[
    //         mcu.delay(500),
    //         mcu.led(mcu.functionArgument('state_1')),
    //         mcu.delay(500),
    //         mcu.led(mcu.functionArgument('state_2'))
    //     ])
    // },mcu.memoryRead(newFunctionParams)));

    
    
    mcu.load(mcu.delay(500))
    mcu.load(mcu.led(0));
    
    // mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.executableStack(1,[
    //     mcu.led(1),
    //     mcu.delay(100),
    //     mcu.led(0),
    //     mcu.postLogger(mcu.memoryRead(1001))
    // ])))

    mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.postLogger(mcu.memoryRead(1001))))
    mcu.load(mcu.controllerEventListener(2000,2001,mcu.inputPin(),mcu.logger(mcu.memoryRead(2001))))

    // mcu.load(mcu.postLogger(mcu.inputPin(1001)))

    // var x10=10
    // while(x10--)
    //     mcu.load(mcu.logger('this is test'))

    
    // mcu.load(mcu.logger(mcu.postLogger('this is test')))

    // mcu.load(mcu.callFunction(newFunction,{state_1:1,state_2:0}))


    // let counter=100;
    // while(counter--)
    mcu.load(mcu.executableStack(0,[
        // mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
        mcu.logger(mcu.inputPin())
        // mcu.executableStack(5,[
        //     mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
        //     mcu.executableStack(5,[
        //         mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))
        //         // mcu.delay(5)
        //     ]),
        //     mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))
        // ]),
        // mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))
    ]))

    // // mcu.load(mcu.logger(mcu.memoryRead(increment)));

    mcu.load(mcu.logger('hello world'));

}


const simplePing=()=>{
    mcu.load(mcu.logger('simple ping'))
}

app.get('/',(req,res)=>{

    // testRunner();
    simplePing();
    
    xtensaLinker.linkerSet("MAIN-THREAD-LOAD");
    res.send('ack');
})

app.post('/upload',(req,res)=>{
    res.send('ack');
    xtensaLinker.linkerSet(JSON.stringify(req.body));
})

testRunner();



