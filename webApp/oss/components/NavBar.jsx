// 'use client'
import React from 'react'
import Tbot from '@/assets/Tbot'
import { Stack,Box,Paper,Card,CardActionArea,Typography } from '@mui/material'

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
    <Box sx={{
			display:'flex',
			padding:'10px 16px 10px 16px',
			gap:'16px',
			
		}}>
			<Box sx={{
				display:'flex',
				gap:'16px',
				width:'calc(100vw - 500px)',
				overflowX:'auto',
				position:'nowrap',
				position:'relative',
			}}>
				{Array.from(Array(props?.navList?.length)).map((__,index)=>
					<Card elevation={6} key={`appNavBar - ${index}`}>
						<CardActionArea>
						<Stack sx={{
							alignItems:'center',
							borderRadius:'10px',
						}}>
							<Box sx={{
								position:'absolute',
								paddingTop:'120px',
								width:'max-content',
							}}>
								<Typography gutterBottom variant="h6" component="div">
									{props?.navList[index]?.title}
								</Typography>
							</Box>
							<Box>
								{props?.navList[index]?.icon}
							</Box>
						</Stack>
						</CardActionArea>
					</Card>
				)}
			</Box>
			<Paper elevation={6} sx={{
				marginLeft: "auto",
				padding: "10px 16px",
				width: "350px",
				height: "160px",
				display: "flex",
				flexDirection: "column",
				alignItems: "flex-start",
				gap: "16px",
				overflowY: "auto",
				borderRadius: "15px",
			}}>
				{props?.notificationPanel.map((elem,index)=>
					<div key={`appNavBar-notificationPanel-${index}`}>
						{elem}
					</div>
				)}
			</Paper>
		</Box>
  )
}
