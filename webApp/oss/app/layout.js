import "./globals.css";
import '../public/css/main.css';

export const metadata = {
  title: "oss",
  description: "omni smart solution",
};

export default function RootLayout({ children }) {
  
  return (
    <html lang="en">
      <body className={``}>
        {children}
      </body>
    </html>
  );
}
