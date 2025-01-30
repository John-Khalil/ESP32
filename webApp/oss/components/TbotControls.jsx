import Tbot from '@/assets/Tbot'
import React from 'react'
// import Stack from '@mui/material/Stack';
// import Grid2 from '@mui/material/Grid2';
// import { Box } from '@mui/system';
import { Stack,Grid2,Button,Box,Paper,Card,CardActionArea,Typography } from '@mui/material'

const TbotControlsCenterPad=(props)=>{
  return(
    <>
       <Stack
          direction="row"
          gap={2}
          sx={{
            justifyContent: "space-around",
            alignItems: "center",
          }}
        >
      
          <Button color="error" variant="outlined" sx={{
            width: 40, 
            height: 40, 
            borderRadius: "50%", 
            minWidth: "unset",
            display:'flex',
            justifyContent:"center",
            alignItems: "center",
          }}>
            <Typography gutterBottom variant="h6" component="div">
              -
            </Typography> 
          </Button>
          <Paper sx={{
            flexGrow: 1
          }}>
            test
          </Paper>
          <Button color="success" variant="outlined">
            <Typography gutterBottom variant="h6" component="div">
              +
            </Typography> 
          </Button>

        </Stack>
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
