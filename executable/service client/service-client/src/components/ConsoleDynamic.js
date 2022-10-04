import React,{ useState,useEffect,useRef } from "react"

const DynamicConsole=({userConsole})=>{
	let defaultTextColor='green';
	let defaultThemeColor='green';
	const consoleIdentifier=userConsole.consoleIdentifier||'ALL-USERS';

	const consoleInput=userConsole.consoleInput|false;
	const consoleOutput=userConsole.consoleOutput|true;

	const mainConsoleOutput=useRef();

	const clearMainConsoleOutput=()=>{
		mainConsoleOutput.current.innerHTML='';
	}

	useEffect(()=>{
		if(userConsole.clearConsole===true)
			clearMainConsoleOutput();

		if(userConsole.consoleData!==undefined){
			mainConsoleOutput.current.innerHTML+=userConsole.consoleData;
			mainConsoleOutput.current.scrollTop=mainConsoleOutput.current.scrollHeight;
		}

	},[userConsole]);

	return(
		<>
			<div className={`p-1 m-1 overflow-scroll rounded-lg bg-gray-900`} style={{
				border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
				color:userConsole.textColor||defaultTextColor
			}}>

				<div className={`w-[calc(100% -24)] p-1 m-1 overflow-scroll rounded-lg bg-black`} style={{
					border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
					color:'white',
					background:userConsole.themeColor||defaultThemeColor
				}}>
					<span className="float-left inline text-white font-bold" style={{}}>{consoleIdentifier}</span>
					<button className="float-right inline underline font-bold mr-3" onClick={()=>{
						clearMainConsoleOutput();
					}}>CLEAR</button>
				</div>

				{(consoleOutput)?(
					<pre className="w-[calc(100% -24)] h-[320px] m-1 p-1 overflow-scroll" ref={mainConsoleOutput}>
						ffs
					</pre>
				):(<></>)}

				{(consoleInput)?(
				
					<div className={`w-[calc(100% -24)] p-1 m-1 overflow-scroll rounded-lg bg-black`} style={{
						border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
						color:userConsole.textColor||defaultTextColor
					}}>

						<input type="text" className="w-[calc(75%)] p-1 m-1 rounded-lg bg-white mt-3" style={{
							border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
							color:userConsole.textColor||defaultTextColor
						}} placeholder=" > Enter Command"/>

						
						

						<div className="float-right p-1 block">
							<div className="block">
								<input type="checkbox" className="inline-block float-right m-1 mt-1.5 mr-1 p-1"/>
								<span className="inline-block float-right mr-1 text-white">NL</span>
							</div>
							<div className="block">
								<input type="checkbox" className="inline-block float-right m-1 mt-1.5 mr-1 p-1"/>
								<span className="inline-block float-right mr-1 text-white">CR</span>
							</div>
							
						</div>

						<button className={`m-1 p-1 pl-2 pr-2 inline rounded-lg float-right mt-3`} style={{
							border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
							color:"white",
							background:userConsole.themeColor||defaultThemeColor
						}}>Send</button>
					</div>
				
				):(<></>)}

				
			</div>
		</>
	);
}

export default function ConsoleDynamic() {
	const [testLog,testLogger]=useState('this is a sample text');
	let testCounter=0;
	useEffect(()=>{
		setInterval(() => {
			testLogger(`text @ ${testCounter++}\n`)
		}, 500);
	},[]);
	return (
		<>
			<DynamicConsole userConsole={{
				consoleInput:true,
				// clearConsole:true,
				// themeColor:'yellow',
				height:200,
				consoleData:testLog
				// textColor:'yellow'
				// themeColor:'rgb(0,0,0)'


			}}/>
		</>
	)
}
