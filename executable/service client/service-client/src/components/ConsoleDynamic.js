import React,{ useState,useEffect,useRef } from "react"

const DynamicConsole=({userConsole})=>{
	let defaultTextColor='green';
	let defaultThemeColor='green';
	const consoleIdentifier=userConsole.consoleIdentifier||'all-users';

	const consoleInput=userConsole.consoleInput|false;
	const consoleOutput=userConsole.consoleOutput|true;

	return(
		<>
			<div className={`w-[calc(100% -24)] h-[320px] p-1 m-1 overflow-scroll rounded-lg bg-gray-900`} style={{
				border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
				color:userConsole.textColor||defaultTextColor
			}}>
				<div className={`w-[calc(100% -32)] p-1 m-1 overflow-scroll rounded-lg `} style={{
					border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
					color:userConsole.textColor||defaultTextColor
				}}>
					<input type="text" className="w-[calc(99.5%)] p-1 m-1 rounded-lg bg-gray-700" style={{
						border:`1px solid ${userConsole.themeColor||defaultThemeColor}`,
						color:userConsole.textColor||defaultTextColor
					}}/>
					<div className="float-rigth">
					<input type="checkbox" className="inline-flex float-right m-1 mr-3 p-1"/>
						<span className="inline-flex float-right mr-1 ">NL</span>
						
					</div>
				</div>

				
			</div>
		</>
	);
}

export default function ConsoleDynamic() {
	return (
		<>
			<DynamicConsole userConsole={{
				consoleInput:true,
				height:200,
				// themeColor:'rgb(0,0,0)'

			}}/>
		</>
	)
}
