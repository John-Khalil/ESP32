const express =require('express');
const app =express();
const port =766;



const digitalOutputOperator=0;
const delayOperator=1;
const loopOperator=2;

const delay=delayValueMS=>{
    return ({
        operator:delayOperator,
        delayValue:delayValueMS
    })
}

const digitalOutput=(outputPort,portStream)=>{
    return({
        operator:digitalOutputOperator,
        portAddress:outputPort,
        outputStream:portStream
    })
}

const loop=(_loopCounter,_loopBody)=>{
    return({
        operator:loopOperator,
        loopCounter:_loopCounter,
        loopBody:_loopBody
    })
}


app.get('/',(req,res)=>{
    console.log(req);
    var finalStack=[];
    finalStack.push(digitalOutput(65535,[1,2,78,563,9552]));
    finalStack.push(delay(500));
    finalStack.push(digitalOutput(65535,[1,2,78,563,9552]));
    finalStack.push(delay(500));
    finalStack.push(digitalOutput(65535,[1,2,78,563,9552]));
    finalStack.push(delay(500));
   
    res.send(JSON.stringify(loop(2,finalStack)));
});

app.listen(port,()=>{
    console.log(`-------- server starte @ port ${port}`);
});