import React from 'react'
import {Link,Outlet} from 'react-router-dom'

export default function Layout() {
	const navBarStyleElements='felx p-1 rounded text-center inline-block border border-sky-500 hover:bg-sky-500 w-16';
  return (
    <>
			<nav className='m-1 p-1 text center bg-black rounded'>
				<Link to="/home"> <div className={navBarStyleElements}>home</div> </Link>
      	<Link to="/console-dynamic"> <div className={navBarStyleElements}>console</div> </Link>
			</nav>
			<div className='m-1 p-1 text center'>
				<Outlet/>
			</div>
    </>
  )
}
