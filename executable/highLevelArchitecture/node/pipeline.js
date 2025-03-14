

export default class pipeline{

    static CALL_BACKS={};
    static CALL_BACKS_COUNTER=0;

    static OPERATOR=           "OPERATOR";
    static DATA=               "DATA";

    static LOOP=               "LOOP";
    static CONSOLE_LOGGER=     "CONSOLE_LOGGER";
    static DIGITAL_OUTPUT=     "DIGITAL_OUTPUT";
    static DIGITAL_INPUT=      "DIGITAL_INPUT";
    static ANALOG_INPUT=       "ANALOG_INPUT";
    static PWM=                "PWM";
    static DELAY=              "DELAY";
    static SEND=               "SEND";
    static MEMORY_WRITE=       "MEMORY_WRITE";
    static MEMORY_READ=        "MEMORY_READ";
    static MEMORY_ADDRESS=     "MEMORY_ADDRESS";
    static MEMORY_DATA=        "MEMORY_DATA";
    static TIMER=              "TIMER";
    static CLOCK_OUTPUT=       "CLOCK_OUTPUT";
    static STORAGE_WRITE=      "STORAGE_WRITE";
    static STORAGE_READ=       "STORAGE_READ";
    static STORAGE_ADDRESS=    "STORAGE_ADDRESS";
    static STORAGE_DATA=       "STORAGE_DATA";

    static REGISTER_WRITE=     "REGISTER_WRITE";
    static REGISTER_READ=      "REGISTER_READ";
    static REGISTER_ADDRESS=   "REGISTER_ADDRESS";
    static REGISTER_DATA=      "REGISTER_DATA";

    static LOOP_COUNTER=       "LOOP_COUNTER";
    static LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";

    static THREAD_ADD=         "ADD_THREAD";
    static THREAD_ID=          "THREAD_ID";
    static THREAD_EXECUTABLE=  "THREAD_EXECUTABLE";
    static THREAD_PRIORITY=    "THREAD_PRIORITY";

    static STARTUP_ADD=        "STARTUP_ADD";
    static STARTUP_ID=         "STARTUP_ID";
    static STARTUP_SCRIPT=     "STARTUP_SCRIPT";

    static ALOP=                "ALOP";
    static FIRST_OPERAND=       "FIRST_OPERAND";
    static SECOND_OPERAND=      "SECOND_OPERAND";

    static SET_OBJECT=          "SET_OBJECT";
    static USER_OBJECT=         "USER_OBJECT";
    static USER_KEY=            "USER_KEY";
    static NEW_VALUE=           "NEW_VALUE";

    static FETCH_API=           "FETCH_API";
    static FETCH_URL=           "FETCH_URL";
    static FETCH_BODY=          "FETCH_BODY";

    static MATH_OPERATORS={
        ADD:"ADD",
        SUB:"SUB",
        MUL:"MUL",
        DIV:"DIV",
        MOD:"MOD",
        _2sComp:"_2sComp",
        XOR:"XOR",
        OR:"OR",
        AND:"AND",
        COMP:"COMP",
        SHIFT_RIGHT:"SHIFT_RIGHT",
        SHIFT_LEFT:"SHIFT_LEFT",
        EQUAL:"EQUAL",
        NOT_EQUAL:"NOT_EQUAL",
        LOGIC_AND:"LOGIC_AND",
        LOGIC_OR:"LOGIC_OR",
        NOT:"NOT",
        SMALLER:"SMALLER",
        GREATER:"GREATER",
        SMALLER_EQUAL:"SMALLER_EQUAL",
        GREATER_EQUAL:"GREATER_EQUAL"
    };

    static REGISTERS={
        GENERIC_RX:"WEB_SERVER_RX",
        MQTT_TX:"MQTT_TX",
        MQTT_RX:"MQTT_RX",
        WEB_SERVER_TX:"WEB_SERVER_TX",
        WEB_SERVER_RX:"WEB_SERVER_RX"
    };

    static ASYNC_VALUE_DEFAULT_REGISTER=pipeline.REGISTERS.MQTT_TX;


    static callbackFunctionList=[];
    fingerPrint='pipeline';
    
    instructionsList=[];

    get=(pipelineObject)=>{
        return (pipelineObject?.fingerPrint==this.fingerPrint)?pipelineObject.instructionsList[0]:pipelineObject;
    }

    getList=(pipelineObject)=>{
        return (pipelineObject?.fingerPrint==this.fingerPrint)?pipelineObject.instructionsList:[pipelineObject];
    }

