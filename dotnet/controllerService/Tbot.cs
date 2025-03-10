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

  public uint feedRateD1;
  public uint feedRateD2;
  public UInt32 stepsPerUnit=1;

  public int vacuum;
  public int roundCount;

  private bool startPacking=false;
  public void start(){
    startPacking=true;
  }

  public Action cycleComplete=()=>{};
  public Action<object> movementUpdate=(object x)=>{};

  public List<uint> placementFeedBack;

  public Func<UInt32> PositionFeedBack;

  public void home(){

  }

  public void vacuumControlInit(){
    utils.appLinker[keys.Output].value=new{
      port=serialPortID,
      value=vacuum
    };
  }

  public void vacuumControl(bool state){
    // Console.WriteLine("vacuumControl");
    utils.appLinker[(state==true)?keys.SetPin:keys.ClearPin].value=new{
      port=serialPortID,
      value=vacuum
    };
    // utils.appLinker[keys.WebSocketBroadCast].value=new Dictionary<string,object>{[keys.TbotMove]=new{
    //   serialPortID,
    //   vacuumControl=state
    // }};
  }

  public void placementFeedBackInit(){
    foreach(uint feedbackPin in placementFeedBack){
        utils.appLinker[keys.InputPullUp].value=new{
          port=serialPortID,
          value=feedbackPin
        };
    }
  }

  public bool itemsInPlace(){
    bool logicAccumulate=true;
    foreach(uint feedbackPin in placementFeedBack){
      logicAccumulate=logicAccumulate&&((int)utils.readPin(serialPortID,feedbackPin)==1);
      if(!logicAccumulate)
        break;
    }
    return logicAccumulate;
  }

  public void init(dynamic setup){
    serialPortID=setup?.serialPortID;
    pickup=setup?.pickup;
    placement=setup?.placement;
    feedRateD1=setup?.feedRateD1;
    feedRateD2=setup?.feedRateD2;
    vacuum=setup?.vacuum;
    // placementFeedBack=setup?.placementFeedBack;
    roundCount=setup?.roundCount;
    cycleComplete=setup?.cycleComplete;
    // stepsPerUnit=setup?.stepsPerUnit;             // apply a method later that would set the steps per unit
    // movementUpdate=setup?.movementUpdate;         //replaced by ws broadcast
    // PositionFeedBack=setup?.PositionFeedBack;     // mechanical feedback mechanism
  }

  public Tbot(object setup){
    init(setup);
    // placementFeedBackInit();
    vacuumControlInit();
    Task.Run(()=>{
      for(;;){
        while(!startPacking);


        // lamda exp to move the arm and provide user feedback
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

          // Console.WriteLine(JsonConvert.SerializeObject((utils.tbotPosition(serialPortID))));
          var currentPosition=JObject.Parse(utils.tbotPosition(serialPortID).ToString());
          currentPosition["serialPortID"]=serialPortID;
          // Console.WriteLine(JsonConvert.SerializeObject(currentPosition));
          utils.appLinker[keys.WebSocketBroadCast].value=new Dictionary<string,object>{[keys.TbotMove]=currentPosition};
        };


        // start cycle
        home();
        int loopCounter=0;
        while(loopCounter++<roundCount){
          moveArm(new Point(int.MinValue,placement.d2),feedRateD2);
          moveArm(new Point(int.MinValue,pickup.d2),feedRateD2);
          moveArm(new Point(pickup.d1,int.MinValue),feedRateD1);
          // while(!itemsInPlace());
          vacuumControl(true);
          moveArm(new Point(0,int.MinValue),feedRateD1);
          moveArm(new Point(int.MinValue,placement.d2),feedRateD2);
          moveArm(new Point(placement.d1,int.MinValue),feedRateD1);
          vacuumControl(false);
          moveArm(new Point(0,int.MinValue),feedRateD1);
        }



        startPacking=false;
        cycleComplete();
      }
    });

  }




}