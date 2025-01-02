using System;
using System.IO.Ports;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Constants;

public class Tbot{
  public class Point{
    public int x=0;
    public int y=0;
  }

  public Point pickup;
  public Point Placement;

  public int FeedRateD1;
  public int FeedRateD2;

}