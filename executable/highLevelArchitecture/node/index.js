import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser from 'body-parser';

import mqttClient from "./mqttClient.js";
import pipeline from './pipeline.js';

const stringToBytes=inputString=>(Array.from(Array(inputString.length)).map((__,index)=>(inputString.charCodeAt(index))));

const bytesToString=inputArray=>{
    let returnString="";
    inputArray.forEach(element=>{
        returnString+=String.fromCharCode(element);
    })
    return returnString;
}

const uint32ArrayToBytesArray=array32bit=>(Array.from(Array(array32bit.length*4)).map((__,index)=>(0xFF&(array32bit[parseInt(index/4)]>>((index%4)*8)))));

console.clear();

const remoteClient=new mqttClient({serverAddress:'mqtt-dashboard.com',topic:mqttClient.MQTT_TOPIC,startup:()=>{
    // remoteClient.send("test node server");

    pipeline.onStart(data=>{

        console.log("data length >> ",JSON.stringify(data).length)

        remoteClient.send(data);
    })

    remoteClient.onData(data=>{

        // console.log(data);
        try {
            Object.keys(data).forEach(key=>{
                pipeline.CALL_BACKS?.[key](data[key]);
            })
        } catch (error) {
            
        }

    });


}});

const app =express();
app.use(cors());
app.use(bodyParser.json());
const port =766;
app.listen(port,()=>{
    console.log(`-------- server started @ port ${port}`);
});

app.get('/run',(req,res)=>{

    
    res.send('ack');

    // new pipeline().consoleLogger(new pipeline().fetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json")).run()
    // new pipeline().loop(500,new pipeline().consoleLogger("this is test this is test this is test this is test")).run();
    
    // (async()=>{
    //     // let test=await new pipeline().getValue(new pipeline().alop("5",pipeline.MATH_OPERATORS.ADD,"6"));
    //     let test=await new pipeline().getValue(new pipeline().fetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json"));
    //     // let test=await new pipeline().getValue(new pipeline().fetch("http://192.168.1.4:766/test",{test:10}));
    //     console.log("getValue >> ",test);
    // })();
    
    
    // new pipeline()

    //     .registerWrite(pipeline.REGISTERS.MQTT_TX,new pipeline().alop("5",pipeline.MATH_OPERATORS.ADD,"6"))
    //     .run()

        // .registerWrite(pipeline.REGISTERS.MQTT_TX,new pipeline().setObject(new pipeline().newObject("test0",new pipeline().alop("5",pipeline.MATH_OPERATORS.ADD,"6")),"test1",new pipeline().newObject("test0",new pipeline().alop("6",pipeline.MATH_OPERATORS.MUL,"6"))))
        // .run()

        // .startupScript("script1",new pipeline().consoleLogger("api machine started"))
        // .startupScript("script2",new pipeline().consoleLogger(" +++++++++++++ "))
        // .startupScript("script3",new pipeline().consoleLogger(" ------------- "))
        // .run()
        
        // .task("task1",50,new pipeline().consoleLogger("this is test from a running ............task1 "))
        // .task("task0",1,new pipeline().consoleLogger("this is test from a running task0 "))
        // .run()
        


        // .memoryWrite(10,10)
        // .storageWrite("consoleData","this is localStorage read Test")
        // .run()
        // // .consoleLogger(new pipeline().memoryRead(10))
        // // .run();
        // .loop(new pipeline().memoryRead(10),new pipeline().loop(new pipeline().memoryRead(10),new pipeline().consoleLogger(new pipeline().storageRead("consoleData")))).run();

    // new pipeline().loop(10,new pipeline().loop(10,new pipeline().loop(10,new pipeline().loop(10,new pipeline().consoleLogger("this is test this is test this is test this is test"))))).run();

    new pipeline().registerWrite("base64decode",btoa(bytesToString(uint32ArrayToBytesArray([
        0x00000001,
        0x00010003,
        0x000205FD,
        0x00F105FD
    ])))).run()
})

app.use("/test",(req,res)=>{
    res.send("testAck")
    console.log("request >> ",req.body);
})

app.post('/upload',(req,res)=>{
    res.send('ack');
    remoteClient.send("test node server");

    console.log(JSON.stringify(req.body));
});




