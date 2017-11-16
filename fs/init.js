load('api_config.js');
load('api_gpio.js');
load('api_net.js');
load('api_sys.js');
load('api_timer.js');

let led = Cfg.get('pins.led');
let getRssi = ffi('int wifi_station_get_rssi(void)');
let queueStart = ffi('bool start_queue()');
let queuePut = ffi('bool queue_put(char *)');
let queuePeek = ffi('char* queue_peek()');

let sequence = 0;


let getInfo = function() {
  return JSON.stringify({
    total_ram: Sys.total_ram(),
    free_ram: Sys.free_ram()
  });
};

// Blink built-in LED every second
GPIO.set_mode(led, GPIO.MODE_OUTPUT);
Timer.set(1000 /* 1 sec */, true /* repeat */, function() {
  let value = GPIO.toggle(led);
  print(value ? 'Tick' : 'Tock', 'uptime:', Sys.uptime(), getInfo(), getRssi());
}, null);


Timer.set(2000 /* 2 sec */, false /* repeat */, function() {
  let startResult = queueStart();
  print("Result: ", startResult);

  Timer.set(2000 /* 2 sec */, true /* repeat */, function() {
    print("##################### Running test function!");
    let result = queuePut(JSON.stringify({
      pm25: 100,
      pm10: 50,
      rssi: getRssi(),
      sequence: sequence,
      time: Timer.now()
    }));
    print("Result: ", result);

    sequence += 1;
  }, null);

  Timer.set(5000, true, function() {
    print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    let data = queuePeek();
    print("Data peeked:", data);
  }, null);
}, null);


// Monitor network connectivity.
Net.setStatusEventHandler(function(ev, arg) {
  let evs = '???';
  if (ev === Net.STATUS_DISCONNECTED) {
    evs = 'DISCONNECTED';
  } else if (ev === Net.STATUS_CONNECTING) {
    evs = 'CONNECTING';
  } else if (ev === Net.STATUS_CONNECTED) {
    evs = 'CONNECTED';
  } else if (ev === Net.STATUS_GOT_IP) {
    evs = 'GOT_IP';
  }
  print('== Net event:', ev, evs);
}, null);
