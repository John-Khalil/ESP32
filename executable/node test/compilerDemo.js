import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser from 'body-parser';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';


const MCU= virtualController;

// const WebSocket =require('ws')
// const axios = require('axios')
// const express =require('express');
// const cors=require('cors');
// const util = require('util')
// const bodyParser = require('body-parser');


const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

const xtensaLinker=new globalLinker(hostServerConfig,globalUserCredentials);

setTimeout(() => {
    xtensaLinker.linkerSend(JSON.stringify(consoleLogger("hello")));    
}, 500);

xtensaLinker.linkerSetAdd((dataFromServer)=>{
    console.log('dataFromServer  >> ',dataFromServer);
})



const app =express();
app.use(cors());

app.use(bodyParser.json());




const port =766;


var variableMemoryBaseAddress=0;
const newVariable=()=>{
    return variableMemoryBaseAddress++;
}


// order should always be maintained

var aluOperationIndex=0;
const virtualControllerALU={};

virtualControllerALU['+']   =   aluOperationIndex++;
virtualControllerALU['-']   =   aluOperationIndex++;
virtualControllerALU['*']   =   aluOperationIndex++;
virtualControllerALU['/']   =   aluOperationIndex++;
virtualControllerALU['%']   =   aluOperationIndex++;
virtualControllerALU['2s']  =   aluOperationIndex++;
virtualControllerALU['^']   =   aluOperationIndex++;
virtualControllerALU['|']   =   aluOperationIndex++;
virtualControllerALU['&']   =   aluOperationIndex++;
virtualControllerALU['~']   =   aluOperationIndex++;
virtualControllerALU['>>']  =   aluOperationIndex++;
virtualControllerALU['<<']  =   aluOperationIndex++;
virtualControllerALU['==']  =   aluOperationIndex++;
virtualControllerALU['!=']  =   aluOperationIndex++;
virtualControllerALU['&&']  =   aluOperationIndex++;
virtualControllerALU['||']  =   aluOperationIndex++;
virtualControllerALU['!']   =   aluOperationIndex++;
virtualControllerALU['<']   =   aluOperationIndex++;
virtualControllerALU['>']   =   aluOperationIndex++;
virtualControllerALU['<=']  =   aluOperationIndex++;
virtualControllerALU['>=']  =   aluOperationIndex++;

const operationSelector=(operationIndex)=>{
    return virtualControllerALU[operationIndex]%aluOperationIndex;
}


var operatorIndex=0;

const digitalOutputOperator=operatorIndex++;
const digitalInputOperator=operatorIndex++;
const delayOperator=operatorIndex++;
const loopOperator=operatorIndex++;

const consoleLoggerOperator=operatorIndex++;
const hardwareIDOperator=operatorIndex++;
const memoryWriteOperator=operatorIndex++;
const memoryReadOperator=operatorIndex++;
const memoryDeleteOperator=operatorIndex++;

const fetchOperator=operatorIndex++;
const addEventOperator=operatorIndex++;
const removeEventOperator=operatorIndex++;
const setEventPollingOperator=operatorIndex++;

const operatorJsonOperator=operatorIndex++;
const createfunctionOperator=operatorIndex++;
const callFunctionOperator=operatorIndex++;

const executableStackOperator=operatorIndex++;

const arithmaticLogicUnitOperator=operatorIndex++;
const serverSendOperator=operatorIndex++;


const JSON_OPERATOR="OP";				// generic operator


// digital output & input
const OUTPUT_STREAM="OS";				// digital output stream
const PORT_ADDRESS="PA";				// digital output port address
const PORT_VALUE="PV";


// delay operator
const DELAY_MICRO_SEC="US";			// micro second delay
const DELAY_MILLI_SEC="MS";			// milli second delay

// loop operator
const LOOP_COUNTER="LC";				// loop counter
const LOOP_BODY="LB";					// loop body

// virtual controller memory
const BUFFER_IDENTIFIER="BI";          
const BUFFER_DATA="BD";     

