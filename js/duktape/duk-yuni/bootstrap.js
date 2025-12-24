print("Hello.\n");

const ptraddr = NCCC.corelib.util_ptraddr;

const identity = NCCC.make_nccc_call("bogus", 
                                     0,
                                     ptraddr,
                                     "l",
                                     "l");

function dumpargs(a,b,c,d){
    print(JSON.stringify([a,b,c,d]));
    print("\n");
    return 12.34;
}

print(identity(1234));



// Bootstrap library
const PortStd = {};
const PortNative = {};

PortStd.performance_now = performance.now;
PortStd.fs_readFileSync = function(nam, typ){
    if(typ == "utf8"){
        return BOOTSTRAP.readtext(nam);
    }else{
        return BOOTSTRAP.readbinary(nam);
    }
};
PortStd.crypto_randomFillSync = function(buf){
    abort();
};

PortNative.nccc = NCCC;

// https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/Array/fill
// Polyfill Array.fill
if (!Array.prototype.fill) {
  Object.defineProperty(Array.prototype, 'fill', {
    value: function(value) {

      // Steps 1-2.
      if (this == null) {
        throw new TypeError('this is null or not defined');
      }

      var O = Object(this);

      // Steps 3-5.
      var len = O.length >>> 0;

      // Steps 6-7.
      var start = arguments[1];
      var relativeStart = start >> 0;

      // Step 8.
      var k = relativeStart < 0 ?
        Math.max(len + relativeStart, 0) :
        Math.min(relativeStart, len);

      // Steps 9-10.
      var end = arguments[2];
      var relativeEnd = end === undefined ?
        len : end >> 0;

      // Step 11.
      var final = relativeEnd < 0 ?
        Math.max(len + relativeEnd, 0) :
        Math.min(relativeEnd, len);

      // Step 12.
      while (k < final) {
        O[k] = value;
        k++;
      }

      // Step 13.
      return O;
    }
  });
}

if(!Uint8Array.prototype.copyWithin){
    Object.defineProperty(Uint8Array.prototype, "copyWithin", {
        value: BOOTSTRAP.copywithin_uint8
    });
}

const console = {};

console.log = function(){
    var depth = 0;
    function is_array(obj){
        if(Object.prototype.toString.call(obj) === '[object Array]'){
            return true;
        }else{
            return false;
        }
    }
    function is_object(obj){
        if(Object.prototype.toString.call(obj) === '[object Object]'){
            return true;
        }else{
            return false;
        }
    }
    function dumpvalue(e){
        depth++
        if(depth > 10){
            print("...");
            return;
        }
        if(e && e.length > 100){
            print("(TOO_LONG)");
        }else if(is_array(e)){
            print("[");
            e.forEach(function(v){
                dumpvalue(v);
                print(",");
            });
            print("]");
        }else if(is_object(e)){
            print("{");
            const k = Object.keys(e);
            k.forEach(function(k){
                print(k);
                print(":");
                dumpvalue(e[k]);
            });
            print("}");
        }else{
            print(e);
        }
    }
    var args = Array.prototype.slice.call(arguments);
    args.forEach(function(e){
        dumpvalue(e);
        print(" ");
    });
    print("\n");
    depth = 0;
}

console.warn = console.log;
console.error = console.log;

Math.imul = function(){
    print("FIXME: imul");
    abort();
}

Math.fround = function(){
    print("FIXME: fround");
    abort();
}

Math.clz32 = BOOTSTRAP.clz32;

// https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/Math/trunc
if (!Math.trunc) {
  Math.trunc = function(v) {
    v = +v;
    if (!isFinite(v)) return v;

    return (v - v % 1)   ||   (v < 0 ? -0 : v === 0 ? v : 0);

    // returns:
    //  0        ->  0
    // -0        -> -0
    //  0.2      ->  0
    // -0.2      -> -0
    //  0.7      ->  0
    // -0.7      -> -0
    //  Infinity ->  Infinity
    // -Infinity -> -Infinity
    //  NaN      ->  NaN
    //  null     ->  0
  };
}

const eventqueue = [];

function setImmediate(func){
    setTimeout(func,0);
}

function setInterval(func, delay){
    print("FIXME: setInterval");
    setTimeout(function(){
        func();
        setTimeout(func, delay);
    }, delay);
}

function setTimeout(func, delay){
    //print("FIXME: setTimeout");
    //console.log("Queue",func,delay);
    eventqueue.push({func: func, delay: delay});
}

function clearInterval(){
    print("FIXME: clearInterval");
}


// Boot
const promisepolyfill = BOOTSTRAP.readtext("promise-polyfill.min.js");
if(promisepolyfill){
    eval(promisepolyfill);
}

const bootfile = BOOTSTRAP.readtext("output-duk/index.js");
if(bootfile){
    try {
        eval(bootfile);
    } catch (e) {
        print(e.stack || e);
    }
}

// MainLoop
while(eventqueue.length != 0){
    var x = eventqueue.shift();
    x.func();
}
