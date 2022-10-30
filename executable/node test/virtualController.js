var variableMemoryBaseAddress=100;
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

const adcReadOperator=operatorIndex++;
const timerOperator=operatorIndex++;

const serialAddOperator=operatorIndex++;
const serialSendOperator=operatorIndex++;
const serialReadOperator=operatorIndex++;


const JSON_OPERATOR="OP";				// generic operator


// digital output & input
const OUTPUT_STREAM="OS";				// digital output stream
const PORT_ADDRESS="PA";				// digital output port address
const PORT_VALUE="PV";
const OUTPUT_INDEX="I";


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

// adc read operator
const ADC_CHANNEL="AC";

//timer operator
const TIMER_SELECTOR="TS";
const millis=0;
const micros=1;


// serial port operator
const RX_PIN="RX";
const TX_PIN="TX";
const BAUD_RATE="BR";
const SERIAL_IDENTIFIER="SI";
const RX_ADDRESS="RA";
const SERIAL_EXECUTABLE="SE";
const SERIAL_DATA="SD";
const SERIAL_INDEX="SI";


const delay=delayValueMS=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=delayOperator;
    returnStack[DELAY_MILLI_SEC]=delayValueMS;
    return returnStack;
}

const digitalOutput=(portWidth,startBit,outputPort,portStream)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=digitalOutputOperator;

    returnStack[PORT_ADDRESS]=(((portWidth-1)&((1<<6)-1))<<22)|((startBit&((1<<6)-1))<<16)|(outputPort&0xFFFF);
    portStream.forEach((element,index) => {
        returnStack[`${OUTPUT_INDEX}${index}`]=element;    
    });
    
    return returnStack;
}

const digitalInput=(portWidth,startBit,inputPort)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=digitalInputOperator;
    returnStack[PORT_ADDRESS]=(((portWidth-1)&((1<<6)-1))<<22)|((startBit&((1<<6)-1))<<16)|(inputPort&0xFFFF);
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

const adcRead=(adcChannel)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=adcReadOperator;
    returnStack[ADC_CHANNEL]=adcChannel;
    return returnStack;
}

const timer=(timerSelector=millis)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=timerOperator;
    returnStack[TIMER_SELECTOR]=timerSelector;
    return returnStack;
}

const serialPortAdd=(rxPin,txPin,baudRate,serialIdentifier,rxAddress,serialExecutable)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=serialAddOperator;
    returnStack[RX_PIN]=rxPin;
    returnStack[TX_PIN]=txPin;
    returnStack[BAUD_RATE]=baudRate;
    returnStack[SERIAL_IDENTIFIER]=serialIdentifier;
    returnStack[RX_ADDRESS]=rxAddress;
    returnStack[SERIAL_EXECUTABLE]=serialExecutable;
    return returnStack;
}

const serialPortSend=(serialIdentifier,serialData)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=serialSendOperator;
    returnStack[SERIAL_IDENTIFIER]=serialIdentifier;
    returnStack[SERIAL_DATA]=serialData;
    return returnStack;
}

const serialPortRead=(serialIdentifier)=>{
    var returnStack={};
    returnStack[JSON_OPERATOR]=serialReadOperator;
    returnStack[SERIAL_IDENTIFIER]=serialIdentifier;
    return returnStack;
}

const virtualController={
    JSON_OPERATOR,
    OUTPUT_STREAM,
    PORT_ADDRESS,
    PORT_VALUE,
    OUTPUT_INDEX,
    DELAY_MICRO_SEC,
    DELAY_MILLI_SEC,
    LOOP_COUNTER,
    LOOP_BODY,
    BUFFER_IDENTIFIER,
    BUFFER_DATA,
    CONSOLE_DATA,
    WEB_HOST,
    POST_BODY,
    REQUEST_PARAM,
    EVENT_ADDRESS,
    EVENT_EXECUTABLE,
    HANDLER_EXECUTABLE,
    ONCHANGE_ADDRESS,
    POLLING_RATE,
    OBJECT_KEY,
    EXECUTABLE_JSON,
    FUNCTION_ADDRESS,
    FUNCTION_OBJECT,
    PARAMETER_ADDRESS,
    STACK_EXECUTABLE,
    RETURN_EXECUTABLE,
    PARAMETER_OBJECT,
    EXECUTABLE_COUNTER,
    EXECUTABLE_STACK,
    ALU_OPERATION,
    FIRST_OPERAND,
    SECOND_OPERAND,
    PACKAGE_IDENTIFIER,
    SERVER_DATA,
    ADC_CHANNEL,
    TIMER_SELECTOR,
    millis,
    micros,
    RX_PIN,
    TX_PIN,
    BAUD_RATE,
    SERIAL_IDENTIFIER,
    RX_ADDRESS,
    SERIAL_EXECUTABLE,
    SERIAL_DATA,
    SERIAL_INDEX,
    newVariable,
    operationSelector,
    delay,
    digitalOutput,
    digitalInput,
    loop,
    consoleLogger,
    hardwareID,
    memoryWrite,
    memoryRead,
    memoryDelete,
    controllerFetch,
    fetch:controllerFetch,
    controllerEventListener,
    controllerRemoveEventListener,
    controllerEventPollingRate,
    operatorJson,
    functionArgument,
    createFunction,
    callFunction,
    executableStack,
    ALU,
    serverSend,
    adcRead,
    timer,
    serialPortAdd,
    serialPortSend,
    serialPortRead
}

