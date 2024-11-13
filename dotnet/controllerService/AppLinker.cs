class AppLinker{

    public class registeredMemoryElement{
        private List<Action<object>> writeCallBackList=[];
        private List<Action> readCallBackList=[];
        private bool disableWriteAction=false;
        private bool disableReadAction=false;

        public string key="";
        public long lastUpdate=0;
        private dynamic _value="";
        public dynamic value{
            get{
                if(!disableReadAction){
                    disableReadAction=true;
                    foreach(var readCallBack in readCallBackList)
                        readCallBack();
                    disableReadAction=false;
                }
                return _value;
            }
            set{
                _value=value;
                lastUpdate=getTime();
                // Console.WriteLine("val >> {0}",value);
                if(!disableWriteAction){
                    disableWriteAction=true;
                    foreach(var writeCallBack in writeCallBackList)
                        writeCallBack(value);
                    disableWriteAction=false;
                }
            }
        }

        public void setAction(Action<object> action){
            this.writeCallBackList.Add(action);
        }

        public void getAction(Action action){
            this.readCallBackList.Add(action);
        }
        
        // public static implicit operator dynamic(registeredMemoryElement insatnce){
        //     return "insatnce.value";
        // }

        public registeredMemoryElement(string name){
            key=name;
        }
        

    }

    public List<registeredMemoryElement> memoryElements=[];

    public registeredMemoryElement this[string key]{
        get{
            var memoryElement=memoryElements.Where(elem=>elem.key==key).ToList();
            if(memoryElement.Any())
                return memoryElement[0];

            memoryElements.Add(new registeredMemoryElement(key));
            return memoryElements[memoryElements.Count-1];
        }
        set{
            var memoryElement=memoryElements.Where(elem=>elem.key==key).ToList();
            if(memoryElement.Any()){
                memoryElement[0].value=value;
                return;
            }

            memoryElements.Add(new registeredMemoryElement(key));
            memoryElements[memoryElements.Count-1]=value;
        }
    }

    public static long getTime(){
        return DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
    }

    public static dynamic resolve(registeredMemoryElement elem){
        var currentTime=getTime();
        while(currentTime>elem.lastUpdate);
        return elem.value;
    }

    // public Action<Action<object>> this[string key]{

    // }

    



    // public static string operator [](string key){
    //     return "5";
    // }
    // public static void init(){
        

    //     Console.WriteLine("init");
    // }
}

