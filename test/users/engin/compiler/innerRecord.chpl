class outerClass {
  var field = 5;
  var r: innerRecord;
  // var r = new innerRecord(); //a possible workaround

  record innerRecord{
    proc innerRecordProc(){
      writeln(outer.field); //outer or no outer doesn't make a difference
    }
  }
}

var c = new outerClass();
c.r.innerRecordProc();
