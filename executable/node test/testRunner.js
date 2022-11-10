// let globalCounter=0;

// const asyncFunc=async ()=>{
//     let localCounter=globalCounter;
//     while(localCounter==globalCounter);
//     console.log(globalCounter);
//     asyncFunc();
// }

// asyncFunc();

// setInterval(() => {
//     globalCounter++;
// }, 500);

// //! as expected this didnt work



let globalCounter=0;
let localCounter;
const asyncFunc=async (recursiveCall=0)=>{
    if(!recursiveCall)
        localCounter=globalCounter;
    if(localCounter!=globalCounter){
        console.log(globalCounter);
        asyncFunc();
    }
    else{
        setTimeout(() => {
            asyncFunc(1);
        }, 0);
    }
        
}
asyncFunc();

setInterval(() => {
    globalCounter++;
}, 1);


