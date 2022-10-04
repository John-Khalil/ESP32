import React, { useState,useEffect } from 'react'
import {Link,Outlet} from 'react-router-dom'

export default function Layout() {
	const navBarStyleElements='felx p-1 text-center inline-block border border-sky-800 hover:bg-sky-600 w-18 font-bold';
	const navBarHeight=67;
	const [windowHeight,setWindowHeight]=useState((window.innerHeight-navBarHeight))
	useEffect(()=>{
		window.addEventListener("resize",()=>{
			setWindowHeight(window.innerHeight-navBarHeight);
		},false);
	},[])
  return (
    <>
			<nav className='m-1 p-1 text-right border border-sky-800 '>
				<Link to="/home"> <div className={navBarStyleElements}>Home</div> </Link>
      	<Link to="/console-dynamic"> <div className={navBarStyleElements}>Console</div> </Link>
				<Link to="/app-settings"> <div className={navBarStyleElements}>Settings</div> </Link>
			</nav>
			<div className='m-1 p-1 text center border border-sky-800'>
				<div className='w-full divHeight overflow-scroll' style={{'--height':`${windowHeight}px`}}>
					<Outlet/>
				</div>
			</div>
    </>
  )
}
