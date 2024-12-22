'use client';
// import "./globals.css";
import '../public/css/main.css';
import { ThemeProvider,createTheme } from '@mui/material/styles';
import { CssBaseline } from '@mui/material';

// export const metadata = (()=>{
//   'use server';
//   return({
//     title: "oss",
//     description: "omni smart solution",
//   });
// })();


export default function RootLayout({ children }) {
  
  return (
    <html lang="en">
      <body className={``}>
        <ThemeProvider theme={createTheme({
          // palette: {
          //   mode: 'dark',
          //   primary: {
          //     main: '#90caf9',  // A modern blue shade
          //   },
          //   secondary: {
          //     main: '#a5d6a7',  // A soft green shade
          //   },
          //   background: {
          //     default: '#121212',  // Material Design recommended dark background
          //     paper: '#1e1e1e',    // Slightly lighter for cards/surfaces
          //   },
          // }
        })}>
          <CssBaseline/>
          {children}
        </ThemeProvider>
      </body>
    </html>
  );
}
