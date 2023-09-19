

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

    static LOOP_COUNTER=       "LOOP_COUNTER";
    static LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";

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
                [pipeline.MEMORY_DATA]:pipeline.get(memoryData)
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