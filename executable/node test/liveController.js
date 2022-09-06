import express  from 'express';
import cors from 'cors';
import util from 'util';
import bodyParser from 'body-parser';

import globalLinker from './globalLinker.js';
import virtualController from './virtualController.js';

console.clear();

const hostServerConfig='https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json';
const globalUserCredentials='anNvbiBkaXJlY3RpdmVzIHRlc3Qg';

const xtensaLinker=new globalLinker(hostServerConfig,globalUserCredentials);

const liveLoadOperator=(liveOperator)=>{
    // liveOperator=liveOperator();
    console.log(liveOperator);
}

let MCU=virtualController;

Object.keys(virtualController).forEach((operator)=>{
    MCU[operator]=()=>{
        liveLoadOperator(virtualController[operator]());
    }
})

MCU.consoleLogger("this is test");