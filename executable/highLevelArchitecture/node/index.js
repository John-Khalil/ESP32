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
    new pipeline().loop(5,new pipeline().consoleLogger("this is test this is test this is test this is test")).run();

    // new pipeline().loop(10,new pipeline().loop(10,new pipeline().loop(10,new pipeline().loop(10,new pipeline().consoleLogger("this is test this is test this is test this is test"))))).run();
})

app.post('/upload',(req,res)=>{
    res.send('ack');
    remoteClient.send("test node server");

    console.log(JSON.stringify(req.body));
});


