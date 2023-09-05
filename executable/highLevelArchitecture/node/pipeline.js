

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
    static MEMORY_VALUE=       "MEMORY_VALUE";
    static TIMER=              "TIMER";
    static CLOCK_OUTPUT=       "CLOCK_OUTPUT";
    static STORAGE_WRITE=      "STORAGE_WRITE";
    static STORAGE_READ=       "STORAGE_READ";

    static LOOP_COUNTER=       "LOOP_COUNTER";
    static LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";

    callbackFunctionList=[];
    
    instructionsList=[];

    
    loop=(loopCounter,loopBody)=>{
        this.instructionsList.push({
            [pipeline.OPERATOR]:pipeline.LOOP,
            [pipeline.LOOP_COUNTER]:(typeof(loopCounter)==='object')?loopCounter.get()[0]:loopCounter,
            [pipeline.LOOP_ELEMENENTS]:(typeof(loopBody)==='object')?loopBody.get():[]
        })
        return this;
    }


    get=()=>{

        return this.instructionsList;
    }

    run=()=>{

        this.callbackFunctionList.forEach(callbackFunction => {
            callbackFunction(JSON.stringify({
                [pipeline.OPERATOR]:pipeline.LOOP,
                [pipeline.LOOP_COUNTER]:1,
                [pipeline.LOOP_ELEMENENTS]:this.instructionsList
            }));
        });

        return this;
    }

    onStart=callbackFunction=>{

        this.callbackFunctionList.push(callbackFunction);

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