    consoleLogger=consoleData=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.CONSOLE_LOGGER,
            [pipeline.DATA]:this.get(consoleData)
        })
        return this;
    }
    
    loop=(loopCounter,loopBody)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.LOOP,
            [pipeline.DATA]:{
                [pipeline.LOOP_COUNTER]:this.get(loopCounter),
                [pipeline.LOOP_ELEMENENTS]:this.getList(loopBody)
            }
        })
        return this;
    }

    memoryWrite=(memoryAddress,memoryData)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.MEMORY_WRITE,
            [pipeline.DATA]:{
                [pipeline.MEMORY_ADDRESS]:this.get(memoryAddress),
                [pipeline.MEMORY_DATA]:this.get(memoryData)
            }
        })
        return this;
    }

    memoryRead=(memoryAddress)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.MEMORY_READ,
            [pipeline.DATA]:{
                [pipeline.MEMORY_ADDRESS]:this.get(memoryAddress)
            }
        })
        return this;
    }

    storageWrite=(storageAddress,storageData)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.STORAGE_WRITE,
            [pipeline.DATA]:{
                [pipeline.STORAGE_ADDRESS]:this.get(storageAddress),
                [pipeline.STORAGE_DATA]:this.get(storageData)
            }
        })
        return this;
    }

    storageRead=(storageAddress)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.STORAGE_READ,
            [pipeline.DATA]:{
                [pipeline.STORAGE_ADDRESS]:this.get(storageAddress)
            }
        })
        return this;
    }

    task=(taskID,priority,task)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.THREAD_ADD,
            [pipeline.DATA]:{
                [pipeline.THREAD_ID]:this.get(taskID),
                [pipeline.THREAD_EXECUTABLE]:{
                    [pipeline.THREAD_PRIORITY]:this.get(priority),
                    [pipeline.THREAD_EXECUTABLE]:this.get(task),
                }
            }
        })
        return this;
    }

    startupScript=(startupID,startupScript)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.STARTUP_ADD,
            [pipeline.DATA]:{
                [pipeline.STARTUP_ID]:this.get(startupID),
                [pipeline.STARTUP_SCRIPT]:this.get(startupScript)
            }
        })
        return this;
    }

    alop=(firstOperand,alop,secondOperand)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.ALOP,
            [pipeline.DATA]:{
                [pipeline.FIRST_OPERAND]:this.get(firstOperand),
                [pipeline.ALOP]:this.get(alop),
                [pipeline.SECOND_OPERAND]:this.get(secondOperand)
            }
        })
        return this;
    }

    registerWrite=(registerAddress,registerData)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.REGISTER_WRITE,
            [pipeline.DATA]:{
                [pipeline.REGISTER_ADDRESS]:this.get(registerAddress),
                [pipeline.REGISTER_DATA]:this.get(registerData)
            }
        })
        return this;
    }

    registerRead=(registerAddress)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.REGISTER_READ,
            [pipeline.DATA]:{
                [pipeline.REGISTER_ADDRESS]:this.get(registerAddress)
            }
        })
        return this;
    }

    setObject=(userObject,userKey,newValue)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.SET_OBJECT,
            [pipeline.DATA]:{
                [pipeline.USER_OBJECT]:this.get(userObject),
                [pipeline.USER_KEY]:this.get(userKey),
                [pipeline.NEW_VALUE]:this.get(newValue)
            }
        })
        return this;
    }

    newObject=(userKey,newValue)=>{
        return this.setObject("{}",userKey,newValue);
    }

    fetch=(url,postBody)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.FETCH_API,
            [pipeline.DATA]:postBody?{
                [pipeline.FETCH_URL]:this.get(url),
                [pipeline.FETCH_BODY]:this.get(postBody)
            }:{
                [pipeline.FETCH_URL]:this.get(url)
            }
        })
        return this;
    }


    run=()=>{

        pipeline.callbackFunctionList.forEach(callbackFunction => {
            callbackFunction((this.instructionsList.length==1)?this.instructionsList[0]:{
                [pipeline.OPERATOR]:pipeline.LOOP,
                [pipeline.DATA]:{
                    [pipeline.LOOP_COUNTER]:1,
                    [pipeline.LOOP_ELEMENENTS]:this.instructionsList
                }
            });
        });

        this.instructionsList=[]

        return this;
    }

    getValue=(asyncValue,connectionRegister)=>{

        new pipeline().registerWrite((connectionRegister||pipeline.ASYNC_VALUE_DEFAULT_REGISTER),new pipeline().newObject(`pipeline_CALL_BACKS_${pipeline.CALL_BACKS_COUNTER}`,this.get(asyncValue))).run();
        return new Promise((resolve,reject)=>{
            pipeline.CALL_BACKS[`pipeline_CALL_BACKS_${pipeline.CALL_BACKS_COUNTER++}`]=(asyncReturnValue)=>{
                resolve(asyncReturnValue);
            }
        });
    }



    static onStart=callbackFunction=>{

        pipeline.callbackFunctionList.push(callbackFunction);

        return this;
    }

    clear=()=>{
        this.instructionsList=[];
        return this;
    }




    constructor(){
        return this;
    }    
}