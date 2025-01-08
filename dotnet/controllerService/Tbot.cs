using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;
using System.Dynamic;

public class Tbot{
  public class Point{
    public int d1=0;  //x
    public int d2=0;  //y
    public Point(int d1, int d2){
        this.d1 = d1;
        this.d2 = d2;
    }
  }
  public string serialPortID;

  public Point pickup;
  public Point placement;

  public int feedRateD1;
  public int feedRateD2;
  public UInt32 stepsPerUnit=1;

  public int vacuumON;

  private bool startPacking=false;
  public void start(){
    startPacking=true;
  }

  public Action cycleComplete=()=>{};
  public Action<object> movementUpdate=(object x)=>{};

  public List<int> placementFeedBack;

  public Func<UInt32> PositionFeedBack;

  public void init(dynamic setup){
    serialPortID=setup?.serialPortID;
    pickup=setup?.pickup;
    placement=setup?.placement;
    feedRateD1=setup?.feedRateD1;
    feedRateD2=setup?.feedRateD2;
    stepsPerUnit=setup?.stepsPerUnit;
    vacuumON=setup?.vacuumON;
    cycleComplete=setup?.cycleComplete;
    movementUpdate=setup?.movementUpdate;
    placementFeedBack=setup?.placementFeedBack;
    PositionFeedBack=setup?.PositionFeedBack;
  }

  public Tbot(object setup){
    init(setup);
    Task.Run(()=>{
      for(;;){
        while(!startPacking);

        var moveArm=(Point p,UInt32 feedRate)=>{
          dynamic TbotMove=new ExpandoObject();

          TbotMove.feedRate=feedRate;
          if(p.d1!=int.MinValue)
            TbotMove.d1=p.d1;
          if(p.d2!=int.MinValue)
            TbotMove.d2=p.d2;

          utils.appLinker[keys.TbotMove].value=new{
            port=serialPortID,
            value=TbotMove
          };

          utils.appLinker[keys.WebSocketBroadCast].value=new{
            eventID=keys.TbotMove,
            data=new{
              port=serialPortID,
              value=TbotMove
            }
          };
          
        };


        startPacking=false;
        cycleComplete();
      }
    });

  }




}