// console logger
const CONSOLE_DATA="CD";

// fetch operator
const WEB_HOST="WH";
const POST_BODY="PB";
const REQUEST_PARAM="RP";


// add event listener operator
const EVENT_ADDRESS="EA";
const EVENT_EXECUTABLE="EE";
const HANDLER_EXECUTABLE="HE";
const ONCHANGE_ADDRESS="CA";
const POLLING_RATE="PR";


// operator json
const OBJECT_KEY="OK";
const EXECUTABLE_JSON="EJ";


// function operator create & call
const FUNCTION_ADDRESS="FA";
const FUNCTION_OBJECT="FO";
const PARAMETER_ADDRESS="PA";
const STACK_EXECUTABLE="SE";
const RETURN_EXECUTABLE="RE";
const PARAMETER_OBJECT="PO";


// ececutable Stack operator
const EXECUTABLE_COUNTER="EC";
const EXECUTABLE_STACK="ES"; 


// ALU operator
const ALU_OPERATION="AO";
const FIRST_OPERAND="FO";
const SECOND_OPERAND="SO";


// server send operator
const PACKAGE_IDENTIFIER="PI";
const SERVER_DATA="SD";



const delay=delayValueMS=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=delayOperator;
    returnStack[DELAY_MILLI_SEC]=delayValueMS;
    return returnStack;
}

const digitalOutput=(outputPort,portStream)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=digitalOutputOperator;
    returnStack[PORT_ADDRESS]=outputPort;
    returnStack[OUTPUT_STREAM]=portStream;
    return returnStack;
}

const digitalInput=(inputPort)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=digitalInputOperator;
    returnStack[PORT_ADDRESS]=inputPort;
    return returnStack;
}

const loop=(_loopCounter,_loopBody)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=loopOperator;
    returnStack[LOOP_COUNTER]=_loopCounter;
    returnStack[LOOP_BODY]=_loopBody;
    return returnStack;
}

const consoleLogger=(consoleData)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=consoleLoggerOperator;
    returnStack[CONSOLE_DATA]=consoleData;
    return returnStack;
}

const hardwareID=()=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=hardwareIDOperator;
    return returnStack;
}

const memoryWrite=(bufferIdentifier,bufferData)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=memoryWriteOperator;
    returnStack[BUFFER_IDENTIFIER]=bufferIdentifier;
    returnStack[BUFFER_DATA]=bufferData;
    return returnStack;
}

const memoryRead=(bufferIdentifier)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=memoryReadOperator;
    returnStack[BUFFER_IDENTIFIER]=bufferIdentifier;
    return returnStack;
}

const memoryDelete=(bufferIdentifier)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=memoryDeleteOperator;
    returnStack[BUFFER_IDENTIFIER]=bufferIdentifier;
    return returnStack;
}

const controllerFetch=(fetchWebHost,fetchData,fetchParams)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=fetchOperator;
    returnStack[WEB_HOST]=fetchWebHost;
    if(fetchData!==undefined)
        returnStack[POST_BODY]=fetchData;
    if((fetchParams!=undefined)&&(fetchData!==undefined))
        returnStack[REQUEST_PARAM]=fetchParams;
    return returnStack;
}

const controllerEventListener=(eventAddress,onchangeAddress,eventExecutable,handlerExecutable)=>{
    var returnStack={};
    var nestedReturnStack={};
    nestedReturnStack[EVENT_EXECUTABLE]=eventExecutable;
    nestedReturnStack[HANDLER_EXECUTABLE]=handlerExecutable;
    nestedReturnStack[ONCHANGE_ADDRESS]=onchangeAddress;
    
    returnStack[JSON_OPERATOR]=addEventOperator;
    returnStack[EVENT_ADDRESS]=eventAddress;
    returnStack[EVENT_EXECUTABLE]=nestedReturnStack;
    return returnStack;
}


const controllerRemoveEventListener=(eventAddress)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=removeEventOperator;
    returnStack[EVENT_ADDRESS]=eventAddress;
    return returnStack;
}


