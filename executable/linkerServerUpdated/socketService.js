const { link } = require('fs');
const net = require('net');
const WebSocket = require('ws');

const responseTime=50;
const xtensaRemotePort = 69;
const devRemotePort = 6969;



const linker={
    xtensaLinker:[],
    devLinker:[]
}

const authLinker=[];
var authKeyCounter=0;
function linkerIndex(credentials){
    for(var i=0;i<authKeyCounter;i++){
        if(credentials==authLinker[i])
            return i;
    }
    authLinker[authKeyCounter]=credentials;
    authKeyCounter++;
    return authKeyCounter-1;
}

function authKeyChecker(credentials){
    for(var i=0;i<authKeyCounter;i++){
        if(credentials==authLinker[i])
            return 1;
    }
    return 0;
}

function validJsonString(jsonStr){
    try{
        JSON.parse(jsonStr);
    }
    catch(err){
        return 0;
    }
    return 1;
}


const xtensaListener=new net.createServer();

xtensaListener.listen(xtensaRemotePort,()=>{
    console.log("\t--server started--\n");
});

xtensaListener.on('connection',xtesnaTcpConnection=>{
    var userCredentials=null;
    var xtensaPayload=null;
    var updatedRegister=0;
    var xtensaRegister=0;

    xtesnaTcpConnection.on('data',xtensaMessage=>{
        xtensaPayload=null;
        if(validJsonString(xtensaMessage.toString())){
            xtensaMessage=JSON.parse(xtensaMessage.toString());
            if(xtensaMessage.auth!=undefined){
                userCredentials=xtensaMessage.auth;
                if(xtensaMessage.payload!=undefined)
                    xtensaPayload=xtensaMessage.payload;
            }
            else if(userCredentials!=null){
                xtensaPayload=JSON.stringify(xtensaMessage);
            }
        }
        else if(userCredentials!=null){
            xtensaPayload=xtensaMessage.toString();
        }

        if(userCredentials!=null&&xtensaPayload!=null){
            var linkerObject={};
            linkerObject.data=xtensaPayload;
            linkerObject.updateRegister=Math.ceil(Math.random()*0xFFFFFFFE)+1;
            linker.xtensaLinker[linkerIndex(userCredentials)]=linkerObject;
            xtensaRegister=linkerObject.updateRegister;
        }
        
        console.log("authLinker >> ",authLinker);
        console.log("final Object >> ",linker);
    });

    xtesnaTcpConnection.on('error', function(err) {
        console.log(`Error: ${err}`);
    });

    xtesnaTcpConnection.on('end',()=>{
        console.log("dev@"+userCredentials+" disconnected");
        userCredentials=null;
    });

    setInterval(() => {
        if((userCredentials!=null)&&authKeyChecker(userCredentials)&&(linker.devLinker[linkerIndex(userCredentials)]!=undefined)){
            if(updatedRegister!=linker.devLinker[linkerIndex(userCredentials)].updateRegister){
                updatedRegister=linker.devLinker[linkerIndex(userCredentials)].updateRegister;
                xtesnaTcpConnection.write(linker.devLinker[linkerIndex(userCredentials)].data);
            }
        }

        setTimeout(()=>{
            if((userCredentials!=null)&&authKeyChecker(userCredentials)&&(linker.xtensaLinker[linkerIndex(userCredentials)]!=undefined)){
                if(xtensaRegister!=linker.xtensaLinker[linkerIndex(userCredentials)].updateRegister){
                    xtensaRegister=linker.xtensaLinker[linkerIndex(userCredentials)].updateRegister;
                    xtesnaTcpConnection.write(linker.xtensaLinker[linkerIndex(userCredentials)].data);
                }
            }
        },responseTime/2);
    },responseTime);
});


const devListener = new WebSocket.Server({ port: devRemotePort });

devListener.on('connection', devWebSocket => {
    var userCredentials=null;
    var devPayload=null;
    var updatedRegister=0;
    var devRegister=0;

    devWebSocket.on('message', devMessage => {
        devPayload=null;
        if(validJsonString(devMessage.toString())){
            devMessage=JSON.parse(devMessage.toString());
            if(devMessage.auth!=undefined){
                userCredentials=devMessage.auth;
                if(devMessage.payload!=undefined)
                    devPayload=devMessage.payload;
            }
            else if(userCredentials!=null){
                devPayload=JSON.stringify(devMessage);
            }
        }
        else if(userCredentials!=null){
            devPayload=devMessage.toString();
        }

        if(userCredentials!=null&&devPayload!=null){
            var linkerObject={};
            linkerObject.data=devPayload;
            linkerObject.updateRegister=Math.ceil(Math.random()*0xFFFFFFFE)+1;
            linker.devLinker[linkerIndex(userCredentials)]=linkerObject;
            devRegister=linkerObject.updateRegister;
        }

        console.log("authLinker >> ",authLinker);
        console.log("final Object >> ",linker);
    });

    devWebSocket.on('close',()=>{
        console.log("ws@"+userCredentials+" disconnected");
        userCredentials=null;
    });

    setInterval(() => {
        if((userCredentials!=null)&&authKeyChecker(userCredentials)&&(linker.xtensaLinker[linkerIndex(userCredentials)]!=undefined)){
            if(updatedRegister!=linker.xtensaLinker[linkerIndex(userCredentials)].updateRegister){
                updatedRegister=linker.xtensaLinker[linkerIndex(userCredentials)].updateRegister;
                devWebSocket.send(linker.xtensaLinker[linkerIndex(userCredentials)].data);
            }
        }

        setTimeout(()=>{
            if((userCredentials!=null)&&authKeyChecker(userCredentials)&&(linker.devLinker[linkerIndex(userCredentials)]!=undefined)){
                if(devRegister!=linker.devLinker[linkerIndex(userCredentials)].updateRegister){
                    devRegister=linker.devLinker[linkerIndex(userCredentials)].updateRegister;
                    devWebSocket.send(linker.devLinker[linkerIndex(userCredentials)].data);
                }
            }
        },responseTime/2);    
    },responseTime);
});