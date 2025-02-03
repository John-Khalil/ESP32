import appLinker from "./utils";

export default class Keys {

    
    static SerialSend;
    static Input;
    static InputPullUp;
    static InputPullDown;
    static Output;
    static SetPin;
    static ClearPin;
    static ReadPin;
    static Clock;
    static AnalogRead;
    static WebSocket;
    static WebSocketBroadCast;
    static WebSocketClients;
    static TbotMove;
    static TbotPosition;


    static ws = class {
        static TbotPosition;
        static TbotManualControl;
        


        static {
            const properties = Object.getOwnPropertyNames(this);
            const validProperties = properties.filter(prop => 
                prop !== 'prototype' && 
                prop !== 'length' && 
                prop !== 'name' &&
                prop !== 'constructor'
            );
            
            validProperties.forEach(prop => {
                this[prop] = `ws-${prop}`;
                appLinker.addListener(this[prop],(data)=>{
                    appLinker.send("ws",
                        typeof data === "string" ? data : JSON.stringify(data)
                    )
                })
            });
        }
    }


    static {
        const properties = Object.getOwnPropertyNames(this);
        const validProperties = properties.filter(prop => 
            prop !== 'prototype' && 
            prop !== 'length' && 
            prop !== 'name' &&
            prop !== 'constructor' &&
            prop !== 'ws'
        );
        
        validProperties.forEach(prop => {
            this[prop] = prop;
        });
    }
}