const controllerEventPollingRate=(pollingRate)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=setEventPollingOperator;
    returnStack[POLLING_RATE]=pollingRate;
    return returnStack;
}


const operatorJson=(objectKey,jsonObject)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=operatorJsonOperator;
    returnStack[OBJECT_KEY]=objectKey;
    returnStack[EXECUTABLE_JSON]=jsonObject;
    return returnStack;
}


var creatingFunctionParameterAddress=0;
const functionArgument=(parameter)=>{
    return operatorJson(parameter,memoryRead(creatingFunctionParameterAddress));
}


const createFunction=(functionAddress,parameterAddress,stackExecutable,returnExecutable)=>{
    creatingFunctionParameterAddress=parameterAddress;
    stackExecutable=stackExecutable();
    var returnStack={};
    var functionObject={};
    functionObject[PARAMETER_ADDRESS]=parameterAddress;
    functionObject[STACK_EXECUTABLE]=stackExecutable;
    functionObject[RETURN_EXECUTABLE]=returnExecutable;

    returnStack[JSON_OPERATOR]=createfunctionOperator;
    returnStack[FUNCTION_ADDRESS]=functionAddress;
    returnStack[FUNCTION_OBJECT]=functionObject;
    return returnStack;
}


const callFunction=(functionAddress,parameterObject)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=callFunctionOperator;
    returnStack[FUNCTION_ADDRESS]=functionAddress;
    returnStack[PARAMETER_OBJECT]=parameterObject;
    return returnStack;
}


const executableStack=(executableCounter,executableStackArray)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=executableStackOperator;
    returnStack[EXECUTABLE_COUNTER]=executableCounter;
    returnStack[EXECUTABLE_STACK]=executableStackArray;
    return returnStack;
}


const ALU=(firstOperand,operation,secondOperand)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=arithmaticLogicUnitOperator;
    returnStack[FIRST_OPERAND]=firstOperand;
    returnStack[ALU_OPERATION]=operationSelector(operation);
    returnStack[SECOND_OPERAND]=secondOperand||0;
    return returnStack;
}

const serverSend=(packageIdentifier,serverData)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=serverSendOperator;
    returnStack[PACKAGE_IDENTIFIER]=packageIdentifier;
    returnStack[SERVER_DATA]=serverData;
    return returnStack;
}


////////////////////////////////////////////////        end of json directives      ///////////////////////////////////////////////

var counter=0;
var staticTesting=0;

