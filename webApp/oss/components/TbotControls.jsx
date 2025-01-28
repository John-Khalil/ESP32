import Tbot from '@/assets/Tbot'
import React from 'react'
// import Stack from '@mui/material/Stack';
// import Grid2 from '@mui/material/Grid2';
// import { Box } from '@mui/system';
import { Stack,Grid2,Button,Box,Paper,Card,CardActionArea,Typography } from '@mui/material'

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
              // width: {
              //   xs: '100%',     // Full width on extra small screens
              //   sm: '600px',    // Fixed width on small screens and up
              //   md: '800px'     // Different fixed width on medium screens
              // },
              width: '400px',
              height: 'max-content',
            }}
          >
            <Grid2 size={1}/>
            <Grid2
              size={2}
              sx={{
                backgroundColor:'#134e4a',
                height: `${manualControlItemHieght}px`,
              }}
            >
              D1+
            </Grid2>
            <Grid2 size={1}/>

            <Grid2
              size={1}
              sx={{
                backgroundColor:'#134e4a',
                height: `${manualControlItemHieght*2}px`,
              }}
            >
              D2-
            </Grid2>
            <Grid2
              size={2}
              sx={{
                backgroundColor:'#134e4a',
                height: `${manualControlItemHieght*2}px`,
              }}
            >
              test
            </Grid2>
            <Grid2
              size={1}
              sx={{
                backgroundColor:'#134e4a',
                height: `${manualControlItemHieght*2}px`,
              }}
            >
              D2+
            </Grid2>

            <Grid2 size={1}/>
            <Grid2
              size={2}
              sx={{
                backgroundColor:'#134e4a',
                height: `${manualControlItemHieght}px`,
              }}
            >
              D1-
            </Grid2>
            <Grid2 size={1}/>
          </Grid2>
          <Typography gutterBottom variant="h6" component="div">
            UNIT mm
          </Typography> 
          <Stack
            direction="row"
          >
            
          </Stack>

        </Stack>
      </Stack>
      <Grid2 container>

      </Grid2>
    </>
  )
}