export default virtualController;





export class  virtualControllerClass{
    stackPush=1;
    
    executableStackPush=(executableObject)=>{
        if(this.stackPush)
            this.executableStackPushCallBack(executableObject);
    }

    variableMemoryBaseAddress=0;
    newVariable=()=>{
        return this.variableMemoryBaseAddress++;
    }
        

    delay=delayValueMS=>{
        
        var returnStack={};
        returnStack[JSON_OPERATOR]=delayOperator;
        returnStack[DELAY_MILLI_SEC]=delayValueMS;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    digitalOutput=(outputPort,portStream)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=digitalOutputOperator;
        returnStack[PORT_ADDRESS]=outputPort;
        returnStack[OUTPUT_STREAM]=portStream;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    digitalInput=(inputPort)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=digitalInputOperator;
        returnStack[PORT_ADDRESS]=inputPort;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    loop=(_loopCounter,_loopBody)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=loopOperator;
        returnStack[LOOP_COUNTER]=_loopCounter;
        returnStack[LOOP_BODY]=_loopBody;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    consoleLogger=(consoleData)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=consoleLoggerOperator;
        returnStack[CONSOLE_DATA]=consoleData;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    hardwareID=()=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=hardwareIDOperator;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    memoryWrite=(bufferIdentifier,bufferData)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=memoryWriteOperator;
        returnStack[BUFFER_IDENTIFIER]=bufferIdentifier;
        returnStack[BUFFER_DATA]=bufferData;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    memoryRead=(bufferIdentifier)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=memoryReadOperator;
        returnStack[BUFFER_IDENTIFIER]=bufferIdentifier;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    memoryDelete=(bufferIdentifier)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=memoryDeleteOperator;
        returnStack[BUFFER_IDENTIFIER]=bufferIdentifier;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    controllerFetch=(fetchWebHost,fetchData,fetchParams)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=fetchOperator;
        returnStack[WEB_HOST]=fetchWebHost;
        if(fetchData!==undefined)
            returnStack[POST_BODY]=fetchData;
        if((fetchParams!=undefined)&&(fetchData!==undefined))
            returnStack[REQUEST_PARAM]=fetchParams;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    controllerEventListener=(eventAddress,onchangeAddress,eventExecutable,handlerExecutable)=>{
        var returnStack={};
        var nestedReturnStack={};
        nestedReturnStack[EVENT_EXECUTABLE]=eventExecutable;
        nestedReturnStack[HANDLER_EXECUTABLE]=handlerExecutable;
        nestedReturnStack[ONCHANGE_ADDRESS]=onchangeAddress;
        
        returnStack[JSON_OPERATOR]=addEventOperator;
        returnStack[EVENT_ADDRESS]=eventAddress;
        returnStack[EVENT_EXECUTABLE]=nestedReturnStack;
        this.executableStackPush(returnStack);
        return returnStack;
    }


    controllerRemoveEventListener=(eventAddress)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=removeEventOperator;
        returnStack[EVENT_ADDRESS]=eventAddress;
        this.executableStackPush(returnStack);
        return returnStack;
    }


    controllerEventPollingRate=(pollingRate)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=setEventPollingOperator;
        returnStack[POLLING_RATE]=pollingRate;
        this.executableStackPush(returnStack);
        return returnStack;
    }


    operatorJson=(objectKey,jsonObject)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=operatorJsonOperator;
        returnStack[OBJECT_KEY]=objectKey;
        returnStack[EXECUTABLE_JSON]=jsonObject;
        this.executableStackPush(returnStack);
        return returnStack;
    }


    creatingFunctionParameterAddress=0;
    functionArgument=(parameter)=>{
        return this.operatorJson(parameter,memoryRead(this.creatingFunctionParameterAddress));
    }


    createFunction=(functionAddress,parameterAddress,stackExecutable,returnExecutable)=>{
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
        this.executableStackPush(returnStack);
        return returnStack;
    }


    callFunction=(functionAddress,parameterObject)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=callFunctionOperator;
        returnStack[FUNCTION_ADDRESS]=functionAddress;
        returnStack[PARAMETER_OBJECT]=parameterObject;
        this.executableStackPush(returnStack);
        return returnStack;
    }


    executableStack=(executableCounter,executableStackArray)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=executableStackOperator;
        returnStack[EXECUTABLE_COUNTER]=executableCounter;
        returnStack[EXECUTABLE_STACK]=executableStackArray;
        this.executableStackPush(returnStack);
        return returnStack;
    }


    ALU=(firstOperand,operation,secondOperand)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=arithmaticLogicUnitOperator;
        returnStack[FIRST_OPERAND]=firstOperand;
        returnStack[ALU_OPERATION]=operationSelector(operation);
        returnStack[SECOND_OPERAND]=secondOperand||0;
        this.executableStackPush(returnStack);
        return returnStack;
    }

    serverSend=(packageIdentifier,serverData)=>{
        var returnStack={};
        returnStack[JSON_OPERATOR]=serverSendOperator;
        returnStack[PACKAGE_IDENTIFIER]=packageIdentifier;
        returnStack[SERVER_DATA]=serverData;
        this.executableStackPush(returnStack);
        return returnStack;
    }
};
