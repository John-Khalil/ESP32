import * as React from "react";
const ConveyorBelt = (props) => (
  <svg
    viewBox="0 0 400 150"
    width={150}
    height={150}
    {...props}
  >
    <rect
      x={60}
      y={70}
      width={280}
      height={30}
      fill="#4CAF50"
      stroke="#2E7D32"
      strokeWidth={2}
      rx={15}
    />
    <rect x={70} y={75} width={260} height={20} fill="#212121" />
    <circle
      cx={70}
      cy={85}
      r={15}
      fill="#9E9E9E"
      stroke="#616161"
      strokeWidth={2}
    />
    <circle
      cx={330}
      cy={85}
      r={15}
      fill="#9E9E9E"
      stroke="#616161"
      strokeWidth={2}
    />
    <rect x={110} y={60} width={30} height={15} fill="#FFC107" rx={3} />
    <rect x={180} y={60} width={30} height={15} fill="#FF5722" rx={3} />
    <rect x={250} y={60} width={30} height={15} fill="#03A9F4" rx={3} />
  </svg>
);
export default ConveyorBelt;
