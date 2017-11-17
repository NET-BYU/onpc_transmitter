load('api_config.js');
load('api_gpio.js');
load('api_mqtt.js');
load('api_net.js');
load('api_sys.js');
load('api_timer.js');

let led = Cfg.get('pins.led');
let getRssi = ffi('int wifi_station_get_rssi(void)');
let queueStart = ffi('bool start_queue()');
let queuePut = ffi('bool queue_put(char *)');
let queuePeek = ffi('void queue_peek(void(*)(char *, userdata), userdata)');
let queueDelete = ffi('bool queue_delete()');
let topic = '/device/x/data';

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


// function upload_data(delay) {
//   Timer.set(delay, false, function(delay) {
//     print("######################### Peeking at data");
//     queuePeek(function(data, delay) {
//       print("Data peeked:", data);

//       if (data === null) {
//         print("######################### No data left, trying again later");
//         upload_data(delay);
//         return;
//       }

//       // Valid data -- let's publish it
//       print("######################### Trying to upload data");
//       let ok = MQTT.pub(topic, data, 1);

//       if (!ok) {
//         // Try again later
//         print("######################### Unable to upload data -- try again later");
//         upload_data(delay);
//       }
//       else {
//         print("######################### Deleting data");
//         queueDelete();
//         // Try getting more data
//         upload_data(500);
//       }
//       print("######################### Done");
//     }, delay);
//   }, delay);
// }


Timer.set(2000 /* 2 sec */, false /* repeat */, function() {
  print("######################### Starting queue");
  let startResult = queueStart();
  print("Result: ", startResult);

  Timer.set(2000 /* 2 sec */, true /* repeat */, function() {
    print("######################### Adding data to queue");
    let result = queuePut(JSON.stringify({
      pm25: 100,
      pm10: 50,
      rssi: getRssi(),
      sequence: sequence,
      time: Timer.now()
    }));
    print("Put result:", result);

    sequence += 1;
  }, null);

  Timer.set(1000 /* 1 sec */, true /* repeat */, function() {
    queuePeek(function(data) {
      print("Peeked data: ", data);
      print("######################### Deleting data from queue");
      let result = queueDelete();
      print("Delete result:", result);
    }, null);


  }, null);

  // print("######################### Starting process to upload data");
  // upload_data(5000);
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
