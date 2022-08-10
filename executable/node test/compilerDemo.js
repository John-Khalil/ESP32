const express =require('express');
const cors=require('cors');
const bodyParser = require('body-parser');


const app =express();
app.use(cors());

app.use(bodyParser.json());

const port =766;

var operatorIndex=0;

const digitalOutputOperator=operatorIndex++;
const delayOperator=operatorIndex++;
const loopOperator=operatorIndex++;

const consoleLoggerOperator=operatorIndex++;
const hardwareIDOperator=operatorIndex++;
const memoryWriteOperator=operatorIndex++;
const memoryReadOperator=operatorIndex++;
const memoryDeleteOperator=operatorIndex++;

const fetchOperator=operatorIndex++;
const addEventOperator=operatorIndex++;



const JSON_OPERATOR="OP";				// generic operator


// digital output
const OUTPUT_STREAM="OS";				// digital output stream
const PORT_ADDRESS="PA";				// digital output port address



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

////////////////////////////////////////////////        end of json directives      ///////////////////////////////////////////////

var counter=0;

app.get('/',(req,res)=>{
    
    var finalStack=[];


    // finalStack.push(consoleLogger(memoryRead(100)));


    // finalStack.push(memoryWrite(100,controllerFetch("https://jsonplaceholder.typicode.com/todos/2")));
    
    
    
    finalStack.push(controllerEventListener(500,200,hardwareID(),hardwareID()));
    
    // finalStack.push(memoryWrite(101,memoryRead(100)));


    // finalStack.push(consoleLogger(memoryRead(101)));

    finalStack.push(consoleLogger(memoryRead(500)));
    // finalStack.push(consoleLogger(memoryRead(500)));

    finalStack.push(delay(500));

    finalStack.push(memoryWrite(100,{test:"samer"}));
    
    finalStack.push(memoryWrite(101,memoryRead(100)));


    finalStack.push(consoleLogger(memoryRead(101)));
    

    finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(101),{test:1245}));
    finalStack.push(controllerFetch("http://192.168.1.15:776",memoryRead(500),{test:6544}));
     
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
    
   
    res.send(JSON.stringify(loop(1,finalStack)));

    console.log(JSON.stringify(finalStack));

});

app.post("/",(req,res)=>{
    console.log("post request body >> ",req.body);
    res.send("ack");
});



app.listen(port,()=>{
    console.clear();
    console.log(`-------- server started @ port ${port}`);
});