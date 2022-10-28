import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser, { json } from 'body-parser';
import { Console } from 'console';
import {NodeAudioVolumeMixer } from 'node-audio-volume-mixer';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';

// NodeAudioVolumeMixer.setMasterVolumeLevelScalar(0.6)

console.clear();

const encode64=(str)=>{
    return Buffer.from(str).toString('base64');
}

const decode64=(str)=>{
    return Buffer.from(str,'base64').toString('utf-8');
}

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


let readFeedBackCounter=0;
xtensaLinker.linkerSetAdd((dataFromServer)=>{
    // console.log(`read feedBack @ ${readFeedBackCounter++} >> `,dataFromServer);
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

mcu.forceStartEnable=true;
mcu.forceStart=()=>{
    if(mcu.forceStartEnable)
        xtensaLinker.linkerSet("MAIN-THREAD-LOAD");
}

const mcuEventsList=[];
const mcuEventsIdentifierList=[];
mcu.addEventListener=(mcuEvent,eventCallBack)=>{
    let callBackIdentifier=mcuEventsIdentifierList[mcuEventsList.indexOf(mcuEvent)];        //^ it will get the value unless it didn't exist then it would be updated
    if(!mcuEventsList.includes(mcuEvent)){
        let eventAddress=mcu.newVariable();
        let onChangeAddress=mcu.newVariable();
        callBackIdentifier=mcu.newVariable();
        mcu.load(mcu.controllerEventListener(eventAddress,onChangeAddress,mcuEvent,mcu.serverSend(callBackIdentifier,mcu.memoryRead(onChangeAddress))));
        mcuEventsList.push(mcuEvent);
        mcuEventsIdentifierList.push(callBackIdentifier);
    }
    xtensaLinker.linkerSetAdd((data)=>{
        if(jsonParse(data)[mcu.PACKAGE_IDENTIFIER]==callBackIdentifier)
            eventCallBack(jsonParse(data)[mcu.SERVER_DATA]);
    })

}


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

let toggleVolumeControl=1;

const testRunner=()=>{



    mcu.load(mcu.logger('code started'));

    // mcu.load(mcu.createFunction(newFunction,newFunctionParams,()=>{
    //     return mcu.executableStack(1,[
    //         mcu.delay(500),
    //         mcu.led(mcu.functionArgument('state_1')),
    //         mcu.delay(500),
    //         mcu.led(mcu.functionArgument('state_2'))
    //     ])
    // },mcu.memoryRead(newFunctionParams)));

    
    
    mcu.load(mcu.led(0));

    const serialPortAddress=mcu.newVariable();
    const serialPortRXAddress=mcu.newVariable();
    

    mcu.load(mcu.serialPortAdd(26,27,38400,serialPortAddress,serialPortRXAddress,mcu.logger(mcu.memoryRead(serialPortRXAddress))));
    // mcu.load(mcu.serialPortAdd(26,27,38400,serialPortAddress,serialPortRXAddress,{}));

    // mcu.load(mcu.serialPortSend(serialPortAddress,"this is new test\n"));

    // mcu.addEventListener(mcu.memoryRead(serialPortRXAddress),(data)=>{
    //     console.log(`data from event >> ${decode64(data)}`)
    // })
    
    // mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.executableStack(100,[
    //     mcu.postLogger(1),
    //     mcu.delay(20),
    //     mcu.postLogger(0),
    //     mcu.delay(20)
    // ])))


    // mcu.addEventListener(mcu.ALU(mcu.adcRead(34),'/',41),(data)=>{
    //     if(toggleVolumeControl){
    //         console.log(`data from event >> ${data}`)
    //         NodeAudioVolumeMixer.setMasterVolumeLevelScalar(data/100)
    //     }
    // })

    // mcu.addEventListener(mcu.inputPin(),(data)=>{
    //     if(data==0)
    //         console.clear();
    // })

    // mcu.addEventListener(mcu.inputPin(),(data)=>{
    //     if(data==1)
    //         toggleVolumeControl=toggleVolumeControl^1;
    // })

    // mcu.addEventListener(mcu.ALU(mcu.timer(),'/',1500),(data)=>{
    //     console.log(` >> @ ${data}`)
    // })







    mcu.load(mcu.logger('last executable'));

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



