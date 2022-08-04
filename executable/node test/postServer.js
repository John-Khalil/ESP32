const express =require('express');
const cors=require('cors');
const bodyParser = require('body-parser');


const app =express();
app.use(cors());

app.use(bodyParser.json());

const port =776;

app.post("/",(req,res)=>{
    console.log("port request body >> ",req.body);
    res.send("ack");
});



app.listen(port,()=>{
    console.log(`-------- server started @ port ${port}`);
});