app.get('/',(req,res)=>{
    
    var finalStack=[];

    // finalStack.push(consoleLogger("just started "));


    // finalStack.push(delay(500));

    // finalStack.push(memoryWrite(100,{test:"memory"}));
    
    // finalStack.push(memoryWrite(101,memoryRead(100)));


    // finalStack.push(consoleLogger(memoryRead(101)));


    // finalStack.push(delay(500));

    // finalStack.push(memoryWrite(100,{test:"samer-15"}));
    // finalStack.push(consoleLogger(memoryRead(100)));
    // finalStack.push(delay(500));
    // finalStack.push(memoryWrite(100,{test:"samer-5"}));
    // finalStack.push(consoleLogger(memoryRead(100)));
    // finalStack.push(delay(500));
    // finalStack.push(memoryWrite(100,{test:"samer-96666666"}));
    // finalStack.push(consoleLogger(memoryRead(100)));
    // finalStack.push(delay(500));
    
    // finalStack.push(memoryWrite(100,memoryRead(100)));
    // finalStack.push(memoryWrite(100,memoryRead(100)));
    // finalStack.push(memoryWrite(100,memoryRead(100)));


    // finalStack.push(consoleLogger("hello world"));


    // finalStack.push(consoleLogger(digitalInput(100)));


    // finalStack.push(memoryWrite(100,controllerFetch("https://jsonplaceholder.typicode.com/todos/2")));
    

    
    
    // finalStack.push(controllerEventListener(
    //     63,
    //     58,
    //     digitalInput(69),
    //     controllerFetch("http://192.168.1.15:776",memoryRead(58),{test:1245})
    // ));


    // finalStack.push(delay(10000));


    // finalStack.push(controllerRemoveEventListener(63));

    // finalStack.push(consoleLogger(memoryRead(63)));
    
    // // // finalStack.push(memoryWrite(101,memoryRead(100)));


    // // finalStack.push(consoleLogger(memoryRead(101)));

    // finalStack.push(consoleLogger(memoryRead(500)));
    // // finalStack.push(consoleLogger(memoryRead(500)));

    // finalStack.push(delay(500));

    // finalStack.push(memoryWrite(100,{test:"samer"}));
    
    // finalStack.push(memoryWrite(101,memoryRead(100)));


    // finalStack.push(consoleLogger(memoryRead(101)));
    

    // finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(101),{test:1245}));
    // finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(500),{test:6544}));
     
    // if((counter++)&1)
    //     finalStack.push(memoryWrite(100,hardwareID()));
    // else   
    //     finalStack.push(memoryDelete(100));

    // finalStack.push(consoleLogger(memoryRead(100)));
    // finalStack.push(consoleLogger(consoleLogger(delay(20))));

    // finalStack.push(consoleLogger(memoryRead(500)));

    

    // finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(101),{test:1245}));
    // finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(500),{test:6544}));

    // finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(101)));

    // finalStack.push(controllerFetch("http://192.168.1.15:776",digitalOutput(1,[564,98])));

    
    // finalStack.push(controllerFetch("http://192.168.1.15:776",controllerFetch("https://jsonplaceholder.typicode.com/todos/2")));
    // finalStack.push(controllerFetch("http://192.168.1.15:776",controllerFetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json")));
    
    
    // finalStack.push(delay(100));
    
    // finalStack.push(controllerFetch("http://192.168.1.15:776",controllerFetch("https://jsonplaceholder.typicode.com/todos/1")));

    // const laodToMemory=newVariable();

    if(!(staticTesting++))
    finalStack.push(createFunction(
        1025,
        1026,
        ()=>{
            return executableStack(1,[
                // memoryWrite(laodToMemory,functionArgument("randomParam")),
                // executableStack(3,[
                //     consoleLogger(memoryRead(laodToMemory)),
                //     delay(1000)
                // ]),
                consoleLogger(functionArgument("randomParam")),
                functionArgument("randomStack"),
                delay(200),
                controllerFetch("http://192.168.1.15:776",functionArgument("randomParam"),functionArgument("callback")),

                serverSend(functionArgument("randomParam"),functionArgument("callback")),

                // callFunction(1025,{randomParam:digitalInput(69),callback:{object:889}})
                // controllerEventListener(
                //     63,
                //     58,
                //     digitalInput(69),
                //     controllerFetch("http://192.168.1.15:776",memoryRead(58),functionArgument("randomParam"))
                // )
            ])
            // return consoleLogger(functionArgument("randomParam"));
            // return controllerEventListener(
            //     63,
            //     58,
            //     digitalInput(69),
            //     controllerFetch("http://192.168.1.15:776",memoryRead(58),functionArgument("randomParam"))
            // )
        },
        hardwareID()
    ));

    finalStack.push(delay(1000));
    // finalStack.push(consoleLogger(memoryRead(1025)));

    // finalStack.push(consoleLogger(callFunction(1025,{randomParam:callFunction(1025,{randomParam:digitalInput(69),callback:{object:889}}),callback:{object:889}})));
    finalStack.push(callFunction(1025,{randomParam:digitalInput(69),callback:{object:889},randomStack:consoleLogger("this test!!")}));
    // finalStack.push(callFunction(1025,{randomParam:digitalInput(69),callback:{object:889}}));
    // finalStack.push(callFunction(1025,{randomParam:digitalInput(69),callback:{object:889}}));

    finalStack.push(delay(1000));
    // // finalStack.push(consoleLogger(memoryRead(1025)));

    // finalStack.push(callFunction(1025,{randomParam:"some random text"}));

    // finalStack.push(memoryWrite(141,'2'));

    // finalStack.push(consoleLogger(ALU(ALU(ALU(ALU(memoryRead(141),'*',2),'*',2),'*',2),'*',2)));
    // finalStack.push(consoleLogger(ALU(ALU(ALU(ALU(memoryRead(141),'*',2),'*',2),'*',2),'*',2)));

    // finalStack.push(consoleLogger(ALU(ALU(ALU(ALU(memoryRead(141),'*',2),'*',2),'*',2),'*',2)));
    // finalStack.push(consoleLogger(ALU(memoryRead(141),'+',255)));

    finalStack.push(memoryWrite(141,'2'));
    finalStack.push(executableStack(5,[
        consoleLogger(ALU(ALU(ALU(ALU(memoryRead(141),'*',2),'*',2),'*',2),'*',2)),
        delay(200)
    ]))


    
   
    res.send(JSON.stringify(loop(1,finalStack)));

    console.log(JSON.stringify(finalStack));
    console.log(util.inspect(JSON.parse(JSON.stringify(finalStack)), false, null, true));

});

