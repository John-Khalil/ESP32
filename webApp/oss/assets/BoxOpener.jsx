import * as React from "react";
const BoxOpener = (props) => (
  <svg viewBox="0 0 400 400" width={150} height={150} {...props}>
    <circle cx={200} cy={270} r={25} fill="#9e9e9e" strokeWidth={4} />
    <line
      x1={200}
      y1={270}
      x2={270}
      y2={270}
      stroke="#585656"
      strokeWidth={12}
      strokeLinecap="round"
    />
    <circle cx={270} cy={270} r={10} fill="#9e9e9e" />
    <rect
      x={190}
      y={50}
      width={20}
      height={60}
      fill="#9E9E9E"
      stroke="#616161"
      strokeWidth={2}
      rx={5}
    />
    <rect x={197} y={110} width={6} height={40} fill="#585656" />
    <circle cx={200} cy={155} r={10} fill="#9e9e9e" />
  </svg>
);
export default BoxOpener;
