import mqtt from "mqtt"

export default class mqttClient{

    static MQTT_TOPIC="elhLTFZibGtqYnZ2Z2l5WlhWbmMgWFpDVktIZmc=";

    readCallbackList=[];

    clientID=(Math.random() + 1).toString(36).substring(7);

    onData=readCallback=>{
        this.readCallbackList.push(readCallback);
    }

    send=()=>{};




    constructor({userName=null,password=null,topic,serverAddress,serverPort, startup=()=>{}}){
        const client = mqtt.connect(`mqtt://${serverAddress}`);

        

        client.on("connect",()=>{
            client.subscribe(topic,error=>{
                if(error)
                    console.log(`couldn't sub @ ${topic}`);
            });

            this.send=data=>{
                client.publish(topic,JSON.stringify({
                    id:this.clientID,
                    data
                }));
            }
    
            client.on("message",(ClientTopic,data)=>{
                this.readCallbackList.forEach(readCallback=>{
                    if(JSON.parse(data.toString()).id!=this.clientID)
                        readCallback(JSON.parse(data.toString()).data);
                });
            });

            (async()=>{
                startup();
            })();


        });




    }

    

    

}