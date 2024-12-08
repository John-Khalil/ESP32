import * as React from "react";
const Tbot = (props) => (
  <svg viewBox="0 50 300 300" width={150} height={150} {...props}>
    <rect x={50} y={260} width={200} height={20} fill="#7D7D7D" />


    <rect
      x={50}
      y={150}
      width={200}
      height={20}
      fill="#7D7D7D"
    //   stroke="#ffffff"
      strokeWidth={2}
    />
    <line x1={65} y1={155} x2={235} y2={155} stroke="#c0c0c0" strokeWidth={2} />
    <line x1={65} y1={165} x2={235} y2={165} stroke="#c0c0c0" strokeWidth={2} />

    <rect
      x={140}
      y={110}
      width={20}
      height={130}
      fill="#585656"
    //   stroke="#2E7D32"
      strokeWidth={2}
    />

    <circle cx={150} cy={240} r={10} fill="#c0c0c0" />
    <line x1={80} y1={270} x2={220} y2={270} stroke="#585656" strokeWidth={4} />
  </svg>
);
export default Tbot;