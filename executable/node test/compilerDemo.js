const express =require('express');
const app =express();
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

const consoleLogger=()=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=consoleLoggerOperator;

    return returnStack;
}

const hardwareID=()=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=hardwareIDOperator;

    return returnStack;
}

const memoryWrite=()=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=memoryWriteOperator;

    return returnStack;
}

const memoryRead=()=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=memoryReadOperator;

    return returnStack;
}

const memoryDelete=()=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=memoryDeleteOperator;

    return returnStack;
}

////////////////////////////////////////////////        end of json directives      ///////////////////////////////////////////////

app.get('/',(req,res)=>{
    console.log(req);
    var finalStack=[];

    finalStack.push(digitalOutput(65535,[1,2,78,563,9552]));
    finalStack.push(delay(100));

    finalStack.push(digitalOutput(65535,[1,2,999,9552]));
    finalStack.push(delay(100));

    
   
    res.send(JSON.stringify(loop(2,[loop(3,finalStack)])));
});

app.listen(port,()=>{
    console.log(`-------- server starte @ port ${port}`);
});