import Tbot from '@/assets/Tbot'
import React, { useReducer, useState } from 'react'
// import Stack from '@mui/material/Stack';
// import Grid2 from '@mui/material/Grid2';
// import { Box } from '@mui/system';
import { Stack,Grid2,Button,Box,Paper,Card,CardActionArea,Typography } from '@mui/material'
import { v4 as uuidv4 } from 'uuid'; 

const TbotControlsCenterPad=(props)=>{
  return(
    <>
      {Array.from(Array(2)).map((__,index)=>(
        (()=>{

          let[parameter,setParameter]=useReducer((state, action)=>{
            
            return (action>0)?action:1;
          },1);


          const abortController1 = new AbortController();
          const abortController2 = new AbortController();

          const[intervalID,setIntervalID]=useState({});


          abortController1.signal.addEventListener("abort", () => {
            clearInterval(intervalID.controller1);
          });
          abortController2.signal.addEventListener("abort", () => {
            clearInterval(intervalID.controller2);
          });
          return(
            <Stack
              key={uuidv4()}
              direction="row"
              gap={2}
              sx={{
                justifyContent: "space-around",
                alignItems: "center",
                paddingTop:'10px'
              }}
            >
          
              <Button color="error" variant="outlined"
                onMouseDown={()=>{
                  setParameter(parameter--)
                  setIntervalID({...intervalID,controller1:setInterval(() => {
                    setParameter(parameter--)
                  }, 100)});
                }}
                onMouseUp={()=>{
                  abortController1.abort()
                }}
                sx={{
                  width: 40, 
                  height: 40, 
                  borderRadius: "20%", 
                  borderWidth: 4,
                  minWidth: "unset",
                  display:'flex',
                  justifyContent:"center",
                  alignItems: "center",
                }
              }>
                <Typography gutterBottom variant="h6" component="div" sx={{
                  paddingTop:'6px'
                }}>
                  -
                </Typography> 
              </Button>
              <Paper elevation={12} sx={{
                flexGrow: 1,
                display:'flex',
                justifyContent:'center'
              }}>
                {`${parameter} ${["mm","mm/s"][index]}`}
              </Paper>
              <Button color="success" variant="outlined"
                onMouseDown={()=>{
                  setParameter(parameter++)
                  setIntervalID({...intervalID,controller2:setInterval(() => {
                    setParameter(parameter++)
                  }, 50)});
                }}
                onMouseUp={()=>{
                  abortController2.abort()
                }}
                sx={{
                  width: 40, 
                  height: 40, 
                  borderRadius: "20%",
                  borderWidth: 4,
                  minWidth: "unset",
                  display:'flex',
                  justifyContent:"center",
                  alignItems: "center",
                }
              }>
                <Typography gutterBottom variant="h6" component="div" sx={{
                  paddingTop:'6px'
                }}>
                  +
                </Typography> 
              </Button>
            </Stack>
        )})()
      ))}

    </>
  );
}

const TbotControlsTypography=(props)=>{
  return(
    <CardActionArea sx={{ height: "100%" }}>
      <Paper elevation={6} sx={{
				marginLeft: "auto",
				padding: "10px 16px",
				height: "100%",
        alignItems: "center",
				display: "flex",
        justifyContent:"center",
				borderRadius: "15px",
			}}>
        <Typography gutterBottom variant="h6" component="div">
          {props?.text}
        </Typography> 
      </Paper>
    </CardActionArea>
  )
}

export default function TbotControls() {
  const manualControlItemHieght=88;
  return (
    <>
      <Stack
        direction="row"
        justifyContent="space-between"
      >
        <Tbot width={500} height={500}/>
        <Stack>
          <Grid2 
            container 
            columns={4}
            spacing={2} 
            sx={{
              width: '400px',
              height: 'max-content',
            }}
          >
            <Grid2 size={1}/>
            <Grid2
              size={2}
              sx={{
                height: `${manualControlItemHieght}px`,
                borderRadius: "15px",
              }}
            >
              <TbotControlsTypography text={'D1+'}/>
            </Grid2>
            <Grid2 size={1}/>

            <Grid2
              size={1}
              sx={{
                height: `${manualControlItemHieght*2}px`,
                borderRadius: "15px",
              }}
            >
              <TbotControlsTypography text={'D2-'}/>
            </Grid2>
            <Grid2
              size={2}
              sx={{
                height: `${manualControlItemHieght*2}px`,
                borderRadius: "15px",
              }}
            >
            <TbotControlsCenterPad/>
            </Grid2>
            <Grid2
              size={1}
              sx={{
                height: `${manualControlItemHieght*2}px`,
                borderRadius: "15px",
              }}
            >
              <TbotControlsTypography text={'D2+'}/>
            </Grid2>

            <Grid2 size={1}/>
            <Grid2
              size={2}
              sx={{
                height: `${manualControlItemHieght}px`,
                borderRadius: "15px",
              }}
            >
            <TbotControlsTypography text={'D1-'}/>
            </Grid2>
            <Grid2 size={1}/>
          </Grid2>

        </Stack>
      </Stack>
      <Grid2 container>

      </Grid2>
    </>
  )
}
