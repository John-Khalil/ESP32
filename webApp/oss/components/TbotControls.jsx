import Tbot from '@/assets/Tbot'
import React from 'react'
import Stack from '@mui/material/Stack';
import Grid2 from '@mui/material/Grid2';
import { Box } from '@mui/system';

export default function TbotControls() {
  return (
    <>
      <Stack>
        <Tbot width={500} height={500}/>
        <Grid2 
          container 
          columns={4}
          spacing={2} 
          sx={{ 
            width: {
              xs: '100%',     // Full width on extra small screens
              sm: '600px',    // Fixed width on small screens and up
              md: '800px'     // Different fixed width on medium screens
            },
            maxWidth: '100%', // Ensures responsiveness
            margin: '0 auto'
          }}
        >
          <Grid2
            columnSpan={3}
            rowSpan={2}
            columnStart={2}   // Starts at column 4
            rowStart={2}      // Starts at row 2
            sx={{
              backgroundColor:'red'
            }}
          >
            test
          </Grid2>

          <Grid2
            size={1}
            rowSpan={2}
            columnStart={2}   // Starts at column 4
            rowStart={2}      // Starts at row 2
            sx={{
              backgroundColor:'red'
            }}
          >
            test
          </Grid2>

        </Grid2>
      </Stack>
      <Grid2 container>

      </Grid2>
    </>
  )
}