var enduranceTest=0;

xtensaLinker.linkerSetAdd((data)=>{
    if(JSON.parse(data)[PACKAGE_IDENTIFIER]==6542){
        xtensaLinker.linkerSend(JSON.stringify(serverSend(6542,{res:enduranceTest++})));
    }
})

setTimeout(() => {
    xtensaLinker.linkerSend(JSON.stringify(serverSend(6542,{res:enduranceTest++})));
}, 3000);

app.post("/",(req,res)=>{

    console.log("post request body >> ",req.body);
    res.send("ack");
});


app.listen(port,()=>{
    // console.clear();
    console.log(`-------- server started @ port ${port}`);
});



/*








mcu.load(mcu.logger('code just started'))


let x=200;

var counter=0;

const increment=MCU.newVariable();
mcu.load(mcu.memoryWrite(increment,0));

// while(x--){
//     mcu.load(mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))),
//     mcu.load(mcu.logger(mcu.memoryRead(increment)))
//     // mcu.load(mcu.logger(mcu.ALU(counter++,'%',x+1)))
// }


mcu.load(mcu.loop(200,[
    mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
    mcu.logger(mcu.memoryRead(increment))
    
    // mcu.executableStack(2,[
    //     mcu.logger(`counter ${counter++}`),
    //     mcu.executableStack(3,[
    //         mcu.logger(`counter ${counter++}`)
    //     ])
    // ])
]))

x=200;

while(x--)
    mcu.load(mcu.logger(counter++))

// while(x--)
// mcu.load(mcu.loop(1,[
//     // mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
//     // mcu.logger(mcu.memoryRead(increment))
    
//     // mcu.executableStack(2,[
//     //     mcu.logger(`counter ${counter++}`),
//     //     mcu.executableStack(3,[
//     //         mcu.logger(`counter ${counter++}`)
//     //     ])
//     // ])
// ]))

// mcu.load(mcu.logger(`counter ${counter++}`))



// const function1=MCU.newVariable();
// const arguments1=MCU.newVariable();

// const function2=MCU.newVariable();
// const arguments2=MCU.newVariable();

// const increment=MCU.newVariable();

// mcu.load(mcu.memoryWrite(increment,0));

// load(MCU.createFunction(function1,arguments1,()=>{
//     // return mcu.logger(`counter  >> ${counter++}`);
//     // return MCU.memoryWrite(arguments1,MCU.ALU(MCU.functionArgument('arg1'),'*',MCU.functionArgument('arg2')));
//     return MCU.loop(1,[
//         // MCU.logger(MCU.functionArgument('arg1')),
//         // MCU.logger(MCU.functionArgument('arg2')),
//         mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
//         MCU.memoryWrite(arguments1,MCU.ALU(MCU.functionArgument('arg1'),'*',MCU.functionArgument('arg2'))),
//         MCU.memoryWrite(arguments1,mcu.ALU(mcu.memoryRead(increment),'*',mcu.memoryRead(arguments1)))
//         // MCU.callFunction(function1,{arg1:5,arg2:6})
        
//     ])
// },MCU.memoryRead(arguments1)))

// // load(MCU.createFunction(function2,arguments2,()=>{
// //     return MCU.executableStack(1,[
// //         MCU.logger(MCU.functionArgument('arg1')),
// //         MCU.logger(MCU.functionArgument('arg2')),
// //         MCU.memoryWrite(arguments2,MCU.ALU(MCU.functionArgument('arg1'),'+',MCU.functionArgument('arg2'))),
// //         // MCU.callFunction(function1,{arg1:5,arg2:6})
// //         // serverConsole(MCU.functionArgument('arg1')),
// //         // serverConsole(MCU.functionArgument('arg1'))
// //     ])
// // },MCU.memoryRead(arguments2)))


// // let testVariable=7;
// // while(testVariable--){
// //     // load(MCU.delay(500));
// //     serverConsole(MCU.ALU(5,'*',6))
// //     serverConsole('string is string')
// // }

// // load(MCU.memoryWrite(1400,5));

// // testVariable=7;
// // while(testVariable--){
    

// //     load(MCU.memoryWrite(1400,MCU.ALU(MCU.memoryRead(1400),'*',MCU.memoryRead(1400))));
    
// //     serverConsole(MCU.memoryRead(1400))
// // // }
// // x=5;
// // while(x--)
// // serverConsole(MCU.callFunction(function1,{arg1:8,arg2:8}))
// // serverConsole(MCU.callFunction(function1,{arg1:8,arg2:8}))

// // load(MCU.controllerEventListener(1005,1006,MCU.digitalInput(69),MCU.callFunction(function1,{arg1:8,arg2:6})));

// load(MCU.controllerEventListener(1005,1006,MCU.digitalInput(69),mcu.logger(MCU.callFunction(function1,{arg1:8,arg2:3}))));


serverConsole("hello world ");

// // // serverConsole(MCU.memoryRead(2008));
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

    
    
    // mcu.load(mcu.delay(500))
    mcu.load(mcu.led(0));
    
    // mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.executableStack(1,[
    //     // mcu.postLogger(mcu.operatorJson(mcu.PORT_VALUE,mcu.memoryRead(1001))),
    //     mcu.postLogger(mcu.memoryRead(1001)),
    //     // mcu.delay(100),
    //     // mcu.led(0),
    //     mcu.logger(mcu.memoryRead(1001))
    // ])))

    // mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.postLogger(mcu.memoryRead(1001))))
    mcu.load(mcu.controllerEventListener(2000,2001,mcu.inputPin(),mcu.logger(mcu.memoryRead(2001))))
    // mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.led(mcu.memoryRead(1001))))
    // mcu.load(mcu.controllerEventListener(1000,1001,mcu.inputPin(),mcu.led(mcu.operatorJson(mcu.PORT_VALUE,mcu.memoryRead(1001)))))

    // mcu.load(mcu.postLogger(mcu.inputPin(1001)))

    // var x10=10
    // while(x10--)
    //     mcu.load(mcu.logger('this is test'))

    
    // mcu.load(mcu.logger(mcu.postLogger('this is test')))

    // mcu.load(mcu.callFunction(newFunction,{state_1:1,state_2:0}))


    // let counter=100;
    // while(counter--)
    // mcu.load(mcu.executableStack(999,[
    //     // mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
    //     mcu.logger(mcu.inputPin())
    //     // mcu.executableStack(5,[
    //     //     mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1)),
    //     //     mcu.executableStack(5,[
    //     //         mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))
    //     //         // mcu.delay(5)
    //     //     ]),
    //     //     mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))
    //     // ]),
    //     // mcu.memoryWrite(increment,mcu.ALU(mcu.memoryRead(increment),'+',1))
    // ]))

    // // mcu.load(mcu.logger(mcu.memoryRead(increment)));

    mcu.load(mcu.logger('hello world'));

}


*/


