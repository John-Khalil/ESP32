import "./globals.css";
import '../public/css/main.css';

export const metadata = {
  title: "oss",
  description: "omni smart solution",
};

export default function RootLayout({ children }) {
  console.log("this startup code");
  return (
    <html lang="en">
      <body className={``}>
        {children}
      </body>
    </html>
  );
}
