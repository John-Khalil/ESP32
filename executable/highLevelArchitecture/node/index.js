import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser from 'body-parser';

import mqttClient from "./mqttClient.js";
import pipeline from './pipeline.js';

console.clear();

const remoteClient=new mqttClient({serverAddress:'mqtt-dashboard.com',topic:mqttClient.MQTT_TOPIC,startup:()=>{
    // remoteClient.send("test node server");

    pipeline.onStart(data=>{

        console.log(JSON.stringify(data).length)

        remoteClient.send(data);
    })

    remoteClient.onData(data=>{
        console.log(data);
    });


}});

const app =express();
app.use(cors());
app.use(bodyParser.json());
const port =766;
app.listen(port,()=>{
    console.log(`-------- server started @ port ${port}`);
});

app.get('/',(req,res)=>{

    
    res.send('ack');

    // new pipeline().consoleLogger("this is test this is test this is test this is test").run()
    // new pipeline().loop(500,new pipeline().consoleLogger("this is test this is test this is test this is test")).run();
    
    
    
    new pipeline()

        .consoleLogger(new pipeline().alop("5",pipeline.MATH_OPERATORS.ADD,"6"))
        .run()

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
})

app.post('/upload',(req,res)=>{
    res.send('ack');
    remoteClient.send("test node server");

    console.log(JSON.stringify(req.body));
});


