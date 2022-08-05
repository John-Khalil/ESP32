const express =require('express');
const cors=require('cors');
const bodyParser = require('body-parser');


const app =express();
app.use(cors());

app.use(bodyParser.json());

const port =776;

app.post("/",(req,res)=>{
    res.send("ack");
    console.log("port request body >> ",req.body);
    
});



app.listen(port,()=>{
    console.log(`-------- server started @ port ${port}`);
});