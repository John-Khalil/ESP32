export const stringToBytes=inputString=>(Array.from(Array(inputString.length)).map((__,index)=>(inputString.charCodeAt(index))));

export const bytesToString=inputArray=>{
    let returnString="";
    inputArray.forEach(element=>{
        returnString+=String.fromCharCode(element);
    })
    return returnString;
}

const uint32ArrayToBytesArray=array32bit=>(Array.from(Array(array32bit.length*4)).map((__,index)=>(0xFF&(array32bit[parseInt(index/4)]>>((index%4)*8)))));

export const Base6432bitBinaries=_32bitList=>(btoa(bytesToString(uint32ArrayToBytesArray([..._32bitList]))));

export class AppLinker{
    usersList=[];

    addListener=(identifier, callBack)=>(this.usersList[identifier]||(this.usersList[identifier]=[])).push(callBack);

    send=(identifier, data)=>(this.usersList[identifier]||[]).map(callBack => {
        callBack(data);
    });
    
}

const baseAddress='BASE_ADDRESS_USER_STORAGE';

const userLocalStorage=()=>{
    if(localStorage.getItem(baseAddress)==null)
        localStorage.setItem(baseAddress,JSON.stringify({}));
    return JSON.parse(localStorage.getItem(baseAddress));
}

export const userStorage={
    set:(key,value)=>{
        const storedData=userLocalStorage();
        storedData[key]=value;
        localStorage.setItem(baseAddress,JSON.stringify(storedData));
        return JSON.parse(localStorage.getItem(baseAddress))[key];
    },
    get:(key,defaultValue)=>{
        return (defaultValue!=undefined)?(userStorage.get(key)?userLocalStorage()[key]:userStorage.set(key,defaultValue)):userLocalStorage()[key];
    },
    storage:(storageData)=>{
        if(storageData!=undefined)
            localStorage.setItem(baseAddress,JSON.stringify(storageData));
        return JSON.parse(localStorage.getItem(baseAddress));
    },
    userLocalStorage,
    baseAddress
}






export {


}




export class runOnce{
    runFlag=1;
    run=(oneTimeRun)=>{
        if(this.runFlag==1)
            oneTimeRun();
        this.runFlag=0;
    }
    constructor(){
        
    }
}


const appLinker =new AppLinker;


export default appLinker;
