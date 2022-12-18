import './App.css';
import { BrowserRouter, Routes, Route } from "react-router-dom";

import Layout from './layouts/Layout';
import HomePage from './components/HomePage';
import ConsoleDynamic from './components/ConsoleDynamic';
import NotFound from './components/NotFound';
import AppSettings from './components/AppSettings';
import SmartHub from './components/SmartHub';




function App() {
	return (
		<BrowserRouter>
			<Routes>
				<Route path='/' element={<Layout/>}>
					<Route index element={<HomePage/>}/>
					<Route path="home" element={<HomePage />} />
					<Route path='smart-hub' element={<SmartHub/>}/>
					<Route path="console-dynamic" element={<ConsoleDynamic />} />
					<Route path="app-settings" element={<AppSettings />} />
					<Route path="*" element={<NotFound />} />
				</Route>
			</Routes>
		</BrowserRouter>
	);
}

export default App;
