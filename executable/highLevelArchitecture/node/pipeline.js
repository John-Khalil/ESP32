

export default class pipeline{

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

    static LOOP_COUNTER=       "LOOP_COUNTER";
    static LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";

    static THREAD_ADD=         "ADD_THREAD";
    static THREAD_ID=          "THREAD_ID";
    static THREAD_EXECUTABLE=  "THREAD_EXECUTABLE";
    static THREAD_PRIORITY=    "THREAD_PRIORITY";

    static STARTUP_ADD=        "STARTUP_ADD";
    static STARTUP_ID=         "STARTUP_ID";
    static STARTUP_SCRIPT=     "STARTUP_SCRIPT";


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