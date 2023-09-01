import mqttClient from "./mqttClient.js";

console.clear();

const remoteClient=new mqttClient({serverAddress:'mqtt-dashboard.com',topic:mqttClient.MQTT_TOPIC,startup:()=>{
    remoteClient.send("test node server");
    remoteClient.onData(data=>{
        console.log(data);
    })
}});


