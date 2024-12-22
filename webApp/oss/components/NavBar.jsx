// 'use client'
import React from 'react'
import Tbot from '@/assets/Tbot'
import { Stack,Box,Paper } from '@mui/material'

export  function NavBar2(props) {
  return (
    <nav className='appNavBar'>
			<div className='navItems'>
				{Array.from(Array(props?.navList?.length)).map((__,index)=>
					<div 
						className='navItem' 
						key={`appNavBar - ${index}`} 
						onClick={props?.navList[index]?.onClick??(()=>{})}
						>
						<h3 className='title'>
							{props?.navList[index]?.title}
						</h3>
						<div className='icon'>
							{props?.navList[index]?.icon}
						</div>
							
					</div>
				)}
			</div>
			<div className='notificationPanel'>
				{props?.notificationPanel.map((elem,index)=>
					<div key={`appNavBar-notificationPanel-${index}`}>
						{elem}
					</div>
				)}
			</div>
    </nav>
  )
}

export default function NavBar(props){
  return (
    <Stack direction="row" spacing={2}>
			<Box sx={{
				display:'flex',
				overflowX:'auto',
				position:'nowrap',
			}}>
				{Array.from(Array(props?.navList?.length)).map((__,index)=>
					<Stack key={`appNavBar - ${index}`} spacing={2} sx={{
						// display:'flex',
						// alignItems:'center'
					}}>
						<Paper elevation={3}>
							<Stack>
								<Box display="flex" justifyContent="center"
    alignItems="center" sx={{
									position:'absolute',
									paddingTop:'100px',
									width:'max-content',
									// alignItems:'center'
								}}>
									{props?.navList[index]?.title}
								</Box>
								{props?.navList[index]?.icon}
							</Stack>
						</Paper>
					</Stack>
				)}
			</Box>
			<Box sx={{
				
			}}>

			</Box>
		</Stack>
  )
}
