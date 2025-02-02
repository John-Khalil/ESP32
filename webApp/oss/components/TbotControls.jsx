import Tbot from '@/assets/Tbot'
import React, { useReducer, useState } from 'react'
// import Stack from '@mui/material/Stack';
// import Grid2 from '@mui/material/Grid2';
// import { Box } from '@mui/system';
import { Stack,Grid2,Button,Box,Paper,Card,CardActionArea,Typography,TextField } from '@mui/material'
import { FormControl, InputLabel, Select, MenuItem } from "@mui/material";
import { v4 as uuidv4 } from 'uuid'; 

const ControlForm=(props)=>{
  const availablePorts=['COM31','COM16']
  const [serialPort,setSerialPort]=useReducer((state,action)=>{

    return action;
  },availablePorts[0]);
  return (
    <>
      <Grid2 
        container 
        spacing={2}
        sx={{
          width: '400px',
          height: '100%',
          padding:'20px'
        }
      }>
        <Grid2 size={12}>
          <FormControl fullWidth>
            <InputLabel>Serial Port</InputLabel>
            <Select
              value={serialPort}
              onChange={(e) => setSerialPort(e.target.value)}
              label="Serial Port"
            >
              {availablePorts.map(elem=><MenuItem value={elem}>{elem}</MenuItem>)}
            </Select>
          </FormControl>
        </Grid2>
        {([
          {
            label:"Pickup D1",
            ref:null,
            onChange:(state)=>{

            }
          },
          {
            label:"Pickup D2",
            ref:null,
            onChange:(state)=>{

            }
          },
          {
            label:"Placement D1",
            ref:null,
            onChange:(state)=>{

            }
          },
          {
            label:"Placement D2",
            ref:null,
            onChange:(state)=>{

            }
          },
          {
            label:"Steps/mm",
            ref:null,
            onChange:(state)=>{

            }
          },
          {
            label:"Round Count",
            ref:null,
            onChange:(state)=>{

            }
          },
        ]).map(elem=>(
          <Grid2 size={6}>
            <TextField 
              label={elem?.label}
              ref={elem?.ref}
              type="number" 
              variant="outlined"
              onChange={event=>{
                elem.onChange(event.target.value)
              }}
            />
          </Grid2>
        ))}
        <Grid2 size={12}>
          <Button variant='contained' color='success' sx={{
            width:'100%'
          }}>
            Save
          </Button>
        </Grid2>
      </Grid2>
    </>
  )
}

const TbotControlsCenterPad=(props)=>{
  return(
    <>

      <Paper elevation={12} sx={{padding:'3px'}}>
        <Grid2 container spacing={2}>
          <Grid2 size={12} sx={{
            display:'flex',
            justifyContent:'center'
          }}>
            Vacuum / Pickup
          </Grid2>
          <Grid2 size={6}>
            <Button color="error" variant="contained" sx={{width:'100%', color: 'white' }}>
              OFF
            </Button>
          </Grid2>
          <Grid2 size={6}>
            <Button color="success" variant="contained" sx={{width:'100%', color: 'white' }}>
              ON
            </Button>
          </Grid2>
        </Grid2>
      </Paper>
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
                onMouseOut={()=>{
                  abortController1.abort()
                }}
                // onMouseLeave={()=>{
                //   abortController1.abort()
                // }}
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
                onMouseOut={()=>{
                  abortController2.abort()
                }}
                // onMouseLeave={()=>{
                //   abortController1.abort()
                // }}
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

const TbotControlPad=(props)=>{
  const manualControlItemHieght=88;
  return(
    <Stack sx={{
      padding:'5px'
    }}>
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
        {/* <TbotControlsCenterPad/> */}
        {props.children}
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
  )
}

export default function TbotControls() {
  return (
    <Paper>
      <Stack
        direction="row"
        justifyContent="space-between"
        sx={{ height: '400px' }}
      >
        <Box sx={{
          height:'100%'
          // flexGrow:1
        }}>

          <Tbot height={'100%'}/>
        </Box>
        <ControlForm/>

        <TbotControlPad>
          <TbotControlsCenterPad/>
        </TbotControlPad>

        
      </Stack>
    </Paper>
  )
}
