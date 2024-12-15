// 'use client'
import React from 'react'
import Tbot from '@/assets/Tbot'

export default function NavBar(props) {
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
