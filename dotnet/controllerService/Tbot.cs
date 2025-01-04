using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;

public class Tbot{
  public class Point{
    public int d1=0;  //x
    public int d2=0;  //y
  }
  public string id;

  public Point pickup;
  public Point Placement;

  public int FeedRateD1;
  public int FeedRateD2;
  public UInt32 en;
  public UInt32 dir1;
  public UInt32 dir2;
  public UInt32 clk;
  public UInt32 stepsPerUnit=1;

  


  public Tbot(dynamic setup){
    id=setup?.id??"";
    FeedRateD1=setup?.FeedRateD1;
    FeedRateD2=setup?.FeedRateD2;
    en=1<<setup?.en;
    dir1=1<<setup?.dir1;
    dir2=1<<setup?.dir2;
    clk=(1<<setup?.clk1)|(1<<setup?.clk2);
    stepsPerUnit=setup?.stepsPerUnit;
  }




}