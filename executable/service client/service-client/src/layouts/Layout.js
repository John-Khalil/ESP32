import React from 'react'
import {Link,Outlet} from 'react-router-dom'

export default function Layout() {
	const navBarStyleElements='felx p-1 text-center inline-block border border-sky-800 hover:bg-sky-600 w-18 font-bold';
  return (
    <>
			<nav className='m-1 p-1 text-right border border-sky-800 '>
				<Link to="/home"> <div className={navBarStyleElements}>Home</div> </Link>
      	<Link to="/console-dynamic"> <div className={navBarStyleElements}>Console</div> </Link>
				<Link to="/app-settings"> <div className={navBarStyleElements}>Settings</div> </Link>
			</nav>
			<div className='m-1 p-1 text center border border-sky-800'>
				<div className='w-full h-screen'>
					<Outlet/>
				</div>
			</div>
    </>
  )
}
